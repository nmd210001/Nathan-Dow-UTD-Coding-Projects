//Nathan Dow
// Animal Guessing Game
// Date: 4/1
// Experimented with syntax, methods, and binary tree algorithums. (Did not work but got interface down)
// Date: 4/2
// Got the algorithum to work and added finishing touches to interface
// Update ideas:
// use ofstream to output the new data to a file so the new questions and guesses can be saved



#include <string>
#include <cctype>
#include <iostream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <cassert>
#include <algorithm>
#include <fstream>

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






struct Animal{
    string animalQuestion;
    string animalGuess;

    struct Animal *yesAns;
    struct Animal *noAns;

};

string currentAns;
bool computerWins, playAgain = true;

//Globals to keep track of inside the gameplay loop
Animal* previouse1Animal, previouse2Animal;

Animal* makeNewAnimalQuestion(string question){
    showflow("entering makeNewAnimalQuestion");
    Animal* newAnimalQues = new Animal;
    newAnimalQues->animalGuess = question;
    newAnimalQues->yesAns = nullptr;
    newAnimalQues->noAns = nullptr;
    showflow("leaving makeNewAnimalQuestion");
    return newAnimalQues;
}
Animal* makeNewAnimalGuess(string guess){
    showflow("entering makeNewAnimalGuess");
    Animal* newAnimalGuess = new Animal;
    newAnimalGuess->animalGuess = guess;
    newAnimalGuess->yesAns = nullptr;
    newAnimalGuess->noAns = nullptr;
    showflow("leaving makeNewAnimalGuess");
    return newAnimalGuess;
}
void addAnimal(string newQues, string newGuess, Animal* curAnimal, bool isNewAniQuesYesForNewAni){
    showflow("entering addAnimal");
    Animal *oldAnimalGuessPtr = makeNewAnimalGuess(curAnimal -> animalGuess);
    Animal *newAnimalGuessPtr = makeNewAnimalGuess(newGuess);
    if(isNewAniQuesYesForNewAni){ // makes sure that the new animal guess will be for the current questions guess
        curAnimal->noAns = oldAnimalGuessPtr;
        curAnimal->yesAns = newAnimalGuessPtr;
    }
    else{ // it does the reverse if that's not the case
        curAnimal->yesAns = oldAnimalGuessPtr;
        curAnimal->noAns = newAnimalGuessPtr;
    }
    curAnimal->animalQuestion = newQues; //makes the current passed question the new question so they can be reorderd
    curAnimal->animalGuess = "";
    showflow("leaving addAnimal");
}
void promptUserToAnsQuetion(string question, string guess){
    showflow("entering promptUser");
    if (guess == ""){ // sees if it's a guess or a question
        cout << "Q: "<< question << endl;
        cin >> currentAns;
    }
    else if (question == ""){
        cout << "Is it a " << guess << "?" << endl;
        cin >> currentAns;
    }
    if (currentAns != "yes" && currentAns != "no"){
        cout << "ERROR please use 'yes' or 'no'" << endl;
        promptUserToAnsQuetion(question, guess);
    }
    showflow("leaving promptUser");
}
void promptUserForNewAnimal(Animal* currentAnimal){
    showflow("entering promptUserForNewAnimal");
    string newDifQuestion, newAnimalGuess, answerForBool;
    bool isNewAniQuestionYesForNewAni;
    cout << "What was your animal?" << endl;
    cin.get();
    getline(cin,newAnimalGuess);
    cout << "What is a differentiating question between " << currentAnimal -> animalGuess << " and " << newAnimalGuess << "?"<< endl;
    getline(cin, newDifQuestion);
    cout << "What is the answer to '" << newDifQuestion << "' for " << newAnimalGuess << "?(yes or no)" << endl;
    cin >> answerForBool;
    if (answerForBool == "yes")
        isNewAniQuestionYesForNewAni = true;
    else if(answerForBool == "no")
        isNewAniQuestionYesForNewAni = false;
    else{
        while (answerForBool != "no" && answerForBool != "yes"){
            cout << "ERROR please use 'yes' or 'no'" << endl;
            cout << "What is the answer to " << newDifQuestion << " for " << newAnimalGuess << "?(yes or no)" << endl;
            cin >> answerForBool;
            if (answerForBool == "yes")
                isNewAniQuestionYesForNewAni = true;
            else if(answerForBool == "no")
                isNewAniQuestionYesForNewAni = false;
        }
    }
    addAnimal(newDifQuestion, newAnimalGuess, currentAnimal, isNewAniQuestionYesForNewAni);
    showflow("exiting promptUserForNewANimal");
}
void gamePlay(Animal* currentAnimal){
    showflow("entering gamePlay");
    promptUserToAnsQuetion( currentAnimal->animalQuestion, currentAnimal->animalGuess);// gets the current answer
    if (currentAns == "yes"){
        if (currentAnimal->yesAns == nullptr){ //means that the guess was correct because the current answer was yes and there were no more Animal structs
            computerWins = true;
            showflow("leaving gamePlay");
            return;
        }
        gamePlay(currentAnimal -> yesAns);
    }
    else if (currentAns == "no"){
        if (currentAnimal->noAns == nullptr){
            computerWins = false;
            promptUserForNewAnimal(currentAnimal);
            showflow("leaving gamePlay");
        }
        else
            gamePlay(currentAnimal -> noAns);
    }
    showflow("leaving gamePlay");
}
main(){
    bool debugPrinting = false;
    bool showflowControl = false;
    Animal* root = makeNewAnimalGuess("lizard");
    while (playAgain){
        cout << "Think of an animal. (hit enter to continue) PLEASE USE 'yes' OR 'no' FOR ALL YES OR NO QUESTIONS" << endl;
        cin.get();
        gamePlay(root);
        if(computerWins)
            cout << "I win!" << endl;
        else if (!computerWins)
            cout  << "You win!" << endl;
        string answer;
        cout << "Do you want to play again?" << endl;
        cin >> answer;
        if (answer != "yes")
            playAgain = false;
    }
}
