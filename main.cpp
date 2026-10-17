#include <iostream>

#include "BoardingManager.h"

void clearScreen();

void displayMenu();
bool registerPet(BoardingManager* manager);
bool checkInPet(BoardingManager* manager);
bool checkOutPet(BoardingManager* manager);

int main()
{
    BoardingManager* boardingManager = new BoardingManager();
    boardingManager->InitData();

    int petId = 0;

    /* Moved into mongodb
    boardingManager->Register("Fluffy", 3, PetType::DOG, petId);

    boardingManager->Register("Pebbles", 5, PetType::CAT, petId);
    boardingManager->CheckIn(petId, false);

    boardingManager->Register("Luna", 1, PetType::CAT, petId);

    boardingManager->Register("Spot", 4, PetType::DOG, petId);
    boardingManager->CheckIn(petId, false);
    */

    std::cout << "Welcome!\n";

    int choice = 0;
    while (choice != 5)
    {
        displayMenu();

        if (!(std::cin >> choice))
        {
            std::cout << "Please enter a number.\n";
            std::cin.clear();
            continue;
        }

        std::cin.ignore();

        switch (choice)
        {
        case 1:
            boardingManager->ShowRegistered();
            boardingManager->ShowCheckIns();
            break;
        case 2:
            if (registerPet(boardingManager))
            {
                std::cout << "\nSuccessfully registered pet.\n";
            }
            break;
        case 3:
            if (checkInPet(boardingManager))
            {
                std::cout << "\nSuccessfully checked in pet.\n";
            }
            break;
        case 4:
            if (checkOutPet(boardingManager))
            {
                std::cout << "\nSuccessfully checked out pet.\n";
            }
            break;
        case 5:
            std::cout << "Goodbye!\n";
            break;
        default:
            std::cout << "Please enter a valid choice.\n";
        }
    }

    delete boardingManager;
    boardingManager = nullptr;
}

void clearScreen()
{
    // Magic schtuff
    std::cout << "\033[2J\033[1;1H";
}

void displayMenu()
{
    std::cout << "\n-- Menu --\n";
    std::cout << "1. Show registered pets and check ins\n";
    std::cout << "2. Register\n";
    std::cout << "3. Check in\n";
    std::cout << "4. Check out\n";
    std::cout << "5. Exit\n";

    std::cout << "\nEnter your choice: ";
}

bool registerPet(BoardingManager* manager)
{
    std::string name;
    std::cout << "\nEnter pet's name: ";
    std::getline(std::cin, name);

    int age;
    std::cout << "\nEnter pet's age: ";
    while (!(std::cin >> age) || age < 0)
    {
        std::cout << "Please enter a valid number: ";
        std::cin.clear();
    }

    int type;
    std::cout << "\n-- Pet types --\n";
    std::cout << "1. Dog\n";
    std::cout << "2. Cat\n";
    std::cout << "\nEnter pet type: ";
    while (!(std::cin >> type) || type < 0 || type > 2)
    {
        std::cout << "Please enter a valid type: ";
        std::cin.clear();
    }

    int id;
    return manager->Register(name, age, PetType(type), id);
}

bool checkInPet(BoardingManager* manager)
{
    int type;
    std::cout << "\n-- Pet types --\n";
    std::cout << "1. Dog\n";
    std::cout << "2. Cat\n";
    std::cout << "\Enter pet type: ";
    while (!(std::cin >> type) || type < 0 || type > 2)
    {
        std::cout << "Please enter a valid type: ";
        std::cin.clear();
    }

    if (!manager->SpaceAvailable(PetType(type)))
    {
        std::cout << "No space available for pet type.\n";
        return false;
    }

    int id;
    std::cout << "\nEnter pet's id: ";
    while (!(std::cin >> id) || id < 0)
    {
        std::cout << "Please enter a valid number: ";
        std::cin.clear();
    }

    return manager->CheckIn(id, false);
}

bool checkOutPet(BoardingManager* manager)
{
    int id;
    std::cout << "\nEnter pet's id: ";
    while (!(std::cin >> id) || id < 0)
    {
        std::cout << "Please enter a valid number: ";
        std::cin.clear();
    }

    return manager->CheckOut(id);
}
