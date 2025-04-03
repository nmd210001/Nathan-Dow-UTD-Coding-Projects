#include <iostream>
using namespace std;

class Cat : public Animal {
private:
    static int catNum;
public:
    int myCatNum;
    Cat(string nameInput = "", int ageInput = 0, int weightInput = 0, string breedInput = "", string colorInput = "", string healthInput = "", string soundInput = "")
        : Animal("Cat", nameInput, ageInput, weightInput, breedInput, colorInput, healthInput, soundInput) {
        catNum++;
        myCatNum = catNum;
    }
    void catIntro() {
        cout << sound << endl;
        cout << "I am a " << type << endl;
        cout << "My name is " << name << endl;
        cout << "I am " << age << " years old." << endl;
        cout << "I weigh " << weight << " pounds" << endl;
        cout << "My breed is " << breed << endl;
        cout << "I am the color " << color << endl;
        cout << "My health is " << health << endl;
        cout << "I am cat number " << myCatNum << endl;
        cout << endl;
    }
};
int Cat::catNum = 0;
