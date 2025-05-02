// Abdullah
// 24I-3164
// SE-C
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
using namespace std;

#include <string>
#include "pets.h"

// Forward declaration
class Guild;

// Maximum number of pets and items a player can have
const int MAX_PETS = 10;
const int MAX_ITEMS = 20;
const int MAX_GUILD_MEMBERS = 10;
const int MAX_GUILD_PETS = 30;

const int PET_ROLE_NONE = 4;

struct GuildPetAssignment
{
    Pet *pet;
    int role; // Using pet role constants
    bool isActive;

    GuildPetAssignment() : pet(nullptr), role(PET_ROLE_NONE), isActive(false) {}
    GuildPetAssignment(Pet *p, int r) : pet(p), role(r), isActive(true) {}
};

class Player
{
private:
    string name;
    int level;
    int experience;
    int experienceToNextLevel;
    int currency;

    // Player's pet collection
    Pet *pets[MAX_PETS];
    int petCount;
    int activePetIndex; // Currently selected pet

    // Inventory for items
    struct Item
    {
        string name;
        string description;
        int quantity;

        Item() : name(""), description(""), quantity(0) {}
        Item(const string &n, const string &desc, int qty)
            : name(n), description(desc), quantity(qty) {}
    };

    Item inventory[MAX_ITEMS];
    int itemCount;

    // Guild membership
    Guild *playerGuild;

    // Battle stats
    int wins;
    int losses;
    int arenaRank;

public:
    // Constructor and Destructor
    Player() {}
    Player(const string &playerName);
    int getActivePetIndex() const { return activePetIndex; }
    
    void setActivePet(int index) {
        if (index >= 0 && index < petCount) {
            activePetIndex = index;
        }
    }
    
    int getPetCount() const { return petCount; }
    
    Pet* getPet(int index) const {
        if (index >= 0 && index < petCount) {
            return pets[index];
        }
        return nullptr;
    }
    

    // Getters
    string getName() const;
    int getLevel() const;
    int getExperience() const;
    int getCurrency() const;
    int getWins() const;
    int getLosses() const;
    int getArenaRank() const;
    Pet *getActivePet() const;
    Guild *getGuild() const;

    // Setters
    void setName(const string &newName);
    void setCurrency(int amount);
    
    void setGuild(Guild *guild);

    // Experience and currency
    void gainExperience(int amount);
    void levelUp();
    void addCurrency(int amount);
    bool spendCurrency(int amount);

    // Pet management
    bool adoptPet(const string &petType, const string &petName);
    void displayPets() const;
   
   
    void releasePet(int index);

    // Guild management
    void joinGuild(Guild *guild);
    void leaveGuild();
    void displayGuildInfo() const;

    // Inventory management
    bool addItem(const string &itemName, const string &description, int quantity = 1);
    bool useItem(const string &itemName, Pet *targetPet = nullptr);
    void displayInventory() const;
    int getItemCount(const string &itemName) const;
    int findItemIndex(const string &itemName) const;

    // Battle related functions
    void recordWin();
    void recordLoss();
    void updateArenaRank(int rankChange);
    void displayBattleStats() const;

    // Training camp functions
    bool trainPet(int petIndex, const string &attribute);

    // Shop functions
    bool buyItem(const string &itemName, const string &description, int price);
    bool sellItem(const string &itemName, int price);

    // Save/Load functions
    string saveToString() const;
    void loadFromString(const string &data);
    bool saveToFile(const string &filename) const;
    bool loadFromFile(const string &filename);

    // Operator overloading
    friend ostream &operator<<(ostream &os, const Player &player);
    bool operator==(const Player &other) const;
    bool operator!=(const Player &other) const;
    Player &operator+=(int expPoints); // For adding experience points
};
class Guild
{
private:
    string name;
    string description;
    Player *guildLeader;

    // Guild members
    Player *members[MAX_GUILD_MEMBERS];
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
    Guild(const string &guildName, Player *leader);
    ~Guild();

