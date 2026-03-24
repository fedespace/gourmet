#include "RecipeController.h"
#include <optional>

// ================================
// SUMMARY OF THE METHODS
// getAllRecipes(request, response)            Fetch all the recipes present in the database
// getRecipeById(request, response, ID)        Fetch from the database the recipe with specified ID
// addRecipe(request, response)                Add single recipe to the database
// editRecipe(request, response, ID)           Amend a single recipe with specified ID
// ================================

// GET all the recipes in the database
void RecipeController::getAllRecipes (
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback
) {
    // Define the database as variable 'db'
    auto db = drogon::app().getDbClient();
    
    // Define the query to the database and responses
    db->execSqlAsync (
        
        // Query
        "SELECT id, name, user_id, pic, rating, difficulty, category_id, prep_time_min, guests FROM recipes",

        // Callback successful
        [callback](const drogon::orm::Result &result) {
            // Returning an array of items
            Json::Value json(Json::arrayValue);
            for (const auto &row : result) {
                Json::Value recipe;
                recipe["id"] = row["id"].as<int64_t>();
                recipe["name"] = row["name"].as<std::string>();
                recipe["user_id"] = row["user_id"].as<int>();
                recipe["rating"] = row["rating"].as<int>();
                recipe["difficulty"] = row["difficulty"].as<int>();
                recipe["category_id"] = row["category_id"].as<int64_t>();
                recipe["prep_time_min"] = row["prep_time_min"].as<int>();
                recipe["guests"] = row["guests"].as<int>();
                json.append(recipe);
            }
            auto response = drogon::HttpResponse::newHttpJsonResponse(json);
            callback(response);
        },

        // Callback failed
        [callback](const drogon::orm::DrogonDbException &e) {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k500InternalServerError);
            response->setBody(e.base().what());
            callback(response);
        }
    );
}

// ================================

// GET the recipe with ID = id from the database
void RecipeController::getRecipeById (
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
    int64_t id
) {
    // Define the database as variable 'db'
    auto db = drogon::app().getDbClient();
    
    // Define the query to the database and responses
    db->execSqlAsync (

        // Query
        "SELECT id, name, user_id, pic, rating, difficulty, category_id, prep_time_min, guests FROM recipes WHERE id = $1",

        // Callback successful
        [callback](const drogon::orm::Result &result) {
            // Check if result is not null in size
            if (result.size() == 0) {
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(drogon::k404NotFound);
                callback(response);
                return;
            }
            // Return a single item (ID)
            Json::Value recipe;
            recipe["id"] = result[0]["id"].as<int64_t>();
            recipe["name"] = result[0]["name"].as<std::string>();
            recipe["user_id"] = result[0]["user_id"].as<int>();
            recipe["rating"] = result[0]["rating"].as<int>();
            recipe["difficulty"] = result[0]["difficulty"].as<int>();
            recipe["category_id"] = result[0]["category_id"].as<int64_t>();
            recipe["prep_time_min"] = result[0]["prep_time_min"].as<int>();
            recipe["guests"] = result[0]["guests"].as<int>();
            auto response = drogon::HttpResponse::newHttpJsonResponse(recipe);
            callback(response);
        }, 

        // Callback failed
        [callback](const drogon::orm::DrogonDbException &e) {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k500InternalServerError);
            response->setBody(e.base().what());
            callback(response);
        },

        // Extra arguments (in this case, the recipe ID)
        id
    );
}

// ================================

