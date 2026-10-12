DELIMITER $$

CREATE PROCEDURE p_customer_open_rentals(IN p_customer_id INT, OUT p_open_cnt INT)
BEGIN
    -- 返回该客户的未归还列表
    SELECT * FROM v_open_rentals 
    WHERE customer_id = p_customer_id;

    -- 统计未归还的数量，写入到p_open_cnt中
    SELECT COUNT(*) INTO p_open_cnt
    FROM v_open_rentals
    WHERE customer_id = p_customer_id;
END $$

DELIMITER ;