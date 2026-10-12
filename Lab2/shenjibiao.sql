CREATE TABLE audit_rental (
    id INT AUTO_INCREMENT PRIMARY KEY,
    rental_id INT NOT NULL,
    action VARCHAR(10) NOT NULL,
    op_time DATETIME NOT NULL,
    op_name VARCHAR(20) NOT NULL
);