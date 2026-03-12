INSERT INTO users (name, email, pwd_hash) VALUES
('Mario Rossi', 'mario@email.com', '$2b$10$abcdefghijklmnopqrstuuVwxyz0123456789ABCDEFGHIJKLMNO'),
('Laura Bianchi', 'laura@email.com', '$2b$10$abcdefghijklmnopqrstuuVwxyz0123456789ABCDEFGHIJKLMNP');

INSERT INTO categories (name, user_id, is_custom) VALUES
('Primi', 1, TRUE),
('Secondi', 2, TRUE);

INSERT INTO recipes (user_id, name, pic, rating, difficulty, category_id, prep_time_min, guests) VALUES
(1, 'Pasta al pomodoro', NULL, 4, 1, 1, 20, 2),
(1, 'Risotto ai funghi', NULL, 5, 3, 1, 35, 4),
(2, 'Saltimbocca', NULL, 4, 4, 2, 25, 2);

INSERT INTO tags (name, user_id, is_custom) VALUES
('Italian', 1, TRUE),
('Italian', 2, TRUE);

INSERT INTO recipe_tags (recipe_id, tag_id) VALUES
(1, 1),
(2, 1),
(3, 2);

INSERT INTO ingredients (name, user_id) VALUES
('Mozzarella', 1),
('Pomodoro', 1),
('Grana Padano', 1),
('Manzo', 2),
('Mozzarella', 2);

INSERT INTO recipe_ingredients (recipe_id, ingredient_id, quantity, unit) VALUES
(1, 1, 1, 'unità'),
(1, 2, NULL, 'qb.'),
(1, 3, NULL, 'qb.'),
(2, 4, 4, 'fette'),
(2, 5, 1, 'unità');

INSERT INTO steps (step_number, recipe_id, description) VALUES
(1, 1, 'Heat the tomato sauce'),
(2, 1, 'Slice the mozzarella and add it when away from fire'),
(3, 1, 'Serve hot'),
(1, 2, 'Cook the beef with a bit of butter'),
(2, 2, 'Place mozzarella on the beef and close it with a stick');