#ifndef FUNCTION_H
#define FUNCTION_H

#define MAX_ANIMALS 100

enum AnimalType {
	COW,
	HORSE,
	SHEEP,
	PIG
};

enum Gender {
	MALE,
	FEMALE
};

enum BreedCow {
	HOLSTEIN,
	ANGUS,
	JERSEY,
	SIMMENTAL,
	HEREFORD
};

enum BreedHorse {
	THOROUGHBRED,
	QUARTER_HORSE,
	APPALOOSA,
	ARABIAN,
	FRIESIAN
};

enum BreedSheep {
	MERINO,
	DORSET,
	SUFFOLK,
	HAMPSHIRE,
	ROMNEY
};

enum BreedPig {
	BERKSHIRE,
	DUROC,
	YORKSHIRE,
	LANDRACE
};

struct Animal {
	int tagNumber;
	char name[50];
	int age;
	enum AnimalType type;
	double weight;
	enum Gender gender;
	int numOffsprings;
	union {
		enum BreedCow cowBreed;
		enum BreedHorse horseBreed;
		enum BreedSheep sheepBreed;
		enum BreedPig pigBreed;
	} breed;
};

struct Farm {
	struct Animal *animals;
	int animalCount;
	int capacity;
};

void initializeFarm(struct Farm *farm, int initialCapacity);
void cleanupFarm(struct Farm *farm);
void populateFarm(struct Farm *farm, int numAnimals);
void printFarm(const struct Farm *farm);
int validateIntegerInput(const char *prompt);

void initDatabase(void);
void addAnimalToDatabase(const struct Animal *animal);
void loadAnimalsFromDatabase(struct Farm *farm);

#endif

