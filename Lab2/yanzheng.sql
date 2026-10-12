-- 插入一条合法的测试数据 (假设 inventory_id=1, customer_id=1, staff_id=1 都是存在的)
INSERT INTO rental (rental_date, inventory_id, customer_id, staff_id) 
VALUES (NOW(), 1, 1, 1);

-- 查询审计表，查看是否自动生成了日志
SELECT * FROM audit_rental;