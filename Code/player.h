//Abdullah
//24I-3164
//SE-C
#ifndef PLAYER_H
#define PLAYER_H

#include<iostream>
using namespace std;

#include <string>
#include "Pet.h"

// Forward declaration
class Guild;

// Maximum number of pets and items a player can have
const int MAX_PETS = 10;
const int MAX_ITEMS = 20;
const int MAX_GUILD_MEMBERS = 10;
const int MAX_GUILD_PETS = 30;

const int PET_ROLE_TANK = 0;
const int PET_ROLE_HEALER = 1;
const int PET_ROLE_DAMAGE_DEALER = 2;
const int PET_ROLE_SUPPORT = 3;
const int PET_ROLE_NONE = 4;

struct GuildPetAssignment {
    Pet* pet;
    int role; // Using pet role constants
    bool isActive;
    
    GuildPetAssignment() : pet(nullptr), role(PET_ROLE_NONE), isActive(false) {}
    GuildPetAssignment(Pet* p, int r) : pet(p), role(r), isActive(true) {}
};

class Player {
private:
    string name;
    int level;
    int experience;
    int experienceToNextLevel;
    int currency;
    
    // Player's pet collection
    Pet* pets[MAX_PETS];
    int petCount;
    int activePetIndex; // Currently selected pet
    
    // Inventory for items
    struct Item {
        string name;
        string description;
        int quantity;
        
        Item() : name(""), description(""), quantity(0) {}
        Item(const  string& n, const  string& desc, int qty) 
            : name(n), description(desc), quantity(qty) {}
    };
    
    Item inventory[MAX_ITEMS];
    int itemCount;
    
    // Guild membership
    Guild* playerGuild;
    
    // Battle stats
    int wins;
    int losses;
    int arenaRank;
    
public:
    // Constructor and Destructor
    Player(const  string& playerName);
    ~Player();
    
    // Getters
     string getName() const;
    int getLevel() const;
    int getExperience() const;
    int getCurrency() const;
    int getWins() const;
    int getLosses() const;
    int getArenaRank() const;
    Pet* getActivePet() const;
    Guild* getGuild() const;
    
    // Setters
    void setName(const  string& newName);
    void setCurrency(int amount);
    void setActivePet(int petIndex);
    void setGuild(Guild* guild);
    
    // Experience and currency
    void gainExperience(int amount);
    void levelUp();
    void addCurrency(int amount);
    bool spendCurrency(int amount);
    
    // Pet management
    bool adoptPet(const  string& petType, const  string& petName);
    void displayPets() const;
    int getPetCount() const;
    Pet* getPet(int index) const;
    void releasePet(int index);
    
    // Guild management
    void joinGuild(Guild* guild);
    void leaveGuild();
    void displayGuildInfo() const;
    
    // Inventory management
    bool addItem(const  string& itemName, const  string& description, int quantity = 1);
    bool useItem(const  string& itemName, Pet* targetPet = nullptr);
    void displayInventory() const;
    int getItemCount(const  string& itemName) const;
    int findItemIndex(const  string& itemName) const;
    
    // Battle related functions
    void recordWin();
    void recordLoss();
    void updateArenaRank(int rankChange);
    void displayBattleStats() const;
    
    // Training camp functions
    bool trainPet(int petIndex, const  string& attribute);
    
    // Shop functions
    bool buyItem(const  string& itemName, const  string& description, int price);
    bool sellItem(const  string& itemName, int price);
    
    // Save/Load functions
    string saveToString() const;
    void loadFromString(const  string& data);
    bool saveToFile(const  string& filename) const;
    bool loadFromFile(const  string& filename);
    
    // Operator overloading
    friend  ostream& operator<<( ostream& os, const Player& player);
    bool operator==(const Player& other) const;
    bool operator!=(const Player& other) const;
    Player& operator+=(int expPoints); // For adding experience points
};
class Guild {
private:
    string name;
    string description;
    Player* guildLeader;
    
    // Guild members
    Player* members[MAX_GUILD_MEMBERS];
    int memberCount;
    
    // Guild pets assigned for guild wars
    GuildPetAssignment guildPets[MAX_GUILD_PETS];
    int petCount;
    
    // Guild stats
    int guildRank;
    int wins;
    int losses;
    int guildPoints;

public:
    // Constructor and Destructor
    Guild(const   string& guildName, Player* leader);
    ~Guild();
    
    // Getters
      string getName() const;
      string getDescription() const;
    Player* getGuildLeader() const;
    int getMemberCount() const;
    int getPetCount() const;
    int getGuildRank() const;
    int getWins() const;
    int getLosses() const;
    int getGuildPoints() const;
    
    // Setters
    void setName(const   string& newName);
    void setDescription(const   string& desc);
    void setGuildLeader(Player* leader);
    void setGuildRank(int rank);
    
    // Member management
    bool addMember(Player* player);
    bool removeMember(Player* player);
    bool isMember(Player* player) const;
    Player* getMember(int index) const;
    void displayMembers() const;
    
    // Pet assignment
    bool assignPet(Pet* pet, PetRole role);
    bool removePet(Pet* pet);
    bool updatePetRole(Pet* pet, PetRole newRole);
    void displayAssignedPets() const;
    GuildPetAssignment* getPetAssignment(int index);
    int findPetAssignmentIndex(Pet* pet) const;
    
    // Guild war functions
    void recordWin();
    void recordLoss();
    void addGuildPoints(int points);
    bool spendGuildPoints(int points);
    void updateGuildRank(int rankChange);
    void displayGuildWarStats() const;
    
    // Team composition checks
    bool hasValidTeamComposition() const;
    int countPetsWithRole(PetRole role) const;
    
    // Save/Load functions
      string saveToString() const;
    static Guild* loadFromString(const   string& data, Player** allPlayers, int playerCount);
    bool saveToFile(const   string& filename) const;
    static Guild* loadFromFile(const   string& filename, Player** allPlayers, int playerCount);
    
    // Operator overloading
    friend   ostream& operator<<(  ostream& os, const Guild& guild);
    bool operator==(const Guild& other) const;
    bool operator!=(const Guild& other) const;
};
#endif // PLAYER_H

