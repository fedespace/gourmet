CREATE TABLE recipe_tags
(   
    recipe_id bigint NOT NULL references recipes(id) ON DELETE CASCADE,
    tag_id bigint NOT NULL references tags(id) ON DELETE CASCADE,
    PRIMARY KEY (recipe_id, tag_id)
);