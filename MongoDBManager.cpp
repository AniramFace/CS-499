#include "MongoDBManager.h"

#include <iostream>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/exception/operation_exception.hpp>
#include <mongocxx/uri.hpp>

MongoDBManager::MongoDBManager()
{
	mInstance = std::make_unique<mongocxx::instance>();
}

bool MongoDBManager::Init(const char* uri, const char* database)
{
	mClient = std::make_unique<mongocxx::client>(mongocxx::uri{ uri });

	if (!mClient)
	{
		std::cout << "Mongo DB client failed to initialize!" << std::endl;
	}

	auto ping_cmd = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("ping", 1));
	std::cout << "Connecting to MongoDB..." << std::endl;

	try
	{
		mDatabase = std::make_unique<mongocxx::database>((*mClient)[database]);
		auto result = (*mDatabase).run_command(ping_cmd.view());

		std::cout << "Successfully connected!" << std::endl;
	}
	catch (const mongocxx::operation_exception& e)
	{
		std::cout << "Failed to connect!" << std::endl;
		return false;
	}

	return true;
}

mongocxx::collection MongoDBManager::GetCollection(const char* collection)
{
	return (*mDatabase).collection(collection);
}

bool MongoDBManager::InsertOne(const char* collection, bsoncxx::document::view document)
{
	auto result = GetCollection(collection).insert_one(document);

	if (result)
	{
		return true;
	}

	return false;
}

bool MongoDBManager::DeleteOne(const char* collection, bsoncxx::document::view filter)
{
	auto result = GetCollection(collection).delete_one(filter);

	if (result)
	{
		return true;
	}

	return false;
}

bool MongoDBManager::UpdateOne(const char* collection, bsoncxx::document::view filter, bsoncxx::document::view update)
{
	mongocxx::options::update options{};
	options.upsert(true); // insert/update

	auto result = GetCollection(collection).update_one(filter, update, options);

	if (result)
	{
		return true;
	}

	return false;
}
