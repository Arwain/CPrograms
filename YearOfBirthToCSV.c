/*
 * The following program creates a csv file from Social Security year of birth lists.
 * This program is currently formatted to create list of the 100 most popular names
 * of the given year, however, this program could be modified to accomodate other outputs.
 *
 * @author - Arwain Giannini-Karlin
 * @last modification - 2/1/2017
 */


#include <stdio.h>
#include <string.h>

#define MAX_NAMES 1000
#define  MAX_RANKS 1000
#define  MAX_STR_LENGTH 30
#define MAX_YOBLISTS 10
#define NAME_CATCH 366
typedef char *string;

/*
 * The following function reads in a yob file and sorts the data by grabbing the pointer to the
 * comma following the name given. A rank is then stored for the name based on the names position
 * in the list.
 */
void readFile(FILE *inFile, char namesIn[MAX_NAMES][MAX_STR_LENGTH], int ranksIn[MAX_RANKS][MAX_YOBLISTS], int yearIn,
              int dataMultiplierIn) {
    int babyCount = 1;
    int j;
    for (j = 0; j < 100; j++) {
        fgets(namesIn[j + dataMultiplierIn], MAX_STR_LENGTH, inFile);
        *strstr(namesIn[j + dataMultiplierIn], ",") = '\0';
        if (dataMultiplierIn + j == 999) {
            ranksIn[j + dataMultiplierIn][yearIn] = babyCount;
        } else {
            ranksIn[j + dataMultiplierIn][yearIn] = babyCount++;
        }
    }

}

/*
 * The following function utilizes bubblesort to alphabetize the data and organize the
 * ranks.
 */
void sortData(char namesIn[MAX_NAMES][MAX_STR_LENGTH], int ranksIn[MAX_RANKS][MAX_YOBLISTS]) {

    int i, j, k;
    for (j = 0; j < 999; j++) {
        for (i = 0; i < 999; i++) {
            // Below utilizes bubblesort to alphabetizes the data.
            if (strcmp(namesIn[i], namesIn[i + 1]) > 0) {
                char temp[MAX_STR_LENGTH];
                int tempRank[MAX_YOBLISTS];
                // Swap ranks.
                memcpy(tempRank, ranksIn[i], sizeof(ranksIn[0]));
                memcpy(ranksIn[i], ranksIn[i + 1], sizeof(ranksIn[0]));
                memcpy(ranksIn[i + 1], tempRank, sizeof(ranksIn[0]));
                // Swap names.
                strcpy(temp, namesIn[i]);
                strcpy(namesIn[i], namesIn[i + 1]);
                strcpy(namesIn[i + 1], temp);
            }
        }
    }
    for (j = 0; j < 1000; j++) {
        for (i = 0; i < 999; i++) {
            // Below utilizes nested loop to condense the data.
            if (strcmp(namesIn[i], namesIn[i + 1]) == 0) {
                int tempRank2[MAX_YOBLISTS];
                memset(tempRank2, 0, sizeof(tempRank2));
                // Combine ranks.
                for (k = 0; k < 10; k++) {
                    if (ranksIn[i][k] != 0) {
                        tempRank2[k] = ranksIn[i][k];
                        ranksIn[i][k] = 0;
                    } else if (ranksIn[i + 1][k] != 0) {
                        tempRank2[k] = ranksIn[i + 1][k];
                        ranksIn[i + 1][k] = 0;
                    }
                }
                memcpy(ranksIn[i], tempRank2, sizeof(ranksIn[0]));
                // Swap names.
                strcpy(namesIn[i], namesIn[i + 1]);
                strcpy(namesIn[i + 1], namesIn[i + 2]);
            }
        }
    }

}

/*
 * The following function formats the data to a csv file. It catches zeros in the
 * ranks and replaces them with commas for empty values in years without rankings.
 */
void writeFile(char namesIn[MAX_NAMES][MAX_STR_LENGTH], int ranksIn[MAX_RANKS][MAX_YOBLISTS]) {
    FILE *fileOut;
    fileOut = fopen("summary.csv", "w+");
    int nameCount;
    fprintf(fileOut, "Name,1920,1930,1940,1950,1960,1970,1980,1990,2000,2010,\n");
    for (nameCount = 0; nameCount < NAME_CATCH; nameCount++) {
        fprintf(fileOut, "%s,", namesIn[nameCount]);
        int columns;
        for (columns = 0; columns < MAX_YOBLISTS; columns++) {
            if (ranksIn[nameCount][columns] == 0) {
                fprintf(fileOut, ",");
            } else {
                fprintf(fileOut, "%d,", ranksIn[nameCount][columns]);
            }
        }
        fprintf(fileOut, "\n");
    }
    fclose(fileOut);
}

/*
 * The following main opens the ten yob files, calls the sortData and
 * writeData functions in order to provide a formatted csv file of the data.
 */
int main(void) {

    char names[MAX_NAMES][MAX_STR_LENGTH];
    int ranks[MAX_RANKS][MAX_YOBLISTS];
    int dataMultiplier;
    memset(ranks, 0, sizeof(ranks));

    string fileNames[10];
    fileNames[0] = "yob1920.txt";
    fileNames[1] = "yob1930.txt";
    fileNames[2] = "yob1940.txt";
    fileNames[3] = "yob1950.txt";
    fileNames[4] = "yob1960.txt";
    fileNames[5] = "yob1970.txt";
    fileNames[6] = "yob1980.txt";
    fileNames[7] = "yob1990.txt";
    fileNames[8] = "yob2000.txt";
    fileNames[9] = "yob2010.txt";

    // Following block loops through files and reads them.
    int numberOfFiles;
    dataMultiplier = 0;
    for (numberOfFiles = 0; numberOfFiles < 10; numberOfFiles++) {
        FILE *file = fopen(fileNames[numberOfFiles], "rb");
        if (!file) {
            printf("Cannot open file.\n");
            return 1;
        }
        readFile(file, names, ranks, numberOfFiles, dataMultiplier);
        dataMultiplier += 100;
        fclose(file);
    }
    sortData(names, ranks);
    writeFile(names, ranks);
    return 0;
}
