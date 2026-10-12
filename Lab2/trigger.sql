DELIMITER $$

CREATE TRIGGER trg_rental_ai 
AFTER INSERT ON rental 
FOR EACH ROW 
BEGIN
    INSERT INTO audit_rental (rental_id, action, op_time, op_name)
    VALUES (NEW.rental_id, 'INSERT', NOW(), '0000000000'); 
END $$

DELIMITER ;