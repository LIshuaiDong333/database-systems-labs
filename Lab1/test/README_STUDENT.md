# Lab1 Student Kit（2026）

本目录用于学生本地自测与提交。

---

## 1. 你会拿到哪些文件

- `学号_姓名_lab1.sql`：作业模板（只有题号框架 + TODO）
- `lab1_selfcheck.py`：自测脚本（对/错 + 预估得分）
- `expected_public_hash.json`：老师发的标准结果（hash 版）
- `lab1_config.json`：数据库连接配置（需填写密码）

---

## 2. 配置数据库连接

打开 `lab1_config.json`，按你的本机 MySQL 修改：

- `host`（默认 127.0.0.1）
- `port`（默认 3306）
- `user`（默认 root）
- `password`（请填你的 root 密码）
- `database`（默认 sakila）

---

## 3. 准备你的作业文件

1) 将`学号_姓名_lab1.sql` 重命名成你真实的学号与姓名，例如：`20261007_吴九_lab1.sql`

2) 在每个题号块下填写 SQL，必须保留题号标记：

```sql
-- Q1
SELECT ...;

-- Q2
SELECT ...;
...
-- Q15
SELECT ...;
```

---

## 4. 自测（推荐）

在本目录运行：

```bash
python lab1_selfcheck.py --sql 学号_姓名_lab1.sql
```

---

## 5. 常见错误与注意事项

- 禁止语句：`DROP / TRUNCATE / ALTER / CREATE DATABASE / COMMIT / SET autocommit`
- Q1~Q10 只能写 `SELECT`
- Q11~Q13 要按题面规则插入/更新/删除（email / first_name / last_name 等字段的格式必须正确）
- Q14~Q15 为附加题，选做，只有结果正确且方法满足要求才加分（本次实验总分封顶 100）
- 请严格按题面要求写 `ORDER BY`（字段与升降序）

---

## 6. 提交

提交你的：`学号_姓名_lab1.sql`（注意学号和姓名要重命名成你真实的学号与姓名）
