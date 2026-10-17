#pragma once

#include <memory>

#include <mongocxx/client.hpp>
#include <mongocxx/database.hpp>
#include <mongocxx/instance.hpp>

class MongoDBManager
{
public:
	MongoDBManager();

	bool Init(const char* uri, const char* database);
	
	mongocxx::collection GetCollection(const char* collection);

	bool InsertOne(const char* collection, bsoncxx::document::view document);
	bool DeleteOne(const char* collection, bsoncxx::document::view filter);
	bool UpdateOne(const char* collection, bsoncxx::document::view filter, bsoncxx::document::view update);

private:
	std::unique_ptr<mongocxx::instance> mInstance;
	std::unique_ptr<mongocxx::client> mClient;
	std::unique_ptr<mongocxx::database> mDatabase;
};
