/**
 * @author Doðukan Arslan
 *
 *	arsland15@itu.edu.tr
 *
 * compile: g++ main.cpp
 * run: ./a.out
 */


 /* PARTS WHERE YOU POSSIBLY LOST POINTS
 *
 1- Shallow copying of objects (refer to: https://en.wikipedia.org/wiki/Object_copying)
 2- Memory leaks
 3- Not implementing a requested function / an attribute
 */


/* Land Class */

// Default Constructor
Land::Land() {
    this->name = "";
    this->holding = "";
    this->next = NULL;
}

// Constructor with arguments
Land::Land(string name, string holding) {
    this->name = name;
    this->holding = holding;
    this->next = NULL;
}

// Copy constructor
Land::Land(const Land& copy) {
    this->name = copy.name;
    this->holding = copy.holding;
    this->next = NULL;
}

/* Character Class */

// Default Constructor
Character::Character() {
    this->lands = NULL;
    this->manpower = 0;
    this->money = 0;
    this->numOfLands = 0;
    this->name = "";
}

// Constructor with arguments
Character::Character(string name, int manpower, int money, int numOfLands, Land* landPtr) {
    this->name = name;
    this->manpower = manpower;
    this->money = money;
    this->numOfLands = numOfLands;
    this->lands = landPtr;
}

// Copy constructor
Character::Character(const Character& copy) {
    this->name = copy.name;
    this->manpower = copy.manpower;
    this->money = copy.money;
    this->numOfLands = copy.numOfLands;

    // traverse trough lands of the Character that will be copied.
    // this->lands = copy.lands makes two Characters' land pointer to point same address,
    // which might cause problems when we delete either of them.

    // copy first element
    this->lands = new Land(*copy.lands);

    Land* copyPtr = copy.lands->next;
    Land** thisPtr = &this->lands;
    while (copyPtr != NULL) {
        (*thisPtr)->next = new Land(copyPtr->getName(), copyPtr->getHolding());
        copyPtr = copyPtr->next;
        thisPtr = &((*thisPtr)->next);
    }
    // when copyPtr = NULL, thisPtr should be NULL too
    thisPtr = NULL;
    
}

// Destructor
Character::~Character() {

    // traverse through linked list and delete them one by one
    Land* tempPtr = this->lands;
    while (tempPtr != NULL) {
        this->lands = tempPtr->next;
        delete tempPtr;
        tempPtr = lands;
    }
}

// Getters & Setters
string Character::getName() {
    return this->name;
}

string Character::getName() const {
    return this->name;
}

int Character::getMoney() {
    return this->money;
}

void Character::setMoney(int money) {
    this->money = money;
}

void Character::setManpower(int mp) {
    this->manpower = mp;
}

int Character::getManpower() {
    return this->manpower;
}

void Character::getTaxes() {

    int tax = 0;

    Land* ptr = this->lands;

    while (ptr != NULL) {
        if (ptr->getHolding() == "village") {
            tax += 5;
        }
        else if (ptr->getHolding() == "castle") {
            tax += 7;
        }
        else if (ptr->getHolding() == "city") {
            tax += 10;
        }

        ptr = ptr->next;
    }

    this->setMoney(this->getMoney() + tax);
}

int Character::getNumOfLands() {
    return this->numOfLands;
}

// Character class functions
void Character::addLand(Land& newLand) {
    // if Character has no lands
    if (this->lands == NULL) {
        this->lands = new Land(newLand);
    }
    // if Character has land(s), add new land at the end of the list
    else {
        Land* ptr = this->lands;

        while (ptr->next != NULL) {
            ptr = ptr->next;
        }
        ptr->next = new Land(newLand);
    }
    this->numOfLands++;
}

