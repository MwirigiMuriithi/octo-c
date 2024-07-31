#include <stdio.h>
#include "function.h"

void displayMenu() {
    printf("1. Add Animals\n");
    printf("2. View Farm Inventory\n");
    printf("3. Exit\n");
}

int main() {
    struct Farm farm;
    initializeFarm(&farm, MAX_ANIMALS); // Initial capacity

    int choice;
    while (1) {
        displayMenu();
        choice = validateIntegerInput("Enter your choice: ");
        switch (choice) {
            case 1:
                populateFarm(&farm, validateIntegerInput("Enter the number of animals: "));
                break;
            case 2:
                printFarm(&farm);
                break;
            case 3:
                cleanupFarm(&farm);
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}
