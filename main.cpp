#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

//  BASE CLASS: Pet
class Pet {
protected:
    string name;
    int health;      // 0 - 100
    int hunger;      // 0 = full, 100 = starving
    int happiness;   // 0 - 100
    int energy;      // 0 - 100

public:
    // Constructor
    Pet(string petName) {
        name      = petName;
        health    = 100;
        hunger    = 30;
        happiness = 70;
        energy    = 80;
    }

    // ---- Getters ----
    string getName()    { return name; }
    int getHealth()     { return health; }
    int getHunger()     { return hunger; }
    int getHappiness()  { return happiness; }
    int getEnergy()     { return energy; }

    // ---- Setters (with bounds check) ----
    void setHealth(int h)    { health    = max(0, min(100, h)); }
    void setHunger(int h)    { hunger    = max(0, min(100, h)); }
    void setHappiness(int h) { happiness = max(0, min(100, h)); }
    void setEnergy(int e)    { energy    = max(0, min(100, e)); }

    // Virtual function for sound (Polymorphism)
    virtual void makeSound() {
        cout << name << " makes a sound!" << endl;
    }

    // Unique action - overridden in derived classes
    virtual void uniqueAction() {
        cout << name << " does something cute!" << endl;
    }

    //  Common Actions
    void feed() {
        if (energy < 10) {
            cout << "\n" << name << " is too tired to eat! Let them sleep first.\n";
            return;
        }
        cout << "\nYou feed " << name << "...\n";
        setHunger(hunger - 30);
        setHealth(health + 10);
        setHappiness(happiness + 5);
        setEnergy(energy - 5);
        cout << name << " enjoyed the meal!\n";
        makeSound();
    }

    void play() {
        if (hunger > 70) {
            cout << "\n" << name << " is too hungry to play! Feed them first.\n";
            return;
        }
        if (energy < 20) {
            cout << "\n" << name << " is too tired to play! Let them sleep first.\n";
            return;
        }
        cout << "\nYou play with " << name << "...\n";
        uniqueAction();
        setHappiness(happiness + 20);
        setEnergy(energy - 20);
        setHunger(hunger + 15);
        setHealth(health + 5);
        makeSound();
    }

    void sleep() {
        cout << "\n" << name << " is now sleeping... Zzz\n";
        setEnergy(energy + 40);
        setHealth(health + 10);
        setHunger(hunger + 10);   // gets a little hungry while sleeping
        cout << name << " woke up feeling refreshed!\n";
    }

    // Display Status
    void showStatus() {
        cout << "\n========== " << name << "'s Status ==========\n";
        cout << "Health   : " << drawBar(health)    << " " << health    << "/100\n";
        cout << "Hunger   : " << drawBar(hunger)    << " " << hunger    << "/100  (0=Full, 100=Starving)\n";
        cout << "Happiness: " << drawBar(happiness) << " " << happiness << "/100\n";
        cout << "Energy   : " << drawBar(energy)    << " " << energy    << "/100\n";
        cout << "Mood     : " << getMood() << "\n";
        cout << "=========================================\n";
    }

    // Mood based on happiness
    string getMood() {
        if (happiness >= 70)
            return "Happy :D";
        else if (happiness >= 40)
            return "Neutral :|";
        else
            return "Sad :(";
    }

    //  Simple text progress bar
    string drawBar(int value) {
        string bar = "[";
        int filled = value / 10;
        for (int i = 0; i < 10; i++) {
            if (i < filled) bar += "#";
            else            bar += "-";
        }
        bar += "]";
        return bar;
    }

    // Passive stat decay (called after each action)
    void passiveDecay() {
        setHunger(hunger + 5);    // gets hungrier over time
        setHappiness(happiness - 3);
        if (hunger > 70)
            setHealth(health - 10);   // health drops if too hungry
    }

    // ---- Check if game over ----
    bool isAlive() {
        return health > 0;
    }

    // Virtual destructor
    virtual ~Pet() {}
};

