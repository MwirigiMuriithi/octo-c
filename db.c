#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include "db.h"

#define DB_PATH "farm.db"

static int callback(void *data, int argc, char **argv, char **azColName) {
	return 0;
}

void initDatabase(void) {
	sqlite3 *db;
	char *errMsg = 0;
	int rc;

	rc = sqlite3_open(DB_PATH, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	const char *createTableSQL =
		"CREATE TABLE IF NOT EXISTS Animals (" \
		"TagNumber INTEGER PRIMARY KEY," \
		"Name TEXT NOT NULL," \
		"Age INTEGER NOT NULL," \
		"Type INTEGER NOT NULL," \
		"Weight REAL NOT NULL," \
		"Gender INTEGER NOT NULL," \
		"NumOffsprings INTEGER NOT NULL," \
		"Breed INTEGER NOT NULL);";

	rc = sqlite3_exec(db, createTableSQL, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}

	sqlite3_close(db);
}

void addAnimalToDatabase(const struct Animal *animal) {
	sqlite3 *db;
	char *errMsg = 0;
	int rc;
	char sql[256];

	rc = sqlite3_open(DB_PATH, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	snprintf(sql, sizeof(sql),
			 "INSERT INTO Animals (TagNumber, Name, Age, Type, Weight, Gender, NumOffsprings, Breed) "
			 "VALUES (%d, '%s', %d, %d, %f, %d, %d, %d);",
			 animal->tagNumber,
			 animal->name,
			 animal->age,
			 animal->type,
			 animal->weight,
			 animal->gender,
			 animal->numOffsprings,
			 animal->type == COW ? animal->breed.cowBreed :
			 animal->type == HORSE ? animal->breed.horseBreed :
			 animal->type == SHEEP ? animal->breed.sheepBreed :
			 animal->breed.pigBreed);

	rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", errMsg);
		sqlite3_free(errMsg);
	}

	sqlite3_close(db);
}

void loadAnimalsFromDatabase(struct Farm *farm) {
	sqlite3 *db;
	char *errMsg = 0;
	int rc;
	const char *selectSQL = "SELECT * FROM Animals;";
	sqlite3_stmt *stmt;

	rc = sqlite3_open(DB_PATH, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}

	rc = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, 0);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db));
		sqlite3_close(db);
		return;
	}

	int index = 0;
	while (sqlite3_step(stmt) == SQLITE_ROW) {
		if (index >= farm->capacity) break;

		struct Animal *animal = &farm->animals[index++];
		animal->tagNumber = sqlite3_column_int(stmt, 0);
		snprintf(animal->name, sizeof(animal->name), "%s", (const char*)sqlite3_column_text(stmt, 1));
		animal->age = sqlite3_column_int(stmt, 2);
		animal->type = sqlite3_column_int(stmt, 3);
		animal->weight = sqlite3_column_double(stmt, 4);
		animal->gender = sqlite3_column_int(stmt, 5);
		animal->numOffsprings = sqlite3_column_int(stmt, 6);
		animal->breed.cowBreed = sqlite3_column_int(stmt, 7);

		farm->animalCount++;
	}

	sqlite3_finalize(stmt);
	sqlite3_close(db);
}
