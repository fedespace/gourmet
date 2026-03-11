CREATE TABLE steps
(   
    id bigserial PRIMARY KEY,
    step_number int NOT NULL,
    recipe_id bigint NOT NULL references recipes(id),
    description varchar(255) NOT NULL
);