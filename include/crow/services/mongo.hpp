#ifndef CROW_SERVICE_MONGO_HPP
#define CROW_SERVICE_MONGO_HPP

#include<string>

namespace service {

bool mongo_start(std::string connection_uri);
void mongo_stop();
const std::string mongo_create_document_oid(std::string db, std::string collection, std::string id, const std::string& json);
const std::string mongo_create_document(std::string db, std::string collection, const std::string& json);
bool mongo_update_document(std::string db, std::string collection, const std::string& id, const std::string& json);
bool mongo_delete_document(std::string db, std::string collection, const std::string& id);
const std::string mongo_get_document(std::string db, std::string collection, const std::string& id);
std::string mongo_get_document_key_str(std::string db, std::string collection, std::string key, std::string value);
std::string mongo_get_document_key_int(std::string db, std::string collection, std::string key, int value);
std::string mongo_get_document_key_float(std::string db, std::string collection, std::string key, float value);
std::string mongo_get_document_key_bool(std::string db, std::string collection, std::string key, bool value);

}

#endif