// POST single recipe with id = ID
void RecipeController::addRecipe (
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback
) {
    // Get the body from the request. This will get a pointer in C++
    auto body = req->getJsonObject();   

    // Check the body pointer is not null
    if (body == nullptr) {
        // Raise error
        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k400BadRequest);
        callback(response);
        return;
    }

    // Fetching the fields from the pointer body
    std::string pic;
    if (body->isMember("pic")) {
       pic = (*body)["pic"].asString();
    } else {
        pic = "";
    }
    std::optional<int> rating;
    if (body->isMember("rating")) {
        rating = (*body)["rating"].asInt();
    }
    std::optional<int> difficulty;
    if (body->isMember("difficulty")) {
        difficulty = (*body)["difficulty"].asInt();
    }
    std::optional<int64_t> category_id;
    if (body->isMember("category_id")) {
        category_id = (*body)["category_id"].asInt();
    }
    int guests;
    if (body->isMember("guests")) {
        guests = (*body)["guests"].asInt();
    } else {
        guests = 1;
    }
    int cooked;
    if (body->isMember("cooked")) {
        cooked = (*body)["cooked"].asInt();
    } else {
        cooked = 0;
    }

    int64_t user_id = (*body)["user_id"].asInt64();
    std::string name = (*body)["name"].asString();
    int prep_time_min = (*body)["prep_time_min"].asInt();

    // Access the database
    auto db = drogon::app().getDbClient();

    // Proceed inserting the data in the correct table 'recipes'
    db->execSqlAsync(
        // Query
        "INSERT INTO recipes (user_id, name, pic, rating, difficulty, category_id, prep_time_min, cooked, guests) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9) RETURNING id",

        // Callback success
        [callback](const drogon::orm::Result &result) {
            Json::Value res_id;
            res_id["id"] = result[0]["id"].as<int64_t>();
            auto response = drogon::HttpResponse::newHttpJsonResponse(res_id);
            response->setStatusCode(drogon::k201Created);
            callback(response);
        }, 

        // Callback fail
        [callback](const drogon::orm::DrogonDbException &e) {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k500InternalServerError);
            response->setBody(e.base().what());
            callback(response);
        },

        // Inputs to the query
        user_id, name, pic, rating, difficulty, category_id, prep_time_min, cooked, guests
    );
}

// ================================

// PUT single recipe with ID = id
void RecipeController::editRecipe (
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    int64_t id
) { 
    // Get the body from the PUT request
    auto body = req->getJsonObject();

    // Check the body pointer is not null
    if (body == nullptr) {
        // Raise error
        auto response = drogon::HttpResponse::newHttpResponse();
        response->setStatusCode(drogon::k400BadRequest);
        callback(response);
        return;
    }

    // Fetching the existing fields from the request, overwriting the existing ones
    std::string pic;
    if (body->isMember("pic")) {
       pic = (*body)["pic"].asString();
    }
    std::optional<int> rating;
    if (body->isMember("rating")) {
        rating = (*body)["rating"].asInt();
    }
    std::optional<int> difficulty;
    if (body->isMember("difficulty")) {
        difficulty = (*body)["difficulty"].asInt();
    }
    std::optional<int64_t> category_id;
    if (body->isMember("category_id")) {
        category_id = (*body)["category_id"].asInt64();
    }
    int guests;
    if (body->isMember("guests")) {
        guests = (*body)["guests"].asInt();
    }
    int cooked;
    if (body->isMember("cooked")) {
        cooked = (*body)["cooked"].asInt();
    }
    int prep_time_min;
    if (body->isMember("prep_time_min")) {
        prep_time_min = (*body)["prep_time_min"].asInt();
    }
    std::string name;
    if (body->isMember("name")) {
        name = (*body)["name"].asString();
    }

    // Access the database
    auto db = drogon::app().getDbClient();

    // Proceed inserting the data in the correct table 'recipes'
    db->execSqlAsync(
        // Query
        "UPDATE recipes \
        SET name = $1, pic = $2, rating = $3, difficulty = $4, category_id = $5, prep_time_min = $6, cooked = $7, guests = $8 \
        WHERE id = $9",

        // Callback success
        [callback](const drogon::orm::Result &result) {
            if (result.affectedRows() == 0) {
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(drogon::k404NotFound);
                callback(response);
                return;
            }
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k200OK);
            callback(response);
        }, 

        // Callback fail
        [callback](const drogon::orm::DrogonDbException &e) {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k500InternalServerError);
            response->setBody(e.base().what());
            callback(response);
        },

        // Inputs to the query
        name, pic, rating, difficulty, category_id, prep_time_min, cooked, guests, id
    );
}

// ================================

// DELETE recipe with ID = id
void RecipeController::deleteRecipe (
    const drogon::HttpRequestPtr &req,
    std::function<void(const drogon::HttpResponsePtr &)> &&callback,
    int64_t id
) { 
    // Fetch the database and define it as variable 'db'
    auto db = drogon::app().getDbClient();

    // SQL Query
    db->execSqlAsync(
        // Query
        "DELETE FROM recipes WHERE id = $1",

        // Callback success
        [callback](const drogon::orm::Result &result) {
            if (result.affectedRows() == 0) {
                auto response = drogon::HttpResponse::newHttpResponse();
                response->setStatusCode(drogon::k404NotFound);
                callback(response);
                return;
            }
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k200OK);
            callback(response);
        },

        // Callback fail
        [callback](const drogon::orm::DrogonDbException &e) {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setStatusCode(drogon::k500InternalServerError);
            response->setBody(e.base().what());
            callback(response);
        },

        id
    );
}
