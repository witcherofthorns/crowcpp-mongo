#include<crow/app.h>
#include<crow/models.hpp>
#include<crow/services/mongo.hpp>

static void route_test_create(crow::App<>& app){

    CROW_ROUTE(app, "/user").methods(crow::HTTPMethod::GET)([](const crow::request& req) {
        if (req.url_params.get("oid") == nullptr){
            return crow::response(crow::BAD_REQUEST);
        }
        
        const std::string oid = req.url_params.get("oid");

        // get mongo document by object-id
        // check if result is null
        const std::string user_result = service::mongo_get_document("test", "users", oid);
        if(user_result.empty()){
            return crow::response(crow::BAD_REQUEST);
        }

        return crow::response(crow::OK, "application/json", user_result);
    });


    CROW_ROUTE(app, "/user").methods(crow::HTTPMethod::POST)([](const crow::request& req) {
        if(req.body.empty()){
            return crow::response(crow::BAD_REQUEST);
        }
        
        // your model for validation
        models::User user;

        // check req body json for user valid model
        if(!models::validate(user, req.body)){
            return crow::response(crow::BAD_REQUEST);
        }

        // data model to json string
        const std::string user_json = models::to_string(user);

        // send user model to mongo
        const std::string user_oid = service::mongo_create_document("test", "users", user_json);
        CROW_LOG_INFO << "Mongo: Created new user " << user_oid;
        return crow::response(crow::CREATED);
    });


    CROW_ROUTE(app, "/user").methods(crow::HTTPMethod::PUT)([](const crow::request& req) {
        if(req.body.empty() || req.url_params.get("oid") == nullptr){
            return crow::response(crow::BAD_REQUEST);
        }

        const std::string oid = req.url_params.get("oid");

        // your model for validation
        models::User user;

        // check req body json for user valid model
        if(!models::validate(user, req.body)){
            return crow::response(crow::BAD_REQUEST);
        }

        // data model to json string
        const std::string user_json = models::to_string(user);

        // return bool value
        if(!service::mongo_update_document("test", "users", oid, user_json)){
            return crow::response(crow::NOT_MODIFIED);
        }

        return crow::response(crow::OK, "application/json", user_json);
    });


    CROW_ROUTE(app, "/user").methods(crow::HTTPMethod::DELETE)([](const crow::request& req) {
        if (req.url_params.get("oid") == nullptr){
            return crow::response(crow::BAD_REQUEST);
        }
        
        const std::string oid = req.url_params.get("oid");

        // return bool value
        if(!service::mongo_delete_document("test", "users", oid)){
            return crow::response(crow::BAD_REQUEST);
        }

        return crow::response(crow::OK);
    });
}




int main() {
    // mongodb start
    if(!service::mongo_start("mongodb://localhost:27018")){
        return -1;
    }

    crow::App<> app;

    // basic crowcpp routing example to
    // demonstrate how mongodb works
    route_test_create(app);

    app.port(18080).multithreaded().run();

    // mongodb stop
    service::mongo_stop();
    return 0;
}