//  DERIVED CLASS: Dog
class Dog : public Pet {
public:
    Dog(string petName) : Pet(petName) {}

    void makeSound() override {
        cout << name << " says: Woof! Woof!\n";
    }

    void uniqueAction() override {
        cout << name << " runs and fetches the ball!\n";
    }
};

//  DERIVED CLASS: Cat
class Cat : public Pet {
public:
    Cat(string petName) : Pet(petName) {}

    void makeSound() override {
        cout << name << " says: Meow~\n";
    }

    void uniqueAction() override {
        cout << name << " curls up and starts to purr...\n";
    }
};

//  DERIVED CLASS: Rabbit
class Rabbit : public Pet {
public:
    Rabbit(string petName) : Pet(petName) {}

    void makeSound() override {
        cout << name << " says: Squeak! Squeak!\n";
    }

    void uniqueAction() override {
        cout << name << " hops around happily!\n";
    }
};


//  OWNER CLASS
class Owner {
private:
    string ownerName;
    Pet*   myPet;       // Pointer to base class (Polymorphism)

public:
    Owner(string name) {
        ownerName = name;
        myPet     = nullptr;
    }

    void adoptPet() {
        int choice;
        string petName;

        cout << "\n====================================\n";
        cout << "  Welcome, " << ownerName << "!\n";
        cout << "  Choose your pet:\n";
        cout << "  1. Dog\n";
        cout << "  2. Cat\n";
        cout << "  3. Rabbit\n";
        cout << "====================================\n";
        cout << "Enter choice (1-3): ";
        cin  >> choice;
        cin.ignore();

        cout << "Enter a name for your pet: ";
        getline(cin, petName);

        // Create the correct derived class object
        if (choice == 1)
            myPet = new Dog(petName);
        else if (choice == 2)
            myPet = new Cat(petName);
        else
            myPet = new Rabbit(petName);

        cout << "\nCongratulations! You adopted " << petName << "!\n";
        myPet->makeSound();
    }

    void startGame() {
        if (myPet == nullptr) {
            cout << "You don't have a pet yet!\n";
            return;
        }

        int action;

        while (true) {
            // Check game over
            if (!myPet->isAlive()) {
                cout << "\n*** Oh no! " << myPet->getName()
                     << "'s health dropped to zero! ***\n";
                cout << "*** GAME OVER - Please take better care next time! ***\n";
                break;
            }

            // Show menu
            cout << "\n-------- What do you want to do? --------\n";
            cout << "1. Feed   " << myPet->getName() << "\n";
            cout << "2. Play with " << myPet->getName() << "\n";
            cout << "3. Put " << myPet->getName() << " to sleep\n";
            cout << "4. Check Status\n";
            cout << "5. Exit Game\n";
            cout << "-----------------------------------------\n";
            cout << "Enter choice: ";
            cin  >> action;

            switch (action) {
                case 1:
                    myPet->feed();
                    myPet->passiveDecay();
                    break;
                case 2:
                    myPet->play();
                    myPet->passiveDecay();
                    break;
                case 3:
                    myPet->sleep();
                    myPet->passiveDecay();
                    break;
                case 4:
                    myPet->showStatus();
                    break;
                case 5:
                    cout << "\nGoodbye! Take care of " << myPet->getName() << "!\n";
                    return;
                default:
                    cout << "Invalid choice! Try again.\n";
            }
        }
    }

    ~Owner() {
        delete myPet;   // free memory
    }
};

//  MAIN FUNCTION
int main() {
    cout << "                                          ========================================" << endl;
    cout << "                                                  VIRTUAL PET SIMULATOR v1.0        " << endl;
    cout << "                                               OOP Project - BSCS 2nd Semester    " << endl;
    cout << "                                          ========================================" << endl;

    string ownerName;
    cout << "Enter your name: ";
    getline(cin, ownerName);

    Owner player(ownerName);
    player.adoptPet();
    player.startGame();

    cout << "\nThank you for playing!\n";
    return 0;
}

