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
1- Not implementing default Constructor or Constructor with optional arguments for Land and Character class
2- Not implementing inline getter/setter methods for Land class
3- Not implementing listCharacters and listLands as friend functions of CharacterList class
4- Not implementing a requested function / an attribute
*/

#include <string>

using namespace std;

class Land {
	string name;		// name of the land
	string holding;		// holding of the land, can be village, castle, city

public:
	Land* next;				// pointer to next land
	Land();					// default constructor
	Land(string = "", string = "");	// constructor with arguments
	Land(const Land&);		// copy constructor

	string getName()  {		// inline method to get land name
		return this->name;
	}

	string getHolding()  {	// inline method to get holding type
		return this->holding;
	}

};

class Character {

	string name;	// name of the character

	int manpower;	// number of soldiers of the character
	int money;		// amount of money the character has
	int numOfLands;	// number of lands the character has
	Land* lands;	// list of lands the character has

public:
	Character();												// default constructor
	Character(string, int = 0, int = 0, int = 0, Land* = NULL);	// constructor with arguments
	Character(const Character&);								// copy constructor
	~Character();												// destructor	
									
	string getName();		// method to get the name value
	string getName() const; // method to get the name of the const objet

	int getMoney();			// method to get the money value
	void setMoney(int);		// method to set the money value

	void setManpower(int);	// method to set manpower of the character
	int getManpower();		// method to get manpower value

	void getTaxes();		// method to collect taxes
	int getNumOfLands();	// method to get number of lands

	void addLand(Land&);	// add a land to the character
	void removeLand(string);// remove a land from the character
	Land* getLands();		// return reference to head of character's lands list
};

class CharacterList {

	friend void listCharacters(CharacterList&);	// list characters
	friend void listLands(CharacterList&);		// list lands

	int size;
	Character* characters;	// array of the characters

public:
	CharacterList();	// constructor
	~CharacterList();	// destructor

	void addCharacter(Character&);		// add character to the list
	void delCharacter(string);			// delete character from the list
	Character* getPlayer();				// get reference to player
	Character* getLandOwner(string);	// get reference to specific land owner
	int getSize();						// get size
};
