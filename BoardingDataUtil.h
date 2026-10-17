#pragma once

#include "BoardingManager.h"

#include <string>
#include <map>
#include <unordered_map>

#include <bsoncxx/builder/basic/document.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/builder/stream/document.hpp>
#include <mongocxx/collection.hpp>

std::map<int, Pet> GetPetRegistryDataFromMongoDBCollection(mongocxx::collection collection)
{
	std::map<int, Pet> data;

	auto cursor = collection.find({});

	for (const auto& doc : cursor)
	{
		if (doc["petId"] && doc["name"] && doc["age"] && doc["type"])
		{
			int petId = doc["petId"].get_int32().value;
			std::string name = std::string(doc["name"].get_string().value);
			int age = doc["age"].get_int32().value;
			int type = doc["type"].get_int32().value;

			data[petId] = { name, age, PetType(type) };
		}
	}

	return data;
}

bool InsertPetRegistryDataToMongoDB(MongoDBManager* manager, int petId, Pet pet)
{
	bsoncxx::builder::stream::document document_builder{};
	bsoncxx::document::value doc = document_builder
		<< "petId" << petId
		<< "name" << pet.GetName()
		<< "age" << pet.GetAge()
		<< "type" << pet.GetType()
		<< bsoncxx::builder::stream::finalize;

	return manager->InsertOne("pet_registry", doc.view());
}

std::unordered_map<int, bool> GetPetCheckedInDataFromMongoDBCollection(mongocxx::collection collection)
{
	std::unordered_map<int, bool> data;

	auto cursor = collection.find({});

	for (const auto& doc : cursor)
	{
		if (doc["petId"] && doc["checkedIn"])
		{
			int petId = doc["petId"].get_int32().value;
			bool checkedIn = doc["checkedIn"].get_bool().value;

			data[petId] = checkedIn;
		}
	}

	return data;
}

bool InsertPetCheckedInDataToMongoDB(MongoDBManager* manager, int petId, bool checkedIn)
{
	bsoncxx::builder::stream::document document_builder{};
	bsoncxx::document::value doc = document_builder
		<< "petId" << petId
		<< "checkedIn" << checkedIn
		<< bsoncxx::builder::stream::finalize;

	return manager->InsertOne("pet_checkin", doc.view());
}

bool DeletePetCheckedInDataToMongoDB(MongoDBManager* manager, int petId)
{
	auto filter = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("petId", petId));

	return manager->DeleteOne("pet_checkin", filter.view());
}

std::unordered_map<int, BoardingManager::PetRecord> GetPetRecordDataFromMongoDBCollection(mongocxx::collection collection)
{
	std::unordered_map<int, BoardingManager::PetRecord> data;

	auto cursor = collection.find({});

	for (const auto& doc : cursor)
	{
		if (doc["petId"] && doc["checkInCount"])
		{
			int petId = doc["petId"].get_int32().value;
			int checkInCount = doc["checkInCount"].get_int32().value;

			data[petId] = { petId, checkInCount };
		}
	}

	return data;
}

bool UpdatePetRecordDataInMongoDB(MongoDBManager* manager, BoardingManager::PetRecord record)
{
	auto filter = bsoncxx::builder::basic::make_document(bsoncxx::builder::basic::kvp("petId", record.PetId));

	bsoncxx::document::value update = bsoncxx::builder::stream::document{}
		<< "$set" << bsoncxx::builder::stream::open_document
		<< "petId" << record.PetId
		<< "checkInCount" << record.CheckInCount
		<< bsoncxx::builder::stream::close_document
		<< bsoncxx::builder::stream::finalize;

	return manager->UpdateOne("pet_record", filter.view(), update.view());
}