void Character::removeLand(string name) {

    // if Character has no lands
    if (this->numOfLands == 0) {
        return;
    }
    else {
        Land* ptr = this->lands;

        // if first element should be deleted
        if (ptr->getName() == name) {

            this->lands = this->lands->next;
            delete ptr;
            this->numOfLands -= 1;
            return;
        }

        // find the land that should be deleted and remove it from the chain 
        // by connecting previous of it to next of it.
        while (ptr->next != NULL) {
            if (ptr->next->getName() == name) {
                Land* buff = ptr->next;

                ptr->next = ptr->next->next;
                delete buff;
                this->numOfLands -= 1;
            }
            else {
                ptr = ptr->next;
            }

        }

    }
}

Land* Character::getLands() {
    return this->lands;

}


/* CharacterList Class */

// Constructor
CharacterList::CharacterList() {
    this->size = 0;
    this->characters = NULL;
}

// Destructor
CharacterList::~CharacterList() {
    if (this->size > 1) {
        delete[] this->characters;
    }
    else if (this->size == 1) {
        delete characters;
    }
}

// CharacterList functions
void CharacterList::addCharacter(Character& newChar) {

    // if there is no Character in the array
    if (this->size == 0) {
        this->characters = new Character(newChar);
    }
    else {
        Character* tempList = new Character[this->size + 1];

        for (int i = 0; i < this->size; i++) {

            // tempList[i] = characters[i] makes two Characters' land pointer to point same address,
            // which might cause problems when we delete either of them.
            // Also, since operator= overloading is out of this homework's scope, solely using copy constructor
            // is not enough because destructor is called after copy constructor which makes us lose the lands.
            // To prevent this, you can use copy constructor or create an object like below first, then
            // add lands one by one. Same logic applies for adding new element out of this for loop and delCharacter function.
            
            *(tempList + i) = Character(characters[i].getName(), characters[i].getManpower(), characters[i].getMoney());
            
            Land* ptr = characters[i].getLands();

            while (ptr != NULL) {
                (tempList + i)->addLand(*ptr);
                ptr = ptr->next;
            }
            
        }

        // add new character
        *(tempList + this->size) = Character(newChar.getName(), newChar.getManpower(), newChar.getMoney());

        Land* ptr = newChar.getLands();

        while (ptr != NULL) {
            (tempList + this->size)->addLand(*ptr);
            ptr = ptr->next;
        }

        // delete old array
        if (this->size == 1) {
            delete this->characters;
        }
        else {
            delete[] this->characters;
        }

        // point to the new array
        this->characters = tempList;
    }
    this->size++;
}

void CharacterList::delCharacter(string name) {

    int buffIndex = 0;

    // if there is no Character in the array
    if (this->size == 0) {
        return;
    }
    // if there is one Character in the array
    else if (this->size == 1 && this->characters->getName() == name) {
        delete this->characters;
        this->characters = NULL;
    }
    else {
        Character* tempList = new Character[this->size - 1];

        for (int i = 0; i < this->size; i++) {
            // pass the matching land, add others
            if (this->characters[i].getName() != name) {

                *(tempList + buffIndex) = Character(characters[i].getName(), characters[i].getManpower(), characters[i].getMoney());

                Land* ptr = characters[i].getLands();

                while (ptr != NULL) {
                    (tempList + buffIndex)->addLand(*ptr);
                    ptr = ptr->next;
                }
                buffIndex++;
            }
        }

        // delete old array
        if (this->size == 1) {
            delete this->characters;
        }
        else {
            delete[] this->characters;
        }

        // point to the new array
        this->characters = tempList;
    }

    this->size = buffIndex;
}

Character* CharacterList::getPlayer() {
    // it is assumed that the player will always be at the end of the array. 
    // any other assumptions are possible.
    return &this->characters[this->size - 1];
}

Character* CharacterList::getLandOwner(string name) {

    // go through characters
    for (int i = 0; i < this->size; i++) {

        // go through every character's lands
        for (int n = 0; n < this->characters[i].getNumOfLands(); n++) {
            Land* ptr = this->characters[i].getLands();

            // check whether the character has that land
            while (ptr != NULL) {
                if (ptr->getName() == name) {
                    return &this->characters[i];
                }
                else {
                    ptr = ptr->next;
                }
            }
        }
    }

    // if we cannot find any character 
    return NULL;
}

int CharacterList::getSize() {
    return this->size;
}