/**
 *
 * @author Arwain Giannini-Karlin
 * @date 2/9/2017
 *
 * The following program reads a data file ("hw4input") of customer orders and organizes
 * the data. This program produces two files, one file of orders by time and
 * the other a file of orders by value.
 *
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define IS_DIGIT(c)    ((c) >= 48 && (c) <= 57)
#define MAX_CUSTOMERS 20
#define MAX_LINE_LENGTH 40
#define MAX_ITEMS 20
#define CONSTANT_MULTIPLIER 10

// Struct for item.
struct item {
    char name[MAX_LINE_LENGTH];
    int numberOrdered;
    double cost;
};
// Struct for customer.
struct customer {
    char name[MAX_LINE_LENGTH];
    struct item items[MAX_ITEMS];
    double totalCost;
};

/*
 * The following function takes in a char array and converts it into
 * an integer. The function returns an integer.
 */
int intReader(char in[MAX_LINE_LENGTH], int *indexIn) {
    int number = 0;
    int digit = 0;
    char input;
    input = in[*indexIn];
    // Following algorithm loops through digits to create an integer.
    do {
        digit = input - '0';
        if (IS_DIGIT(input)) {
            number = number * CONSTANT_MULTIPLIER + digit;
        } else {
            number = digit;
        }
        (*indexIn)++;
        input = in[*indexIn];
    } while (IS_DIGIT(input));
    return number;
}

/*
 * The following function reads data from the file and stores it within the customer
 * and item structs.
 */
void readFile(FILE *fileIn, struct customer customerIn[MAX_CUSTOMERS]) {

    char tempLine[MAX_LINE_LENGTH];
    int index, tempNumberOrdered, addCount, loopCount;
    char c;
    bool foundName, firstRun;
    foundName = false;
    firstRun = false;
    addCount = 0;
    loopCount = 0;
    if (fileIn != NULL) {

        /*
         * The following line pulls the space at the begining of the file.
         * WARNING: If there is not a space before the data this line will
         * cause the algorithm to fail to grab the first order. If there is not
         * a space in the file before the data please comment out this line.
         */
        fgets(tempLine, MAX_LINE_LENGTH, fileIn);

        /*
         * The following block loops through each line of the data and
         * organizes the data within the structs.
         */
        while ((c = (char) fgetc(fileIn)) != EOF) {
            index = 0;
            tempNumberOrdered = 0;
            char tempName[MAX_LINE_LENGTH];
            char tempItem[MAX_LINE_LENGTH];
            char tempCost[MAX_LINE_LENGTH];
            strcpy(tempName, "");
            strcpy(tempItem, "");
            strcpy(tempCost, "");
            strncat(tempName, &c, 1);

            // Grabs a line from the file.
            fgets(tempLine, MAX_LINE_LENGTH, fileIn);

            while (c != ' ') {
                c = tempLine[index++];
                strncat(tempName, &c, 1);
            }

            // Set char to end of line.
            c = '\0';

            // Set customer's item tempNumberOrdered.
            tempNumberOrdered = intReader(tempLine, &index);

            // The following loop grabs the item name.
            index++;
            while (c != ' ') {
                c = tempLine[index++];
                strncat(tempItem, &c, 1);
            }

            // Set char to end of line.
            c = '\0';

            // The following loop grabs the item price.
            index++;
            while (c != ' ') {
                c = tempLine[index++];
                strncat(tempCost, &c, 1);
            }

            // Set char to end of line.
            c = '\0';

            if (addCount == 0) {
                strcpy(customerIn[addCount].name, tempName);
                strcpy(customerIn[addCount].items[addCount].name, tempItem);
                customerIn[addCount].items[addCount].numberOrdered = tempNumberOrdered;
                customerIn[addCount].items[addCount].cost = atof(tempCost);
                customerIn[addCount].totalCost = (customerIn[addCount].items[addCount].cost) *
                                                 (customerIn[addCount].items[addCount].numberOrdered);
                strcpy(tempName, "");
                firstRun = true;
                addCount++;
            }
            for (index = 0; index < MAX_CUSTOMERS; index++) {
                if (strcmp(customerIn[index].name, tempName) == 0 && !firstRun) {
                    strcpy(customerIn[index].items[loopCount].name, tempItem);
                    customerIn[index].items[loopCount].numberOrdered = tempNumberOrdered;
                    customerIn[index].items[loopCount].cost = atof(tempCost);
                    customerIn[index].totalCost += (customerIn[index].items[loopCount].cost) *
                                                   (customerIn[index].items[loopCount].numberOrdered);
                    foundName = true;
                    break;
                } else {
                    foundName = false;
                }
            }
            if (foundName == false && !firstRun) {
                strcpy(customerIn[addCount].name, tempName);
                strcpy(customerIn[addCount].items[0].name, tempItem);
                customerIn[addCount].items[0].numberOrdered = tempNumberOrdered;
                customerIn[addCount].items[0].cost = atof(tempCost);
                customerIn[addCount].totalCost += (customerIn[addCount].items[0].cost) *
                                                  (customerIn[addCount].items[0].numberOrdered);
                addCount++;

            }
            firstRun = false;
            loopCount++;

        }
    }


}

