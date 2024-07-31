#ifndef DB_H
#define DB_H

#include "function.h"

void initDatabase(void);
void addAnimalToDatabase(const struct Animal *animal);
void loadAnimalsFromDatabase(struct Farm *farm);

#endif
