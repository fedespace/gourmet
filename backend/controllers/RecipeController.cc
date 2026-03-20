#include "RecipeController.h"

// ================================
// SUMMARY OF THE METHODS
// getAllRecipes(request, response)            Fetch all the recipes present in the database
// getRecipeById(request, response, ID)        Fetch from the database the recipe with specified ID
// addRecipe(request, response, JSONRecipe)    Add single recipe to the database
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
        "SELECT id, name FROM recipes",

        // Callback successful
        [callback](const drogon::orm::Result &result) {
            // Returning an array of items
            Json::Value json(Json::arrayValue);
            for (const auto &row : result) {
                Json::Value recipe;
                recipe["id"] = row["id"].as<int64_t>();
                recipe["name"] = row["name"].as<std::string>();
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
        "SELECT id, name FROM recipes WHERE id = $1",

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
    int64_t user_id = (*body)["user_id"].asInt64();
    std::string name = (*body)["name"].asString();
    std::string pic = (*body)["pic"].asString();
    int rating = (*body)["rating"].asInt64();
    int difficulty = (*body)["difficulty"].asInt64();
    int64_t category_id = (*body)["category_id"].asInt64();
    int prep_time_min = (*body)["prep_time_min"].asInt64();
    int guests = (*body)["guests"].asInt64();

    // Access the database
    auto db = drogon::app().getDbClient();

    // Proceed inserting the data in the correct table 'recipes'
    db->execSqlAsync(
        // Query
        "INSERT INTO recipes (user_id, name, pic, rating, difficulty, category_id, prep_time_min, guests) VALUES ($1, $2, $3, $4, $5, $6, $7, $8) RETURNING id",

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
        user_id, name, pic, rating, difficulty, category_id, prep_time_min, guests
    );
}
