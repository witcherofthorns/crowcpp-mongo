#ifndef CROW_MONGO_MODELS_HPP
#define CROW_MONGO_MODELS_HPP

#include<string>
#include<vector>
#include<nlohmann/json.hpp>

#define JSON_MODEL NLOHMANN_DEFINE_TYPE_INTRUSIVE

namespace models {

struct User {
    int age;
    std::string name;
    std::string nick;
    std::string photo;
    JSON_MODEL(User, age, name, nick, photo)
};

}


namespace models {

/// Return True if model is valid, and False not valid
/// If True, return valid your struct<T>& model
template<typename T> bool inline validate(T& model, std::string json_str){
    if(json_str.empty()) return false;
    try { model = nlohmann::json::parse(json_str); return true; }
    catch(const nlohmann::detail::exception& e) {
        CROW_LOG_ERROR << typeid(T).name() << " | validate:" << e.what();
        return false;
    }
    return true;
}

/// Return string from model
template<typename T> const std::string inline to_string(T& model){
    try { return nlohmann::json(model).dump(); }
    catch(const nlohmann::detail::exception& e) {
        CROW_LOG_ERROR << typeid(T).name() << " | validate:" << e.what();
        return {};
    }
}

}

#endif