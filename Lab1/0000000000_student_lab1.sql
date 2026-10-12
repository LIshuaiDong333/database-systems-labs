-- Lab1 (2026) 作业模板
-- 请将本文件重命名为：学号_姓名_lab1.sql（例如：20261007_吴九_lab1.sql）
-- 然后在每个题号块下填写对应 SQL。
--

use sakila;

-- Q1
-- TODO
SELECT 
    c.customer_id, 
    CONCAT(c.first_name, ' ', c.last_name) AS customer_name, 
    a.address, 
    a.district, 
    ci.city
FROM customer c
JOIN address a ON c.address_id = a.address_id
JOIN city ci ON a.city_id = ci.city_id
JOIN country co ON ci.country_id = co.country_id
WHERE co.country = 'Japan'
ORDER BY c.customer_id ASC;

-- Q2
-- TODO
SELECT f.title
FROM film f
JOIN film_category fc ON f.film_id = fc.film_id
JOIN category c ON fc.category_id = c.category_id
WHERE c.name = 'Documentary' AND f.title LIKE 'M%'
ORDER BY f.title ASC;

-- Q3
-- TODO
SELECT 
    c.first_name, 
    c.last_name, 
    SUM(p.amount) AS total_amount
FROM customer c
JOIN payment p ON c.customer_id = p.customer_id
GROUP BY c.customer_id
HAVING SUM(p.amount) >= 150 AND SUM(p.amount) < 170
ORDER BY total_amount DESC, c.last_name ASC;

-- Q4
-- TODO
SELECT 
    f.film_id, 
    f.title, 
    SUM(p.amount) AS total_amount
FROM film f
JOIN inventory i ON f.film_id = i.film_id
JOIN rental r ON i.inventory_id = r.inventory_id
JOIN payment p ON r.rental_id = p.rental_id
WHERE i.store_id = 2
GROUP BY f.film_id
ORDER BY total_amount DESC
LIMIT 1;

-- Q5
-- TODO
SELECT 
    CONCAT(a.first_name, ' ', a.last_name) AS actor_name, 
    COUNT(fa.film_id) AS film_count
FROM actor a
JOIN film_actor fa ON a.actor_id = fa.actor_id
GROUP BY a.actor_id
HAVING COUNT(fa.film_id) >= 37
ORDER BY film_count DESC, actor_name ASC;

-- Q6
-- TODO
SELECT CONCAT(c.first_name, ' ', c.last_name) AS customer_name
FROM customer c
WHERE c.customer_id NOT IN (
    SELECT r.customer_id
    FROM rental r
    JOIN inventory i ON r.inventory_id = i.inventory_id
    JOIN film f ON i.film_id = f.film_id
    WHERE f.title = 'MURDER ANTITRUST'
)
ORDER BY customer_name ASC;

-- Q7
-- TODO
SELECT CONCAT(a.first_name, ' ', a.last_name) AS actor_name
FROM actor a
WHERE a.actor_id IN (
    SELECT fa.actor_id FROM film_actor fa
    JOIN film f ON fa.film_id = f.film_id WHERE f.title = 'MUSCLE BRIGHT'
)
AND a.actor_id IN (
    SELECT fa.actor_id FROM film_actor fa
    JOIN film f ON fa.film_id = f.film_id WHERE f.title = 'MOONWALKER FOOL'
)
ORDER BY actor_name ASC;

-- Q8
-- TODO
SELECT 
    c.category_id, 
    c.name, 
    COUNT(fc.film_id) AS film_count
FROM category c
JOIN film_category fc ON c.category_id = fc.category_id
GROUP BY c.category_id, c.name
ORDER BY c.category_id ASC;

-- Q9
-- TODO
SELECT 
    f.title, 
    COUNT(DISTINCT r.customer_id) AS renter_cnt
FROM film f
JOIN inventory i ON f.film_id = i.film_id
JOIN rental r ON i.inventory_id = r.inventory_id
WHERE i.store_id = 2
GROUP BY f.film_id, f.title
ORDER BY renter_cnt DESC, f.title ASC
LIMIT 1;

-- Q10
-- TODO
SELECT 
    c.first_name, 
    c.last_name, 
    MAX(TIMESTAMPDIFF(SECOND, r.rental_date, r.return_date)) AS duration_sec
FROM customer c
JOIN rental r ON c.customer_id = r.customer_id
WHERE r.return_date IS NOT NULL
GROUP BY c.customer_id, c.first_name, c.last_name
ORDER BY duration_sec DESC, c.last_name ASC
LIMIT 6;

-- Q11
-- TODO
INSERT INTO customer (store_id, first_name, last_name, email, address_id, active)
VALUES (
    1, 
    'STU0000000000', 
    'INIT', 
    '0000000000@lab.local', 
    1, 
    1
);

-- Q12
-- TODO
UPDATE customer 
SET last_name = 'UPDATED'
WHERE customer_id = (SELECT customer_id FROM (SELECT customer_id FROM customer WHERE email = '0000000000@lab.local') AS tmp);

-- Q13
-- TODO
DELETE FROM customer 
WHERE customer_id = (SELECT customer_id FROM (SELECT customer_id FROM customer WHERE email = '0000000000@lab.local') AS tmp);

-- Q14（选做）
-- TODO
SELECT 
    film_id, 
    title, 
    renter_cnt
FROM (
    SELECT 
        film_id, 
        title, 
        renter_cnt,
        DENSE_RANK() OVER (ORDER BY renter_cnt DESC) AS rnk
    FROM (
        SELECT 
            f.film_id, 
            f.title, 
            COUNT(DISTINCT r.customer_id) AS renter_cnt
        FROM film f
        JOIN inventory i ON f.film_id = i.film_id
        JOIN rental r ON i.inventory_id = r.inventory_id
        WHERE i.store_id = 2
        GROUP BY f.film_id, f.title
    ) AS FilmRentals
) AS RankedFilms
WHERE rnk <= 3
ORDER BY renter_cnt DESC, film_id ASC;

-- Q15（选做）
-- TODO
SELECT 
    c.customer_id, 
    CONCAT(c.first_name, ' ', c.last_name) AS customer_name
FROM customer c
WHERE NOT EXISTS (
    -- 找出这两部目标电影中
    SELECT 1
    FROM film f
    WHERE f.title IN ('CIDER DESIRE', 'HEAVYWEIGHTS BEAST')
    AND NOT EXISTS (
        -- 该顾客没有租借过的记录
        SELECT 1
        FROM rental r
        JOIN inventory i ON r.inventory_id = i.inventory_id
        WHERE r.customer_id = c.customer_id
        AND i.film_id = f.film_id
    )
)
ORDER BY c.customer_id ASC;
