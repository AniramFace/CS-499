#pragma once

#include "Pet.h"

#include <string>
#include <map>
#include <unordered_map>

class MongoDBManager;

class BoardingManager
{
public:
	struct PetRecord
	{
		int PetId;
		int CheckInCount;

		PetRecord() :
			PetId(-1),
			CheckInCount(0)
		{

		}

		PetRecord(int id, int count)
		{
			PetId = id;
			CheckInCount = count;
		}
	};

	BoardingManager();

	bool InitData();

	bool SpaceAvailable(PetType type);
	bool Register(std::string name, int age, PetType type, int& petId);
	bool CheckIn(int petId, bool groom);
	bool CheckOut(int petId);

	void ShowRegistered();
	void ShowCheckIns();

private:
	bool IsRegistered(int petId);

private:
	static constexpr int MAX_DOG_SPACES = 10;
	static constexpr int MAX_CAT_SPACES = 5;
	static constexpr int MIN_STAYS_FOR_GROOMING = 2;

	int mUniqueId = 100;

	int mDogSpacesOccupied;
	int mCatSpacesOccupied;

	std::map<int, Pet> mPetRegistryById;
	std::unordered_map<int, bool> mPetCheckInById;
	std::unordered_map<int, PetRecord> mPetRecordById;

	MongoDBManager* mMongoDBManager;
};

