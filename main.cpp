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
 1- Not implementing general as constant Character object
 2- Not removing characters from the game, who have no lands left after the war 
 2- Memory leaks
 3- Not implementing a requested function / an attribute
 */

#include <iostream>
#include <fstream>
#include "GreatWarrior.h"
#include "GreatWarrior.cpp"

void readData(string, CharacterList&);
void printGameMenu();
void listCharacters(CharacterList&);
void listLands(CharacterList&);
bool endOfTurn(Character&, int);

int main() {
    cout << "Welcome to the Great Warriors. Create your character and attack other lands to be a great warrior.\n" << endl;

    CharacterList charList;         // create a list object to store the characters

    string fileName = "characters.txt";
    readData(fileName, charList);   // read characters.txt file

    /* player's character */
    string name, land, general;
    cout << "Enter your name: ";
    cin >> name;

    cout << "Enter name of your capital: ";
    cin >> land;

    cout << "Enter name of your general: ";
    cin >> general;

    const Character deputy(general);        // create general

    Character new_player(name, 3, 20, 0);   // create player's character
    Land newLand(land, "village");          // create player's land

    new_player.addLand(newLand);
    charList.addCharacter(new_player);
    
    Character *player = charList.getPlayer();   // get player object to use it below

    /* In-game loop */
    int choice = 0, round = 0;
    while (choice != 6) {
        printGameMenu();

        cin >> choice;

        switch (choice) {
        case 1: {
            cout << "You have rested in your palace." << endl;
            cout << "You've talked with your general " << deputy.getName() << "." << endl;

            round++;
            break;
        }
        case 2: {
            listLands(charList);
            cout << "Enter name of the land to attack." << endl;

            string land;
            cin >> land;

            // get reference to the attacked character
            Character* attacked = charList.getLandOwner(land);

            // check for valid inputs (non-existent lands or player's own land)
            if (attacked == NULL || attacked->getName() == player->getName()) {
                cout << "Please enter a valid land name." << endl;
                break;
            }

            // battle takes place here
            int result = player->getManpower() - attacked->getManpower();
            
            // if player wins
            if (result >= 0) {
                // there will be casualties for the player
                player->setManpower(player->getManpower() - attacked->getManpower());
                cout << "You've won the battle and conquered " << land << "." << endl;

                // find holding type of the land to give it to the player
                string holding = "";
                for (int i = 0; i < attacked->getNumOfLands(); i++) {
                    Land* ptr = attacked->getLands();

                    while (ptr != NULL) {
                        if (ptr->getName() == land) {
                            name = land;
                            holding = ptr->getHolding();
                            break;
                        }
                        ptr = ptr->next;
                    }
                }
                // add conquered land to the player
                Land temp(name, holding);
                player->addLand(temp);

                // remove conquered land from attacked character
                attacked->removeLand(land);

                // if attacked has no land left, delete him from the list.
                // since deCharacter function changes addresses, we get
                //  reference to the player again.
                if (attacked->getNumOfLands() == 0) {
                    charList.delCharacter(attacked->getName());
                    player = charList.getPlayer();
                }

            }
            // if player loses
            else {
                cout << "You've lost the battle and " << player->getManpower() << " manpower." << endl;
                player->setManpower(0);
            }

            round++;
            break;
        }
        case 3:
            cout << "How much manpower do you need? (1 manpower=5 gold)" << endl;

            int order;
            cin >> order;

            if (order * 5 <= player->getMoney()) {
                player->setMoney(player->getMoney() - order * 5);
                player->setManpower(player->getManpower() + order);
                cout << "Order successful. You have " << player->getManpower() << " manpower." << endl;
            }
            else {
                cout << "You do not have enough money." << endl;
            }

            // round++
            break;
        case 4:
            listCharacters(charList);
            break;
        case 5:
            listLands(charList);
            break;
        case 6:
            return 0;
        default:
            cout << "You entered an invalid value. Try again." << endl;
        }

        // note that we round++ only when choice is 1 or 2
        // check whether game ends
        if (choice < 3 && !endOfTurn(*player, round)) {
            return 0;
        }
        else if (charList.getSize() == 1) {
            cout << player->getName() << " is the only great warrior now." << endl;
            return 0;
        }
  }
}

void readData(string fileName, CharacterList& charList) {

    fstream dataFile(fileName.c_str()); // file object to read the data from the file

    if (dataFile.is_open()) {   // check whether file is open

        string word;    // string object to read file
        while (dataFile >> word) {  // >> operator separates on whitespace 

            string name, title;
            int money, manpower, numOfLands;

            // words in characters.txt ordered like name, title, manpower, money, number of lands and lands
            name = word;
            dataFile >> manpower;
            dataFile >> money;
            dataFile >> numOfLands;

            // create character
            Character newChar(name, manpower, money, 0);

            string landName, holding;

            for (int i = 0; i < numOfLands; i++) {
                dataFile >> landName;
                dataFile >> holding;

                // create land and add it to the character
                Land temp(landName, holding);
                newChar.addLand(temp);
            }

            // add character to the list
            charList.addCharacter(newChar);

        }

        dataFile.close();
    }
    else cout << "Unable to open the file" << endl;

    return;
}

void printGameMenu() {
    cout << endl;
    cout << "1. Stay in your palace" << endl;
    cout << "2. Attack to a land" << endl;
    cout << "3. Buy manpower" << endl;
    cout << "4. List characters" << endl;
    cout << "5. List lands" << endl;
    cout << "6. Exit" << endl;
    cout << endl;
}

void listCharacters(CharacterList& charList) {
    cout << endl;
    for (int i = 0; i < charList.size; i++) {
        cout << charList.characters[i].getName() << endl;
    }
}

void listLands(CharacterList& charList) {
    cout << endl;
    for (int i = 0; i < charList.size; i++) {
        Land* buffPtr = charList.characters[i].getLands();

        while (buffPtr != NULL) {
            cout << buffPtr->getName() << " " << buffPtr->getHolding() << " owned by " << charList.characters[i].getName() << endl;
            buffPtr = buffPtr->next;
        }

    }
    cout << endl;
}

bool endOfTurn(Character& player, int round) {

    /* End of turn evaluations */
    player.getTaxes();
    player.setMoney(player.getMoney() - player.getManpower());


    if (player.getNumOfLands() == 0) {  // if player has no land game is over
        cout << "You are no longer a great warrior. You survived " << round << " turns." << endl;
        cout << endl;
        cout << "GAME OVER." << endl;
        return false;
    }
    else if (player.getManpower() <= 0) {   // if player has no manpower
        player.removeLand(player.getLands()->getName());

        cout << "You lost one of your lands to rebellions since you don't have enough army." << endl;
    }
    else if (player.getMoney() - player.getManpower() < 0) {
        int runAways = player.getManpower() - player.getMoney();
        player.setManpower(player.getManpower() - runAways);

        cout << runAways << " soldiers run away from your army because you don't have enough gold to feed them." << endl;
    }

    cout << "Turn " << round << ": " << " " << player.getName() << " has " << player.getNumOfLands()
        << " land(s), " << player.getManpower() << " manpower and " << player.getMoney() << " golds." << endl;
    

    return true;
}

