#include "BoardingManager.h"

#include <iostream>

#include "MongoDBManager.h"
#include "BoardingDataUtil.h"

BoardingManager::BoardingManager() :
	mDogSpacesOccupied(0),
	mCatSpacesOccupied(0),
	mMongoDBManager(nullptr)
{
}

bool BoardingManager::InitData()
{
	if (mMongoDBManager == nullptr)
	{
		mMongoDBManager = new MongoDBManager();
		mMongoDBManager->Init("mongodb://localhost:27017", "pet_grooming_db");
	}

	mPetRegistryById = GetPetRegistryDataFromMongoDBCollection(mMongoDBManager->GetCollection("pet_registry"));
	mPetCheckInById = GetPetCheckedInDataFromMongoDBCollection(mMongoDBManager->GetCollection("pet_checkin"));
	mPetRecordById = GetPetRecordDataFromMongoDBCollection(mMongoDBManager->GetCollection("pet_record"));

	if (!mPetRegistryById.empty())
	{
		int highestPetId = mPetRegistryById.rbegin()->first;
		mUniqueId = highestPetId; // pre-incremented when registering
	}

	return true;
}

bool BoardingManager::SpaceAvailable(PetType type)
{
	switch (type)
	{
	case DOG:
		return mDogSpacesOccupied < MAX_DOG_SPACES;
	case CAT:
		return mCatSpacesOccupied < MAX_CAT_SPACES;
	default:
		std::cout << "Pet type is not supported!";
	}

	return false;
}

bool BoardingManager::Register(std::string name, int age, PetType type, int& petId)
{
	petId = ++mUniqueId;

	Pet pet = { name, age, type };
	PetRecord record = { petId, 0 };

	mPetRegistryById[petId] = pet;
	mPetCheckInById[petId] = false;
	mPetRecordById[petId] = record;

	InsertPetRegistryDataToMongoDB(mMongoDBManager, petId, pet);

	return true;
}

bool BoardingManager::CheckIn(int petId, bool groom)
{
	if (!IsRegistered(petId))
	{
		std::cout << "Pet is not registered with id: " << petId << '\n';
		return false;
	}

	if (!SpaceAvailable(mPetRegistryById[petId].GetType()))
	{
		std::cout << "No space available for pet type: " << mPetRegistryById[petId].GetTypeDescription() << '\n';
		return false;
	}

	if (mPetCheckInById[petId])
	{
		std::cout << "Pet is already checked in with id: " << petId << '\n';
		return false;
	}

	if (groom)
	{
		if (mPetRegistryById[petId].GetType() != PetType::DOG)
		{
			std::cout << "Grooming is only available for dogs.\n";
			return false;
		}

		if (mPetRecordById[petId].CheckInCount < MIN_STAYS_FOR_GROOMING)
		{
			std::cout << "More stays are required before grooming is available.\n";
			return false;
		}
	}

	mPetCheckInById[petId] = true;

	auto it = mPetRecordById.find(petId);
	if (it == mPetRecordById.end())
	{
		// First check in! Init record
		mPetRecordById[petId] = { petId, 1 };
	}
	else
	{
		mPetRecordById[petId].CheckInCount++;
	}

	InsertPetCheckedInDataToMongoDB(mMongoDBManager, petId, true);
	UpdatePetRecordDataInMongoDB(mMongoDBManager, mPetRecordById[petId]);

	switch (mPetRegistryById[petId].GetType())
	{
	case DOG:
		mDogSpacesOccupied++;
		break;
	case CAT:
		mCatSpacesOccupied++;
		break;
	default:
		// Do nothing
		break;
	}

	return true;
}

bool BoardingManager::CheckOut(int petId)
{
	if (!IsRegistered(petId))
	{
		std::cout << "No registered pet was found with id: " << petId << '\n';
		return false;
	}

	if (!mPetCheckInById[petId])
	{
		std::cout << "Pet is not currently checked in with id: " << petId << '\n';
		return false;
	}

	mPetCheckInById[petId] = false;

	DeletePetCheckedInDataToMongoDB(mMongoDBManager, petId);

	switch (mPetRegistryById[petId].GetType())
	{
	case DOG:
		mDogSpacesOccupied--;
		break;
	case CAT:
		mCatSpacesOccupied--;
		break;
	default:
		// Do nothing
		break;
	}

	return true;
}

void BoardingManager::ShowRegistered()
{
	bool empty = true;

	std::cout << "\n--- Registered pets ---\n";
	for (const auto& pair : mPetRegistryById) {
		int petId = pair.first;

		const Pet& pet = mPetRegistryById.at(pair.first);
		std::cout << "- [" << petId << "] " << "Name: " << pet.GetName();
		std::cout << ", Age: " << pet.GetAge();
		std::cout << ", Type: " << pet.GetTypeDescription();
		std::cout << " (Stays: " << mPetRecordById[petId].CheckInCount << ")\n";
		empty = false;
	}

	if (empty) std::cout << "No pets are currently registered.\n";
}

void BoardingManager::ShowCheckIns()
{
	bool empty = true;

	std::cout << "\n--- Pets checked in ---\n";
	for (const auto& pair : mPetCheckInById) {
		int petId = pair.first;

		if (pair.second) { // If true, pet is checked in
			const Pet& pet = mPetRegistryById.at(pair.first);
			std::cout << "- [" << petId << "] " << pet.GetName() << " (" << pet.GetTypeDescription() << ")\n";
			empty = false;
		}
	}

	if (empty) std::cout << "No pets are currently checked in.\n";
}

bool BoardingManager::IsRegistered(int petId)
{
	return mPetRegistryById.find(petId) != mPetRegistryById.end();
}
