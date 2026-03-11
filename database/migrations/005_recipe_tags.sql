CREATE TABLE recipe_tags
(   
    recipe_id bigint NOT NULL references recipes(id),
    tag_id bigint NOT NULL references tags(id),
    PRIMARY KEY (recipe_id, tag_id)
);