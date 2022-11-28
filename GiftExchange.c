/*
This program simulates people drawing names from a hat in order to determine who they are exchanging a gift with. 
The user enters how many people are participating and the first and last name of each person. 
The program will then match everyone with the person they are going to get a gift for and then print the results. 
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// struc used to hold the first, last, and the name of the person they drew
typedef struct Person {
    char* firstName;
    char* lastName;
    struct Person* nameDrawn;
} Person;


// Fuction Declarations
void getInformation(Person[], int);
void drawFromHat(Person[], int);
void printResults(Person[], int);
int printToFile(Person[], int);
int printToFiles(Person[], int);
void printToConsole(Person[], int);
void removePeople(Person[], int);
void removePerson(Person*);

int main () {
    int numberOfPeople = 0;

    do {
        // See how many people are joining the gift exchange
        printf("How many people are joining the gift exchange?\n");
        scanf("%d", &numberOfPeople);

        // Check that more then one person is participating in the gift exchange.
        if (numberOfPeople <= 1) {
            printf("There needs to be more then one person participating in the gift exchange.\n");
        }
    } while (numberOfPeople <= 1);

    // This array holds the first, last, and who they drew of all the people doing the gift exchange
    Person *people = malloc(numberOfPeople * sizeof *people);

    // Get everyone's information
    getInformation(people, numberOfPeople);

    // Match each person with someone else
    drawFromHat(people, numberOfPeople);

    // Ask the user if they want to print the results to the console, to a file, or to print them to one file for each person 
    printResults(people, numberOfPeople);
    
    // deallocate the person array
    removePeople(people, numberOfPeople);

    return 0;
}
// Collect information from the user
// Needs the people array and the number of people
void getInformation(Person people[], int numberOfPeople) {
    // Get everyone's name
    int i = 0;
    
    printf("Lets get started!\n");
    while (i < numberOfPeople) {
        // Used to get the user input size
        char userInput1[90];
        char userInput2[90];

        // Ask user for the person's first and last name
        printf("What is the person's first and last name?\n");
        scanf("%s %s", userInput1, userInput2);
        
        // Set the size of the first and last name in the array
        people[i].firstName = malloc((strlen(userInput1) + 1) * sizeof(char));
        people[i].lastName = malloc((strlen(userInput2) + 1) * sizeof(char));

        // Place the names into the array and clear out the input
        strcpy(people[i].firstName, userInput1);
        strcpy(people[i].lastName, userInput2);
        userInput1[0] = '\0';
        userInput2[0] = '\0';

        // Ask user if all the information is correct
        int vaildResponse = 0;
        do {
            printf("Is the following information correct (Y/N)?\n + First Name: %s\n + Last Name: %s\n", 
                    people[i].firstName, 
                    people[i].lastName);
            scanf("%s", userInput1);

            // if the information is correct go on to the next person else repeat the same person
            if (userInput1[0] == 'Y' || userInput1[0] == 'y') {
                printf("Excellent!\n");
                userInput1[0] = '\0';
                vaildResponse = 1;
                i++;
                // If the is another person tell the user that we will move on to the next person
                if(i < numberOfPeople) {
                    printf("Lets move on to the next person\n");
                }
            } else if (userInput1[0] == 'N' || userInput1[0] == 'n') {
                printf("Sorry about that lets try again\n");
                // clear out any old information
                userInput1[0] = '\0';
                vaildResponse = 1;
                // Free any mem for the names
                free(people[i].firstName);
                free(people[i].lastName);
            } else {
                printf("Invaild Input\n");
                // clear out any old information
                userInput1[0] = '\0';
            }
        } while (!vaildResponse);
    }
    return;
}
// simulate drawing people's names from a hat 
void drawFromHat(Person people[], int numberOfPeople) {
    int i = 0;
    int hat[numberOfPeople]; // The hat where everyone's names are in
    time_t seed; // seed for rand number generator
    int drawing = 0; // The name that is drawn

    // seed the random number generator
    srand((unsigned) time(&seed));

    // Since no names have been drawn set the values in the hat to 0
    for ( i=0; i < numberOfPeople; i++ ) {
        hat[i] = 0;
    }

    for ( i = 0; i < numberOfPeople; i++) {
        // Select a name from the hat at random until you find a name that was not selected
        do {
             drawing = rand() % numberOfPeople;
        } while (!(!(hat[drawing]) && (drawing != i)));

        // Give the person the name that was drawn and remove it from the hat
        people[i].nameDrawn = &people[drawing];
        hat[drawing] = 1;
    }
    
    return;
}
// Print out the results for the user to the console or to files base on what they prefer
void printResults(Person people[], int numberOfPeople) {
    int resultsOption = 0;
    int validResponseAndSuccess = 0;

    do {
        printf("Enter the number that corresponds to how would you like the results to be shown?\n");
        printf(" 1. Show the results in the console\n");
        printf(" 2. Save the results in a file\n");
        printf(" 3. Save a file for each person that is participating with the person they are getting a gift for\n");
        scanf("%d", &resultsOption);

        switch (resultsOption)
        {
            // Print the results to the console
            case 1:
                printToConsole(people, numberOfPeople);
                validResponseAndSuccess = 1;
                break;
            // Save the results to a single file
            case 2:
                validResponseAndSuccess = printToFile(people, numberOfPeople);
                if (validResponseAndSuccess) {
                    printf("The results have been saved to a file in the current directory.\n");
                } else {
                    printf("An error occurred when the file was being made.\n");
                }
                break;
            // Save a file for each person that is participating with the person they are getting a gift for  
            case 3:
                validResponseAndSuccess = printToFiles(people, numberOfPeople);
                if (validResponseAndSuccess) {
                    printf("The results have been saved to files in current directory.\n");
                } else {
                    printf("An error occurred when the files were being made.\n");
                }
                break;
            default:
                printf("Invalid Response. Please try again.\n");
                break;
        }
    } while (!validResponseAndSuccess);

    return;
}
// Print a Summary of the results at the end
void printToConsole(Person people[], int numberOfPeople) {
    int i = 0;
    printf("---------------------------------------------------\n");
    printf("Here are the names everyone drew!\n");
    for (i = 0; i < numberOfPeople; i++) {
        printf("Person's Name: %s %s\n + Name they drew: %s %s\n", 
                people[i].firstName, 
                people[i].lastName,
                people[i].nameDrawn->firstName,
                people[i].nameDrawn->lastName);
    }
    printf("Hope you guys have fun!\n");
    printf("---------------------------------------------------\n");
    return;
}
// Save the results to a single file
int printToFile(Person people[], int numberOfPeople) {
    int i = 0;
    FILE* file = NULL;

    // Open file
    // The file will be saved in the current directory in the following naming convention: Results.txt 
    char* fileLocation = malloc((strlen("Results.txt") + 1) * sizeof(char));
    strcpy(fileLocation, "");
    strcat(fileLocation, "Results.txt");
    
    // Create a file to put the results
    file = fopen(fileLocation, "w");

    // Check that the file is open if not return with an error
    if (file == NULL) {
        return 0;
    }

    fprintf(file, "Here are the names everyone drew!\n");
    for (i = 0; i < numberOfPeople; i++) {
        fprintf(file, "Person's Name: %s %s\n + Name they drew: %s %s\n", 
                people[i].firstName, 
                people[i].lastName,
                people[i].nameDrawn->firstName,
                people[i].nameDrawn->lastName);
    }
    fprintf(file, "Hope you guys have fun!\n");

    // close the file
    fclose(file);

    // Free the file location from mem and set file to null
    free(fileLocation);
    file = NULL;
    
    return 1;
}
// Save a file for each person that is participating with the person they are getting a gift for  
int printToFiles(Person people[], int numberOfPeople) {
    int i = 0;
    FILE* file = NULL;
    char dirName[15] = "Results";
    
    // go through each name and print the results to a file for that person
    for (i = 0; i < numberOfPeople; i++) {
        // Open file
        // Each file will be saved in the current directory in the following naming convention: fistName_lastName.txt 
        char* fileLocation = malloc(strlen("./") + strlen(dirName) + strlen("/") + (strlen(people[i].firstName) + strlen("_") + strlen(people[i].lastName) + strlen(".txt") + 1) * sizeof(char));
        strcpy(fileLocation, "./");
        strcat(strcat(strcat(strcat(strcat(strcat(fileLocation, dirName), "/"), people[i].firstName), "_"), people[i].lastName), ".txt");
        
        // Create a file in the directory with the person's name
        file = fopen(fileLocation, "w");
        // Check that the file is open if not try creating the dir and opening the file again
        if (file == NULL) {
            // Try creating the dir and opening the file again
            char cmd[40] = "mkdir ";
            strcat(cmd, dirName);
            system(cmd);
            // Create a file in the directory with the person's name
            file = fopen(fileLocation, "w");
            // Check that the file is open if not return with an error
            if (file == NULL) {
                return 0;
            }
        }

        // Write to the file
        fprintf(file,"Hello %s %s,\n", 
                    people[i].firstName, 
                    people[i].lastName);
        fprintf(file, "You have drawn: %s %s\n",
                    people[i].nameDrawn->firstName,
                    people[i].nameDrawn->lastName);
        fprintf(file, "Hope you have fun!\n");

        // close the file
        fclose(file);

        // Free the file location from mem and set file to null
        free(fileLocation);
        file = NULL;
    }
    return 1;
}
// deallocate the given person array
// Need the person array and size of the array 
void removePeople(Person people[], int sizeOfArray) {
    int i = 0;
    for (i = 0; i < sizeOfArray; i++) {
        // deallocate the person
        removePerson(people);
    }
    // deallocate the person array
    free(people);

    return;
}
void removePerson(Person* person) {
    if (person == NULL) {
        return;
    }
    // deallocate the first name
    free(person->firstName);
    // deallocate the last name
    free(person->lastName);
    // deallocate the Name Drawn
    removePerson(person->nameDrawn);
    // deallocate the person array
    free(person);

    return;
}