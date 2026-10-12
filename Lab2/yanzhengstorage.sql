SET @cnt = 0;
CALL p_customer_open_rentals(15, @cnt);
SELECT @cnt;