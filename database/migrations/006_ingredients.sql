CREATE TABLE ingredients
(   
    id bigserial PRIMARY KEY,
    name varchar(255) NOT NULL,
    user_id bigint references users(id) ON DELETE CASCADE
);