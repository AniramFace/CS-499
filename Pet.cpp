#include "Pet.h"

std::string Pet::GetTypeDescription() const
{
	switch (mType)
	{
	case DOG:
		return "Dog";
	case CAT:
		return "Cat";
	default:
		// Do nothing
		break;
	}

	return std::string();
}

Pet::Pet(std::string name, int age, PetType type) :
	mName(name),
	mAge(age),
	mType(type)
{

}