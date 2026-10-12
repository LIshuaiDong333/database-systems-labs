CREATE VIEW v_open_rentals AS
SELECT 
    r.rental_id,
    c.customer_id,
    CONCAT(c.first_name, ' ', c.last_name) AS customer_name,
    i.inventory_id,
    i.store_id,
    r.rental_date
FROM rental r
JOIN inventory i ON r.inventory_id = i.inventory_id
JOIN customer c ON r.customer_id = c.customer_id
WHERE r.return_date IS NULL;