    // Getters
    string getName() const;
    string getDescription() const;
    Player *getGuildLeader() const;
    int getMemberCount() const;
    int getPetCount() const;
    int getGuildRank() const;
    int getWins() const;
    int getLosses() const;
    int getGuildPoints() const;

    // Setters
    void setName(const string &newName);
    void setDescription(const string &desc);
    void setGuildLeader(Player *leader);
    void setGuildRank(int rank);

    // Member management
    bool addMember(Player *player);
    bool removeMember(Player *player);
    bool isMember(Player *player) const;
    Player *getMember(int index) const;
    void displayMembers() const;

    // Pet assignment
    bool assignPet(Pet *pet, GuildPetAssignment role);
    bool removePet(Pet *pet);
    bool updatePetRole(Pet *pet, GuildPetAssignment newRole);
    void displayAssignedPets() const;
    GuildPetAssignment *getPetAssignment(int index);
    int findPetAssignmentIndex(Pet *pet) const;

    // Guild war functions
    void recordWin();
    void recordLoss();
    void addGuildPoints(int points);
    bool spendGuildPoints(int points);
    void updateGuildRank(int rankChange);
    void displayGuildWarStats() const;

    // Team composition checks
    bool hasValidTeamComposition() const;
    int countPetsWithRole(GuildPetAssignment role) const;

    // Save/Load functions
    string saveToString() const;
    static Guild *loadFromString(const string &data, Player **allPlayers, int playerCount);
    bool saveToFile(const string &filename) const;
    static Guild *loadFromFile(const string &filename, Player **allPlayers, int playerCount);

    // Operator overloading
    friend ostream &operator<<(ostream &os, const Guild &guild);
    bool operator==(const Guild &other) const;
    bool operator!=(const Guild &other) const;
};
const int NUM_LAYERS =5;
struct Layer{
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    float offset;
};
class ParallaxBackground{
    Layer layers[NUM_LAYERS];
    int counter=0;
    int width;
    int height;
    public:
    ParallaxBackground(int windowWidth, int windowHeight): width(windowWidth), height(windowHeight) {}

    bool addLayer(string fileName, float speed)
    {
        if(counter >=NUM_LAYERS)
        {
            cout<<"Max Layer Reached!"<<endl;
            return false;
        }
        if(!layers[counter].texture.loadFromFile(fileName))
        {
            cout<<"Faild to load bg!"<<endl;
            return false;
        }



        layers[counter].sprite.setTexture(layers[counter].texture);
        layers[counter].speed = speed;
        layers[counter].offset =0;

        layers[counter].texture.setRepeated(true);
        layers[counter].sprite.setTextureRect(sf::IntRect(0,0,width, height));
        
        counter++;
        return true;

    }
    void update(float time)
    {
        for(int i=0; i<counter; i++)
        {
            layers[i].offset += layers[i].speed * time;
            if(layers[i].offset > layers[i].texture.getSize().x) 
            {
                layers[i].offset -= layers[i].texture.getSize().x;
            }
            layers[i].sprite.setTextureRect(sf::IntRect(static_cast<int>(layers[i].offset), 0, width, height));
        }
    }
    void draw(sf::RenderWindow & window)
    {
        for(int i=0; i<counter; i++)
        {
            window.draw(layers[i].sprite);
        }
    }
};
const int NUM_BUTTONS = 4;
class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    bool isHovered;
    
public:
    Button(float x, float y, float width, float height, 
           sf::Font& font, std::string buttonText, 
           sf::Color idleColor = sf::Color(70, 70, 70, 200),
           sf::Color hoverColor = sf::Color(150, 150, 150, 200),
           sf::Color activeColor = sf::Color(20, 20, 20, 200));
    
    void update(const sf::Vector2f& mousePos);
    
    void render(sf::RenderTarget& target);
    
    
    bool isPressed() const;
    
    
    const sf::RectangleShape& getShape() const { return shape; }
    const sf::Text& getText() const { return text; }
};
#endif 
