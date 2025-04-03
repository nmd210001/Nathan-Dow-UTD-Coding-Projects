//Nathan Dow
// Animal Shelter
// Done all in one day bc of studying for finals
//  - created the class Animal and the Subclasses Cat and Dog
//  - used previousely used file opening code
//  - convert CSV turns the segments including the comma into just the segments
//  - getAnimalInfo goes through each character of the line and gives it CSV every time it comes accross a comma
//    it then uses the number of commas found to determine what kind of info it is reading
//  - I then use a while loop in createAnimals to go through each line and give each line to getAnimal info
//    then use the info to create a new animal for each line while differcianting cats and dogs
//  - In main, I skip the header line
//    Then loop through the vectors where the Animals were added to call the introductions for gen animals cats and dogs
//    The differing information printed for cats and dogs are their cat or dog number
//  - I then used rdbuf to create a buffer switch to print to both the consol and the output file



#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Animal.h"
#include "Cat.h"
#include "Dog.h"

// myDebugging-Mini.h
//
// changelog:
//	6/1/20 - pruned down to essentials for class work
// notes:
//  Change ckDbgCnt to always spit out message.
//  Add rest of debugger commands at some point.
//

#ifndef MYDEBUGGING_H
#define MYDEBUGGING_H


using namespace std;

int dbgCnt=-1; //debug counter
bool debugPrinting;
bool showflowControl;

void showflow (string msg) {
    if (showflowControl)
		cout << "--- at: " << msg << endl;
} // showflow
void showflow (string msg, int num) {
    if (showflowControl)
		cout << "--- at: " << msg << " with " << num << endl;
} // showflow
void showflow (string msg, int num1, int num2) {
    if (showflowControl)
		cout << "--- at: " << msg << " with " << num1
            << " " << num2 << endl;
} // showflow
void showflow (string msg,string msg2) {
    if (showflowControl)
		cout << "--- at: " << msg << ' ' << msg2 << endl;
} // showflow
void setDbgCnt (int cnt) {dbgCnt = cnt;}
void ckDbgCnt (string msg) {
    int n;
    if (debugPrinting && (dbgCnt-- <= 1)) {
        cout << "--->> " << msg << endl;
        cin >> n;
        if (n > 0) dbgCnt = n;
    }
} //ckDbgCnt
void dprint (string msg) {
    ckDbgCnt(msg);
} // dprint
void dprint (string msg, int val) {
    ckDbgCnt(msg+to_string(val));
} // dprint
void dprint (string msg, string val) {
    ckDbgCnt(msg+val);
} // dprint
#endif //MYDEBUGGING_H




void Animal::setType(string typeInput){
    type = typeInput;
}
void Animal::setName(string nameInput){
    name = nameInput;
}
void Animal::setAge(int ageInput){
    if (ageInput >= 0){
        age = ageInput;
    } else {
        cout << "Invalid age. Please enter a number greater or equal to 0.\n";
        exit(EXIT_FAILURE);
    }
}
void Animal::setWeight(int weightInput){
    if (weightInput >= 0){
        weight = weightInput;
    } else {
        cout << "Invalid weight. Please enter a number greater or equal to 0.\n";
        exit(EXIT_FAILURE);
    }

}
void Animal::setBreed(string breedInput){
    breed = breedInput;
}
void Animal::setColor(string colorInput){
    color = colorInput;
}
void Animal::setHealth(string healthInput){
    health = healthInput;
}
void Animal::setSound(string soundInput){
    sound = soundInput;
}




bool firstRunMain = true;

vector<Animal> animals;
vector<Cat> cats;
vector<Dog> dogs;


int main(string fileName);

