#include <iostream>
using namespace std;

class Dog : public Animal {
private:
    static int dogNum;
public:
    int myDogNum;
    Dog(string nameInput = "", int ageInput = 0, int weightInput = 0, string breedInput = "", string colorInput = "", string healthInput = "", string soundInput = "")
        : Animal("Dog", nameInput, ageInput, weightInput, breedInput, colorInput, healthInput, soundInput) {
        dogNum++;
        myDogNum = dogNum;
    }
    void dogIntro() {
        cout << sound << endl;
        cout << "I am a " << type << endl;
        cout << "My name is " << name << endl;
        cout << "I am " << age << " years old." << endl;
        cout << "I weigh " << weight << " pounds" << endl;
        cout << "My breed is " << breed << endl;
        cout << "I am the color " << color << endl;
        cout << "My health is " << health << endl;
        cout << "I am dog number " << myDogNum << endl;
        cout << endl;
    }
};
int Dog::dogNum = 0;
