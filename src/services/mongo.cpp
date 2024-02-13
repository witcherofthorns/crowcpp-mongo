#include<crow/services/mongo.hpp>
#include<crow/logging.h>
#include<bsoncxx/builder/stream/document.hpp>
#include<bsoncxx/json.hpp>
#include<mongocxx/client.hpp>
#include<mongocxx/uri.hpp>
#include<mongocxx/instance.hpp>
#include<sstream>

using bsoncxx::builder::stream::document;
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::basic::make_document;
using bsoncxx::builder::basic::kvp;
using bsoncxx::v_noabi::document::value;
static mongocxx::instance* instance;
static mongocxx::client* client;

namespace service {

bool mongo_start(std::string connection_uri){
    mongocxx::options::client client_options{};
    instance = new mongocxx::instance{};
    client = new mongocxx::client { mongocxx::uri(connection_uri), client_options };

    // default ping to collection admin for check connections
    // exit if throw mongodb ping
    try { client->database("admin").run_command(make_document(kvp("ismaster", 1))); }
    catch(const std::exception& error) {
        CROW_LOG_ERROR << "Service MongoDB: " << error.what();
        return false;
    }
    
    CROW_LOG_INFO << "Service MongoDB: Start";
    return true;
}

void mongo_stop(){
    delete client;
    delete instance;
    CROW_LOG_INFO << "Service MongoDB Stop";
}

static value mongo_create_filter_objectid(std::string id){
    return make_document(kvp("_id", bsoncxx::oid{bsoncxx::stdx::string_view(id.data())}));
}

const std::string mongo_create_document_oid(std::string db, std::string collection, std::string id, const std::string& json){
    if(id.size() != 24){
        CROW_LOG_ERROR << "Warning, MongoDB used ObjectID string only 24 char lenth string! Check [service_mongo_create_document_oid]";
        return {};
    }
    auto bson = bsoncxx::from_json(json);
    auto doc = document{} << "_id" << bsoncxx::oid{bsoncxx::stdx::string_view(id.data())} << bsoncxx::builder::stream::concatenate(bson.view()) << finalize;
    auto result = client->database(db).collection(collection).insert_one(doc.view());
    if(result->result().inserted_count() >= 1){
        return std::string(result->inserted_id().get_oid().value.to_string());
    }
    return {};
}

const std::string mongo_create_document(std::string db, std::string collection, const std::string& json){
    auto bson = bsoncxx::from_json(json);
    auto result = client->database(db).collection(collection).insert_one(bson.view());
    if(result->result().inserted_count() >= 1){
        return std::string(result->inserted_id().get_oid().value.to_string());
    }
    return {};
}

bool mongo_update_document(std::string db, std::string collection, const std::string& id, const std::string& json){
    auto bson = bsoncxx::from_json(json);
    auto doc = make_document(kvp("$set", bson));
    auto result = client->database(db).collection(collection).update_one(mongo_create_filter_objectid(id).view(), doc.view());
    return result->result().modified_count() >= 1;
}

bool mongo_delete_document(std::string db, std::string collection, const std::string& id){
    auto result = client->database(db).collection(collection).delete_one(mongo_create_filter_objectid(id).view());
    return result->deleted_count() >= 1;
}

const std::string mongo_get_document(std::string db, std::string collection, const std::string& id){
    auto result = client->database(db).collection(collection).find_one(mongo_create_filter_objectid(id).view());
    if(result) return bsoncxx::to_json(result.value());
    else return {};
}

std::string mongo_get_document_key_str(std::string db, std::string collection, std::string key, std::string value){
    auto filter = document{} << key << value << finalize;
    auto result = client->database(db).collection(collection).find_one(filter.view());
    if(result) return bsoncxx::to_json(result.value());
    else return {};
}

std::string mongo_get_document_key_int(std::string db, std::string collection, std::string key, int value){
    auto filter = document{} << key << value << finalize;
    auto result = client->database(db).collection(collection).find_one(filter.view());
    if(result) return bsoncxx::to_json(result.value());
    else return {};
}

std::string mongo_get_document_key_float(std::string db, std::string collection, std::string key, float value){
    auto filter = document{} << key << value << finalize;
    auto result = client->database(db).collection(collection).find_one(filter.view());
    if(result) return bsoncxx::to_json(result.value());
    else return {};
}

std::string mongo_get_document_key_bool(std::string db, std::string collection, std::string key, bool value){
    auto filter = document{} << key << value << finalize;
    auto result = client->database(db).collection(collection).find_one(filter.view());
    if(result) return bsoncxx::to_json(result.value());
    else return {};
}

}