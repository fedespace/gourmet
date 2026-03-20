#pragma once
#include <drogon/drogon.h>

// ================================
// SUMMARY OF THE METHODS
// getAllRecipes(request, response)            Fetch all the recipes present in the database
// getRecipeById(request, response, ID)        Fetch from the database the recipe with specified ID
// addRecipe(request, response, JSONRecipe)    Add single recipe to the database
// ================================

class RecipeController: public drogon::HttpController<RecipeController> {
    public:

        // ============ LISTS OF METHODS ============ //
        METHOD_LIST_BEGIN
            // GET all recipes
            ADD_METHOD_TO(RecipeController::getAllRecipes, "/recipes", drogon::Get);
            // GET recipe with id
            ADD_METHOD_TO(RecipeController::getRecipeById, "/recipes/{id}", drogon::Get);
            // POST recipe
            ADD_METHOD_TO(RecipeController::addRecipe, "/recipes", drogon::Post); 
        METHOD_LIST_END


        // ================================

        // GET all recipes
        void getAllRecipes(
            const drogon::HttpRequestPtr &req, 
            std::function<void(const drogon::HttpResponsePtr &)> &&callback
        );

        // GET recipe with ID
        void getRecipeById(
            const drogon::HttpRequestPtr &req, 
            std::function<void(const drogon::HttpResponsePtr &)> &&callback, 
            int64_t id
        );

        // POST recipe
        void addRecipe(
            const drogon::HttpRequestPtr &req, 
            std::function<void(const drogon::HttpResponsePtr &)> &&callback
        );
};