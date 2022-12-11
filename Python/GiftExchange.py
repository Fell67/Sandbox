"""
This program simulates people drawing names from a hat in order to determine who they are exchanging a gift with. 
The user enters how many people are participating and the first and last name of each person. 
The program will then match everyone with the person they are going to get a gift for and then print the results. 
"""
import random
import os

# This class will hold the first, last, and email of each person
class Person:
    def __init__(self, name):
        self.name = name
        self.nameDrawn = ""

    def setNameDrawn(self, nameDrawn):
        self.nameDrawn = nameDrawn

    def getFileName(self):
        return str(self.name).replace(" ", "_")
        
    def __str__(self):
        if self.nameDrawn != "":
            return f"Person's Name: {self.name}\n + Name they drew: {self.nameDrawn}"
        else:
            return f"Name: {self.name}"

# Collect information from the user
def getInformation(numberOfPeople):
    i = 0
    people = []
    print("Lets get started!")
    while i < numberOfPeople:
        # Ask user for the person's first and last name
        person = Person(input("What is the person's first and last name?\n"))

        # Ask the user if all the information is correct
        validResponse = False
        while not validResponse:
            print("Is the following information correct (Y/N)?")
            print(" + " + str(person))
            userInput = input()
            if userInput[0] == "Y" or userInput[0] == "y":
                print("Excellent!")
                people.append(person)
                validResponse = True
                i = i + 1
                if i < numberOfPeople:
                    print("Lets move on to the next person")
            elif userInput[0] == "N" or userInput[0] == "n":
                print("Sorry about that lets try again")
                validResponse = True
            else:
                print("Invalid Input")
            
    return people

# simulate drawing people's names from a hat 
def drawFromHat(people):
    hat = []
    drawing = 0
    validOption = False

    # Since no names have been drawn set the values in the hat to False
    for i in range(len(people)):
        hat.append(False)

    for person in range(len(people)):
        # Select a name from the hat at random until you find a name that was not selected
        while not validOption:
            drawing = random.randrange(0, len(people))
            if (drawing != person) and not (hat[drawing]):
                validOption = True
            else:
                validOption = False
        
        # Give the person the name that was drawn and remove it from the hat
        people[person].setNameDrawn(people[drawing].name)
        hat[drawing] = True
        validOption = False

# Print out the results for the user to the console or to files base on what they prefer
def printResults(people):
    resultsOption = 0
    validResponseAndSuccess = False

    while not validResponseAndSuccess:
        print("Enter the number that corresponds to how would you like the results to be shown?")
        print(" 1. Show the results in the console")
        print(" 2. Save the results in a file")
        print(" 3. Save a file for each person that is participating with the person they are getting a gift for")
        resultsOption = int(input())

        match resultsOption:
            # Print the results to the console
            case 1:
                printToConsole(people)
                validResponseAndSuccess = True
            # Save the results to a single file
            case 2:
                validResponseAndSuccess = printToFile(people)
                if (validResponseAndSuccess):
                    print("The results have been saved to a file in the current directory.")
                else:
                    print("An error occurred when the file was being made.")
            # Save a file for each person that is participating with the person they are getting a gift for  
            case 3:
                validResponseAndSuccess = printToFiles(people)
                if (validResponseAndSuccess):
                    print("The results have been saved to files in the current directory.")
                else:
                    print("An error occurred when the file was being made.")
            # If the user entered something else then the response is invalid
            case _:
                print("Invalid Response. Please try again.")

# Print a Summary of the results at the end
def printToConsole(people):
    print("---------------------------------------------------")
    print("Here are the names everyone drew!")
    for person in people:
        print(person)
    print("Hope you guys have fun!")
    print("---------------------------------------------------")
    return

# Save the results to a single file
def printToFile(people):
    # Open/Create the file and put int the results
    # The file will be saved in the current directory in the following naming convention: Results.txt
    try: 
        file = open("Results.txt", "w")
        # Write the information to the file
        file.write("Here are the names everyone drew!\n")
        for person in people:
            file.write(str(person) + "\n")
    except:
        return False
    finally:
        file.close()

    return True

# Save a file for each person that is participating with the person they are getting a gift for
def printToFiles(people):
    # Set the path for where the files are going to go and create the directory
    dirName = "Results"
    parentDir = "./"
    try:
        path = os.path.join(parentDir, dirName)
        # if the directory does not already exist then create it
        if not os.path.exists(path):
            os.mkdir(path)
    except:
        return False
    
    # go through each name and print the results to a file for that person
    for person in people:        
        # try to open a file
        # Each file will be saved in a folder in the current directory in the following naming convention: fistName_lastName.txt 
        try:
            file = open(path + "/" + str(person.getFileName()) + ".txt", "w")
            # Write to the file
            file.write("Hello " + str(person.name) + ",\n")
            file.write("You have drawn: " + str(person.nameDrawn) + "\n")
            file.write("Hope you have fun!\n")
        except:
            return False
        finally:
            # Close the file
            file.close()

    return True

if __name__ == "__main__" :
    numberOfPeople = 0
    while numberOfPeople <= 1:
        # See how many people are joining the gift exchange
        numberOfPeople = int (input("How many people are joining the gift exchange?\n"))
        
        # Check that more then one person is participating in the gift exchange.
        if numberOfPeople <= 1:
            print("There needs to be more then one person participating in the gift exchange.")
    # Get everyone's information
    people = getInformation(numberOfPeople)

    # Match each person with someone else
    drawFromHat(people)

    # Ask the user if they want to print the results to the console, to a file, or to print them to one file for each person 
    printResults(people)