void handleFileOpenFailure(string fileName) {
    showflow("Entering handleFileOpenFailure");
    string newFileName;
    cout << "Oops, can't open the specified input file: No such file or directory" << endl;
    cout << "The file name used was: " << fileName << endl;
    cout << "Enter another file name to use (or quit): ";  getline(cin, newFileName);
    if (newFileName == "exit" || newFileName == "quit") exit(0);
    main(newFileName); // use recurtion and the catch statments in main eventually running the program after file is successful
    showflow("Leaving handleFileOpenFailure");
}
string convertCSV(string segment){
    showflow("entering convertCSV");
    string returnString;
    for(int index = 0; index < segment.length(); index++){
       if(segment[index] != ',') returnString += segment[index];
    }
    showflow("leaving convertCSV");
    return returnString;
}
void getAnimalInfo(string lineInput, string &typeInput, string &nameInput, string &ageStringInput, string &weightStringInput, string &breedInput, string &colorInput, string &healthInput, string &soundInput){
    showflow("entering getAnimalInfo");
    int commaCount = 0;
    string stringSeg;
    for (int index = 0; index < lineInput.length(); index++){
        if(lineInput[index] != ',') {
            stringSeg += lineInput[index];
        } else {
            commaCount++;
            if(commaCount == 1) typeInput =  convertCSV(stringSeg);
            else if(commaCount == 2) nameInput = convertCSV(stringSeg);
            else if(commaCount == 3) ageStringInput = convertCSV(stringSeg);
            else if(commaCount == 4) weightStringInput = convertCSV(stringSeg);
            else if(commaCount == 5) breedInput = convertCSV(stringSeg);
            else if(commaCount == 6) colorInput = convertCSV(stringSeg);
            else if(commaCount == 7) healthInput = convertCSV(stringSeg);
            stringSeg = "";
        }
    }
    // assigns the remaining stringSeg to the sound
    soundInput = convertCSV(stringSeg);
    showflow("leaving getAnimalInfo");
}
void createAnimals(ifstream& inputFile) {
    showflow("entering createAnimals");
    string line;
    while (getline(inputFile, line)) {
        string typeStr, nameStr, ageString, weightString, breedStr, colorStr, healthStr, soundStr;
        int ageI, weightI;
        getAnimalInfo(line, typeStr, nameStr, ageString, weightString, breedStr, colorStr, healthStr, soundStr);
        if (ageString == "") ageString = "-1"; // avoids errors with stoi by making the empty strings numbers
        if (weightString == "") weightString = "-1";
        ageI = stoi(ageString);
        weightI = stoi(weightString);
        if (typeStr == "cat") {
            Cat cat(nameStr, ageI, weightI, breedStr, colorStr, healthStr, soundStr);
            cats.push_back(cat);
            animals.push_back(cat);  // Add cat object to the base class vector
        } else if (typeStr == "dog") {
            Dog dog(nameStr, ageI, weightI, breedStr, colorStr, healthStr, soundStr);
            dogs.push_back(dog);
            animals.push_back(dog);  // Add dog object to the base class vector
        } else {
            Animal animal(typeStr, nameStr, ageI, weightI, breedStr, colorStr, healthStr, soundStr);
            animals.push_back(animal);
        }
    }
    showflow("leaving createAnimals");
}
int main(string newFileName){
    debugPrinting = false;
    showflowControl = false;
    ifstream inputFile;
    if (firstRunMain) { //handles the first occurrence of getting the file from the user
        string fileName;
        cout << "Enter the file name: " << endl;
        getline(cin, fileName);
        firstRunMain = false; // makes sure the else procedure is performed, not the initial procedure
        inputFile.open(fileName);
        if (inputFile.fail())
            handleFileOpenFailure(fileName); //checks if the file was opened successfully and runs the procedure for this;
    } else { // handles all the rest of getting the file from the user if the first attempt was not successful
        cout << "--- The new file name is: " << newFileName << endl;
        inputFile.open(newFileName);
        if (inputFile.fail())
            handleFileOpenFailure(newFileName);
    }
    // gets rid of the header line
    string headerLine;
    getline(inputFile, headerLine);
    createAnimals(inputFile);
    ofstream outputFile;
    outputFile.open("HW6-CS1337-Nathan-Dow-Ouput.txt");
    streambuf* coutBuffer = cout.rdbuf(); // creates a buffer that can be turned on or off
    cout.rdbuf(outputFile.rdbuf()); //turns the buffer on so cout now goes to the output file
    cout << "Total number of animals created: " << animals.size() << endl;
    cout << "Total number of cats created: " << cats.size() << endl;
    cout << "Total number of dogs created: " << dogs.size() << endl;
    cout << "Total number of other animals: " << animals.size() - (cats.size() + dogs.size()) << endl;
    cout << endl;
    cout << "Here are all the animals:" << endl;
    cout << "-------------------------------" << endl;
    for (int index = 0; index < animals.size(); index++) {
        animals[index].introduction();
    }
    cout << "Here are all the cats:" << endl;
    cout << "-------------------------------" << endl;
    for (int index = 0; index < cats.size(); index++) {
        cats[index].catIntro();
    }
    cout << "Here are all the dogs:" << endl;
    cout << "-------------------------------" << endl;
    for (int index = 0; index < dogs.size(); index++) {
        dogs[index].dogIntro();
    }
    cout.rdbuf(coutBuffer); // turns the buffer off so all the cout goes to the consol again
    cout << "Total number of animals created: " << animals.size() << endl;
    cout << "Total number of cats created: " << cats.size() << endl;
    cout << "Total number of dogs created: " << dogs.size() << endl;
    cout << "Total number of other animals: " << animals.size() - (cats.size() + dogs.size()) << endl;
    cout << endl;
    cout << "Here are all the animals:" << endl;
    cout << "-------------------------------" << endl;
    for (int index = 0; index < animals.size(); index++) {
        animals[index].introduction();
    }
    cout << "Here are all the cats:" << endl;
    cout << "-------------------------------" << endl;
    for (int index = 0; index < cats.size(); index++) {
        cats[index].catIntro();
    }
    cout << "Here are all the dogs:" << endl;
    cout << "-------------------------------" << endl;
    for (int index = 0; index < dogs.size(); index++) {
        dogs[index].dogIntro();
    }
    inputFile.close();
    outputFile.close();
}
