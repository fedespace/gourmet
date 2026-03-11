CREATE TABLE tags
(   
    id bigserial PRIMARY KEY,
    name varchar(255) NOT NULL,
    user_id bigint references users(id),
    is_custom boolean
);