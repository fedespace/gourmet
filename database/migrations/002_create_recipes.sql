CREATE TABLE recipes
(   
    id bigserial PRIMARY KEY,
    user_id bigint NOT NULL references users(id),
    name varchar(255) NOT NULL,
    pic varchar(255),
    rating int CHECK (rating between 1 and 5),
    difficulty int CHECK (difficulty between 1 and 5),
    category_id BIGINT REFERENCES categories(id),
    prep_time_min int NOT NULL,
    guests int DEFAULT 1,
    cooked int DEFAULT 0,
    created_at timestamp default NOW()
);