/*
 * The following function creates a formatted file of customer orders by time.
 */
void writeTimeFile(struct customer customer1[MAX_CUSTOMERS]) {
    int customerCount, index;
    customerCount = 0;
    index = 0;
    FILE *fileOut = fopen("time.txt", "wb");
    char name[MAX_LINE_LENGTH];
    strcpy(name, customer1[0].name);
    while (strcmp(name, "") != 0) {
        if (strcmp(name, "") != 0) {
            fprintf(fileOut, "%s\n", name);
            for (index = 0; index < MAX_ITEMS; index++) {
                if (strcmp(customer1[customerCount].items[index].name, "") != 0) {
                    fprintf(fileOut, "%s %d $%.2f\n", customer1[customerCount].items[index].name,
                            customer1[customerCount].items[index].numberOrdered,
                            customer1[customerCount].items[index].cost);
                }
            }
        }
        fprintf(fileOut, "\n");
        customerCount++;
        strcpy(name, customer1[customerCount].name);
    }

}

/*
 * The following function utilizes a bubble sort algorithm to sort the data by total
 * cost per customer.
 */
void sortOrdersByAmounts(struct customer customer1[MAX_CUSTOMERS]) {
    int customerCount, index, itemIndex;
    struct customer tempCustomer;
    struct item tempItem;
    for (customerCount = 0; customerCount < MAX_CUSTOMERS; customerCount++) {
        for (index = 0; index < MAX_ITEMS; index++) {
            if (customer1[customerCount].totalCost > customer1[index].totalCost) {

                // Below swaps the customers.
                tempCustomer = customer1[customerCount];
                customer1[customerCount] = customer1[index];
                customer1[index] = tempCustomer;
            }
        }
    }
}

/*
 * The following function utilizes a bubble sort algorithm to sort
 * the data by cost per item order.
 */
void sortItemsByValue(struct customer customer1[MAX_CUSTOMERS]) {
    int customerCount, index, itemIndex;
    struct customer tempCustomer;
    struct item tempItem;
    for (customerCount = 0; customerCount < MAX_CUSTOMERS; customerCount++) {
        for (index = 0; index < MAX_ITEMS; index++) {
            for (itemIndex = 0; itemIndex < MAX_ITEMS; itemIndex++) {

                // Conditional below compares individual item orders.
                if ((customer1[customerCount].items[index].cost) *
                    (customer1[customerCount].items[index].numberOrdered) >
                    (customer1[customerCount].items[itemIndex].cost) *
                    (customer1[customerCount].items[itemIndex].numberOrdered)) {

                    // Below swaps the items.
                    tempItem = customer1[customerCount].items[index];
                    customer1[customerCount].items[index] = customer1[customerCount].items[itemIndex];
                    customer1[customerCount].items[itemIndex] = tempItem;
                }
            }
        }
    }
}

/*
 * The following function creates a formatted file of customer orders by value.
 */
void writeMoneyFile(struct customer customer1[MAX_CUSTOMERS]) {

    // First sorting algorithms are called.
    sortOrdersByAmounts(customer1);
    sortItemsByValue(customer1);

    // File is opened and formatted.
    FILE *fileOut = fopen("money.txt", "wb");
    int customerCount, index;
    customerCount = 0;
    char name[MAX_LINE_LENGTH];
    strcpy(name, customer1[0].name);
    while (strcmp(name, "") != 0) {
        if (strcmp(name, "") != 0) {
            // This breaks before garbage data is reached.
            if(strcmp(name,"C") == 0){
                break;
            }
            fprintf(fileOut, "%s, Total Order = $%.2f\n", name, customer1[customerCount].totalCost);
            for (index = 0; index < MAX_ITEMS; index++) {
                if (strcmp(customer1[customerCount].items[index].name, "") != 0) {
                    fprintf(fileOut, "%s %d $%.2f, Item Value = $%.2f\n", customer1[customerCount].items[index].name,
                            customer1[customerCount].items[index].numberOrdered,
                            customer1[customerCount].items[index].cost, (customer1[customerCount].items[index].cost) *
                                                                        (customer1[customerCount].items[index].numberOrdered));
                }
            }
        }
        fprintf(fileOut, "\n");
        customerCount++;
        strcpy(name, customer1[customerCount].name);
    }


}

int main(void) {
    struct customer customers[MAX_CUSTOMERS];
    // Open the file.
    FILE *file = fopen("input.txt", "rb");
    if (!file) {
        printf("Cannot open file.\n");
        return 1;
    }
    // Pass the file and array to readFile.
    readFile(file, customers);

    // Pass the file and array to writeTimeFile.
    writeTimeFile(customers);

    // Pass the file and array to writeMoneyFile.
    writeMoneyFile(customers);
    return 0;
}
