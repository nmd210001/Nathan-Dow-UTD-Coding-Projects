#ifndef ANIMAL_H_INCLUDED
#define ANIMAL_H_INCLUDED

#include <iostream>
using namespace std;

#endif // ANIMAL_H_INCLUDED
class Animal{
    public:
        static int totalAnimals;
        int myAnimalNum;
        string type;
        string name;
        int age;
        int weight;
        string breed;
        string color;
        string health;
        string sound;
        Animal(string typeInput = "", string nameInput = "", int ageInput = -1, int weightInput = -1, string breedInput = "", string colorInput = "", string healthInput = "", string soundInput = ""){
            type = typeInput;
            name = nameInput;
            age = ageInput;
            weight = weightInput;
            breed = breedInput;
            color = colorInput;
            health = healthInput;
            sound = soundInput;
            totalAnimals++;
            myAnimalNum = totalAnimals;
        }
        void setType(string);
        void setName(string);
        void setAge(int);
        void setWeight(int);
        void setBreed(string);
        void setColor(string);
        void setHealth(string);
        void setSound(string);
        string getType() const{return type;}
        string getName() const{return name;}
        int getAge() const{return age;}
        int getWeight() const{return weight;}
        string getBreed() const{return breed;}
        string getColor() const{return color;}
        string getHealth() const{return health;}
        string getSound() const{return sound;}

        int getTotalAnimals() const;

        void introduction(){
            cout << sound << endl;
            cout << "I am a " << type << endl;
            cout << "My name is " << name << endl;
            cout << "I am " << age << " years old." << endl;
            cout << "I weigh " << weight << " pounds" << endl;
            cout << "My breed is " << breed << endl;
            cout << "I am the color " << color << endl;
            cout << "My health is " << health << endl;
            cout << "I am animal number " << myAnimalNum << endl;
            cout << endl;
        }

};
int Animal::totalAnimals = 0;
