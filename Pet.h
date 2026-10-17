#pragma once

#include <string>

enum PetType
{
	DOG = 1,
	CAT = 2
};

class Pet
{
public:
	std::string GetName() const { return mName; }
	int GetAge() const { return mAge; }
	PetType GetType() const { return mType; }
	std::string GetTypeDescription() const;

	void SetName(std::string name) { mName = name; }
	void SetAge(int age) { mAge = age; }
	void SetType(PetType type) { mType = type; }

	Pet() = default;
	Pet(std::string name, int age, PetType type);

private:
	std::string mName;
	int mAge;
	PetType mType;
};

