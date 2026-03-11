CREATE TABLE recipe_ingredients
(   
    recipe_id bigint NOT NULL references recipes(id),
    ingredient_id bigint NOT NULL references ingredients(id),
    quantity float,
    unit varchar(255),
    PRIMARY KEY (recipe_id, ingredient_id)
);