#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "function.h"

void initializeFarm(struct Farm *farm, int initialCapacity) {
    farm->animals = malloc(initialCapacity * sizeof(struct Animal));
    farm->animalCount = 0;
    farm->capacity = initialCapacity;
}

void cleanupFarm(struct Farm *farm) {
    free(farm->animals);
}

int validateIntegerInput(const char *prompt) {
    int value;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &value) == 1 && value >= 0) {
            // Clear input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            return value;
        } else {
            printf("Invalid input. Please enter a non-negative integer.\n");
            // Clear input buffer
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }
}

void readAnimalDetails(struct Animal *animal) {
    animal->tagNumber = validateIntegerInput("Tag Number: ");
    printf("Name: ");
    scanf("%49s", animal->name);
    animal->age = validateIntegerInput("Age: ");
    animal->type = (enum AnimalType)validateIntegerInput("Type (0: Cow, 1: Horse, 2: Sheep, 3: Pig): ");
    animal->weight = validateIntegerInput("Weight: ");
    animal->gender = (enum Gender)validateIntegerInput("Gender (0: Male, 1: Female): ");
    animal->numOffsprings = validateIntegerInput("Number of Offsprings: ");

    switch (animal->type) {
        case COW:
            animal->breed.cowBreed = (enum BreedCow)validateIntegerInput("Breed (0: Holstein, 1: Angus, 2: Jersey, 3: Simmental, 4: Hereford): ");
            break;
        case HORSE:
            animal->breed.horseBreed = (enum BreedHorse)validateIntegerInput("Breed (0: Thoroughbred, 1: Quarter Horse, 2: Appaloosa, 3: Arabian, 4: Friesian): ");
            break;
        case SHEEP:
            animal->breed.sheepBreed = (enum BreedSheep)validateIntegerInput("Breed (0: Merino, 1: Dorset, 2: Suffolk, 3: Hampshire, 4: Romney): ");
            break;
        case PIG:
            animal->breed.pigBreed = (enum BreedPig)validateIntegerInput("Breed (0: Berkshire, 1: Duroc, 2: Yorkshire, 3: Landrace): ");
            break;
        default:
            break;
    }
}

const char* animalTypeToString(enum AnimalType type) {
    switch (type) {
        case COW: return "Cow";
        case HORSE: return "Horse";
        case SHEEP: return "Sheep";
        case PIG: return "Pig";
        default: return "Unknown";
    }
}

const char* genderToString(enum Gender gender) {
    switch (gender) {
        case MALE: return "Male";
        case FEMALE: return "Female";
        default: return "Unknown";
    }
}

const char* breedToString(const struct Animal *animal) {
    switch (animal->type) {
        case COW:
            switch (animal->breed.cowBreed) {
                case HOLSTEIN: return "Holstein";
                case ANGUS: return "Angus";
                case JERSEY: return "Jersey";
                case SIMMENTAL: return "Simmental";
                case HEREFORD: return "Hereford";
                default: return "Unknown";
            }
        case HORSE:
            switch (animal->breed.horseBreed) {
                case THOROUGHBRED: return "Thoroughbred";
                case QUARTER_HORSE: return "Quarter Horse";
                case APPALOOSA: return "Appaloosa";
                case ARABIAN: return "Arabian";
                case FRIESIAN: return "Friesian";
                default: return "Unknown";
            }
        case SHEEP:
            switch (animal->breed.sheepBreed) {
                case MERINO: return "Merino";
                case DORSET: return "Dorset";
                case SUFFOLK: return "Suffolk";
                case HAMPSHIRE: return "Hampshire";
                case ROMNEY: return "Romney";
                default: return "Unknown";
            }
        case PIG:
            switch (animal->breed.pigBreed) {
                case BERKSHIRE: return "Berkshire";
                case DUROC: return "Duroc";
                case YORKSHIRE: return "Yorkshire";
                case LANDRACE: return "Landrace";
                default: return "Unknown";
            }
        default:
            return "Unknown";
    }
}

void printFarm(const struct Farm *farm) {
    printf("+------------+------------------+-----+----------+--------+--------+----------+------------------+\n");
    printf("| Tag Number | Name             | Age | Type     | Weight | Gender | Offsprings | Breed            |\n");
    printf("+------------+------------------+-----+----------+--------+--------+----------+------------------+\n");

    for (int i = 0; i < farm->animalCount; ++i) {
        const char *typeStr = animalTypeToString(farm->animals[i].type);
        const char *genderStr = genderToString(farm->animals[i].gender);
        const char *breedStr = breedToString(&farm->animals[i]);

        printf("| %10d | %-16s | %3d | %-8s | %6.2f | %-6s | %10d | %-16s |\n",
               farm->animals[i].tagNumber,
               farm->animals[i].name,
               farm->animals[i].age,
               typeStr,
               farm->animals[i].weight,
               genderStr,
               farm->animals[i].numOffsprings,
               breedStr);
    }

    printf("+------------+------------------+-----+----------+--------+--------+----------+------------------+\n");
}

void populateFarm(struct Farm *farm, int numAnimals) {
    if (numAnimals > farm->capacity) {
        printf("Error: Number of animals exceeds farm capacity.\n");
        return;
    }

    for (int i = 0; i < numAnimals; ++i) {
        if (farm->animalCount >= farm->capacity) {
            printf("Farm is at full capacity. Cannot add more animals.\n");
            break;
        }

        printf("Enter details for animal %d:\n", i + 1);
        readAnimalDetails(&farm->animals[farm->animalCount]);
        farm->animalCount++;
    }
}
