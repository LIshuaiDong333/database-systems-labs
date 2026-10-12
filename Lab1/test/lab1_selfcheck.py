#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Lab1 自测脚本（2026版，支持计分 + 附加题方法检测）
------------------------------------------------
学生用法（推荐）：
  python lab1_selfcheck.py --sql lab1.sql 
  # 若脚本同目录存在 expected_public_hash.json，会自动读取并给出分数/对错。

可选：手动指定 expected
  python lab1_selfcheck.py --sql 学号_姓名_lab1.sql  --expected expected_public_hash.json

说明：
- 默认读取同目录 lab1_config.json 作为数据库连接配置；
- 可用命令行覆盖：--host --port --user --password --db
- Q11~Q13 会放在同一事务里执行，脚本最后会 ROLLBACK，避免污染数据库。

⚠️ 出现以下语句会直接终止（禁止语句）：
- DROP / TRUNCATE / ALTER / CREATE DATABASE
- COMMIT / SET autocommit
"""
from __future__ import annotations

import argparse
import hashlib
import json
import os
import re
import sys
from pathlib import Path
from typing import Any, Dict, List, Tuple, Optional

try:
    import mysql.connector
except ImportError:
    print("缺少依赖：mysql-connector-python。请先执行：pip install mysql-connector-python", file=sys.stderr)
    raise

# 题目范围
BASE_QIDS = [f"Q{i}" for i in range(1, 14)]          # Q1~Q13（基础题，计入 100 分）
EXTRA_QIDS = ["Q14", "Q15"]                          # 附加题（选做，加分但总分封顶 100）
ALL_QIDS = BASE_QIDS + EXTRA_QIDS

# 分值（基础题凑满 100；Q11~Q13 各 10）
BASE_POINTS: Dict[str, int] = {
    "Q1": 7, "Q2": 7, "Q3": 7, "Q4": 7, "Q5": 7,
    "Q6": 7, "Q7": 7, "Q8": 7, "Q9": 7, "Q10": 7,
    "Q11": 10, "Q12": 10, "Q13": 10,
}
EXTRA_POINTS: Dict[str, int] = {"Q14": 10, "Q15": 10}

# 结果是否需要保持顺序（按实验指导书：这些题都要求 ORDER BY）
ORDERED_QIDS = set([f"Q{i}" for i in range(1, 11)] + ["Q14", "Q15"])

BANNED_PATTERN = re.compile(
    r"\b(drop|truncate|alter|create\s+database|commit|set\s+autocommit)\b",
    re.IGNORECASE,
)

def load_json(path: str) -> Dict[str, Any]:
    return json.loads(Path(path).read_text(encoding="utf-8"))

def load_config(config_path: str) -> Dict[str, Any]:
    return load_json(config_path)

def parse_lab_sql(sql_text: str) -> Dict[str, str]:
    """
    根据 -- Q1 ... -- Q15 切分。
    返回 { "Q1": "...", ... }（每题对应 SQL 片段，可能包含多条语句）。
    """
    t = sql_text.replace("\r\n", "\n").replace("\r", "\n")
    pat = re.compile(r"^\s*--\s*(Q(?:1[0-5]|[1-9]))\b.*$", re.IGNORECASE | re.MULTILINE)
    matches = list(pat.finditer(t))
    blocks: Dict[str, str] = {}
    for idx, m in enumerate(matches):
        qid = m.group(1).upper()
        start = m.end()
        end = matches[idx + 1].start() if idx + 1 < len(matches) else len(t)
        blocks[qid] = t[start:end].strip()
    return blocks

def split_statements(sql_block: str) -> List[str]:
    """
    以分号粗略切分语句；忽略空白与纯注释行。
    """
    lines: List[str] = []
    for line in sql_block.splitlines():
        if re.match(r"^\s*--", line):
            continue
        lines.append(line)
    cleaned = "\n".join(lines).strip()
    parts = [p.strip() for p in cleaned.split(";")]
    return [p for p in parts if p]


def pick_main_select_statement(stmts: List[str]) -> Optional[str]:
    """
    在题号块的多条语句中，选择最后一条 SELECT 作为主判题语句。
    这样允许：
        SET @x = 1;
        SELECT ...
    """
    selects = [s.strip() for s in stmts if s.strip().lower().startswith("select")]
    return selects[-1] if selects else None

def infer_sid_from_filename(sql_path: str) -> str:
    """
    从提交文件名推断学号（与 lab1_grade.py 保持一致）：

    推荐命名：学号_姓名_*.sql
    - sid = 第一个 '_' 之前的部分

    若不符合推荐命名，则宽松兜底：
    - sid = 文件名（去扩展）开头第一段（以 '_' / '-' / 空格 分隔）
    """
    filename = os.path.basename(sql_path)
    stem = os.path.splitext(filename)[0]

    if "_" in stem:
        parts = stem.split("_")
        sid = parts[0].strip() if len(parts) >= 1 else None
        if sid:
            return sid

    m = re.match(r"(?P<sid>[^_\-\s]+)(?:[_\-\s]+(?P<name>[^_\-\s]+))?", stem)
    if not m or not m.group("sid"):
        raise ValueError("无法从文件名推断学号，请使用 --sid 显式指定。")
    return m.group("sid")

def normalize_rows(rows: List[Tuple[Any, ...]], ordered: bool) -> List[List[str]]:
    out: List[List[str]] = []
    for r in rows:
        out.append([("" if v is None else str(v)) for v in r])
    if not ordered:
        out.sort()
    return out

def rows_digest(cols: List[str], rows: List[List[str]]) -> str:
    h = hashlib.sha256()
    h.update(("|".join(map(str, cols)) + "\n").encode("utf-8"))
    for r in rows:
        h.update(("|".join(r) + "\n").encode("utf-8"))
    return h.hexdigest()

def consume_unread(cursor) -> None:
    """mysql-connector 有时需要把未读结果读完，避免下一次 execute 出错。"""
    try:
        if getattr(cursor, "with_rows", False):
            cursor.fetchall()
    except Exception:
        pass

def run_select(cursor, stmt: str, ordered: bool) -> Tuple[List[str], List[List[str]]]:
    cursor.execute(stmt)
    cols = [d[0] for d in (cursor.description or [])]
    rows = cursor.fetchall()
    return cols, normalize_rows(rows, ordered=ordered)

def strip_sql_for_method_check(sql: str) -> str:
    """
    删除注释与字符串常量，降低“在注释里塞关键字”逃避检测的概率。
    目的：仅用于方法检测（不是 SQL 解析器）。
    """
    s = sql.replace("\r\n", "\n").replace("\r", "\n")
    # /* ... */ 注释
    s = re.sub(r"/\*.*?\*/", " ", s, flags=re.DOTALL)
    # -- 行注释
    s = re.sub(r"--.*?$", " ", s, flags=re.MULTILINE)
    # 单引号字符串
    s = re.sub(r"'(?:''|[^'])*'", "''", s)
    # 统一空白
    s = re.sub(r"\s+", " ", s)
    return s.strip().lower()

def method_ok_q14(stmt: str) -> bool:
    s = strip_sql_for_method_check(stmt)
    has_rank = re.search(r"\b(dense_rank)\s*\(", s) is not None
    has_over = re.search(r"\bover\b", s) is not None
    has_limit = re.search(r"\blimit\b", s) is not None
    return bool(has_rank and has_over and (not has_limit))

def method_ok_q15(stmt: str) -> bool:
    s = strip_sql_for_method_check(stmt)
    not_exists_cnt = len(re.findall(r"\bnot\s+exists\b", s))
    has_group_or_having = re.search(r"\bgroup\s+by\b|\bhaving\b", s) is not None
    return bool(not_exists_cnt >= 2 and (not has_group_or_having))

def compare_with_expected(
    qid: str,
    cols: List[str],
    rows: List[List[str]],
    expected: Dict[str, Any],
) -> Tuple[bool, str]:
    """
    返回 (是否匹配, 详细原因字符串仅供 verbose 输出)
    """
    exp = expected.get(qid)
    mode = (expected.get("_meta", {}) or {}).get("mode", "hash")
    if exp is None:
        return False, "expected 文件中缺少该题标准项"

    if mode == "hash":
        exp_cols = exp.get("columns", [])
        exp_rowcount = exp.get("rowcount", None)
        exp_digest = exp.get("digest", "")
        digest = rows_digest(cols, rows)

        col_ok = (list(map(str, cols)) == list(map(str, exp_cols)))
        row_ok = (exp_rowcount == len(rows) and digest == exp_digest)

        if col_ok and row_ok:
            return True, ""
        parts = []
        if not col_ok:
            parts.append(f"列名/列顺序不一致：你的={cols} 标准={exp_cols}")
        if exp_rowcount != len(rows):
            parts.append(f"行数不一致：你的={len(rows)} 标准={exp_rowcount}")
        elif digest != exp_digest:
            parts.append("内容不一致（digest 不同）")
        return False, "；".join(parts)

    # full
    exp_cols = exp.get("columns", [])
    exp_rows = exp.get("rows", [])
    col_ok = (list(map(str, cols)) == list(map(str, exp_cols)))
    row_ok = (rows == exp_rows)
    if col_ok and row_ok:
        return True, ""
    parts = []
    if not col_ok:
        parts.append(f"列名/列顺序不一致：你的={cols} 标准={exp_cols}")
    if not row_ok:
        parts.append(f"行内容/顺序不一致：你的行数={len(rows)} 标准行数={len(exp_rows)}")
    return False, "；".join(parts)

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--sql", required=True, help="lab1.sql 路径")
    ap.add_argument("--expected", default=None, help="可选：expected 文件（JSON）。不提供则自动尝试读取同目录 expected_public_hash.json")
    ap.add_argument("--config", default="lab1_config.json", help="数据库连接配置文件（默认同目录 lab1_config.json）")

    ap.add_argument("--host", default=None)
    ap.add_argument("--port", type=int, default=None)
    ap.add_argument("--user", default=None)
    ap.add_argument("--password", default=None)
    ap.add_argument("--db", default=None)

    ap.add_argument("--verbose", action="store_true", help=argparse.SUPPRESS)

    args = ap.parse_args()

    sql_path = os.path.abspath(args.sql)
    sql_text = Path(sql_path).read_text(encoding="utf-8")

    # banned check
    if BANNED_PATTERN.search(sql_text):
        print("检测到禁止语句（DROP/TRUNCATE/ALTER/CREATE DATABASE/COMMIT/SET autocommit）。请删除后再自测。")
        return 2

    blocks = parse_lab_sql(sql_text)

    # expected auto-load
    expected: Optional[Dict[str, Any]] = None
    expected_path = args.expected
    if expected_path is None:
        here = os.path.dirname(os.path.abspath(__file__))
        cand = os.path.join(here, "expected_public_hash.json")
        if os.path.exists(cand):
            expected_path = cand
    if expected_path:
        expected = load_json(expected_path)

    if expected is None:
        print("未找到 expected 文件，无法计算对错与分数。")
        print("请将 expected_public_hash.json 放到脚本同目录，或用 --expected 指定。")
        return 2

    # config path
    config_path = args.config
    if not os.path.isabs(config_path):
        here = os.path.dirname(os.path.abspath(__file__))
        cand = os.path.join(here, config_path)
        if os.path.exists(cand):
            config_path = cand

    cfg = load_config(config_path)
    host = args.host or cfg.get("host", "127.0.0.1")
    port = args.port or int(cfg.get("port", 3306))
    user = args.user or cfg.get("user", "root")
    password = args.password if args.password is not None else cfg.get("password", "")
    db = args.db or cfg.get("database", "sakila")

    conn = mysql.connector.connect(
        host=host,
        port=port,
        user=user,
        password=password,
        database=db,
        connection_timeout=int(cfg.get("connect_timeout", 10)),
        autocommit=False,
        use_pure=True,
    )
    # buffered=True：减少“未读结果”带来的状态问题
    cur = conn.cursor(buffered=True)

    print("Lab1 自测（2026版）")

    try:
        cur.execute(f"USE {db}")
    except Exception as e:
        print(f"[致命错误] 无法 USE {db}: {e}")
        return 3
    try:
        sid = infer_sid_from_filename(args.sql)
    except Exception as e:
        print(f"[致命错误] {e}")
        return 2

    email = f"{sid}@lab.local"
    # 计分
    base_score = 0
    extra_score = 0
    base_all_correct = True
    extra_done = 0
    extra_correct = 0

    # 缺题提醒（基础题缺少视为错误；附加题缺少不扣基础分）
    missing_base = [q for q in BASE_QIDS if q not in blocks]
    if missing_base:
        print("[警告] 缺少基础题题号块：", ", ".join(missing_base))
        print("       基础题缺题会导致该题 0 分。")
        print("")
        base_all_correct = False

    # Q11 前置是否通过（用于避免 Q12/Q13 误判）
    q11_ok = False

    # 为避免污染数据库：从 Q11 开始开启事务（Q1~Q10 纯查询，不影响）
    try:
        conn.start_transaction()
    except Exception:
        pass

    def grade_base_question(qid: str, ok: bool):
        nonlocal base_score, base_all_correct
        pts = BASE_POINTS.get(qid, 0)
        if ok:
            base_score += pts
        else:
            base_all_correct = False

    def grade_extra_question(qid: str, ok: bool):
        nonlocal extra_score, extra_done, extra_correct
        extra_done += 1
        if ok:
            extra_score += EXTRA_POINTS.get(qid, 0)
            extra_correct += 1

    # ====== Q1~Q10（查询） ======
    for qid in [f"Q{i}" for i in range(1, 11)]:
        print(f"== {qid} ==")

        if qid not in blocks:
            if qid in EXTRA_QIDS:
                print("未作答（选做）：+0 分\n")
            else:
                print("缺失：未找到该题 SQL（请用 -- Qx 标记）")
                print(f"本题：❌ 错误（+0 / {BASE_POINTS.get(qid,0)} 分）\n")
            if qid in BASE_QIDS:
                grade_base_question(qid, False)
            continue

        stmts = split_statements(blocks[qid])
        if not stmts:
            if qid in EXTRA_QIDS:
                print("空题（选做）：+0 分\n")
            else:
                print("空题：题号块下没有可执行语句。")
                print(f"本题：❌ 错误（+0 / {BASE_POINTS.get(qid,0)} 分）\n")
                grade_base_question(qid, False)
            continue

        main_stmt = pick_main_select_statement(stmts)
        if not main_stmt:
            if qid in EXTRA_QIDS:
                print("错误：附加题应为 SELECT 查询（不计分）。\n")
            else:
                print("错误：本题应为 SELECT 查询。")
                print(f"本题：❌ 错误（+0 / {BASE_POINTS.get(qid,0)} 分）\n")
                grade_base_question(qid, False)
            continue

        ordered = (qid in ORDERED_QIDS)

        try:
            cols, rows = run_select(cur, main_stmt, ordered=ordered)

            # 比对 expected
            ok, detail = compare_with_expected(qid, cols, rows, expected)

            # 附加题：还要方法检测
            if qid == "Q14" and ok and (not method_ok_q14(main_stmt)):
                ok = False
                detail = "方法不符合要求（需窗口函数，且不得使用 LIMIT）"
                print("结果正确，但方法不符合要求：❌ +0 分\n")
                if args.verbose:
                    print("  [verbose] " + detail + "\n")
                grade_extra_question(qid, False)
                continue

            if qid == "Q15" and ok and (not method_ok_q15(main_stmt)):
                ok = False
                detail = "方法不符合要求（需双层 NOT EXISTS，且不得使用 GROUP BY/HAVING）"
                print("结果正确，但方法不符合要求：❌ +0 分\n")
                if args.verbose:
                    print("  [verbose] " + detail + "\n")
                grade_extra_question(qid, False)
                continue

            if qid in EXTRA_QIDS:
                if ok:
                    pts = EXTRA_POINTS[qid]
                    print(f"✅ 正确（+{pts} 分）\n")
                    grade_extra_question(qid, True)
                else:
                    print("❌ 错误（+0 分）\n")
                    if args.verbose and detail:
                        print("  [verbose] " + detail + "\n")
                    grade_extra_question(qid, False)
            else:
                pts = BASE_POINTS[qid]
                if ok:
                    print(f"✅ 正确（+{pts} / {pts} 分）\n")
                    grade_base_question(qid, True)
                else:
                    print(f"❌ 错误（+0 / {pts} 分）\n")
                    if args.verbose and detail:
                        print("  [verbose] " + detail + "\n")
                    grade_base_question(qid, False)

        except Exception as e:
            if qid in EXTRA_QIDS:
                print(f"执行失败：{e}")
                print("本题：❌ 错误（+0 分）\n")
                grade_extra_question(qid, False)
            else:
                pts = BASE_POINTS[qid]
                print(f"执行失败：{e}")
                print(f"本题：❌ 错误（+0 / {pts} 分）\n")
                grade_base_question(qid, False)

    # ====== Q11~Q13（增删改） ======
    for qid in ["Q11", "Q12", "Q13"]:
        print(f"== {qid} ==")

        if qid not in blocks:
            print("缺失：未找到该题 SQL（请用 -- Qx 标记）")
            print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
            grade_base_question(qid, False)
            continue

        stmts = split_statements(blocks[qid])
        if not stmts:
            print("空题：题号块下没有可执行语句。")
            print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
            grade_base_question(qid, False)
            continue

        # 依赖：Q12/Q13 要求先 Q11 成功（避免“误判删除成功”）
        if qid in ["Q12", "Q13"] and (not q11_ok):
            print("跳过：Q11 未通过或未按规则插入记录，无法评测本题（请先修复 Q11）。")
            print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
            grade_base_question(qid, False)
            continue

        try:
            for s in stmts:
                cur.execute(s)
                consume_unread(cur)

            if qid == "Q11":
                cur.execute("SELECT customer_id, first_name, last_name, email FROM customer WHERE email=%s", (email,))
                row = cur.fetchone()
                if not row:
                    print(f"错误：未找到 email={email} 的 customer（Q11 必须按规则插入）。")
                    print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
                    grade_base_question(qid, False)
                else:
                    cid, fn, ln, em = row
                    ok = (em == email and fn == f"STU{sid}")
                    if ok:
                        q11_ok = True
                        pts = BASE_POINTS[qid]
                        print(f"✅ 通过（+{pts} / {pts} 分）\n")
                        grade_base_question(qid, True)
                    else:
                        print("错误：Q11 规则不满足（email 必须为 学号@lab.local；first_name 必须为 STU学号）。")
                        print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
                        grade_base_question(qid, False)

            elif qid == "Q12":
                cur.execute("SELECT last_name FROM customer WHERE email=%s", (email,))
                row = cur.fetchone()
                ok = bool(row) and (row[0] == "UPDATED")
                if ok:
                    pts = BASE_POINTS[qid]
                    print(f"✅ 通过（+{pts} / {pts} 分）\n")
                    grade_base_question(qid, True)
                else:
                    print("❌ 未通过（last_name 未更新为 UPDATED）")
                    print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
                    grade_base_question(qid, False)

            elif qid == "Q13":
                cur.execute("SELECT COUNT(*) FROM customer WHERE email=%s", (email,))
                cnt = cur.fetchone()[0]
                ok = (cnt == 0)
                if ok:
                    pts = BASE_POINTS[qid]
                    print(f"✅ 通过（+{pts} / {pts} 分）\n")
                    grade_base_question(qid, True)
                else:
                    print("❌ 未通过（目标记录仍存在）")
                    print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
                    grade_base_question(qid, False)

        except Exception as e:
            print(f"执行失败：{e}")
            print(f"本题：❌ 错误（+0 / {BASE_POINTS[qid]} 分）\n")
            grade_base_question(qid, False)

    # ====== Q14~Q15（附加题查询） ======
    for qid in EXTRA_QIDS:
        print(f"== {qid} ==")

        if qid not in blocks:
            print("未作答（选做）：+0 分\n")
            continue

        stmts = split_statements(blocks[qid])
        if not stmts:
            print("空题（选做）：+0 分\n")
            continue

        main_stmt = pick_main_select_statement(stmts)
        if not main_stmt:
            print("错误：附加题应为 SELECT 查询（不计分）。\n")
            grade_extra_question(qid, False)
            continue

        ordered = (qid in ORDERED_QIDS)

        try:
            cols, rows = run_select(cur, main_stmt, ordered=ordered)
            ok, detail = compare_with_expected(qid, cols, rows, expected)

            if qid == "Q14" and ok and (not method_ok_q14(main_stmt)):
                detail = "方法不符合要求（需窗口函数，且不得使用 LIMIT）"
                print("结果正确，但方法不符合要求：❌ +0 分\n")
                if args.verbose:
                    print("  [verbose] " + detail + "\n")
                grade_extra_question(qid, False)
                continue

            if qid == "Q15" and ok and (not method_ok_q15(main_stmt)):
                detail = "方法不符合要求（需双层 NOT EXISTS，且不得使用 GROUP BY/HAVING）"
                print("结果正确，但方法不符合要求：❌ +0 分\n")
                if args.verbose:
                    print("  [verbose] " + detail + "\n")
                grade_extra_question(qid, False)
                continue

            if ok:
                pts = EXTRA_POINTS[qid]
                print(f"✅ 正确（+{pts} 分）\n")
                grade_extra_question(qid, True)
            else:
                print("❌ 错误（+0 分）\n")
                if args.verbose and detail:
                    print("  [verbose] " + detail + "\n")
                grade_extra_question(qid, False)

        except Exception as e:
            print(f"执行失败：{e}")
            print("本题：❌ 错误（+0 分）\n")
            grade_extra_question(qid, False)

    # 回滚（避免污染）
    try:
        conn.rollback()
    except Exception:
        pass
    cur.close()
    conn.close()

    total = min(100, base_score + extra_score)

    print("====== 计分汇总 ======")
    print(f"基础题：{base_score} / 100")
    print(f"附加题：{extra_score} / 20")
    print(f"总分：  {total} / 100（封顶）")
    print("")

    if extra_done == 0:
        extra_msg = "❌ 附加题未作答"
    elif extra_correct == len(EXTRA_QIDS):
        extra_msg = "✅ 附加题全对"
    else:
        extra_msg = "❌ 附加题存在错误"

    base_msg = "✅ 基础题全对" if base_all_correct else "❌ 基础题存在错误"
    print(f"自测结论：{base_msg}\t{extra_msg}")
    return 0 if base_all_correct else 1

if __name__ == "__main__":
    raise SystemExit(main())
