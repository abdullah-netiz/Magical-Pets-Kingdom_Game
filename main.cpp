
// Name: Abdullah
// 24I-3164
// SE-C

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;

class Pet;
class Guild;
class Player;
class Battle;
class TrainingCamp;
class Shop;
class Item;
//========================classes used for SFML Graphics =====================

//To use animation for the pets
class PetAnimation{
public:
    sf::Texture texture;
    int frameWidth;
    int frameHeight;
    int animationRow;
    int numFrames;
    float scale;
    float posX;
    float posY;

    void loadDragon()
    {
        if(texture.loadFromFile("assets/Dragon/sheet.png"))
        {
            frameWidth = 191;
            frameHeight = 161;
            animationRow = 1;
            numFrames = 3;
            scale = 1.0f;
            posX = 300.0f;
            posY = 80.0f;
        }
    }

    void loadPhoenix()
    {
        if(texture.loadFromFile("assets/Phoenix/sheet.jpg"))
        {
            frameWidth = 95;
            frameHeight = 90;
            animationRow = 0;
            numFrames = 3;
            scale = 1.5f;
            posX = 300.0f;
            posY = 80.0f;
        }
    }

    void loadUnicorn()
    {
        if(texture.loadFromFile("assets/Unicorn/sheet.png"))
        {
            frameWidth = 64;
            frameHeight = 85;
            animationRow = 3;
            numFrames = 3;
            scale = 1.5f;
            posX = 300.0f;
            posY = 80.0f;
        }
    }

    void loadGriffin()
    {
        if(texture.loadFromFile("assets/Griffin/sheet.png"))
        {
            frameWidth = 64;
            frameHeight = 85;
            animationRow = 1;
            numFrames = 3;
            scale = 1.0f;
            posX = 300.0f;
            posY = 80.0f;
        }
    }
};
//to use muics in the game
class MusicPlayer{
private:
    sf::Music music;
    string currentTrack;
    float volume = 50.f;
    bool isLooping = true;

public:
    void play(const string &filename)
    {
        if(currentTrack != filename)
        {
            currentTrack = filename;
            if(music.openFromFile("sound/" + filename))
            {
                music.setLoop(isLooping);
                music.setVolume(volume);
                music.play();
            }
        }
    }

    void setVolume(float vol)
    {
        volume = clamp(vol, 0.f, 100.f);
        music.setVolume(volume);
    }

};
const int NUM_BUTTONS = 4;

// buttons class to draw buttons on scren
class Button{
private:
    sf::RectangleShape shape;
    sf::Text text;
    sf::Color idleColor;
    sf::Color hoverColor;
    sf::Color activeColor;
    sf::Color textColor;
    bool isHovered;
    bool visible; 

public:
    Button() : visible(true), isHovered(false) {}
    Button(float x, float y, float width, float height, sf::Font &font, string buttonText,sf::Color idleColor = sf::Color(15, 15, 40, 255), sf::Color hoverColor =sf::Color(10, 10, 30, 255), sf::Color activeColor= sf::Color(0, 10, 30, 255))
    : idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor), isHovered(false), visible(true)
    {
    shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(idleColor);
    shape.setOutlineThickness(1.f);
    shape.setOutlineColor(sf::Color::White);

    text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(17);
    text.setFillColor(sf::Color::White);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width / 2.0f,
                   textRect.top + textRect.height / 2.0f);
    text.setPosition(x + width / 2.0f, y + height / 2.0f);
    }

    void update(const sf::Vector2f &mousePos)
    {
        if(!visible)
            return;

        isHovered = shape.getGlobalBounds().contains(mousePos);

        if(isHovered)
        {
            shape.setFillColor(hoverColor);
            shape.setOutlineColor(sf::Color(50, 155, 255, 255));
            text.setScale(0.8f, 0.8f);
        }
        else
        {
            shape.setFillColor(idleColor);
            shape.setOutlineColor(sf::Color(255, 255, 255, 200));
            text.setScale(1.f, 1.f);
        }
    }

    void render(sf::RenderTarget &target)
    {
        if(!visible)
            return;
        target.draw(shape);
        target.draw(text);
    }

    bool isPressed() const
    {
        return isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);
    }


    void setText(const string &buttonText)
    {
        text.setString(buttonText);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,textRect.top + textRect.height / 2.0f);
        text.setPosition(shape.getPosition().x + shape.getSize().x / 2.0f,shape.getPosition().y + shape.getSize().y / 2.0f);
    }

    void setVisible(bool isVisible)
    {
        visible = isVisible;
    }

    bool isVisible() const
    {
        return visible;
    }


    sf::RectangleShape &getShape() { return shape; }
    sf::Text &getText() { return text; }
    void setColors(sf::Color selected, sf::Color hovered, sf::Color click, sf::Color text)
    {
        idleColor = selected;
        hoverColor = hovered;
        activeColor = click;
        this->textColor = text;
    }
};

const int numLayers = 7;
struct Layer{
    sf::Texture texture;
    sf::Sprite sprite;
    float speed;
    float offset;
};
//code for moving background
class ParallaxBackground
{
    Layer layers[numLayers];
    int counter = 0;
    int width;
    int height;

public:
    ParallaxBackground(int windowWidth, int windowHeight) : width(windowWidth), height(windowHeight) {}
    ParallaxBackground() {}
    void WandH(int windowWidth, int windowHeight)
    {
        width = windowWidth;
        height = windowHeight;
    }
    bool addLayer(string fileName, float speed)
    {
        if(counter >= numLayers)
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
        layers[counter].offset = 0;

        layers[counter].texture.setRepeated(true);
        layers[counter].sprite.setTextureRect(sf::IntRect(0, 0, width, height));

        counter++;
        return true;
    }
    void update(float time)
    {
        for (int i = 0; i < counter; i++)
        {
            layers[i].offset += layers[i].speed * time;
            if(layers[i].offset > layers[i].texture.getSize().x)
            {
                layers[i].offset -= layers[i].texture.getSize().x;
            }
            layers[i].sprite.setTextureRect(sf::IntRect(static_cast<int>(layers[i].offset), 0, width, height));
        }
    }
    void draw(sf::RenderWindow &window)
    {
        for (int i = 0; i < counter; i++)
        {
            window.draw(layers[i].sprite);
        }
    }
};

//========================classes used for Game=====================
class Pet{
protected:
    string name;
    int health;
    int maxHealth;
    int attack;
    int speed;
    int defense;
    int level;
    int xpPoints;
    bool isAlive;
    int Id;

public:
    Pet(string petName, int hp, int atk, int spd, int def)
    {
        name = petName;
        maxHealth = hp;
        health = maxHealth;
        attack = atk;
        speed = spd;
        defense = def;
        level = 1;
        xpPoints = 0;
        isAlive = true;
        Id = 0;
    }
    virtual ~Pet() {}
    string getName() const { return name; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getAttack() const { return attack; }
    int getSpeed() const { return speed; }

    int getDefense() const { return defense; }
    int getLevel() const { return level; }
    bool getIsAlive() const { return isAlive; }
    
    void takeDamage(int damage)
    {
        int actualDamage = damage - (defense / 4);
        if(actualDamage < 1)
            actualDamage = 1;
        health -= actualDamage;
        cout<<name << " takes " << actualDamage << " damage!"<<endl;

        if(health <= 0)
        {
            health = 0;
            isAlive = false;
            cout<<name << " has died!"<<endl;
        }
    }

    void heal(int amount)
    {
        health += amount;
        if(health > maxHealth)
        {
            health = maxHealth;
        }
        cout<<name << " healed "<<amount<< " HP!"<<endl;
    }

    void revive()
    {
        health = maxHealth / 2;
        isAlive = true;
        cout<<name<< " has revived!"<<endl;
    }

    void gainExperience(int exp)
    {
        xpPoints += exp;
        cout<<name << " gained "<<exp<<" exp points!"<<endl;
        if(xpPoints >= level * 100)
        {
            levelUp();
        }
    }
    void setHealth(int newHealth)
    {
        health = newHealth;
        if(health <= 0)
        {
            health = 0;
            isAlive = false;
        }
        else if(health > maxHealth)
        {
            health = maxHealth;
        }
        isAlive = health > 0;
    }

    void levelUp()
    {
        level++;
        maxHealth += 10;
        attack += 5;
        speed += 3;
        defense += 4;
        health = maxHealth;

        cout<<"\n";
        cout<< name << " leveled up to level " << level << "!"<<endl;
        cout<<"Stats BOOSTED: HP+" << 10 << ", ATK+" << 5;
        cout<<", SPD+" << 3 << ", DEF+" << 4<<endl;
    }
    bool setIsAlive(bool alive)
    {
        isAlive = alive;
        return isAlive;
    }
    int getStrengthScore() const
    {
        return health+attack*2+defense;
    }
    bool operator>(const Pet &other) const
    {
        return getStrengthScore() > other.getStrengthScore();
    }
    virtual void displayStats() const
    {
        cout<<endl;
        cout<<"===== " << name << "'s Stats ====="<<endl;
        cout<<"Type: Generic Pet"<<endl;
        cout<<"Level: " << level<<endl;
        cout<<"HP: " <<health << "/" << maxHealth<<endl;
        cout<<"Attack: "  <<attack<<endl;
        cout<<"Speed: " <<speed<<endl;
        cout<<"Defense: " <<defense<<endl;
        cout<<"EXP: " <<xpPoints << "/" << (level * 100)<<endl;
    }

    virtual int useBasicAttack(Pet *target)
    {
        int damage = (attack * 0.05) + (rand() % (attack / 4));
        cout<<name << " attacks " << target->getName() << "!"<<endl;
        target->takeDamage(damage);
        return damage;
    }

    // pure virtual functions for every pet type dragon , unicorn etc
    virtual void useAbility1(Pet *target) = 0;
    virtual void useAbility2(Pet *target) = 0;
    virtual string getType() const = 0;
    virtual string getAbility1() const = 0;
    virtual string getAbility2() const = 0;
};

class Dragon : public Pet{
private:
    int fireballPower;
    int roarStrength;

public:
    Dragon(string petName) : Pet(petName, 150, 75, 40, 60)
    {
        fireballPower = 15;
        roarStrength = 10;
    }

    string getType() const override
    {
        return "Dragon";
    }

    void useAbility1(Pet *target) override
    {
        cout<<name << " used Fireball Attack on " << target->getName() << "!"<<endl;
        int damage = (attack + fireballPower) * (rand() % 21 + 55) / 100;
        target->takeDamage(damage);
    }

    void useAbility2(Pet *target) override
    {
        cout<<name << " used Dragon's Roar stunning enemies!"<<endl;
        int damage = roarStrength * (rand() % 16 + 70) / 100;
        target->takeDamage(damage);
        cout<<target->getName() << " is stunned!"<<endl;
    }

    string getAbility1() const override
    {
        return "Fireball Attack";
    }

    string getAbility2() const override
    {
        return "Dragon's Roar";
    }

    void displayStats() const override
    {
        cout<<endl;
        cout<<"===== " << name<< "'s Stats ====="<<endl;
        cout<<"Type: Dragon"<<endl;
        cout<<"Level: " << level<<endl;
        cout<<"HP: " << health<< "/" << maxHealth<<endl;
        cout<<"Attack: " << attack<<endl;
        cout<<"Speed: " << speed<<endl;
        cout<<"Defense: " << defense<<endl;
        cout<<"Fireball Power: " << fireballPower<<endl;
        cout<<"Roar Strength: " << roarStrength<<endl;
        cout<<"EXP: " << xpPoints << "/" << (level * 100)<<endl;
    }

    void levelUp()
    {
        Pet::levelUp();
        fireballPower += 3;
        roarStrength += 2;
        cout<<"Fireball Power+" << 3 << ", Roar Strength+" << 2<<endl;
    }
};

class Phoenix : public Pet
{
private:
    int healPower;
    bool rebirth;

public:
    Phoenix(string petName) : Pet(petName, 120, 50, 30, 85)
    {
        healPower = 20;
        rebirth = true;
    }

    string getType() const override
    {
        return "Phoenix";
    }

    void useAbility1(Pet *target) override
    {
        cout<<name << " used Flame Healing on ";
        if(target == this)
        {
            cout<<"itself!"<<endl;
        }
        else
        {
            cout<<target->getName() << "!"<<endl;
        }

        int healAmount = healPower + (level * 3);
        target->heal(healAmount);
    }

    void useAbility2(Pet *target) override
    {
        if(rebirth)
        {
            cout<<name << " used Rebirth!"<<endl;
            health = maxHealth;
            isAlive = true;
            rebirth = false;
            cout<<name << " fully restored!"<<endl;
        }
        else
        {
            cout<<name << " tried Rebirth but already used in battle!"<<endl;
        }
    }

    string getAbility1() const override
    {
        return "Flame Healing";
    }

    string getAbility2() const override
    {
        return "Rebirth";
    }

    void resetRebirth()
    {
        rebirth = true;
    }

    void displayStats() const override
    {
        cout<<endl;
        cout<<"===== " << name << "'s Stats ====="<<endl;
        cout<<"Type: Phoenix"<<endl;
        cout<<"Level: " << level<<endl;
        cout<<"HP: " << health << "/" << maxHealth<<endl;
        cout<<"Attack: " << attack<<endl;
        cout<<"Speed: " << speed<<endl;
        cout<<"Defense: " << defense<<endl;
        cout<<"Heal Power: " << healPower<<endl;
        cout<<"Rebirth Available: " << (rebirth ? "Yes" : "No")<<endl;
        cout<<"EXP: " << xpPoints << "/" <<(level * 100)<<endl;
    }

    void levelUp()
    {
        Pet::levelUp();
        healPower += 5;
        cout<<"Heal Power+" << 5<<endl;
    }
};

class Unicorn : public Pet
{
private:
    int speedBoost;
    int shieldStrength;

public:
    Unicorn(string petName) : Pet(petName, 100, 55, 85, 50)
    {
        speedBoost = 15;
        shieldStrength = 20;
    }

    string getType() const override
    {
        return "Unicorn";
    }

    void useAbility1(Pet *target) override
    {
        cout<<name << " used Speed Burst!"<<endl;
        speedBoost = true;
        cout<<name << "'s speed temporarily increased!"<<endl;
    }

    void useAbility2(Pet *target) override
    {
        cout<<name << " used Magic Shield!"<<endl;
        defense += shieldStrength;
        cout<<name << "'s defense temporarily increased!"<<endl;
    }

    string getAbility1() const override
    {
        return "Speed Burst";
    }

    string getAbility2() const override
    {
        return "Magic Shield";
    }
// when pets get temprory buffs , to restore them
    void resetTemporaryBuffs()
    {
        speed = attack * level / 2 + 85;   
        defense = attack * level / 3 + 50; 
    }

    void displayStats() const override
    {
        cout<<endl;
        cout<<"===== " << name << "'s Stats ====="<<endl;
        cout<<"Type: Unicorn"<<endl;
        cout<<"Level: " << level<<endl;
        cout<<"HP: " << health << "/" << maxHealth<<endl;
        cout<<"Attack: " << attack<<endl;
        cout<<"Speed: " << speed<<endl;
        cout<<"Defense: "<< defense<<endl;
        cout<<"Speed Boost: "<<speedBoost<<endl;
        cout<<"Shield Strength: " <<shieldStrength<<endl;
        cout<<"EXP: " <<xpPoints<<"/" << (level * 100)<<endl;
    }

    void levelUp()
    {
        Pet::levelUp();
        speedBoost += 3;
        shieldStrength += 4;
        cout<<"Speed Boost+" << 3 << ", Shield Strength+" << 4<<endl;
    }
};

class Griffin : public Pet{
private:
    int clawPower;
    int divePower;

public:
    Griffin(string petName) : Pet(petName, 130, 65, 60, 55)
    {
        clawPower = 12;
        divePower = 15;
    }

    string getType() const override
    {
        return "Griffin";
    }

    void useAbility1(Pet *target) override
    {
        cout<<name << " used Claw on " << target->getName() << "!"<<endl;
        int damage = (attack + clawPower) * (rand() % 21 + 65) / 100;
        target->takeDamage(damage);
    }

    void useAbility2(Pet *target) override
    {
        cout<<name << " used Dive Bomb on " << target->getName() << "!"<<endl;
        int damage = (attack + divePower) * (rand() % 21 + 65) / 100;
        target->takeDamage(damage);
        cout<<target->getName() << " stunned by dive bomb"<<endl;
    }

    string getAbility1() const override
    {
        return "Claw Strike";
    }

    string getAbility2() const override
    {
        return "Dive Bomb";
    }

    void displayStats() const override
    {
        cout<<endl;
        cout<<"===== " << name << "'s Stats ====="<<endl;
        cout<<"Type: Griffin"<<endl;
        cout<<"Level: " << level<<endl;
        cout<<"HP: " << health << "/" << maxHealth<<endl;
        cout<<"Attack: " << attack<<endl;
        cout<<"Speed: " << speed<<endl;
        cout<<"Defense: " << defense<<endl;
        cout<<"Claw Power: " << clawPower<<endl;
        cout<<"Dive Power: " << divePower<<endl;
        cout<<"EXP: " << xpPoints << "/" << (level * 100)<<endl;
    }

    void levelUp()
    {
        Pet::levelUp();
        clawPower += 3;
        divePower += 4;
        cout<<"Claw Power+" << 3 << ", Dive Power+" << 4<<endl;
    }
};

//class for all the items in the game
class Item{
protected:
    string name;
    string description;
    int price;

public:
    Item(string itemName, string itemDesc, int itemPrice)
    {
        name = itemName;
        description = itemDesc;
        price = itemPrice;
    }

    virtual ~Item() {}

    string getName() const
    {
        return name;
    }

    string getDescription() const
    {
        return description;
    }

    int getPrice() const
    {
        return price;
    }

    virtual void use(Pet *target) = 0;
    virtual string getType() const = 0;
};

// heals
class HealingPotion : public Item
{
private:
    int healAmount;

public:
    HealingPotion() : Item("Healing Potion", "Restore 50 HP to a pet", 50)
    {
        healAmount = 50;
    }

    void use(Pet *target) override
    {
        cout<<target->getName() << " used healing potion!"<<endl;
        target->heal(healAmount);
    }

    string getType() const override
    {
        return "Healing";
    }
};

// mana potion item
class ManaPotion : public Item{
public:
    ManaPotion() : Item("Mana Potion", "Allows freq use of abilities", 75) {}

    void use(Pet *target) override
    {
        cout<<target->getName() << " used a Mana Potion!"<<endl;
        cout<<"Abilities can be used immediately!"<<endl;
    }

    string getType() const override
    {
        return "Mana";
    }
};


class Shield : public Item{
private:
    int defenseBoost;

public:
    Shield() : Item("Shield", "Increases defense by 20", 100)
    {
        defenseBoost = 20;
    }

    void use(Pet *target) override
    {
        cout<<target->getName() <<" used a Shield!"<<endl;
        cout<<target->getName() <<"'s defense increased by " <<defenseBoost<<endl;
    }

    string getType() const override
    {
        return "Shield";
    }
};

class Guild{
private:
    string name;
    string description;
    Pet *members[10];
    int memberCount;
    const int maxMembers = 10;
    int guildLevel;
    string emblemPath; 
    int guildID;   

public:
    Guild(const string &guildName, const string &guildDesc = "", int id = 0)
        : name(guildName), description(guildDesc), memberCount(0), guildLevel(1), emblemPath("assets/emb.png"), guildID(id)
    {
        for (int i = 0; i < maxMembers; i++)
        {
            members[i] = nullptr;
        }
    }

    ~Guild() {}

    // getters
    const string &getName() const { return name; }
    const string &getDescription() const { return description; }
    int getGuildLevel() const { return guildLevel; }
    const string &getEmblemPath() const { return emblemPath; }
    int getMemberCount() const { return memberCount; }
    int getMaxMembers() const { return maxMembers; }
    int getGuildID() const { return guildID; }

    // setters
    void setName(const string &newName) { name = newName; }
    void setDescription(const string &newDesc) { description = newDesc; }
    void setEmblemPath(const string &path) { emblemPath = path; }
    void increaseGuildLevel() { guildLevel++; }
    void setGuildID(int id) { guildID = id; }


    bool addMember(Pet *pet)
    {
        if(!pet)
            return false;

        for (int i = 0; i < memberCount; i++)
        {
            if(members[i] == pet)
            {
                return false;
            }
        }

        if(memberCount < maxMembers)
        {
            members[memberCount++] = pet;
            return true;
        }
        return false;
    }

    bool removeMember(int index)
    {
        if(index >= 0 && index < memberCount)
        {
            for (int i = index; i < memberCount - 1; i++)
            {
                members[i] = members[i + 1];
            }
            members[--memberCount] = nullptr; 
            return true;
        }
        return false;
    }

    bool removeMemberByPet(Pet *pet)
    {
        if(!pet)
            return false;

        for (int i = 0; i < memberCount; i++)
        {
            if(members[i] == pet)
            {
                return removeMember(i);
            }
        }
        return false; // Pet not found
    }

    Pet *getMember(int index) const
    {
        return (index >= 0 && index < memberCount) ? members[index] : nullptr;
    }

    bool isMember(Pet *pet) const
    {
        if(!pet)
            return false;

        for (int i = 0; i < memberCount; i++)
        {
            if(members[i] == pet)
            {
                return true;
            }
        }
        return false;
    }

    bool isFull() const
    {
        return memberCount >= maxMembers;
    }

    // 
    int guildPower() const
    {
        int totalPower = 0;
        for (int i = 0; i < memberCount; i++)
        {
            if(members[i])
            {
                totalPower =totalPower+ members[i]->getLevel() * 10; 
            }
        }
        return totalPower;
    }

};
// player class to manage game state
class Player{
private:
    string name;
    int gold;
    Pet **pets;
    int petCount;
    int maxPets;
    Guild **guilds;
    int guildCount;
    int maxGuilds;
    Item **inventory;
    int inventoryCount;
    int maxInventory;
    string password;
    int wins;
    int losses;

public:
    Player(string playerName, string pass = "")
    {
        name = playerName;
        gold = 0;
        maxPets = 20;
        petCount = 0;
        pets = new Pet *[maxPets];

        password = pass;
        wins = 0;
        losses = 0;

        maxGuilds = 5;
        guildCount = 0;
        guilds = new Guild *[maxGuilds];

        maxInventory = 10;
        inventoryCount = 0;
        inventory = new Item *[maxInventory];

        for (int i = 0; i < maxPets; i++)
            pets[i] = nullptr;

        for (int i = 0; i < maxGuilds; i++)
            guilds[i] = nullptr;

        for (int i = 0; i < maxInventory; i++)
            inventory[i] = nullptr;
    }

    ~Player()
    {
        for (int i = 0; i < petCount; i++)
        {
            if(pets[i] != nullptr)
                delete pets[i];
        }
        delete[] pets;


        for (int i = 0; i < guildCount; i++)
        {
            if(guilds[i] != nullptr)
                delete guilds[i];
        }
        delete[] guilds;

        for (int i = 0; i < inventoryCount; i++)
        {
            if(inventory[i] != nullptr)
                delete inventory[i];
        }
        delete[] inventory;
    }
    string getName() const { return name; }
    string getPassword() const { return password; }
    int getWins() const { return wins; }
    int getLosses() const { return losses; }
    void incrementWins() { wins++; }
    void incrementLosses() { losses++; }
    void setGold(int amount)
    {
        gold = amount;
    }
    int getGold() const { return gold; }
    void addGold(int amount)
    {
        gold += amount;
    }
    bool spendGold(int amount)
    {
        if(amount <= gold)
        {
            gold -= amount;
            return true;
        }
        else
        {
            cout<<"Not enough gold!"<<endl;
            return false;
        }
    }
    int getInventoryCount() const { return inventoryCount; }

    Item *getInventoryItem(int index)
    {
        if(index >= 0 && index < inventoryCount)
        {
            return inventory[index];
        }
        return nullptr;
    }
    bool addPet(Pet *pet)
    {
        if(petCount < maxPets)
        {
            pets[petCount] = pet;
            petCount++;
            cout<<pet->getName() << " added to your collection!"<<endl;
            return true;
        }
        else
        {
            cout<<"Max pet limit reached!"<<endl;
            return false;
        }
    }

    Pet *getPet(int index) const
    {
        if(index >= 0 && index < petCount)
        {
            return pets[index];
        }
        else
        {
            return nullptr;
        }
    }
    string getPetByName(int index) const
    {

        if(pets[index]->getType() == "Phoenix")
        {
            return "Phoenix";
        }
        else if(pets[index]->getType() == "Dragon")
        {
            return "Dragon";
        }
        else if(pets[index]->getType() == "Griffin")
        {
            return "Griffin";
        }
        else if(pets[index]->getType() == "Unicorn")
        {
            return "Unicorn";
        }

        return "";
    }
    int getPetCount() const
    {
        return petCount;
    }

    bool createGuild(string guildName)
    {
        if(guildCount < maxGuilds)
        {
            guilds[guildCount] = new Guild(guildName);
            guildCount++;
            cout<<"Guild " << guildName << " created!"<<endl;
            return true;
        }
        else
        {
            cout<<"Max guild limit reached! Cannot create more guilds."<<endl;
            return false;
        }
    }

    Guild *getGuild(int index) const
    {
        if(index >= 0 && index < guildCount)
        {
            return guilds[index];
        }
        else
        {
            return nullptr;
        }
    }

    int getGuildCount() const
    {
        return guildCount;
    }

    bool addItemToInventory(Item *item)
    {
        if(inventoryCount < maxInventory)
        {
            inventory[inventoryCount] = item;
            inventoryCount++;
            cout<<item->getName() << " added to inventory!"<<endl;
            return true;
        }
        else
        {
            cout<<"Inventory is full!"<<endl;
            return false;
        }
    }

    Item *useItem(int index, Pet *target)
    {
        if(index >= 0 && index < inventoryCount)
        {
            Item *item = inventory[index];
            item->use(target);
            cout<<item->getName() << " used on " << target->getName() << "!"<<endl;

            // Remove item from inventory
            delete item;
            for (int i = index; i < inventoryCount - 1; i++)
            {
                inventory[i] = inventory[i + 1];
            }
            inventory[inventoryCount - 1] = nullptr;
            inventoryCount--;

            return item;
        }
        else
        {
            cout<<"Invalid item index!"<<endl;
            return nullptr;
        }
    }
  
};

class GuildMenu{
private:
    sf::RenderWindow &window;
    sf::Font &font;
    Player *player;
    Guild *currentGuild;
    ParallaxBackground bg;

    sf::Texture emblemTexture;

    sf::Texture boardTexture;
    sf::Sprite emblemSprite;
    sf::Sprite boardSprite;
    sf::RectangleShape panelBg;

    sf::Text titleText;
    sf::Text descriptionText;
    sf::Text statsText;
    sf::Text memberTexts[10];
    Button memberButtons[10];
    Button addButton;
    Button removeButton;
    Button createButton;
    Button backButton;
    sf::Text statusText;
    int selectedIndex;
    int currentGuildIndex;

    bool isEditing;
    string inputBuffer;
    sf::Text inputText;

    float animationTimer;
    bool showingPopup;
    sf::RectangleShape popup;
    sf::Text popupText;
    bool close = false;

    sf::Text strongestPetText;
    int selectedBtn; 

public:
    GuildMenu(sf::RenderWindow &win, sf::Font &f, Player *p) : window(win),font(f),player(p),currentGuild(nullptr),selectedIndex(-1),currentGuildIndex(0),isEditing(false),animationTimer(0.f),showingPopup(false),selectedBtn(0)
    {

        bg.WandH(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);

        if(!boardTexture.loadFromFile("assets/board.png"))
        {
            cout<<"Failed to load board" << endl;
        }
    //all the sprite and text for screen
        boardSprite.setTexture(boardTexture);
        boardSprite.setPosition(20, 10);
        boardSprite.setScale(1.5f, 1.3f);

        panelBg.setSize(sf::Vector2f(536, 284));
        panelBg.setPosition(20, 20);
        panelBg.setFillColor(sf::Color(20, 20, 50, 200));
        panelBg.setOutlineColor(sf::Color(100, 100, 200));
        panelBg.setOutlineThickness(2);

        titleText.setFont(font);
        titleText.setCharacterSize(24);
        titleText.setPosition(170, 30);
        titleText.setFillColor(sf::Color::White);
        titleText.setOutlineColor(sf::Color::Black);
        titleText.setOutlineThickness(1);

        descriptionText.setFont(font);
        descriptionText.setCharacterSize(12);
        descriptionText.setPosition(170, 60);
        descriptionText.setFillColor(sf::Color(200, 200, 200));

        statsText.setFont(font);
        statsText.setCharacterSize(12);
        statsText.setPosition(420, 160);
        statsText.setFillColor(sf::Color(180, 180, 255));

        emblemSprite.setPosition(40, 30);
        emblemSprite.setScale(0.5f, 0.5f);

        for (int i = 0; i < 10; i++)
        {
            memberButtons[i] = Button(40, 90 + i * 20, 200, 18, font, "");
            memberTexts[i].setFont(font);
            memberTexts[i].setCharacterSize(12);
            memberTexts[i].setPosition(45, 92 + i * 20);
            memberTexts[i].setFillColor(sf::Color::White);
        }

        
        addButton = Button(400, 90, 120, 25, font, "Add Member");
        removeButton = Button(400, 120, 120, 25, font, "Remove Member");
        createButton = Button(200, 150, 150, 30, font, "Create Guild");
        backButton = Button(400, 270, 120, 25, font, "Back");


        statusText.setFont(font);
        statusText.setCharacterSize(12);
        statusText.setPosition(40, 210);
        statusText.setFillColor(sf::Color(200, 200, 100));
        statusText.setOutlineColor(sf::Color::Black);
        statusText.setOutlineThickness(0.5);

        inputText.setFont(font);
        inputText.setCharacterSize(14);
        inputText.setPosition(170, 190); 
        inputText.setFillColor(sf::Color::Yellow);

        
        popup.setSize(sf::Vector2f(240, 120));
        popup.setPosition(170, 100);
        popup.setFillColor(sf::Color(40, 40, 80, 230));
        popup.setOutlineColor(sf::Color(100, 100, 200));
        popup.setOutlineThickness(2);

        popupText.setFont(font);
        popupText.setCharacterSize(14);
        popupText.setPosition(180, 110);
        popupText.setFillColor(sf::Color::White);

        strongestPetText.setFont(font);
        strongestPetText.setCharacterSize(12);
        strongestPetText.setPosition(420, 140);
        strongestPetText.setFillColor(sf::Color(180, 180, 255));

        // load player guild if exists
        if(player->getGuildCount() > 0)
        {
            currentGuild = player->getGuild(0);
            loadGuildEmblem();
        }
    }

    void run()
    {
        sf::Clock clock;

        while (window.isOpen() && !close)
        {
            float deltaTime = clock.restart().asSeconds();
            animationTimer += deltaTime;

            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }

                if(isEditing)
                {
                    handleTextInput(event);
                }
                else
                {
                    handleInput(event);
                }
            }

            updateDisplay();
            render(deltaTime);
        }
    }

private:
    void handleInput(sf::Event &event)
    {
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Escape && !showingPopup)
            {
                close = true;
                return; 
            }

            if(!currentGuild && !showingPopup)
            {
                // keyboard navigation input for both button
                if(event.key.code == sf::Keyboard::Up)
                {
                    selectedBtn = (selectedBtn - 1 + 2) % 2;
                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    selectedBtn = (selectedBtn + 1) % 2;
                }
                else if(event.key.code == sf::Keyboard::Enter)
                {
                    if(selectedBtn == 0 && player->getGuildCount() == 0)
                    {
                        startEditing("Enter guild name:", true);
                    }
                    else if(selectedBtn == 1)
                    {
                        close = true;
                        return; 
                    }
                }
            }
            else if(currentGuild && !showingPopup)
            {
                // navigation for Add, Remove ,Back button
                if(event.key.code == sf::Keyboard::Up)
                {
                    selectedBtn = (selectedBtn - 1 + 3) % 3; 
                }
                else if(event.key.code == sf::Keyboard::Down)
                {
                    selectedBtn = (selectedBtn + 1) % 3;
                }
                else if(event.key.code == sf::Keyboard::Enter)
                {
                    if(selectedBtn == 0)
                    { 
                        showPetSelectionDialog();
                    }
                    else if(selectedBtn == 1 && currentGuild->getMemberCount() > 1)
                    { 
                        if(selectedIndex >= 0)
                        {
                            if(currentGuild->removeMember(selectedIndex))
                            {
                                showPopup("Member removed from guild!");
                                selectedIndex = -1;
                            }
                        }
                    }
                    else if(selectedBtn == 2)
                    {
                        close = true; 
                        return;       
                    }
                }
            }
        }

        // member selection with keyboard btns
        if(event.type == sf::Event::KeyPressed && currentGuild && !showingPopup && !isEditing)
        {
            if(event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
            {
                if(currentGuild->getMemberCount() > 0)
                {
                    selectedIndex = (selectedIndex + 1) % currentGuild->getMemberCount();
                    showMemberDetails(selectedIndex);
                }
            }
        }

//for popup
        if(event.type == sf::Event::KeyPressed && showingPopup)
        {
            if(event.key.code == sf::Keyboard::Enter)
            {
                showingPopup = false;
            }
        }
    }

    void handleTextInput(sf::Event &event)
    {
        if(event.type == sf::Event::TextEntered)
        {
            //backspace
            if(event.text.unicode == 8)
            { 
                if(!inputBuffer.empty())
                {
                    inputBuffer.pop_back();
                }
            }
            //pressing enter
            else if(event.text.unicode == 13)
            { 
                finishEditing();
            }
            else if(event.text.unicode >= 32 && event.text.unicode < 128)
            {
                inputBuffer += static_cast<char>(event.text.unicode);
            }
            inputText.setString(inputBuffer);
        }
        else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        {
            isEditing = false;
            inputBuffer.clear();
            inputText.setString("");
            statusText.setString("");
        }
    }

    void startEditing(const string &prompt, bool isCreating)
    {
        isEditing = true;
        inputBuffer.clear();
        inputText.setString("");
        statusText.setString(prompt);
    }

    void finishEditing()
    {
        if(inputBuffer.empty())
        {
            statusText.setString("Name cannot be empty!");
            isEditing = true; 
            return;
        }

        isEditing = false;

        if(player->getGuildCount() == 0)
        {
            // creating new guild if zero
            if(player->createGuild(inputBuffer))
            {
                currentGuild = player->getGuild(0);
                currentGuildIndex = 0;
                loadGuildEmblem();
                showPopup("Guild \"" + inputBuffer + "\" created!");
                statusText.setString(""); 

                if(player->getPetCount() > 0)
                {
                    currentGuild->addMember(player->getPet(0));
                }
            }
            else
            {
                statusText.setString("Failed to create guild!");
            }
        }
        inputBuffer.clear();
        inputText.setString("");
    }
    void loadGuildEmblem()
    {
        if(!currentGuild)
            return;

        if(!emblemTexture.loadFromFile(currentGuild->getEmblemPath()))
        {
            if(!emblemTexture.loadFromFile("assets/emb.png"))
            {
                cout<<"Failed to load emblem texture!" << endl;
            }
        }
        emblemSprite.setTexture(emblemTexture);
    }

    void showPetSelectionDialog()
    {
        if(!currentGuild)
        {
            showPopup("No guild selected!");
            return;
        }

        if(currentGuild->isFull())
        {
            showPopup("Guild is full! (Max " + to_string(currentGuild->getMaxMembers()) + " members)");
            return;
        }

        bool added = false;
        for (int i = 0; i < player->getPetCount(); i++)
        {
            Pet *pet = player->getPet(i);
            if(pet && !currentGuild->isMember(pet))
            {
                if(currentGuild->addMember(pet))
                {
                    showPopup("Added " + pet->getName() + " to the guild!");
                    added = true;
                    break; // Add one pet and exit loop
                }
            }
        }

        if(!added)
        {
            showPopup("No eligible pets available to add!");
        }
    }

    void showMemberDetails(int index)
    {
        if(!currentGuild)
            return;

        Pet *pet = currentGuild->getMember(index);
        if(pet)
        {
            string details = "Name: " + pet->getName() + "\n" +
                                  "Type: " + pet->getType() + "\n" +
                                  "Level: " + to_string(pet->getLevel()) + "\n" +
                                  "HP: " + to_string(pet->getHealth()) + "/" +
                                  to_string(pet->getMaxHealth());

            showPopup(details);
        }
    }

    void showPopup(const string &text)
    {
        showingPopup = true;
        popupText.setString(text);

        sf::Vector2f windowCenter = sf::Vector2f(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
        sf::FloatRect popupBounds = popup.getGlobalBounds();
        popup.setPosition(windowCenter.x - popupBounds.width / 2, windowCenter.y - popupBounds.height / 2);

        sf::FloatRect textBounds = popupText.getLocalBounds();
        popupText.setPosition(
            popup.getPosition().x + (popup.getSize().x - textBounds.width) / 2,
            popup.getPosition().y + 20);
    }

    void updateDisplay()
    {
        if(currentGuild)
        {
            titleText.setString(currentGuild->getName());
            descriptionText.setString(currentGuild->getDescription().empty() ? "No description" : currentGuild->getDescription());

            string stats = "Guild Level: " + to_string(currentGuild->getGuildLevel()) + "\n" +"Members: " + to_string(currentGuild->getMemberCount()) + "/" +to_string(currentGuild->getMaxMembers()) + "\n" +"Guild Power: " + to_string(currentGuild->guildPower());
            statsText.setString(stats);
            Pet* strongestPet = nullptr;
            for(int i=0; i<currentGuild->getMemberCount(); i++)
            {
                Pet* member = currentGuild->getMember(i);
                if(member && (!strongestPet || (*member > *strongestPet)))
                {
                    strongestPet = member;
                }
            }
            strongestPetText.setString(strongestPet ? "Strongest: "+ strongestPet->getName(): "Strongest :None");


            for (int i = 0; i < 10; i++)
            {
                Pet *member = currentGuild->getMember(i);
                if(member)
                {
                    string text = member->getName() + " (Lv." + to_string(member->getLevel()) +" " + member->getType() + ")";
                    memberButtons[i].setText(text);
                }
                else
                {
                    memberButtons[i].setText(i < currentGuild->getMaxMembers() ? "Empty Slot" : "");
                }

                if(i == selectedIndex)
                {
                    memberButtons[i].setColors(
                        sf::Color(100, 100, 180, 220),
                        sf::Color(120, 120, 200, 240),
                        sf::Color(140, 140, 220, 255),
                        sf::Color::White);
                }
                else
                {
                    memberButtons[i].setColors(
                        sf::Color(60, 60, 100, 180),
                        sf::Color(80, 80, 150, 200),
                        sf::Color(100, 100, 180, 220),
                        sf::Color(200, 200, 255));
                }
            }

            removeButton.setVisible(currentGuild->getMemberCount() > 1);

            addButton.getShape().setFillColor(selectedBtn == 0 ? sf::Color(10, 20, 60, 255) : sf::Color(15, 15, 40, 255));
            addButton.getText().setFillColor(selectedBtn == 0 ? sf::Color::Cyan : sf::Color::White);

            removeButton.getShape().setFillColor(selectedBtn == 1 ? sf::Color(10, 20, 60, 255) : sf::Color(15, 15, 40, 255));
            removeButton.getText().setFillColor(selectedBtn == 1 ? sf::Color::Cyan : sf::Color::White);

            backButton.getShape().setFillColor(selectedBtn == 2 ? sf::Color(10, 20, 60, 255) : sf::Color(15, 15, 40, 255));
            backButton.getText().setFillColor(selectedBtn == 2 ? sf::Color::Cyan : sf::Color::White);
        }
        else
        {
            createButton.getShape().setFillColor(selectedBtn == 0 ? sf::Color(10, 20, 60, 255) : sf::Color(15, 15, 40, 255));
            createButton.getText().setFillColor(selectedBtn == 0 ? sf::Color::Cyan : sf::Color::White);
            backButton.getShape().setFillColor(selectedBtn == 1 ? sf::Color(10, 20, 60, 255) : sf::Color(15, 15, 40, 255));
            backButton.getText().setFillColor(selectedBtn == 1 ? sf::Color::Cyan : sf::Color::White);
        }
    }

    void render(float deltaTime)
    {
        window.clear();

        bg.update(deltaTime);
        bg.draw(window);

        window.draw(panelBg);
        window.draw(boardSprite);

        window.draw(titleText);

        if(currentGuild)
        {
            window.draw(emblemSprite);
            window.draw(descriptionText);
            window.draw(statsText);
            window.draw(strongestPetText);

            for (int i = 0; i < 10; i++)
            {
                memberButtons[i].render(window);
            }

            addButton.render(window);
            if(currentGuild->getMemberCount() > 1)
            {
                removeButton.render(window);
            }
        }
        else
        {
            createButton.render(window);
        }

        backButton.render(window);
        window.draw(statusText);

        if(isEditing)
        {
            sf::RectangleShape inputBg;
            inputBg.setSize(sf::Vector2f(300, 30));
            inputBg.setPosition(160, 185);
            inputBg.setFillColor(sf::Color(0, 0, 0, 150));
            inputBg.setOutlineColor(sf::Color::Yellow);
            inputBg.setOutlineThickness(1);

            window.draw(inputBg);
            window.draw(inputText);

            if(static_cast<int>(animationTimer * 2) % 2 == 0)
            {
                sf::RectangleShape cursor;
                cursor.setSize(sf::Vector2f(2, 20));
                cursor.setPosition(170 + inputText.getLocalBounds().width + 5, 190);
                cursor.setFillColor(sf::Color::Yellow);
                window.draw(cursor);
            }
        }

        if(showingPopup)
        {
            window.draw(popup);
            window.draw(popupText);
        }

        window.display();
    }
};
//shop class that has array of three items
class Shop{
private:
    sf::RenderWindow &window;
    sf::Font &font;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    Player *player;
    ParallaxBackground bg;
    Item *items[3];
    int itemCount;

    sf::Text titleText;
    sf::Text goldText;
    sf::Text itemNameTexts[3];
    sf::Text itemDescTexts[3];
    sf::Text itemPriceTexts[3];
    Button buyButtons[3];
    Button exitButton;
    sf::Texture board;
    sf::Sprite brd;
    
    sf::Sprite itemSprites[3];
    sf::Texture itemTextures[3];

    bool shouldReturnToMenu = false;

public:
    Shop(sf::RenderWindow &win, sf::Font &f, Player *p) : window(win), font(f), player(p), itemCount(3), shouldReturnToMenu(false)
    {


        bg.WandH(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);

        board.loadFromFile("assets/board.png");
        brd.setTexture(board);
        brd.setPosition(10, 15);
        brd.setScale(2.3f, 1.2f);

        buffer.loadFromFile("assets/sound/coin.wav");
        sound.setBuffer(buffer);
        sound.setVolume(50);
        sound.setPitch(1.5f);
    
        items[0] = new HealingPotion();
        items[1] = new ManaPotion();
        items[2] = new Shield();

//laoding assets for items
        itemTextures[0].loadFromFile("assets/p1.png");
        itemTextures[1].loadFromFile("assets/p2.png");
        itemTextures[2].loadFromFile("assets/p3.png");


        titleText.setString("ITEM SHOP");
        titleText.setFont(font);
        titleText.setCharacterSize(30);
        titleText.setPosition(210, 10);
        titleText.setOutlineColor(sf::Color(10, 20, 60, 255));
        titleText.setOutlineThickness(2);
        titleText.setFillColor(sf::Color::White);


        for (int i = 0; i < itemCount; i++)
        {
            itemSprites[i].setTexture(itemTextures[i]);
            itemSprites[i].setScale(2.9f, 2.9f);
            itemSprites[i].setPosition(80 + i * 160, 60);

            itemNameTexts[i].setFont(font);
            itemNameTexts[i].setCharacterSize(18);
            itemNameTexts[i].setPosition(60 + i * 180, 150);
            itemNameTexts[i].setString(items[i]->getName());
            itemNameTexts[i].setFillColor(sf::Color::White);

            itemDescTexts[i].setFont(font);
            itemDescTexts[i].setCharacterSize(10);
            itemDescTexts[i].setPosition(50 + i * 180, 190);
            itemDescTexts[i].setString(items[i]->getDescription());
            itemDescTexts[i].setFillColor(sf::Color::White);

            itemPriceTexts[i].setFont(font);
            itemPriceTexts[i].setCharacterSize(15);
            itemPriceTexts[i].setPosition(60 + i * 180, 200);
            itemPriceTexts[i].setString("Price: " + to_string(items[i]->getPrice()) + "g");
            itemPriceTexts[i].setFillColor(sf::Color::White);

            buyButtons[i] = Button(60 + i * 180, 240, 100, 30, font, "BUY");
        }

        exitButton = Button(50, 40, 50, 20, font, "EXIT");
        updateGoldDisplay();
    }
    ~Shop()
    {
        for (int i = 0; i < itemCount; i++)
        {
            delete items[i];
        }
    }
    //the main game loop which run this shop window
    void run()
    {
        while (window.isOpen() && !shouldReturnToMenu)
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                }
                handleInput(event);
            }

            render();
        }
    }
    //this function handles the input for buy and exit button 
    void handleInput(sf::Event &event)
    {
        if(event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            
            for (int i = 0; i < itemCount; i++)
            {
                if(buyButtons[i].getShape().getGlobalBounds().contains(mousePos))
                {
                    sound.play();
                    buyButtons[i].getShape().setFillColor(sf::Color(10, 20, 60, 255));
                    attemptPurchase(i);
                }
            }

            
            if(exitButton.getShape().getGlobalBounds().contains(mousePos))
            {
                shouldReturnToMenu = true;
                return;
            }
        }
    }
//this draws everythign on the screen the text sprites etc
    void render()
    {
        window.clear();
        bg.draw(window);
        window.draw(brd);
        window.draw(titleText);
        window.draw(goldText);

        for (int i = 0; i < itemCount; i++)
        {
            window.draw(itemSprites[i]);
            window.draw(itemNameTexts[i]);
            window.draw(itemDescTexts[i]);
            window.draw(itemPriceTexts[i]);
            buyButtons[i].render(window);
        }

        exitButton.render(window);
        window.display();
    }
    //when items is purchased in the shop
    void attemptPurchase(int index)
    {
        if(player->getGold() >= items[index]->getPrice())
        {
            player->spendGold(items[index]->getPrice());

            if(items[index]->getType() == "Healing")
            {
                player->addItemToInventory(new HealingPotion());
            }
            else if(items[index]->getType() == "Mana")
            {
                player->addItemToInventory(new ManaPotion());
            }
            else if(items[index]->getType() == "Shield")
            {
                player->addItemToInventory(new Shield());
            }
            //the gold get update
            updateGoldDisplay();

            flashItem(index, sf::Color::Green);
        }
        else
        {
            flashItem(index, sf::Color::Red);
        }
    }
    void flashItem(int index, sf::Color color)
    {
        itemNameTexts[index].setFillColor(color);
        render();
        sf::sleep(sf::milliseconds(200));
        itemNameTexts[index].setFillColor(sf::Color::White);
    }

    void updateGoldDisplay()
    {
        goldText.setString("GOLD: " + to_string(player->getGold()));
        goldText.setFont(font);
        goldText.setCharacterSize(24);
        goldText.setPosition(400, 30);
        goldText.setFillColor(sf::Color::White);
    }
};

//for the items that are in player inventory
class InventoryMenu{
private:
    sf::RenderWindow &window;
    sf::Font &font;
    Player *player;
    ParallaxBackground bg;
    sf::SoundBuffer buffer;
    sf::Sound sound;

    sf::Text titleText;
    sf::Text goldText;
    sf::Text itemNameTexts[10];
    sf::Text itemDescTexts[10];
    sf::Text itemCountTexts[10];
    Button useButtons[10];
    Button exitButton;
    sf::Texture boardTexture;
    sf::Sprite boardSprite;
    sf::Texture itemTextures[3];
    sf::Sprite itemSprites[10];


    int selectedItemIndex;
    int selectedPetIndex;
    bool selectingPet;
    bool close = false;

    //for positions
    const float itemX = 50.0f;
    const float itemYStart = 80.0f;
    const float itemYspacing = 60.0f;

public:
    InventoryMenu(sf::RenderWindow &win, sf::Font &f, Player *p) : window(win), font(f), player(p), selectedItemIndex(-1),
                                                                   selectedPetIndex(-1), selectingPet(false)
    {

        bg.WandH(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);

        if(!boardTexture.loadFromFile("assets/board.png"))
        {
            cout<<"Failed to load board texture!" << endl;
        }
        boardSprite.setTexture(boardTexture);
        boardSprite.setPosition(10, 15);
        boardSprite.setScale(2.3f, 1.2f);

        itemTextures[0].loadFromFile("assets/p1.png"); 
        itemTextures[1].loadFromFile("assets/p2.png"); 
        itemTextures[2].loadFromFile("assets/p3.png"); 


        titleText.setFont(font);
        titleText.setString("INVENTORY");
        titleText.setCharacterSize(30);
        titleText.setPosition(220, 20);
        titleText.setFillColor(sf::Color::White);
        titleText.setOutlineColor(sf::Color(10, 20, 60, 255));
        titleText.setOutlineThickness(2);

        goldText.setFont(font);
        goldText.setCharacterSize(20);
        goldText.setPosition(400, 25);
        goldText.setFillColor(sf::Color::Yellow);

        buffer.loadFromFile("assets/sound/coin.wav");
        sound.setBuffer(buffer);
        sound.setVolume(50);
        sound.setPitch(1.5f);

        for (int i = 0; i < 10; i++)
        {
            itemSprites[i].setScale(1.5f, 1.5f);

            itemNameTexts[i].setFont(font);
            itemNameTexts[i].setCharacterSize(16);
            itemNameTexts[i].setFillColor(sf::Color::White);

            itemDescTexts[i].setFont(font);
            itemDescTexts[i].setCharacterSize(12);
            itemDescTexts[i].setFillColor(sf::Color(200, 200, 200));

            itemCountTexts[i].setFont(font);
            itemCountTexts[i].setCharacterSize(14);
            itemCountTexts[i].setFillColor(sf::Color::Yellow);

            useButtons[i] = Button(400, itemYStart + i * itemYspacing - 10, 100, 30, font, "Use");
            useButtons[i].setVisible(false);
        }

        exitButton = Button(450, 270, 100, 30, font, "Exit");
        updateGoldDisplay();
        updateInventoryDisplay();
    }
//this runt the loop 
    void run()
    {
        sf::Clock clock;

        while (window.isOpen() && !close)
        {
            float deltaTime = clock.restart().asSeconds();

            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }

                handleInput(event);
            }

            update(deltaTime);
            render();
        }
    }

private:
    void handleInput(sf::Event &event)
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Escape)
            {
                close = true;
                return;
            }
        }
        if(event.type == sf::Event::MouseButtonPressed)
        {
            if(event.mouseButton.button == sf::Mouse::Left)
            {
                sound.play();
                if(exitButton.getShape().getGlobalBounds().contains(mousePos))
                {
                    return; 
                }

                if(selectingPet)
                {
                    // this one handle pet selection
                    for (int i = 0; i < player->getPetCount(); i++)
                    {
                        if(itemNameTexts[i].getGlobalBounds().contains(mousePos))
                        {
                            selectedPetIndex = i;
                            useSelectedItem();
                            selectingPet = false;
                            updateInventoryDisplay();
                            break;
                        }
                    }
                }
                else
                {
                    // this one handle item selection
                    for (int i = 0; i < player->getInventoryCount(); i++)
                    {
                        if(useButtons[i].getShape().getGlobalBounds().contains(mousePos))
                        {
                            selectedItemIndex = i;
                            if(player->getPetCount() > 0)
                            {
                                if(player->getPetCount() == 1)
                                {
                                    // if only one pet, use directly
                                    selectedPetIndex = 0;
                                    useSelectedItem();
                                    updateInventoryDisplay();
                                }
                                else
                                {
                                    // multiple pets,need to select which one
                                    selectingPet = true;
                                    updateInventoryDisplay();
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
//updating button state
    void update(float deltaTime)
    {
        bg.update(deltaTime);

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        for (int i = 0; i < player->getInventoryCount(); i++)
        {
            useButtons[i].update(mousePos);
        }
        exitButton.update(mousePos);
    }
//fucntion for drawing
    void render()
    {
        window.clear();
        bg.draw(window);
        window.draw(boardSprite);

        window.draw(titleText);
        window.draw(goldText);

        for (int i = 0; i < player->getInventoryCount(); i++)
        {
            window.draw(itemSprites[i]);
            window.draw(itemNameTexts[i]);
            window.draw(itemDescTexts[i]);
            window.draw(itemCountTexts[i]);

            if(!selectingPet)
            {
                useButtons[i].render(window);
            }
        }

        // if selecting pet show pet selection interface
        if(selectingPet)
        {
            drawPetSelection();
        }

        // always shows exit button
        exitButton.render(window);

        window.display();
    }

    void updateGoldDisplay()
    {
        goldText.setString("Gold: " + to_string(player->getGold()));
    }

    void updateInventoryDisplay()
    {
        // for counting each type of item
        int healingCount = 0;
        int manaCount = 0;
        int shieldCount = 0;

        for (int i = 0; i < player->getInventoryCount(); i++)
        {
            Item *item = player->getInventoryItem(i);
            if(!item)
                continue;

            string itemType = item->getType();
            if(itemType == "Healing")
            {
                healingCount++;
            }
            else if(itemType == "Mana")
            {
                manaCount++;
            }
            else if(itemType == "Shield")
            {
                shieldCount++;
            }
        }

        int displayIndex = 0;

        if(healingCount > 0)
        {
            itemSprites[displayIndex].setTexture(itemTextures[0]);
            itemSprites[displayIndex].setPosition(itemX, itemYStart + displayIndex * itemYspacing);

            itemNameTexts[displayIndex].setString("Healing Potion");
            itemNameTexts[displayIndex].setPosition(itemX + 50, itemYStart + displayIndex * itemYspacing);

            itemDescTexts[displayIndex].setString("Restores 50 HP to a pet");
            itemDescTexts[displayIndex].setPosition(itemX + 50, itemYStart + displayIndex * itemYspacing + 20);

            itemCountTexts[displayIndex].setString("x" + to_string(healingCount));
            itemCountTexts[displayIndex].setPosition(itemX + 250, itemYStart + displayIndex * itemYspacing);

            useButtons[displayIndex].setVisible(true);
            displayIndex++;
        }

        // Mana Potions
        if(manaCount > 0)
        {
            itemSprites[displayIndex].setTexture(itemTextures[1]);
            itemSprites[displayIndex].setPosition(itemX, itemYStart + displayIndex * itemYspacing);

            itemNameTexts[displayIndex].setString("Mana Potion");
            itemNameTexts[displayIndex].setPosition(itemX + 50, itemYStart + displayIndex * itemYspacing);

            itemDescTexts[displayIndex].setString("Allows immediate use of abilities");
            itemDescTexts[displayIndex].setPosition(itemX + 50, itemYStart + displayIndex * itemYspacing + 20);

            itemCountTexts[displayIndex].setString("x" + to_string(manaCount));
            itemCountTexts[displayIndex].setPosition(itemX + 250, itemYStart + displayIndex * itemYspacing);

            useButtons[displayIndex].setVisible(true);
            displayIndex++;
        }


        if(shieldCount > 0)
        {
            itemSprites[displayIndex].setTexture(itemTextures[2]);
            itemSprites[displayIndex].setPosition(itemX, itemYStart + displayIndex * itemYspacing);

            itemNameTexts[displayIndex].setString("Shield");
            itemNameTexts[displayIndex].setPosition(itemX + 50, itemYStart + displayIndex * itemYspacing);

            itemDescTexts[displayIndex].setString("Increases defense by 20");
            itemDescTexts[displayIndex].setPosition(itemX + 50, itemYStart + displayIndex * itemYspacing + 20);

            itemCountTexts[displayIndex].setString("x" + to_string(shieldCount));
            itemCountTexts[displayIndex].setPosition(itemX + 250, itemYStart + displayIndex * itemYspacing);

            useButtons[displayIndex].setVisible(true);
            displayIndex++;
        }

        // hiding unused buttons
        for (int i = displayIndex; i < 10; i++)
        {
            useButtons[i].setVisible(false);
        }
    }
//this function draws the pet selection screen
    void drawPetSelection()
    {
        sf::RectangleShape overlay(sf::Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(sf::Color(0, 0, 0, 150));
        window.draw(overlay);

        sf::Text prompt("Select a pet to use item on:", font, 20);
        prompt.setPosition(150, 100);
        prompt.setFillColor(sf::Color::White);
        window.draw(prompt);

        for (int i = 0; i < player->getPetCount(); i++)
        {
            Pet *pet = player->getPet(i);

            sf::Text petText(pet->getName() + " (Lv." + to_string(pet->getLevel()) + " " + pet->getType() + ")",
                             font, 16);
            petText.setPosition(150, 140 + i * 30);
            petText.setFillColor(i == selectedPetIndex ? sf::Color::Yellow : sf::Color::White);
            window.draw(petText);

            itemNameTexts[i].setPosition(petText.getPosition());
            itemNameTexts[i].setString(petText.getString());
        }
    }
//this function uses the selected item on the selected pet
    void useSelectedItem()
    {
        if(selectedItemIndex >= 0 && selectedItemIndex < player->getInventoryCount() &&
            selectedPetIndex >= 0 && selectedPetIndex < player->getPetCount())
        {

            Item *item = player->getInventoryItem(selectedItemIndex);
            Pet *pet = player->getPet(selectedPetIndex);

            if(item && pet)
            {
                player->useItem(selectedItemIndex, pet);
                updateGoldDisplay();
                updateInventoryDisplay();
                selectedItemIndex = -1;
                selectedPetIndex = -1;
            }
        }
    }
};

//this class is for the training camp for pet
class TrainingCamp
{
private:
    int trainingCost;
    sf::RenderWindow &window;
    sf::Font &font;
    Player *player;
    ParallaxBackground bg;
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text statsText;
    Button trainButton;
    Button exitButton;
    int selectedPetIndex;
    float trainingTimer;
    bool isTraining;
    PetAnimation petAnim;
    sf::Sprite petSprite;
    bool shouldReturnToMenu;

    sf::SoundBuffer buffer;
    sf::Sound sound;

public:
    TrainingCamp(sf::RenderWindow &win, sf::Font &f, Player *p) : window(win), font(f), player(p), selectedPetIndex(0),trainingTimer(0), isTraining(false), shouldReturnToMenu(false)
    {
        buffer.loadFromFile("assets/sound/coin.wav");
        sound.setBuffer(buffer);
        sound.setVolume(50);
        sound.setPitch(1.5f);

        bg.WandH(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/Bg2/2.png", 20.f);
        bg.addLayer("assets/Bg2/3.png", 20.f);
        bg.addLayer("assets/Bg2/4.png", 20.f);
        bg.addLayer("assets/Bg2/5.png", 20.f);
        bg.addLayer("assets/Bg2/6.png", 20.f);

//loading the board and all the text
        titleText.setString("TRAINING CAMP");
        titleText.setFont(font);
        titleText.setCharacterSize(40);
        titleText.setPosition(170, 20);
        titleText.setOutlineColor(sf::Color::White);
        titleText.setOutlineThickness(2);
        titleText.setFillColor(sf::Color(10, 20, 60, 255));

        instructionText.setFont(font);
        instructionText.setCharacterSize(18);
        instructionText.setPosition(50, 70);
        instructionText.setFillColor(sf::Color::White);

        statsText.setFont(font);
        statsText.setCharacterSize(16);
        statsText.setPosition(300, 120);
        statsText.setFillColor(sf::Color::White);
        statsText.setStyle(sf::Text::Bold);

        trainButton = Button(150, 250, 150, 40, font, "Train (50g)");
        exitButton = Button(350, 250, 150, 40, font, "Exit");
        updatePetDisplay();
    }
//game loop
    void run()
    {
        sf::Clock clock;
        while (window.isOpen() && !shouldReturnToMenu)
        {
            sf::Time deltaTime = clock.restart();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                handleInput(event);
            }

            update(deltaTime.asSeconds());
            render();
        }
        {
            float deltaTime = clock.restart().asSeconds();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                handleInput(event);
            }

            update(deltaTime);
            render();
        }
        
    }
    void reset()
    {
        shouldReturnToMenu = false;
        selectedPetIndex = 0;
        isTraining = false;
        trainingTimer = 0;
        updatePetDisplay();
    }
 bool shouldReturnToMainMenu() const
    {
        return shouldReturnToMenu;
    }
    void handleInput(sf::Event &event)
    {
        if(isTraining)
            return;
        if(event.type == sf::Event::KeyPressed)
        {
            if(event.key.code == sf::Keyboard::Left)
            {
                sound.play();
                selectedPetIndex = (selectedPetIndex - 1 + player->getPetCount()) % player->getPetCount();
                updatePetDisplay();
            }
            else if(event.key.code == sf::Keyboard::Right)
            {
                sound.play();
                selectedPetIndex = (selectedPetIndex + 1) % player->getPetCount();
                updatePetDisplay();
            }
        }
        else if(event.type == sf::Event::MouseButtonPressed)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            if(trainButton.getShape().getGlobalBounds().contains(mousePos) && !isTraining)
            {
                sound.play();
                if(player->getGold() >= 50)
                {
                    startTraining();
                }
                else
                {
                    instructionText.setString("Not enough gold! Need 50 gold.");
                }
            }
            else if(exitButton.getShape().getGlobalBounds().contains(mousePos) && !isTraining)
            {
                sound.play();
                shouldReturnToMenu = true;
            }
        }
    }

    void update(float deltaTime)
    {
        bg.update(deltaTime);

        if(isTraining)
        {
            trainingTimer += deltaTime;
            if(trainingTimer >= 3.0f)
            { // 3 second training duration
                completeTraining();
            }
            else
            {
                
                petSprite.rotate(-5 * deltaTime); 
            }
        }
    }

    void render()
    {
        window.clear();
        bg.draw(window);

        window.draw(titleText);
        window.draw(instructionText);
        window.draw(statsText);
        window.draw(petSprite);

        if(!isTraining)
        {
            trainButton.render(window);
            exitButton.render(window);
        }
        else
        {
            sf::Text trainingText("Training in progress...", font, 20);
            trainingText.setPosition(250, 200);
            window.draw(trainingText);
        }

        window.display();
    }

private:
    void updatePetDisplay()
    {
        if(player->getPetCount() == 0)
            return;

        Pet *pet = player->getPet(selectedPetIndex);

        if(pet->getType() == "Dragon")
            petAnim.loadDragon();
        else if(pet->getType() == "Phoenix")
            petAnim.loadPhoenix();
        else if(pet->getType() == "Unicorn")
            petAnim.loadUnicorn();
        else if(pet->getType() == "Griffin")
            petAnim.loadGriffin();

        petAnim.animationRow = 1;
        petSprite.setTexture(petAnim.texture);
        petSprite.setTextureRect(sf::IntRect(0, petAnim.animationRow * petAnim.frameHeight, petAnim.frameWidth, petAnim.frameHeight));
        petSprite.setScale(0.5f, 0.5f);
        petSprite.setPosition(130, 150);


        instructionText.setString(
            "Pet " + to_string(selectedPetIndex + 1) + "/" + to_string(player->getPetCount()) +
            "\n" + pet->getName() + " (Lv." + to_string(pet->getLevel()) + " " + pet->getType() + ")" +
            "\nUse LEFT/RIGHT to select pet");

        statsText.setString(
            "HP: " + to_string(pet->getHealth()) + "/" + to_string(pet->getMaxHealth()) +
            "\nATK: " + to_string(pet->getAttack()) +
            "\nDEF: " + to_string(pet->getDefense()) +
            "\nSPD: " + to_string(pet->getSpeed()) +
            "\nEXP: " + to_string(pet->getLevel() * 100));

        trainButton.getText().setString("Train(" + to_string(player->getGold()) + "/50g)");
        if(player->getGold() < 50)
        {
            trainButton.getShape().setFillColor(sf::Color(100, 100, 100));
        }
        else
        {
            trainButton.getShape().setFillColor(sf::Color(10, 20, 60, 255));
        }
    }

    void startTraining()
    {
        player->spendGold(50);
        if(player->getGold() < 50)
        {
            instructionText.setString("Not enough gold! Need 50 gold.");
            return;
        }
        isTraining = true;
        trainingTimer = 0;
        instructionText.setString("Training " + player->getPet(selectedPetIndex)->getName() + "...");
        trainButton.getShape().setFillColor(sf::Color(40, 30, 60, 255)); 
    }

    void completeTraining()
    {
        isTraining = false;
        Pet *pet = player->getPet(selectedPetIndex);
        pet->gainExperience(25); 
        petSprite.setRotation(0);
        trainButton.getShape().setFillColor(sf::Color(10, 20, 60, 255));

        updatePetDisplay();
        instructionText.setString(
            "Training complete!\n" +
            pet->getName() + " gained 25 EXP!\n" +
            "Current gold: " + to_string(player->getGold()));
    }

    int getTrainingCost() const
    {
        return trainingCost;
    }

    void setTrainingCost(int cost)
    {
        trainingCost = cost;
    }
};

//this class is for the battle system handling single battle and the guild war
class Battle
{
private:
    struct Projectile
    {
        sf::Sprite sprite;
        sf::Texture texture;
        float speed;
        int damage;
        bool active;
        bool isPlayerProjectile;
    };

    sf::SoundBuffer buffer;
    sf::Sound sound;
    Projectile projectiles[50];

public:
Battle()
{
    buffer.loadFromFile("assets/sound/attack.wav");
    sound.setBuffer(buffer);
    sound.setVolume(50);
    sound.setPitch(1.5f);
}
//this function spawns the projectile like fireball and slashes
    void spawnProjectile(Pet *source, Pet *target, bool isPlayerProjectile, const string &attackType, float sourceX, float sourceY, float speed)
    {
        
        for (int i = 0; i < 50; i++)
        {
            if(!projectiles[i].active)
            {
                projectiles[i].active = true;
                projectiles[i].isPlayerProjectile = isPlayerProjectile;

                string texturePath;
                if(attackType == "Basic")
                {
                    texturePath = "assets/Dragon/slash2.png";
                    projectiles[i].damage = source->getAttack() / 4;
                }
                else if(attackType == "Fireball Attack")
                {
                    texturePath = "assets/Dragon/fireball.png";
                    projectiles[i].damage = source->getAttack() / 3;
                }
                else if(attackType == "Dragon's Roar")
                {
                    texturePath = "assets/Dragon/slash1.png";
                    projectiles[i].damage = source->getAttack() / 5;
                }
                else if(attackType == "Flame Healing")
                {
                    texturePath = "assets/Dragon/claw.png";
                    projectiles[i].damage = 0;
                }
                else if(attackType == "Rebirth")
                {
                    texturePath = "assets/Dragon/rebirth.png";
                    projectiles[i].damage = 0;
                }
                else if(attackType == "Speed Burst")
                {
                    texturePath = "assets/Dragon/claw.png";
                    projectiles[i].damage = 0;
                }
                else if(attackType == "Magic Shield")
                {
                    texturePath = "assets/Dragon/slash2.png";
                    projectiles[i].damage = 0;
                }
                else if(attackType == "Claw Strike")
                {
                    texturePath = "assets/projectiles/claw.png";
                    projectiles[i].damage = source->getAttack() / 3;
                }
                else if(attackType == "Dive Bomb")
                {
                    texturePath = "assets/Dragon/slash1.png";
                    projectiles[i].damage = source->getAttack() / 3;
                }
                else
                {
                    texturePath = "assets/Dragon/slash1.png";
                    projectiles[i].damage = source->getAttack() / 4;
                }

                if(!projectiles[i].texture.loadFromFile(texturePath))
                {
                    projectiles[i].texture.loadFromFile("assets/Dragon/fireball.png");
                }
                projectiles[i].sprite.setTexture(projectiles[i].texture);
                projectiles[i].sprite.setScale(0.5f, 0.5f);

                if(isPlayerProjectile)
                {
                    projectiles[i].sprite.setPosition(sourceX, sourceY);
                    projectiles[i].speed = 200.f;
                }
                else
                {
                    projectiles[i].sprite.setPosition(sourceX + 20, sourceY);
                    projectiles[i].speed = -200.f;
                    if(attackType != "Flame Healing" && attackType != "Rebirth" &&
                        attackType != "Speed Burst" && attackType != "Magic Shield")
                    {
                        projectiles[i].sprite.setScale(-0.5f, 0.5f);
                    }
                }
                cout<<"Spawned projectile at " << sourceX + 20 << ", " << sourceY << " for " << attackType<<endl;
                break;
            }
        }
    }
//this function handles the enemy action
    void enemyAction(Pet *playerPet, Pet *enemyPet, const string &enemyType, bool &rebirthUsed, float sourceX, float sourceY, float speed)
    {
        int actionChoice = rand() % 100;
        bool lowHealth = enemyPet->getHealth() < enemyPet->getMaxHealth() / 3;
        bool playerLowHealth = playerPet->getHealth() < playerPet->getMaxHealth() / 2;

        if(enemyType == "Phoenix")
        {
            if(lowHealth && actionChoice < 50)
            {
                enemyPet->useAbility1(enemyPet);
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility1(), sourceX, sourceY, speed);
            }
            else if(actionChoice < 20 && !rebirthUsed)
            {
                enemyPet->useAbility2(enemyPet);
                if(!enemyPet->getIsAlive())
                    rebirthUsed = true;
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility2(), sourceX, sourceY, speed);
            }
            else
            {
                enemyPet->useBasicAttack(playerPet);
                spawnProjectile(enemyPet, playerPet, false, "Basic", sourceX, sourceY, speed);
            }
        }
        else if(enemyType == "Dragon")
        {
            if(playerLowHealth && actionChoice < 40)
            {
                enemyPet->useAbility1(playerPet);
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility1(), sourceX, sourceY, speed);
            }
            else if(actionChoice < 20)
            {
                enemyPet->useAbility2(playerPet);
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility2(), sourceX, sourceY, speed);
            }
            else
            {
                enemyPet->useBasicAttack(playerPet);
                spawnProjectile(enemyPet, playerPet, false, "Basic", sourceX, sourceY, speed);
            }
        }
        else if(enemyType == "Unicorn")
        {
            if(actionChoice < 30)
            {
                enemyPet->useAbility1(enemyPet);
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility1(), sourceX, sourceY, speed);
            }
            else if(actionChoice < 50)
            {
                enemyPet->useAbility2(enemyPet);
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility2(), sourceX, sourceY, speed);
            }
            else
            {
                enemyPet->useBasicAttack(playerPet);
                spawnProjectile(enemyPet, playerPet, false, "Basic", sourceX, sourceY, speed);
            }
        }
        else if(enemyType == "Griffin")
        {
            if(playerLowHealth && actionChoice < 40)
            {
                enemyPet->useAbility1(playerPet);
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility1(), sourceX, sourceY, speed);
            }
            else if(actionChoice < 20)
            {
                enemyPet->useAbility2(playerPet);
                spawnProjectile(enemyPet, playerPet, false, enemyPet->getAbility2(), sourceX, sourceY, speed);
            }
            else
            {
                enemyPet->useBasicAttack(playerPet);
                spawnProjectile(enemyPet, playerPet, false, "Basic", sourceX, sourceY, speed);
            }
        }
        else
        {
            enemyPet->useBasicAttack(playerPet);
            spawnProjectile(enemyPet, playerPet, false, "Basic", sourceX, sourceY, speed);
        }
    }
//this handles the single battle
    int battle(sf::RenderWindow &window, Pet *playerPet, Pet *enemyPet, sf::Font &font)
    {
        ParallaxBackground bg;
        bg.WandH(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/Bg3/1.png", 20.f);
        bg.addLayer("assets/Bg3/2.png", 20.f);
        bg.addLayer("assets/Bg3/3.png", 20.f);
        bg.addLayer("assets/Bg3/4.png", 20.f);

        PetAnimation playerAnim, enemyAnim;
        string playerType = playerPet->getType();
        string enemyType = enemyPet->getType();
        bool rebirthUsed = false;

        if(playerType == "Dragon")
            playerAnim.loadDragon();
        else if(playerType == "Phoenix")
            playerAnim.loadPhoenix();
        else if(playerType == "Unicorn")
            playerAnim.loadUnicorn();
        else if(playerType == "Griffin")
            playerAnim.loadGriffin();

        if(enemyType == "Dragon")
            enemyAnim.loadDragon();
        else if(enemyType == "Phoenix")
            enemyAnim.loadPhoenix();
        else if(enemyType == "Unicorn")
            enemyAnim.loadUnicorn();
        else if(enemyType == "Griffin")
            enemyAnim.loadGriffin();

        sf::Sprite playerSprite, enemySprite;
        playerSprite.setTexture(playerAnim.texture);
        enemySprite.setTexture(enemyAnim.texture);
        playerSprite.setTextureRect(sf::IntRect(0, playerAnim.animationRow * playerAnim.frameHeight, playerAnim.frameWidth, playerAnim.frameHeight));
        enemySprite.setTextureRect(sf::IntRect(0, enemyAnim.animationRow * enemyAnim.frameHeight, enemyAnim.frameWidth, enemyAnim.frameHeight));
        if(enemyType == "Griffin")
        {
            enemySprite.setScale(-enemyAnim.scale, enemyAnim.scale); 
        }
        else
        {
            enemySprite.setScale(enemyAnim.scale, enemyAnim.scale);
        }
        playerSprite.setScale(playerAnim.scale, playerAnim.scale);
        playerSprite.setPosition(50, 150);
        enemySprite.setPosition(400, 150);

        for (int i = 0; i < 50; i++)
        {
            projectiles[i].active = false;
        }
        int projectileCount = 0;

        // for health bars
        sf::RectangleShape playerHealthBar;
        sf::RectangleShape playerHealthBg;
        sf::RectangleShape enemyHealthBar;
        sf::RectangleShape enemyHealthBg;

        playerHealthBg.setSize(sf::Vector2f(150, 10));
        playerHealthBg.setPosition(20, 20);
        playerHealthBg.setFillColor(sf::Color(100, 100, 100));

        playerHealthBar.setSize(sf::Vector2f(150, 10));
        playerHealthBar.setPosition(20, 20);
        playerHealthBar.setFillColor(sf::Color::Green);

        enemyHealthBg.setSize(sf::Vector2f(150, 10));
        enemyHealthBg.setPosition(406, 20);
        enemyHealthBg.setFillColor(sf::Color(100, 100, 100));

        enemyHealthBar.setSize(sf::Vector2f(150, 10));
        enemyHealthBar.setPosition(406, 20);
        enemyHealthBar.setFillColor(sf::Color::Green);

        // for timer
        sf::Text timerText;
        timerText.setFont(font);
        timerText.setCharacterSize(16);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(250, 10);
        float battleTime = 70.0f;
        sf::Clock battleClock;

        // instruciton to play the game
        sf::Text instructions("A/D: Move, W/S: Up/Down, 1: Basic, 2/3: Abilities", font, 12);
        instructions.setPosition(10, 300);
        instructions.setFillColor(sf::Color::White);

        // for animation
        const float timePerFrame = 0.2f;
        float currentFrame = 0;
        float accumulatedTime = 0.f;
        float attackCooldown = 0.f;
        float enemyAttackCooldown = 0.f;
        const float attackRange = 350.f;
        bool enemyFacingRight = enemySprite.getScale().x < 0;
        sf::Clock clock;
        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();
            battleTime -= deltaTime;
            accumulatedTime += deltaTime;
            attackCooldown -= deltaTime;
            enemyAttackCooldown -= deltaTime;

            if(enemyAttackCooldown < -1.f)
            {
                enemyAttackCooldown = 0.f;
            }

            
            int secondsLeft = static_cast<int>(battleTime);
            timerText.setString("Time: " + to_string(secondsLeft) + "s");

            if(battleTime <= 0)
            {
                if(playerPet->getHealth() > enemyPet->getHealth())
                {
                    playerPet->gainExperience(50 + enemyPet->getLevel() * 10);
                    return 1;
                }
                else
                {
                    return 0;
                }
            }

            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                {
                    return -1;
                }
            }

            // for the player movement
            float playerSpeed = 100.f;
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && playerSprite.getPosition().x > 0)
            {
                playerSprite.move(-playerSpeed * deltaTime, 0);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && playerSprite.getPosition().x < 300)
            {
                playerSprite.move(playerSpeed * deltaTime, 0);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && playerSprite.getPosition().y > 50)
            {
                playerSprite.move(0, -playerSpeed * deltaTime);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerSprite.getPosition().y < 250)
            {
                playerSprite.move(0, playerSpeed * deltaTime);
            }

            // enemy movement in the battle 
            float enemySpeed = 50.f;
            float dy = playerSprite.getPosition().y - enemySprite.getPosition().y;
            if(dy > 10 && enemySprite.getPosition().y < 250)
            {
                enemySprite.move(0, enemySpeed * deltaTime);
            }
            else if(dy < -10 && enemySprite.getPosition().y > 50)
            {
                enemySprite.move(0, -enemySpeed * deltaTime);
            }
            if(rand() % 100 < 5)
            {
                float dx = (rand() % 2 == 0 ? 1 : -1) * enemySpeed * deltaTime;
                if(enemySprite.getPosition().x + dx > 300 && enemySprite.getPosition().x + dx < 500)
                {
                    enemySprite.move(dx, 0);
                }
            }

            // calculating distance
            float dx = enemySprite.getPosition().x - playerSprite.getPosition().x;
            dy = enemySprite.getPosition().y - enemySprite.getPosition().y;
            float distance = sqrt(dx * dx + dy * dy);

            // attack buttons num1 num2 and num3
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && attackCooldown <= 0 && projectileCount < 50)
            {
                sound.play();
                playerPet->useBasicAttack(enemyPet);
                spawnProjectile(playerPet, enemyPet, true, "Basic",playerSprite.getPosition().x + playerAnim.frameWidth * playerAnim.scale,playerSprite.getPosition().y, enemyFacingRight ? 200.f : -200.f);
                projectileCount++;
                attackCooldown = 1.f;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && attackCooldown <= 0 && projectileCount < 50)
            {
                sound.play();
                playerPet->useAbility1(enemyPet);
                spawnProjectile(playerPet, enemyPet, true, playerPet->getAbility1(),playerSprite.getPosition().x + playerAnim.frameWidth * playerAnim.scale,playerSprite.getPosition().y, enemyFacingRight ? 200.f : -200.f);
                projectileCount++;
                attackCooldown = 1.f;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && attackCooldown <= 0 && projectileCount < 50)
            {
                sound.play();
                playerPet->useAbility2(playerPet);
                spawnProjectile(playerPet, enemyPet, true, playerPet->getAbility2(),playerSprite.getPosition().x + playerAnim.frameWidth * playerAnim.scale,playerSprite.getPosition().y, enemyFacingRight ? 200.f : -200.f);
                if(!playerPet->getIsAlive() && playerType == "Phoenix")
                    rebirthUsed = true;
                projectileCount++;
                attackCooldown = 1.f;
            }

            // enemy attacks
            if(distance < attackRange && enemyAttackCooldown <= 0 && projectileCount < 50)
            {
                sf::Vector2f enemyPos = enemySprite.getPosition();
                //  facing right
                enemyAction(playerPet, enemyPet, enemyType, rebirthUsed, enemyPos.x + (enemyFacingRight ? enemySprite.getGlobalBounds().width : 0), enemyPos.y + enemySprite.getGlobalBounds().height / 2, enemyFacingRight ? 200.f : -200.f);
                projectileCount++;
                enemyAttackCooldown = 0.5f;
            }

            // update firing projectiles
            for (int i = 0; i < 50; i++)
            {
                if(projectiles[i].active)
                {
                    projectiles[i].sprite.move(projectiles[i].speed * deltaTime, 0);
                    float travelDistance = abs(projectiles[i].sprite.getPosition().x - (projectiles[i].isPlayerProjectile ? 300 : 400));
                    sf::FloatRect projBounds = projectiles[i].sprite.getGlobalBounds();
                    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
                    sf::FloatRect enemyBounds = enemySprite.getGlobalBounds();

                    if(travelDistance > 50 && projectiles[i].isPlayerProjectile && projBounds.intersects(enemyBounds))
                    {
                        enemyPet->takeDamage(projectiles[i].damage);
                        projectiles[i].active = false;
                    }
                    else if(travelDistance > 50 && !projectiles[i].isPlayerProjectile && projBounds.intersects(playerBounds))
                    {
                        playerPet->takeDamage(projectiles[i].damage);
                        projectiles[i].active = false;
                    }
                    else if(projBounds.left < 0 || projBounds.left > 576)
                    {
                        projectiles[i].active = false;
                    }
                }
            }
            // reseting projectile count to actual active count
            projectileCount = 0;
            for (int i = 0; i < 50; i++)
            {
                if(projectiles[i].active)
                {
                    projectileCount++;
                }
            }

            // updated health bars
            float playerHealthRatio = static_cast<float>(playerPet->getHealth()) / playerPet->getMaxHealth();
            float enemyHealthRatio = static_cast<float>(enemyPet->getHealth()) / enemyPet->getMaxHealth();
            playerHealthBar.setSize(sf::Vector2f(150 * playerHealthRatio, 10));
            enemyHealthBar.setSize(sf::Vector2f(150 * enemyHealthRatio, 10));
            playerHealthBar.setFillColor(playerHealthRatio < 0.3f ? sf::Color::Red : sf::Color::Green);
            enemyHealthBar.setFillColor(enemyHealthRatio < 0.3f ? sf::Color::Red : sf::Color::Green);

            // animation frame time
            if(accumulatedTime >= timePerFrame)
            {
                accumulatedTime = 0.f;
                currentFrame = static_cast<int>(currentFrame + 1) % playerAnim.numFrames;
                playerSprite.setTextureRect(sf::IntRect(currentFrame * playerAnim.frameWidth, playerAnim.animationRow * playerAnim.frameHeight, playerAnim.frameWidth, playerAnim.frameHeight));
                enemySprite.setTextureRect(sf::IntRect(currentFrame * enemyAnim.frameWidth, enemyAnim.animationRow * enemyAnim.frameHeight, enemyAnim.frameWidth, enemyAnim.frameHeight));
            }

            if(playerPet->getHealth() <= 0)
            {
                playerPet->setHealth(0);
                if(playerType == "Phoenix" && !rebirthUsed)
                {
                    playerPet->useAbility2(playerPet);
                    rebirthUsed = !playerPet->getIsAlive();
                }
                else
                {
                    return 0;
                }
            }
            if(enemyPet->getHealth() <= 0)
            {
                enemyPet->setHealth(0);
                if(enemyType == "Phoenix" && !rebirthUsed)
                {
                    enemyPet->useAbility2(enemyPet);
                    rebirthUsed = !enemyPet->getIsAlive();
                }
                else
                {
                    playerPet->gainExperience(50 + enemyPet->getLevel() * 10);
                    return 1;
                }
            }

            window.clear();
            bg.update(deltaTime);
            bg.draw(window);
            window.draw(playerSprite);
            window.draw(enemySprite);
            for (int i = 0; i < 50; i++)
            {
                if(projectiles[i].active)
                {
                    window.draw(projectiles[i].sprite);
                }
            }
            window.draw(playerHealthBg);
            window.draw(playerHealthBar);
            window.draw(enemyHealthBg);
            window.draw(enemyHealthBar);
            window.draw(timerText);
            window.draw(instructions);
            window.display();
        }
        return -1;
    }

    int guildBattle(sf::RenderWindow &window, Guild *playerGuild, sf::Font &font)
    {
        // check if player guild has pet membere
        if(playerGuild->getMemberCount() == 0)
        {
            sf::Text noPetsText("Your guild has no pets to battle with!", font, 20);
            noPetsText.setPosition(150, 150);
            noPetsText.setFillColor(sf::Color::Red);
            window.clear();
            window.draw(noPetsText);
            window.display();
            sf::sleep(sf::seconds(2));
            return -1;
        }

        // creating enemy guild with 4 pets one of each type
        Guild enemyGuild("Enemy Guild", "A formidable opponent");
        enemyGuild.addMember(new Dragon("Enemy Dragon"));
        enemyGuild.addMember(new Phoenix("Enemy Phoenix"));
        enemyGuild.addMember(new Unicorn("Enemy Unicorn"));
        enemyGuild.addMember(new Griffin("Enemy Griffin"));

        // selecting first player pet
        int playerPetIndex = 0;
        Pet *playerPet = playerGuild->getMember(playerPetIndex);
        if(!playerPet)
        {
            sf::Text errorText("Could not select a player pet!", font, 20);
            errorText.setPosition(150, 150);
            errorText.setFillColor(sf::Color::Red);
            window.clear();
            window.draw(errorText);
            window.display();
            sf::sleep(sf::seconds(2));
            return -1;
        }

        // Select initial enemy pet
        int enemyPetIndex = rand() % 4;
        Pet *enemyPet = enemyGuild.getMember(enemyPetIndex);
        if(!enemyPet)
        {
            sf::Text errorText("Could not select an enemy pet!", font, 20);
            errorText.setPosition(150, 150);
            errorText.setFillColor(sf::Color::Red);
            window.clear();
            window.draw(errorText);
            window.display();
            sf::sleep(sf::seconds(2));
            return -1;
        }

        // loading animations
        string playerType = playerPet->getType();
        string enemyType = enemyPet->getType();
        PetAnimation playerAnim, enemyAnim;
        if(playerType == "Dragon")
            playerAnim.loadDragon();
        else if(playerType == "Phoenix")
            playerAnim.loadPhoenix();
        else if(playerType == "Unicorn")
            playerAnim.loadUnicorn();
        else if(playerType == "Griffin")
            playerAnim.loadGriffin();

        if(enemyType == "Dragon")
            enemyAnim.loadDragon();
        else if(enemyType == "Phoenix")
            enemyAnim.loadPhoenix();
        else if(enemyType == "Unicorn")
            enemyAnim.loadUnicorn();
        else if(enemyType == "Griffin")
            enemyAnim.loadGriffin();

        sf::Sprite playerSprite, enemySprite;
        playerSprite.setTexture(playerAnim.texture);
        enemySprite.setTexture(enemyAnim.texture);
        playerSprite.setTextureRect(sf::IntRect(0, playerAnim.animationRow * playerAnim.frameHeight, playerAnim.frameWidth, playerAnim.frameHeight));
        enemySprite.setTextureRect(sf::IntRect(0, enemyAnim.animationRow * playerAnim.frameHeight, enemyAnim.frameWidth, enemyAnim.frameHeight));
        playerSprite.setScale(playerAnim.scale, playerAnim.scale);
        enemySprite.setScale(-enemyAnim.scale, enemyAnim.scale); // enemy faces left at first
        playerSprite.setPosition(100, 80);
        enemySprite.setPosition(476, 80);


        ParallaxBackground bg(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);

        // start the health bars
        sf::RectangleShape playerHealthBg(sf::Vector2f(150, 10));
        sf::RectangleShape enemyHealthBg(sf::Vector2f(150, 10));
        sf::RectangleShape playerHealthBar(sf::Vector2f(150, 10));
        sf::RectangleShape enemyHealthBar(sf::Vector2f(150, 10));
        playerHealthBg.setPosition(50, 20);
        enemyHealthBg.setPosition(376, 20);
        playerHealthBar.setPosition(50, 20);
        enemyHealthBar.setPosition(376, 20);
        playerHealthBg.setFillColor(sf::Color(100, 100, 100));
        enemyHealthBg.setFillColor(sf::Color(100, 100, 100));
        playerHealthBar.setFillColor(sf::Color::Green);
        enemyHealthBar.setFillColor(sf::Color::Green);

        // timer and instructions
        sf::Text timerText("Time: 70", font, 15);
        timerText.setPosition(263, 10);
        timerText.setFillColor(sf::Color::White);
        sf::Text instructions("WASD: Move  1: Basic  2: Ability1  3: Ability2  4: Switch Pet", font, 15);
        instructions.setPosition(50, 290);
        instructions.setFillColor(sf::Color::White);

        // guild member counts
        sf::Text playerGuildCount("Your Guild: " + to_string(playerGuild->getMemberCount()) + " pets alive", font, 15);
        playerGuildCount.setPosition(50, 40);
        playerGuildCount.setFillColor(sf::Color::White);
        sf::Text enemyGuildCount("Enemy Guild: 4 pets alive", font, 15);
        enemyGuildCount.setPosition(376, 40);
        enemyGuildCount.setFillColor(sf::Color::White);

        float attackCooldown = 0.f;
        float enemyAttackCooldown = 0.f;
        float battleTime = 60.f;
        int projectileCount = 0;
        bool rebirthUsed = false;
        int currentFrame = 0;
        const float timePerFrame = 0.2f;
        float accumulatedTime = 0.f;
        float moveSpeed = 100.f;
        bool facingRight = true;
        bool enemyFacingRight = false;

        for (int i = 0; i < 50; i++)
        {
            projectiles[i].active = false;
        }

        sf::Clock clock;
        while (window.isOpen())
        {
            float deltaTime = clock.restart().asSeconds();
            accumulatedTime += deltaTime;
            battleTime -= deltaTime;
            attackCooldown -= deltaTime;
            enemyAttackCooldown -= deltaTime;

            if(battleTime <= 0)
                break;

            timerText.setString("Time: " + to_string(static_cast<int>(battleTime)));

            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return -1;
                }
            }

            // player movement with w a s d keys
            sf::Vector2f playerPos = playerSprite.getPosition();
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) && playerPos.y > 0)
            {
                playerSprite.move(0, -moveSpeed * deltaTime);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerPos.y + playerSprite.getGlobalBounds().height < window.getSize().y)
            {
                playerSprite.move(0, moveSpeed * deltaTime);
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && playerPos.x > 0)
            {
                playerSprite.move(-moveSpeed * deltaTime, 0);
                if(facingRight)
                {
                    playerSprite.setScale(-playerAnim.scale, playerAnim.scale);
                    facingRight = false;
                }
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) && playerPos.x + playerSprite.getGlobalBounds().width < window.getSize().x)
            {
                playerSprite.move(moveSpeed * deltaTime, 0);
                if(!facingRight)
                {
                    playerSprite.setScale(playerAnim.scale, playerAnim.scale);
                    facingRight = true;
                }
            }

            // enemy movement 
            sf::Vector2f enemyPos = enemySprite.getPosition();
            sf::Vector2f playerCenter(playerPos.x + playerSprite.getGlobalBounds().width / 2, playerPos.y + playerSprite.getGlobalBounds().height / 2);
            sf::Vector2f enemyCenter(enemyPos.x + enemySprite.getGlobalBounds().width / 2, enemyPos.y + enemySprite.getGlobalBounds().height / 2);
            sf::Vector2f direction = playerCenter - enemyCenter;
            float distance = sqrt(direction.x * direction.x + direction.y * direction.y);
            if(distance > 50)
            {
                direction /= distance;
                enemySprite.move(direction * moveSpeed * deltaTime);
                if(direction.x > 0 && enemySprite.getScale().x > 0)
                {
                    enemySprite.setScale(-enemyAnim.scale, enemyAnim.scale);
                    enemyFacingRight = false;
                }
                else if(direction.x < 0 && enemySprite.getScale().x < 0)
                {
                    enemySprite.setScale(enemyAnim.scale, enemyAnim.scale);
                    enemyFacingRight = true;
                }
            }

            // player attacking  
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num1) && attackCooldown <= 0 && projectileCount < 50)
            {
                sound.play();
                playerPet->useBasicAttack(enemyPet);
                spawnProjectile(playerPet, enemyPet, true, "Basic",playerPos.x + (facingRight ? playerSprite.getGlobalBounds().width : 0),playerPos.y + playerSprite.getGlobalBounds().height / 2,facingRight ? 200.f : -200.f);
                projectileCount++;
                attackCooldown = 1.f;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num2) && attackCooldown <= 0 && projectileCount < 50)
            {
                sound.play();
                playerPet->useAbility1(enemyPet);
                spawnProjectile(playerPet, enemyPet, true, playerPet->getAbility1(),playerPos.x + (facingRight ? playerSprite.getGlobalBounds().width : 0),playerPos.y + playerSprite.getGlobalBounds().height / 2,facingRight ? 200.f : -200.f);
                projectileCount++;
                attackCooldown = 1.f;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num3) && attackCooldown <= 0 && projectileCount < 50)
            {
                sound.play();
                playerPet->useAbility2(playerPet);
                spawnProjectile(playerPet, enemyPet, true, playerPet->getAbility2(),playerPos.x + (facingRight ? playerSprite.getGlobalBounds().width : 0),playerPos.y + playerSprite.getGlobalBounds().height / 2,facingRight ? 200.f : -200.f);
                if(!playerPet->getIsAlive() && playerType == "Phoenix")
                {
                    rebirthUsed = true;
                }
                projectileCount++;
                attackCooldown = 1.f;
            }

            // switch pet 
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Num4) && attackCooldown <= 0)
            {
                int originalIndex = playerPetIndex;
                do
                {
                    playerPetIndex = (playerPetIndex + 1) % playerGuild->getMemberCount();
                    playerPet = playerGuild->getMember(playerPetIndex);
                } while (!playerPet->getIsAlive() && playerPetIndex != originalIndex);

                if(playerPet->getIsAlive())
                {
                    playerType = playerPet->getType();
                    if(playerType == "Dragon")
                        playerAnim.loadDragon();
                    else if(playerType == "Phoenix")
                        playerAnim.loadPhoenix();
                    else if(playerType == "Unicorn")
                        playerAnim.loadUnicorn();
                    else if(playerType == "Griffin")
                        playerAnim.loadGriffin();
                    playerSprite.setTexture(playerAnim.texture);
                    playerSprite.setTextureRect(sf::IntRect(0, playerAnim.animationRow * playerAnim.frameHeight, playerAnim.frameWidth, playerAnim.frameHeight));
                    playerSprite.setScale(playerAnim.scale, playerAnim.scale);
                    playerSprite.setPosition(100, 80);
                    facingRight = true;
                    currentFrame = 0;
                    attackCooldown = 1.f;
                    cout<<"Switched to " << playerPet->getName()<<endl;
                }
            }

            // Enemy attacks 
            if(distance < 250.f && enemyAttackCooldown <= 0 && projectileCount < 50)
            {
                enemyAction(playerPet, enemyPet, enemyType, rebirthUsed,enemyPos.x + (enemyFacingRight ? enemySprite.getGlobalBounds().width : 0),enemyPos.y + enemySprite.getGlobalBounds().height / 2,enemyFacingRight ? 200.f : -200.f);
                projectileCount++;
                enemyAttackCooldown = 0.5f;
            }

            for (int i = 0; i < 50; i++)
            {
                if(projectiles[i].active)
                {
                    projectiles[i].sprite.move(projectiles[i].speed * deltaTime, 0);
                    sf::FloatRect projBounds = projectiles[i].sprite.getGlobalBounds();
                    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();
                    sf::FloatRect enemyBounds = enemySprite.getGlobalBounds();

                    if(projectiles[i].isPlayerProjectile && projBounds.intersects(enemyBounds))
                    {
                        enemyPet->takeDamage(projectiles[i].damage);
                        projectiles[i].active = false;
                    }
                    else if(!projectiles[i].isPlayerProjectile && projBounds.intersects(playerBounds))
                    {
                        playerPet->takeDamage(projectiles[i].damage);
                        projectiles[i].active = false;
                    }
                    else if(projBounds.left < 0 || projBounds.left > window.getSize().x)
                    {
                        projectiles[i].active = false;
                    }
                }
            }

            projectileCount = 0;
            for (int i = 0; i < 50; i++)
            {
                if(projectiles[i].active)
                {
                    projectileCount++;
                }
            }

            float playerHealthRatio = static_cast<float>(playerPet->getHealth()) / playerPet->getMaxHealth();
            float enemyHealthRatio = static_cast<float>(enemyPet->getHealth()) / enemyPet->getMaxHealth();
            playerHealthBar.setSize(sf::Vector2f(150 * playerHealthRatio, 10));
            enemyHealthBar.setSize(sf::Vector2f(150 * enemyHealthRatio, 10));
            playerHealthBar.setFillColor(playerHealthRatio < 0.3f ? sf::Color::Red : sf::Color::Green);
            enemyHealthBar.setFillColor(enemyHealthRatio < 0.3f ? sf::Color::Red : sf::Color::Green);

            if(accumulatedTime >= timePerFrame)
            {
                accumulatedTime = 0.f;
                currentFrame = (currentFrame + 1) % playerAnim.numFrames;
                playerSprite.setTextureRect(sf::IntRect(currentFrame * playerAnim.frameWidth, playerAnim.animationRow * playerAnim.frameHeight, playerAnim.frameWidth, playerAnim.frameHeight));
                enemySprite.setTextureRect(sf::IntRect(currentFrame * enemyAnim.frameWidth, enemyAnim.animationRow * enemyAnim.frameHeight, enemyAnim.frameWidth, enemyAnim.frameHeight));
            }

            //  player pet status 
            if(playerPet->getHealth() <= 0)
            {
                playerPet->setHealth(0);
                if(playerType == "Phoenix" && !rebirthUsed)
                {
                    
                    playerPet->useAbility2(playerPet);
                    rebirthUsed = !playerPet->getIsAlive(); 
                }
                else
                {
                    bool playerHasAlivePet = false;
                    for (int i = 0; i < playerGuild->getMemberCount(); i++)
                    {
                        if(playerGuild->getMember(i)->getIsAlive())
                        {
                            playerHasAlivePet = true;
                            break;
                        }
                    }
                    if(!playerHasAlivePet)
                    {
                        sf::Text loseText("Defeat! Your guild has been defeated!", font, 20);
                        loseText.setPosition(140, 140);
                        loseText.setFillColor(sf::Color::Red);
                        window.clear();
                        window.draw(loseText);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        return 0;
                    }
                    int originalIndex = playerPetIndex;
                    do
                    {
                        playerPetIndex = (playerPetIndex + 1) % playerGuild->getMemberCount();
                        playerPet = playerGuild->getMember(playerPetIndex);
                    } while (!playerPet->getIsAlive() && playerPetIndex != originalIndex);
                    if(playerPet->getIsAlive())
                    {
                        playerType = playerPet->getType();
                        if(playerType == "Dragon")
                            playerAnim.loadDragon();
                        else if(playerType == "Phoenix")
                            playerAnim.loadPhoenix();
                        else if(playerType == "Unicorn")
                            playerAnim.loadUnicorn();
                        else if(playerType == "Griffin")
                            playerAnim.loadGriffin();
                        playerSprite.setTexture(playerAnim.texture);
                        playerSprite.setTextureRect(sf::IntRect(0, playerAnim.animationRow * playerAnim.frameHeight, playerAnim.frameWidth, playerAnim.frameHeight));
                        playerSprite.setScale(playerAnim.scale, playerAnim.scale);
                        playerSprite.setPosition(100, 80);
                        facingRight = true;
                        currentFrame = 0;
                        cout<<"Switched to " << playerPet->getName()<<endl;
                    }
                }
            }

            if(enemyPet->getHealth() <= 0)
            {
                enemyPet->setHealth(0);
                if(enemyType == "Phoenix" && !rebirthUsed)
                {
                    enemyPet->useAbility2(enemyPet);
                    rebirthUsed = !enemyPet->getIsAlive(); // if ability was used successfully, update rebirth
                }
                else
                {
                    enemyGuild.removeMember(enemyPetIndex);
                    if(enemyGuild.getMemberCount() == 0)
                    {
                        sf::Text winText("Victory! Enemy guild defeated!", font, 20);
                        winText.setPosition(150, 150);
                        winText.setFillColor(sf::Color::Green);
                        window.clear();
                        window.draw(winText);
                        window.display();
                        sf::sleep(sf::seconds(2));
                        for (int i = 0; i < playerGuild->getMemberCount(); i++)
                        {
                            Pet *member = playerGuild->getMember(i);
                            if(member)
                            {
                                member->gainExperience(100);
                            }
                        }
                        playerGuild->increaseGuildLevel();
                        return 1;
                    }
                    do
                    {
                        enemyPetIndex = rand() % enemyGuild.getMemberCount();
                        enemyPet = enemyGuild.getMember(enemyPetIndex);
                    } while (!enemyPet->getIsAlive());
                    enemyType = enemyPet->getType();
                    if(enemyType == "Dragon")
                        enemyAnim.loadDragon();
                    else if(enemyType == "Phoenix")
                        enemyAnim.loadPhoenix();
                    else if(enemyType == "Unicorn")
                        enemyAnim.loadUnicorn();
                    else if(enemyType == "Griffin")
                        enemyAnim.loadGriffin();
                    enemySprite.setTexture(enemyAnim.texture);
                    enemySprite.setTextureRect(sf::IntRect(0, enemyAnim.animationRow * enemyAnim.frameHeight, enemyAnim.frameWidth, enemyAnim.frameHeight));
                    enemySprite.setScale(-enemyAnim.scale, enemyAnim.scale);
                    enemySprite.setPosition(476, 80);
                    enemyFacingRight = false;
                    currentFrame = 0;
                    cout<<"Enemy switched to " + enemyPet->getName()<<endl;
                }
            }

            window.clear();
            bg.update(deltaTime);
            bg.draw(window);
            window.draw(playerSprite);
            window.draw(enemySprite);
            for (int i = 0; i < 50; i++)
            {
                if(projectiles[i].active)
                {
                    window.draw(projectiles[i].sprite);
                }
            }
            window.draw(playerHealthBg);
            window.draw(playerHealthBar);
            window.draw(enemyHealthBg);
            window.draw(enemyHealthBar);
            window.draw(timerText);
            window.draw(instructions);
            window.draw(playerGuildCount);
            window.draw(enemyGuildCount);
            window.display();
        }


        for (int i = 0; i < enemyGuild.getMemberCount(); i++)
        {
            delete enemyGuild.getMember(i);
        }
        return 0;
    }
};

// gameController class to handle the main game loop
class gameController
{
private:
    Player **players; 
    int playerCount;  
    int playerCapacity;
    Player *player;
    Shop *shop;
    TrainingCamp *trainingCamp;
    bool gameRunning;
    MusicPlayer musicPlayer;
    int currentPetIndex;

    
public:
public:
    gameController()
    {
        player = nullptr;
        shop = nullptr;
        trainingCamp = nullptr;
        gameRunning = true;
        currentPetIndex = 0;
        srand(time(0));
        players = nullptr;
        playerCount = 0;
        playerCapacity = 0;
        loadPlayers();
    }

    ~gameController()
    {
        savePlayers();
        for (int i = 0; i < playerCount; i++)
        {
            if(players[i] != player)
                delete players[i]; 
        }
        delete[] players;
        if(player)
            delete player;
        delete shop;
        delete trainingCamp;
    }


    void increasePlayerCapacity()
    {
        if(playerCount >= playerCapacity)
        {
            int newCapacity = (playerCapacity == 0) ? 10 : playerCapacity * 2;
            Player **newPlayers = new Player *[newCapacity];
            for (int i = 0; i < playerCount; i++)
            {
                newPlayers[i] = players[i];
            }
            for (int i = playerCount; i < newCapacity; i++)
            {
                newPlayers[i] = nullptr;
            }
            delete[] players;
            players = newPlayers;
            playerCapacity = newCapacity;
        }
    }

  void savePlayers()
{
    ofstream file("players.txt");
    if(!file.is_open())
    {
        cout<<"Failed to open players.txt for writing\n";
        return;
    }

    for (int i = 0; i < playerCount; i++)
    {
        Player *p = players[i];
        file << p->getName() << "|" << p->getPassword() << "|"
             << to_string(p->getGold()) << "|"
             << to_string(p->getWins()) << "|"
             << to_string(p->getLosses()) << "|";

        // pet count and pet data
        file << to_string(p->getPetCount()) << "|";
        for (int j = 0; j < p->getPetCount(); j++)
        {
            Pet *pet = p->getPet(j);
            file << pet->getType() << "," << pet->getName() << ","
                 << to_string(pet->getLevel()) << ","
                 << to_string(pet->getHealth()) << ","
                 << to_string(pet->getMaxHealth()) << ","
                 << to_string(pet->getAttack()) << ","
                 << to_string(pet->getSpeed()) << ","
                 << to_string(pet->getDefense()) << ","
                 << (pet->getIsAlive() ? "1" : "0") << ";";
        }

        //  guild data
        file << to_string(p->getGuildCount()) << "|";
        for (int j = 0; j < p->getGuildCount(); j++)
        {
            Guild *guild = p->getGuild(j);
            file << guild->getName() << "," << to_string(guild->getMemberCount());
            for (int k = 0; k < guild->getMemberCount(); k++)
            {
                Pet *member = guild->getMember(k);
                file << "," << member->getName() << "," << to_string(member->getLevel()) << ";";
            }
        }

        // Save inventory 
        file << to_string(p->getInventoryCount()) << "|";
        for (int j = 0; j < p->getInventoryCount(); j++)
        {
            Item *item = p->getInventoryItem(j);
            if(item)
            {
                file << item->getType() << "," << item->getName() << ";";
            }
        }
        file << "\n"; 
    }
    file.close();
    cout<<"Saved " << playerCount << " players\n";
}

void loadPlayers()
{
    for (int i = 0; i < playerCount; i++)
    {
        delete players[i];
    }
    delete[] players;
    playerCount = 0;
    playerCapacity = 0;
    players = nullptr;

    ifstream file("players.txt");
    if(!file.is_open())
    {
        cout<<"Failed to open players.txt\n";
        return;
    }

    char buffer[1024];
    while (file.getline(buffer, 1024))
    {
        string line = buffer;
        int pos = 0;

        
        int seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data: Missing '|' sepratoriter for name\n";
            continue;
        }
        string name = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        cout<<"Loaded name: " << name << endl;

        // password
        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data: Missing '|' sepratoriter for password\n";
            continue;
        }
        string password = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        cout<<"Loaded password: " << password << endl;

        //  gold
        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data: Missing '|' sepratoriter for gold\n";
            continue;
        }
        string goldStr = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        int gold = 0;
        for (char c : goldStr)
        {
            if(c < '0' || c > '9')
                break;
            gold = gold * 10 + (c - '0');
        }
        cout<<"Loaded gold: " << gold << endl;

        //  wins
        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data: Missing '|' sepratoriter for wins\n";
            continue;
        }
        string winsStr = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        int wins = 0;
        for (char c : winsStr)
        {
            if(c < '0' || c > '9')
                break;
            wins = wins * 10 + (c - '0');
        }
        cout<<"Loaded wins: " << wins << endl;

        //  losses
        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data: Missing '|' sepratoriter for losses\n";
            continue;
        }
        string lossesStr = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        int losses = 0;
        for (char c : lossesStr)
        {
            if(c < '0' || c > '9')
                break;
            losses = losses * 10 + (c - '0');
        }
        cout<<"Loaded losses: " << losses << endl;

        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data, missing '|' for pet count\n";
            continue;
        }
        string petCountStr = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        int petCount = 0;
        for (char c : petCountStr)
        {
            if(c < '0' || c > '9')
                break;
            petCount = petCount * 10 + (c - '0');
        }
        cout<<"Loaded pet count: " << petCount << endl;

        Player *p = new Player(name, password);
        p->setGold(gold); // Set gold directly to the loaded value
        for (int i = 0; i < wins; i++)
            p->incrementWins();
        for (int i = 0; i < losses; i++)
            p->incrementLosses();

        // Parse pets (unchanged)
        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data, missing '|' for pet data\n";
            continue;
        }
        string petData = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        int petPos = 0;
        for (int i = 0; i < petCount; i++)
        {
            int comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data: missing ',' for type\n";
                break;
            }
            string type = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;

            comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data missing ',' for pet name\n";
                break;
            }
            string petName = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;

            comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data Missing ',' for level\n";
                break;
            }
            string levelStr = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;
            int level = 0;
            for (char c : levelStr)
            {
                if(c < '0' || c > '9')
                    break;
                level = level * 10 + (c - '0');
            }

            comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data missing ',' for health\n";
                break;
            }
            string healthStr = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;
            int health = 0;
            for (char c : healthStr)
            {
                if(c < '0' || c > '9')
                    break;
                health = health * 10 + (c - '0');
            }

            comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data missing ','for maxHealth\n";
                break;
            }
            string maxHealthStr = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;
            int maxHealth = 0;
            for (char c : maxHealthStr)
            {
                if(c < '0' || c > '9')
                    break;
                maxHealth = maxHealth * 10 + (c - '0');
            }

            comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data missing ','  for attack\n";
                break;
            }
            string attackStr = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;
            int attack = 0;
            for (char c : attackStr)
            {
                if(c < '0' || c > '9')
                    break;
                attack = attack * 10 + (c - '0');
            }

            comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data missing ','  for speed\n";
                break;
            }
            string speedStr = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;
            int speed = 0;
            for (char c : speedStr)
            {
                if(c < '0' || c > '9')
                    break;
                speed = speed * 10 + (c - '0');
            }

            comma = petData.find(',', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data missing ',' for defense\n";
                break;
            }
            string defenseStr = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;
            int defense = 0;
            for (char c : defenseStr)
            {
                if(c < '0' || c > '9')
                    break;
                defense = defense * 10 + (c - '0');
            }

            comma = petData.find(';', petPos);
            if(comma == string::npos)
            {
                cout<<"Invalid pet data missing ';' for isAlive\n";
                break;
            }
            string aliveStr = petData.substr(petPos, comma - petPos);
            petPos = comma + 1;
            bool isAlive = (aliveStr == "1");

            Pet *pet = nullptr;
            if(type == "Dragon")
                pet = new Dragon(petName);
            else if(type == "Phoenix")
                pet = new Phoenix(petName);
            else if(type == "Unicorn")
                pet = new Unicorn(petName);
            else if(type == "Griffin")
                pet = new Griffin(petName);

            if(pet)
            {
                for (int j = 1; j < level; j++)
                    pet->levelUp();
                pet->setHealth(health);
                if(health <= 0)
                    pet->setIsAlive(false);
                else
                    pet->setIsAlive(isAlive);
                p->addPet(pet);
                cout<<"Loaded pet: " << petName << ", type: " << type << ", health: " << health << ", alive: " << pet->getIsAlive() << endl;
            }
        }

        // guild count
        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"Invalid data missing '|' for guild count\n";
            continue;
        }
        string guildCountStr = line.substr(pos, seprator - pos);
        pos = seprator + 1;
        int guildCount = 0;
        for (char c : guildCountStr)
        {
            if(c < '0' || c > '9')
                break;
            guildCount = guildCount * 10 + (c - '0');
        }
        cout<<"Loaded guild count: " << guildCount << endl;

        // Parse guilds
        seprator = line.find('|', pos);
        string guildData = (seprator == string::npos) ? "" : line.substr(pos, seprator - pos);
        pos = (seprator == string::npos) ? line.length() : seprator + 1;
        if(!guildData.empty())
        {
            int guildPos = 0;
            for (int i = 0; i < guildCount; i++)
            {
                int comma = guildData.find(',', guildPos);
                if(comma == string::npos)
                {
                    cout<<"Invalid guild data missing ','  for guild name\n";
                    break;
                }
                string guildName = guildData.substr(guildPos, comma - guildPos);
                guildPos = comma + 1;

                comma = guildData.find(',', guildPos);
                if(comma == string::npos)
                {
                    cout<<"Invalid guild data missing ',' for member count\n";
                    break;
                }
                string memberCountStr = guildData.substr(guildPos, comma - guildPos);
                guildPos = comma + 1;
                int memberCount = 0;
                for (char c : memberCountStr)
                {
                    if(c < '0' || c > '9')
                        break;
                    memberCount = memberCount * 10 + (c - '0');
                }

                p->createGuild(guildName);
                Guild *guild = p->getGuild(i);

                for (int j = 0; j < memberCount; j++)
                {
                    comma = guildData.find(',', guildPos);
                    if(comma == string::npos)
                    {
                        cout<<"Invalid guild data missing ',' for pet name\n";
                        break;
                    }
                    string petName = guildData.substr(guildPos, comma - guildPos);
                    guildPos = comma + 1;

                    comma = guildData.find(';', guildPos);
                    if(comma == string::npos)
                    {
                        cout<<"Invalid guild data: missing ';'  for level\n";
                        break;
                    }
                    string levelStr = guildData.substr(guildPos, comma - guildPos);
                    guildPos = comma + 1;

                    for (int k = 0; k < p->getPetCount(); k++)
                    {
                        Pet *pet = p->getPet(k);
                        if(pet->getName() == petName)
                        {
                            guild->addMember(pet);
                            break;
                        }
                    }
                }
                cout<<"Loaded guild: " << guildName << " with " << memberCount << " members\n";
            }
        }
        else if(guildCount > 0)
        {
            cout<<"guild data missing guild count is " << guildCount << ". Skipping \n";
        }
        seprator = line.find('|', pos);
        if(seprator == string::npos)
        {
            cout<<"missing '|', 0 items\n";
            pos = line.length(); 
        }
        else
        {
            string invCountStr = line.substr(pos, seprator - pos);
            pos = seprator + 1;
            int invCount = 0;
            for (char c : invCountStr)
            {
                if(c < '0' || c > '9')
                    break;
                invCount = invCount * 10 + (c - '0');
            }
            cout<<"Loaded inventory count: " << invCount << endl;

            string invData = line.substr(pos);
            int invPos = 0;
            for (int i = 0; i < invCount; i++)
            {
                int comma = invData.find(',', invPos);
                if(comma == string::npos || comma == invPos)
                {
                    cout<<"Invalid inventory data: Missing ',' for type or empty\n";
                    break;
                }
                string type = invData.substr(invPos, comma - invPos);
                invPos = comma + 1;

                comma = invData.find(';', invPos);
                if(comma == string::npos || comma == invPos)
                {
                    cout<<"Invalid inventory data missing ';' for item name or empty\n";
                    break;
                }
                string itemName = invData.substr(invPos, comma - invPos);
                invPos = comma + 1;

                Item *item = nullptr;
                if(type == "Healing")
                    item = new HealingPotion();
                else if(type == "Mana")
                    item = new ManaPotion();
                else if(type == "Shield")
                    item = new Shield();
                if(item)
                    p->addItemToInventory(item);
                cout<<"Loaded item: " << itemName << ", type: " << type << endl;
            }
        }

        increasePlayerCapacity();
        players[playerCount++] = p; 
    }
    file.close();
    cout<<"Loaded " << playerCount << " players\n";
}

//creating the new playe when click new game
void createNewPlayer(sf::RenderWindow &window, sf::Font &font)
    {
        sf::Texture board;
        board.loadFromFile("assets/board.png");
        sf::Sprite brd(board);
        brd.setPosition(10, 15);
        brd.setScale(2.3f, 1.2f);

        ParallaxBackground bg(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);

        sf::Text title("Creating New Player", font, 15);
        title.setPosition(200, 45);
        title.setFillColor(sf::Color::White);
        title.setStyle(sf::Text::Bold);

        sf::Text namePrompt("Enter your name:", font, 15);
        namePrompt.setPosition(70, 90);
        namePrompt.setStyle(sf::Text::Bold);

        sf::Text nameText("", font, 15);
        nameText.setPosition(219, 93);
        nameText.setFillColor(sf::Color::Green);
        nameText.setStyle(sf::Text::Bold);

        string playerName;
        bool nameEntered = false;
        sf::Clock clock;
        while (window.isOpen() && !nameEntered)
        {
            float dTime = clock.restart().asSeconds();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }

                if(event.type == sf::Event::TextEntered)
                {
                    if(event.text.unicode == '\b' && !playerName.empty())
                    {
                        playerName.pop_back();
                    }
                    else if(event.text.unicode < 128 && event.text.unicode != '\r')
                    {
                        playerName += static_cast<char>(event.text.unicode);
                    }
                    nameText.setString(playerName);
                }
                else if(event.type == sf::Event::KeyPressed &&
                         event.key.code == sf::Keyboard::Enter &&
                         !playerName.empty())
                {
                    nameEntered = true;
                }
            }
            bg.update(dTime);
            window.clear();
            bg.draw(window);
            window.draw(brd);
            window.draw(title);
            window.draw(namePrompt);
            window.draw(nameText);
            window.display();
        }
        sf::Text passPrompt("Enter your password:", font, 15);
        passPrompt.setPosition(70, 90);
        passPrompt.setStyle(sf::Text::Bold);

        sf::Text passText("", font, 15);
        passText.setPosition(265, 93);
        passText.setFillColor(sf::Color::Green);
        passText.setStyle(sf::Text::Bold);

        string password;
        bool passEntered = false;
        while (window.isOpen() && !passEntered)
        {
            float dTime = clock.restart().asSeconds();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }

                if(event.type == sf::Event::TextEntered)
                {
                    if(event.text.unicode == '\b' && !password.empty())
                    {
                        password.pop_back();
                    }
                    else if(event.text.unicode < 128 && event.text.unicode != '\r')
                    {
                        password += static_cast<char>(event.text.unicode);
                    }
                    passText.setString(string(password.length(), '*'));
                }
                else if(event.type == sf::Event::KeyPressed &&
                         event.key.code == sf::Keyboard::Enter &&
                         !password.empty())
                {
                    passEntered = true;
                }
            }
            bg.update(dTime);
            window.clear();
            bg.draw(window);
            window.draw(brd);
            window.draw(title);
            window.draw(passPrompt);
            window.draw(passText);
            window.display();
        }

        // For Pet selection
        sf::Text petPrompt("Choose your starter pet:", font, 15);
        petPrompt.setPosition(100, 80);
        petPrompt.setFillColor(sf::Color::White);
        petPrompt.setStyle(sf::Text::Bold);

        sf::Text petOptions[4] = {
            sf::Text("1. Dragon (High health & damage)", font, 15),
            sf::Text("2. Phoenix (High defense & healing)", font, 15),
            sf::Text("3. Unicorn (High speed & buffs)", font, 15),
            sf::Text("4. Griffin (Balanced stats)", font, 15)};

        for (int i = 0; i < 4; i++)
        {
            petOptions[i].setPosition(100, 130 + i * 35);
            petOptions[i].setFillColor(sf::Color::White);
            petOptions[i].setStyle(sf::Text::Bold);
        }

        int petChoice = 0;
        bool petSelected = false;

        while (window.isOpen() && !petSelected)
        {
            float dTime = clock.restart().asSeconds();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }

                if(event.type == sf::Event::KeyPressed)
                {
                    if(event.key.code >= sf::Keyboard::Num1 &&
                        event.key.code <= sf::Keyboard::Num4)
                    {
                        petChoice = event.key.code - sf::Keyboard::Num1 + 1;
                        petSelected = true;
                    }
                }
            }
            bg.update(dTime);
            window.clear();
            bg.draw(window);
            window.draw(brd);
            window.draw(title);
            window.draw(petPrompt);
            for (int i = 0; i < 4; i++)
            {
                window.draw(petOptions[i]);
            }
            window.display();
        }

        // Pet naming
        sf::Text namePetPrompt("Name your pet:", font, 15);
        namePetPrompt.setPosition(50, 90);
        namePetPrompt.setFillColor(sf::Color::White);
        namePetPrompt.setStyle(sf::Text::Bold);

        string petName;
        sf::Text petNameText("", font, 15);
        petNameText.setStyle(sf::Text::Bold);

        petNameText.setString("");
        petNameText.setPosition(185, 93);
        petNameText.setFillColor(sf::Color::Green);

        bool petNamed = false;

        while (window.isOpen() && !petNamed)
        {
            float dTime = clock.restart().asSeconds();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }

                if(event.type == sf::Event::TextEntered)
                {
                    if(event.text.unicode == '\b' && !petName.empty())
                    {
                        petName.pop_back();
                    }
                    else if(event.text.unicode < 128 && event.text.unicode != '\r')
                    {
                        petName += static_cast<char>(event.text.unicode);
                    }
                    petNameText.setString(petName);
                }
                else if(event.type == sf::Event::KeyPressed &&
                         event.key.code == sf::Keyboard::Enter &&
                         !petName.empty())
                {
                    petNamed = true;
                }
            }
            bg.update(dTime);
            window.clear();
            bg.draw(window);
            window.draw(brd);
            window.draw(title);
            window.draw(namePetPrompt);
            window.draw(petNameText);
            window.display();
        }

        // creating the pet and player
        Pet *starterPet = nullptr;
        switch (petChoice)
        {
        case 1:
            starterPet = new Dragon(petName);
            break;
        case 2:
            starterPet = new Phoenix(petName);
            break;
        case 3:
            starterPet = new Unicorn(petName);
            break;
        case 4:
            starterPet = new Griffin(petName);
            break;
        }

        player = new Player(playerName, password);
        player->addPet(starterPet);
        player->setGold(500);
        increasePlayerCapacity();
        players[playerCount++] = player; 
        savePlayers();

        sf::Text confirmation(playerName + ", you now have a " + starterPet->getType() + "\nNamed: " + petName + "!", font, 20);
        confirmation.setOrigin(288, 162);
        confirmation.setPosition(358, 230);
        confirmation.setFillColor(sf::Color::White);
        float dTime = clock.restart().asSeconds();
        bg.update(dTime);
        window.clear();
        bg.draw(window);
        window.draw(brd);
        window.draw(confirmation);
        window.display();
        sf::sleep(sf::seconds(3));
    }

    bool authenticate(string name, string password)
    {
        for (int i = 0; i < playerCount; i++)
        { 
            if(players[i]->getName() == name && players[i]->getPassword() == password)
            {
                player = players[i];
                return true;
            }
        }
        return false;
    }
    bool loadGame(sf::RenderWindow &window, sf::Font &font)
    {
        ParallaxBackground bg(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/Bg2/1.png", 20.f);
        bg.addLayer("assets/Bg2/2.png", 20.f);
        bg.addLayer("assets/Bg2/3.png", 20.f);
        bg.addLayer("assets/Bg2/4.png", 20.f);
        bg.addLayer("assets/Bg2/5.png", 20.f);
        bg.addLayer("assets/Bg2/6.png", 20.f);

        sf::RectangleShape loginBg(sf::Vector2f(400, 200));
        loginBg.setFillColor(sf::Color(50, 50, 50));
        loginBg.setPosition(88, 62);

        sf::Text title("Login to Continue", font, 20);
        title.setPosition(200, 70);
        title.setFillColor(sf::Color::White);

        sf::Text namePrompt("Username:", font, 15);
        namePrompt.setPosition(120, 100);
        sf::Text nameText("", font, 15);
        nameText.setPosition(200, 100);
        nameText.setFillColor(sf::Color::Green);

        sf::Text passPrompt("Password:", font, 15);
        passPrompt.setPosition(120, 140);
        sf::Text passText("", font, 15);
        passText.setPosition(200, 140);
        passText.setFillColor(sf::Color::Green);

        Button loginButton(220, 180, 100, 30, font, "Login");
        loginButton.setColors(sf::Color::Blue, sf::Color::Cyan, sf::Color::Blue, sf::Color::White);

        sf::Text errorText("", font, 15);
        errorText.setFillColor(sf::Color::Red);
        errorText.setPosition(120, 220);

        string inputName, inputPass;
        int activeField = 1; 
        sf::Clock clock;

        while (window.isOpen() && !authenticate(inputName, inputPass))
        {
            float dTime = clock.restart().asSeconds();
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return false;
                }

                if(event.type == sf::Event::TextEntered)
                {
                    if(activeField == 1 && event.text.unicode < 128 && event.text.unicode != '\r')
                    {
                        if(event.text.unicode == '\b' && !inputName.empty())
                        {
                            inputName.pop_back();
                        }
                        else if(event.text.unicode != '\b')
                        { // excluding backspace from adding characters
                            inputName += static_cast<char>(event.text.unicode);
                        }
                        nameText.setString(inputName);
                    }
                    else if(activeField == 2 && event.text.unicode < 128 && event.text.unicode != '\r')
                    {
                        if(event.text.unicode == '\b' && !inputPass.empty())
                        {
                            inputPass.pop_back();
                        }
                        else if(event.text.unicode != '\b')
                        {
                            inputPass += static_cast<char>(event.text.unicode);
                        }
                        passText.setString(string(inputPass.length(), '*'));
                    }
                }

                if(event.type == sf::Event::KeyPressed)
                {
                    if(event.key.code == sf::Keyboard::Tab || event.key.code == sf::Keyboard::Down)
                    {
                        activeField = (activeField % 3) + 1; 
                    }
                    else if(event.key.code == sf::Keyboard::Up)
                    {
                        activeField = (activeField - 2 + 3) % 3 + 1; 
                    }
                    else if(event.key.code == sf::Keyboard::Enter)
                    {
                        if(activeField == 3)
                        { 
                            if(authenticate(inputName, inputPass))
                            {
                                player = nullptr;
                                for (int i = 0; i < playerCount; i++)
                                {
                                    if(players[i]->getName() == inputName && players[i]->getPassword() == inputPass)
                                    {
                                        player = players[i];
                                        break;
                                    }
                                }
                                return player != nullptr;
                            }
                            else
                            {
                                errorText.setString("Invalid username or password!");
                            }
                        }
                    }
                }
            }

//active fileds
            nameText.setFillColor(activeField == 1 ? sf::Color::Cyan : sf::Color::Green);
            passText.setFillColor(activeField == 2 ? sf::Color::Cyan : sf::Color::Green);
            loginButton.getText().setFillColor(activeField == 3 ? sf::Color::Cyan : sf::Color::White);

            bg.update(dTime);
            window.clear();
            bg.draw(window);
            window.draw(loginBg);
            window.draw(title);
            window.draw(namePrompt);
            window.draw(nameText);
            window.draw(passPrompt);
            window.draw(passText);
            window.draw(errorText);
            loginButton.render(window);
            window.display();
        }
        return true;
    }

    void showLeaderboard(sf::RenderWindow &window, sf::Font &font)
    {
        ParallaxBackground bg(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);

        struct LeaderboardEntry
        {
            int wins;
            string name;
        };
        LeaderboardEntry *leaderboard = new LeaderboardEntry[playerCount];
        for (int i = 0; i < playerCount; i++)
        {
            leaderboard[i].wins = players[i]->getWins();
            leaderboard[i].name = players[i]->getName();
        }

        // sorting the players
        for (int i = 0; i < playerCount - 1; i++)
        {
            for (int j = 0; j < playerCount - i - 1; j++)
            {
                if(leaderboard[j].wins < leaderboard[j + 1].wins)
                {
                    LeaderboardEntry temp = leaderboard[j];
                    leaderboard[j] = leaderboard[j + 1];
                    leaderboard[j + 1] = temp;
                }
            }
        }

        sf::RectangleShape panel(sf::Vector2f(400, 250));
        panel.setFillColor(sf::Color(50, 50, 50));
        panel.setPosition(88, 37);

        sf::Text title("Leaderboard", font, 25);
        title.setPosition(230, 40);
        title.setFillColor(sf::Color::White);

        sf::Text *entries = new sf::Text[playerCount];
        int y = 80;
        for (int i = 0; i < playerCount; i++)
        {
            entries[i].setFont(font);
            entries[i].setString(leaderboard[i].name + ": " + to_string(leaderboard[i].wins) + " wins");
            entries[i].setCharacterSize(15);
            entries[i].setPosition(100, y);
            entries[i].setFillColor(sf::Color::White);
            y += 30;
        }

        Button backButton(220, 260, 100, 30, font, "Back");
        backButton.setColors(sf::Color::Red, sf::Color::Red, sf::Color::Red, sf::Color::White);

        sf::Clock clock;
        bool showing = true;
        while (window.isOpen() && showing)
        {
            float dTime = clock.restart().asSeconds();
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                {
                    window.close();
                    return;
                }
                if(event.type == sf::Event::MouseButtonReleased && backButton.getShape().getGlobalBounds().contains(mousePos))
                {
                    showing = false;
                }
            }

            backButton.update(mousePos);
            bg.update(dTime);
            window.clear();
            bg.draw(window);
            window.draw(panel);
            window.draw(title);
            for (int i = 0; i < playerCount; i++)
                window.draw(entries[i]);
            backButton.render(window);
            window.display();
        }

        delete[] leaderboard;
        delete[] entries;
    }

    void shopMenu(sf::RenderWindow &window, sf::Font &font, sf::Sound &sound)
    {
        Shop shop(window , font, player);
        shop.run();
    }

    void trainingMenu(sf::RenderWindow &window, sf::Font &font, sf::Sound &sound)
    {

        trainingCamp = new TrainingCamp(window, font, player);
        trainingCamp->run();
    }

    void guildMenu(sf::RenderWindow &window, sf::Font &font, sf::Sound &sound)
    {
        GuildMenu guildMenu(window, font, player);
        guildMenu.run();
    }

    void inventoryMenu(sf::RenderWindow &window, sf::Font &font)
    {
        InventoryMenu inventory(window, font, player);
        inventory.run();
    }

    void battleMenu(sf::RenderWindow &window, sf::Font font, sf::Sound &sound)
    {
        bool battling = true;

        while (battling)
        {

            ParallaxBackground bg(window.getSize().x, window.getSize().y);
            bg.addLayer("assets/BattleBG/1.png", 20.f);
            bg.addLayer("assets/BattleBG/2.png", 20.f);
            bg.addLayer("assets/BattleBG/3.png", 20.f);
            bg.addLayer("assets/BattleBG/4.png", 20.f);
            bg.addLayer("assets/BattleBG/5.png", 20.f);
            bg.addLayer("assets/BattleBG/6.png", 20.f);
            bg.addLayer("assets/BattleBG/7.png", 20.f);

            sf::Font title;
            title.loadFromFile("assets/tl.ttf");
            sf::Text MenuTxt("Choose your battle type!", title, 20);
            MenuTxt.setPosition(190, 80);
            MenuTxt.setFillColor(sf::Color(10, 20, 60, 255));
            MenuTxt.setOutlineThickness(2);
            MenuTxt.setOutlineColor(sf::Color::White);

            sf::Text battleMenu("--ARENA BATTLES--", title, 35);
            battleMenu.setPosition(170, 20);
            battleMenu.setFillColor(sf::Color(10, 20, 60, 255));

            battleMenu.setOutlineThickness(2);
            battleMenu.setOutlineColor(sf::Color::White);
            string options[] = {"Single Battle", "Guild War", "Return"};
            const int btn = 3; 

            Button buttons[btn] = {
                Button(210, 120, 150, 50, font, options[0]),
                Button(210, 190, 150, 50, font, options[1]),
                Button(210, 270, 150, 50, font, options[2])};

            int choice = 0;

            bool menuActive = true;
            sf::Clock clock;
            while (menuActive && window.isOpen())
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                float dTime = clock.restart().asSeconds();
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if(event.type == sf::Event::Closed)
                    {
                        window.close();
                        menuActive = false;
                        battling = false;
                    }
                    if(event.type == sf::Event::KeyPressed)
                    {

                        if(event.key.code == sf::Keyboard::Up)
                        {
                            sound.play();
                            choice = (choice - 1 + btn) % btn;
                        }
                        else if(event.key.code == sf::Keyboard::Down)
                        {
                            sound.play();
                            choice = (choice + 1) % btn;
                        }
                        else if(event.key.code == sf::Keyboard::Enter)
                        {
                            sound.play();
                            menuActive = false;
                        }
                    }
                }

                for (int i = 0; i < btn; i++)
                {
                    if(i == choice)
                    {
                        buttons[i].getShape().setFillColor(sf::Color(10, 20, 60, 255));
                        buttons[i].getText().setFillColor(sf::Color::Cyan);
                    }
                    else
                    {
                        buttons[i].update(mousePos);
                        buttons[i].getText().setFillColor(sf::Color::White);
                    }
                }

                bg.update(dTime);
                window.clear();
                bg.draw(window);

                window.draw(battleMenu);
                window.draw(MenuTxt);

                for (int i = 0; i < btn; i++)
                {
                    buttons[i].render(window);
                }
                window.display();
            }

            if(battling)
            {
                switch (choice + 1)
                {
                case 1:
                    singleBattle(window, font, sound);
                    break;
                case 2:
                    guildBattle(window, font, sound);
                    break;
                case 3:
                    battling = false;
                    break;
                default:
                    cout<<"Invalid choice!"<<endl;
                    break;
                }
            }
        }
    }

    void singleBattle(sf::RenderWindow &window, sf::Font &font, sf::Sound &sound)
    {
        if(player->getPetCount() == 0)
        {
            sf::Text noPetsText("You have no pets to battle with!", font, 20);
            noPetsText.setPosition(150, 150);
            noPetsText.setFillColor(sf::Color::Red);
            window.clear();
            window.draw(noPetsText);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

        // selected pet from petMenu 
        Pet *playerPet = player->getPet(currentPetIndex); 
        if(!playerPet || !playerPet->getIsAlive())
        {
            sf::Text errorText("Selected pet is not available or fainted!", font, 20);
            errorText.setPosition(130, 130);
            errorText.setFillColor(sf::Color::Red);
            window.clear();
            window.draw(errorText);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

        // random enemy pet
        Pet *enemyPet = nullptr;
        int randPet = rand() % 4;
        string enemyName = "Enemy" + to_string(rand() % 5);
        switch (randPet)
        {
        case 0:
            enemyPet = new Dragon(enemyName);
            break;
        case 1:
            enemyPet = new Phoenix(enemyName);
            break;
        case 2:
            enemyPet = new Unicorn(enemyName);
            break;
        case 3:
            enemyPet = new Griffin(enemyName);
            break;
        }

        // starting the battle
        Battle battle;
        int result = battle.battle(window, playerPet, enemyPet, font);

        // battle result
        if(result == 1)
        {
            player->addGold(50);                                       
            playerPet->gainExperience(50 + enemyPet->getLevel() * 10); 
            sf::Text winText("Victory! Gained 50 gold and XP!", font, 20);
            winText.setPosition(150, 150);
            winText.setFillColor(sf::Color::Green);
            window.clear();
            window.draw(winText);
            window.display();
            sf::sleep(sf::seconds(2));
            savePlayers();
        }
        else if(result == 0)
        {
            sf::Text loseText("Defeat! Better luck next time!", font, 20);
            loseText.setPosition(150, 150);
            loseText.setFillColor(sf::Color::Red);
            window.clear();
            window.draw(loseText);
            window.display();
            sf::sleep(sf::seconds(2));
        }
        savePlayers();

        delete enemyPet;
    }
    void guildBattle(sf::RenderWindow &window, sf::Font &font, sf::Sound &sound)
    {
        if(player->getGuildCount() == 0)
        {
            sf::Text noGuildText("You have no guilds! Create one first.", font, 20);
            noGuildText.setPosition(150, 150);
            noGuildText.setFillColor(sf::Color::Red);
            window.clear();
            window.draw(noGuildText);
            window.display();
            sf::sleep(sf::seconds(2));
            return;
        }

        //using first guild 
        Guild *playerGuild = player->getGuild(0);
        Battle battle;
        int result = battle.guildBattle(window, playerGuild, font);

        if(result == 1)
            player->incrementWins();
        else if(result == 0)
            player->incrementLosses();
        savePlayers();
    }


    int petMenu(sf::RenderWindow &window, sf::Font &font)
    {
        PetAnimation dragonAnim, phoenixAnim, unicornAnim, griffinAnim;
        dragonAnim.loadDragon();
        phoenixAnim.loadPhoenix();
        unicornAnim.loadUnicorn();
        griffinAnim.loadGriffin();

        sf::Texture board;
        board.loadFromFile("assets/board.png");
        sf::Sprite brd(board);
        brd.setPosition(10, 15);
        brd.setScale(2.3f, 1.2f);

        int Count = player->getPetCount();
        int currentPetIndex = 0;
        string petType = player->getPetByName(currentPetIndex);

        if(Count == 0)
        {
//no pets
            sf::Text noPetsText("You have no pets!", font, 20);
            noPetsText.setPosition(150, 150);
            noPetsText.setFillColor(sf::Color::White);

            while (window.isOpen())
            {
                sf::Event event;
                while (window.pollEvent(event))
                {
                    if(event.type == sf::Event::Closed)
                        window.close();
                    if(event.type == sf::Event::KeyPressed &&
                        event.key.code == sf::Keyboard::Escape)
                        return -1;
                }

                window.clear();
                window.draw(noPetsText);
                window.display();
            }
            return -1;
        }

        PetAnimation *currentAnim;
        if(petType == "Dragon")
            currentAnim = &dragonAnim;
        else if(petType == "Phoenix")
            currentAnim = &phoenixAnim;
        else if(petType == "Unicorn")
            currentAnim = &unicornAnim;
        else if(petType == "Griffin")
            currentAnim = &griffinAnim;

        sf::Sprite currentSprite;
        currentSprite.setTexture(currentAnim->texture);
        currentSprite.setTextureRect(sf::IntRect(0, currentAnim->animationRow * currentAnim->frameHeight,
                                                 currentAnim->frameWidth, currentAnim->frameHeight));
        currentSprite.setScale(currentAnim->scale, currentAnim->scale);
        currentSprite.setPosition(currentAnim->posX, currentAnim->posY);

        float animationTimer = 0.0f;

        sf::Text petCount("Pet " + to_string(currentPetIndex + 1) + " of " + to_string(Count), font, 19);
        petCount.setPosition(50, 50);
        petCount.setFillColor(sf::Color::White);

        sf::Text petName("", font, 15);
        petName.setPosition(50, 80);
        petName.setFillColor(sf::Color::White);
        petName.setStyle(sf::Text::Bold);

        sf::Text petStats("", font, 12);
        petStats.setPosition(50, 110);
        petStats.setFillColor(sf::Color::White);
        petStats.setStyle(sf::Text::Bold);

        sf::Text instructions("Press Numpad 1-" + to_string(Count) + " to view pets", font, 12);
        instructions.setPosition(50, 230);
        instructions.setFillColor(sf::Color::White);
        instructions.setStyle(sf::Text::Bold);

        // buttons
        Button buyPetButton(350, 230, 150, 30, font, "Buy New Pet");
        Button revivePetButton(350, 270, 150, 30, font, "Revive Pet");
        int selectedBtn = 0; 
        bool isBuyingPet = false;
        bool isNamingPet = false;
        string newPetName;
        sf::Text newPetNameText("", font, 15);
        newPetNameText.setPosition(200, 200);
        newPetNameText.setFillColor(sf::Color::Green);
        newPetNameText.setStyle(sf::Text::Bold);
        sf::Text buyPrompt("", font, 15);
        buyPrompt.setPosition(100, 150);
        buyPrompt.setFillColor(sf::Color::White);
        buyPrompt.setStyle(sf::Text::Bold);
        int newPetChoice = 0;

        updatePetSprite(currentSprite, *currentAnim, player->getPet(0),petName, petStats, font);
        ParallaxBackground bg(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);

        int currentFrame = 0;
        sf::Clock animationClock;
        const float timePerFrame = 0.2f;
        float accumulatedTime = 0.f;

        sf::Clock frameClock;

        while (window.isOpen())
        {
            float dTime = frameClock.restart().asSeconds();
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            sf::Event event;
            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();
                if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && !isBuyingPet && !isNamingPet)
                    return -1;

                if(isBuyingPet)
                {
                    // pet selection
                    if(event.type == sf::Event::KeyPressed)
                    {
                        if(event.key.code >= sf::Keyboard::Num1 &&
                            event.key.code <= sf::Keyboard::Num4)
                        {
                            newPetChoice = event.key.code - sf::Keyboard::Num1 + 1;
                            isBuyingPet = false;
                            isNamingPet = true;
                            buyPrompt.setString("Name your new pet:");
                            newPetName.clear();
                            newPetNameText.setString("");
                        }
                        else if(event.key.code == sf::Keyboard::Escape)
                        {
                            isBuyingPet = false;
                            buyPrompt.setString("");

                            updatePetSprite(currentSprite, *currentAnim, player->getPet(currentPetIndex),petName, petStats, font);
                            instructions.setString("Press Numpad 1-" + to_string(Count) + " to view pets");
                        }
                    }
                }
                else if(isNamingPet)
                {
                    if(event.type == sf::Event::TextEntered)
                    {
                        if(event.text.unicode == '\b' && !newPetName.empty())
                        {
                            newPetName.pop_back();
                        }
                        else if(event.text.unicode < 128 && event.text.unicode != '\r')
                        {
                            newPetName += static_cast<char>(event.text.unicode);
                        }
                        newPetNameText.setString(newPetName);
                    }
                    else if(event.type == sf::Event::KeyPressed &&
                             event.key.code == sf::Keyboard::Enter &&
                             !newPetName.empty())
                    {
                        Pet *newPet = nullptr;
                        switch (newPetChoice)
                        {
                        case 1:
                            newPet = new Dragon(newPetName);
                            break;
                        case 2:
                            newPet = new Phoenix(newPetName);
                            break;
                        case 3:
                            newPet = new Unicorn(newPetName);
                            break;
                        case 4:
                            newPet = new Griffin(newPetName);
                            break;
                        }
                        if(newPet && player->addPet(newPet))
                        {
                            player->spendGold(100);
                            Count = player->getPetCount();
                            currentPetIndex = Count - 1; 
                            petCount.setString("Pet " + to_string(currentPetIndex + 1) + " of " + to_string(Count));
                            
                            // Update the pet sprite and stats
                            string petType = player->getPetByName(currentPetIndex);
                            if(petType == "Dragon")
                                currentAnim = &dragonAnim;
                            else if(petType == "Phoenix")
                                currentAnim = &phoenixAnim;
                            else if(petType == "Unicorn")
                                currentAnim = &unicornAnim;
                            else if(petType == "Griffin")
                                currentAnim = &griffinAnim;
                            currentFrame = 0;
                            updatePetSprite(currentSprite, *currentAnim, player->getPet(currentPetIndex),petName, petStats, font);
                            instructions.setString("Press Numpad 1-" + to_string(Count) + " to view pets");
                        }
                        else if(newPet)
                        {
                            delete newPet; 
                        }
                        isNamingPet = false;
                        buyPrompt.setString("");
                        newPetNameText.setString("");
                    }
                    else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                    {
                        isNamingPet = false;
                        buyPrompt.setString("");
                        newPetNameText.setString("");
                        // show again pet display
                        updatePetSprite(currentSprite, *currentAnim, player->getPet(currentPetIndex),
                                        petName, petStats, font);
                        instructions.setString("Press Numpad 1-" + to_string(Count) + " to view pets");
                    }
                }
                else
                {
                    // pet selection
                    if(event.type == sf::Event::KeyPressed)
                    {
                        if(event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Up)
                        {
                            if(selectedBtn == 0)
                            {
                                currentPetIndex = (currentPetIndex - 1 + Count) % Count;
                                petCount.setString("Pet " + to_string(currentPetIndex + 1) + " of " + to_string(Count));
                                string petType = player->getPetByName(currentPetIndex);
                                if(petType == "Dragon")
                                    currentAnim = &dragonAnim;
                                else if(petType == "Phoenix")
                                    currentAnim = &phoenixAnim;
                                else if(petType == "Unicorn")
                                    currentAnim = &unicornAnim;
                                else if(petType == "Griffin")
                                    currentAnim = &griffinAnim;
                                currentFrame = 0;
                                updatePetSprite(currentSprite, *currentAnim, player->getPet(currentPetIndex),petName, petStats, font);
                            }
                            else
                            {
                                selectedBtn = (selectedBtn - 1 + (player->getPet(currentPetIndex)->getIsAlive() ? 1 : 2)) % (player->getPet(currentPetIndex)->getIsAlive() ? 2 : 3);
                            }
                        }
                        else if(event.key.code == sf::Keyboard::Right || event.key.code == sf::Keyboard::Down)
                        {
                            if(selectedBtn == 0)
                            {
                                currentPetIndex = (currentPetIndex + 1) % Count;
                                petCount.setString("Pet " + to_string(currentPetIndex + 1) + " of " + to_string(Count));
                                string petType = player->getPetByName(currentPetIndex);
                                if(petType == "Dragon")
                                    currentAnim = &dragonAnim;
                                else if(petType == "Phoenix")
                                    currentAnim = &phoenixAnim;
                                else if(petType == "Unicorn")
                                    currentAnim = &unicornAnim;
                                else if(petType == "Griffin")
                                    currentAnim = &griffinAnim;
                                currentFrame = 0;
                                updatePetSprite(currentSprite, *currentAnim, player->getPet(currentPetIndex),petName, petStats, font);
                            }
                            else
                            {
                                selectedBtn = (selectedBtn + 1) % (player->getPet(currentPetIndex)->getIsAlive() ? 2 : 3);
                            }
                        }
                        else if(event.key.code == sf::Keyboard::Enter)
                        {
                            if(selectedBtn == 1)
                            {
                                // buy new pet
                                if(player->getGold() >= 100)
                                {
                                    isBuyingPet = true;
                                    buyPrompt.setString("Choose pet type:\n1. Dragon\n2. Phoenix\n3. Unicorn\n4. Griffin");
                                    
                                    instructions.setString("");
                                }
                                else
                                {
                                    buyPrompt.setString("Not enough gold! Need 100 gold.");
                                }
                            }
                            else if(selectedBtn == 2 && !player->getPet(currentPetIndex)->getIsAlive())
                            {
                                // revive pet
                                player->getPet(currentPetIndex)->revive();
                                updatePetSprite(currentSprite, *currentAnim, player->getPet(currentPetIndex),
                                                petName, petStats, font);
                            }
                        }
                    }
                    //for mouse input
                    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                    {
                        if(buyPetButton.getShape().getGlobalBounds().contains(mousePos))
                        {
                            if(player->getGold() >= 100)
                            {
                                isBuyingPet = true;
                                buyPrompt.setString("Choose pet type:\n1. Dragon\n2. Phoenix\n3. Unicorn\n4. Griffin");
                                instructions.setString("");
                            }
                            else
                            {
                                buyPrompt.setString("Not enough gold! Need 100 gold.");
                            }
                        }
                        else if(revivePetButton.getShape().getGlobalBounds().contains(mousePos) &&
                                 !player->getPet(currentPetIndex)->getIsAlive())
                        {
                            player->getPet(currentPetIndex)->revive();
                            updatePetSprite(currentSprite, *currentAnim, player->getPet(currentPetIndex),
                                            petName, petStats, font);
                        }
                    }
                }
            }

            buyPetButton.update(mousePos);
            revivePetButton.update(mousePos);
            if(selectedBtn == 1)
            {
                buyPetButton.getShape().setFillColor(sf::Color(10, 20, 60, 255));
                buyPetButton.getText().setFillColor(sf::Color::Cyan);
            }
            else
            {
                buyPetButton.getText().setFillColor(sf::Color::White);
            }
            if(selectedBtn == 2 && !player->getPet(currentPetIndex)->getIsAlive())
            {
                revivePetButton.getShape().setFillColor(sf::Color(10, 20, 60, 255));
                revivePetButton.getText().setFillColor(sf::Color::Cyan);
            }
            else
            {
                revivePetButton.getText().setFillColor(sf::Color::White);
            }
            revivePetButton.setVisible(!player->getPet(currentPetIndex)->getIsAlive());

            accumulatedTime += dTime;
            if(accumulatedTime >= timePerFrame)
            {
                accumulatedTime = 0.f;
                currentFrame = (currentFrame + 1) % currentAnim->numFrames;

                currentSprite.setTextureRect(sf::IntRect(
                    currentFrame * currentAnim->frameWidth,
                    currentAnim->animationRow * currentAnim->frameHeight,
                    currentAnim->frameWidth,
                    currentAnim->frameHeight));
            }

            bg.update(dTime);
            window.clear();
            bg.draw(window);
            window.draw(brd);
            if(!isBuyingPet && !isNamingPet)
            {
                window.draw(currentSprite);
                window.draw(petCount);
                window.draw(petName);
                window.draw(petStats);
                window.draw(instructions);
            }
            window.draw(buyPetButton.getShape());
            window.draw(buyPetButton.getText());
            if(!player->getPet(currentPetIndex)->getIsAlive())
            {
                window.draw(revivePetButton.getShape());
                window.draw(revivePetButton.getText());
            }
            if(isBuyingPet || isNamingPet)
            {
                window.draw(buyPrompt);
                if(isNamingPet)
                    window.draw(newPetNameText);
            }
            window.display();
        }
        this->currentPetIndex = currentPetIndex;
        return currentPetIndex;
    }

    void updatePetSprite(sf::Sprite &sprite, PetAnimation &anim, Pet *pet,sf::Text &petNameText, sf::Text &petStatsText, sf::Font &font)
    {
        // updating the sprite animation properties
        sprite.setTexture(anim.texture);
        sprite.setTextureRect(sf::IntRect(0, anim.animationRow * anim.frameHeight,
                                          anim.frameWidth, anim.frameHeight));
        sprite.setScale(anim.scale, anim.scale);
        sprite.setPosition(anim.posX, anim.posY);


        petNameText.setString(pet->getName() + " (Lv." + to_string(pet->getLevel()) + " " + pet->getType() + ")");
        petNameText.setFont(font);
        petNameText.setCharacterSize(17);
        petNameText.setFillColor(sf::Color::White);
        petNameText.setPosition(50, 100);

        // stats
        string statsText;
        statsText += "HP: " + to_string(pet->getHealth()) + "/" + to_string(pet->getMaxHealth()) + "\n";
        statsText += "Attack: " + to_string(pet->getAttack()) + "\n";
        statsText += "Defense: " + to_string(pet->getDefense()) + "\n";
        statsText += "Speed: " + to_string(pet->getSpeed()) + "\n";
        statsText += "Status: " + string(pet->getIsAlive() ? "Active" : "Fainted");

        petStatsText.setString(statsText);
        petStatsText.setFont(font);
        petStatsText.setCharacterSize(16);
        petStatsText.setFillColor(pet->getIsAlive() ? sf::Color::White : sf::Color::Cyan);
        petStatsText.setPosition(50, 130);


        if(!pet->getIsAlive())
        {
            sprite.setColor(sf::Color(150, 150, 150, 200)); // gray if died
        }
        else if(pet->getHealth() < pet->getMaxHealth() / 4)
        {
            sprite.setColor(sf::Color(255, 100, 100, 255)); // red  if low health
        }
        else
        {
            sprite.setColor(sf::Color::White);
        }
    }
    //the first main menu screen 
    void start(sf::RenderWindow &window, sf::Font &font, sf::Sound &sound)
    {
        musicPlayer.play("music.ogg");
        ParallaxBackground bg(window.getSize().x, window.getSize().y);
        bg.addLayer("assets/BattleBG/1.png", 20.f);
        bg.addLayer("assets/BattleBG/2.png", 20.f);
        bg.addLayer("assets/BattleBG/3.png", 20.f);
        bg.addLayer("assets/BattleBG/4.png", 20.f);
        bg.addLayer("assets/BattleBG/5.png", 20.f);
        bg.addLayer("assets/BattleBG/6.png", 20.f);
        bg.addLayer("assets/BattleBG/7.png", 20.f);
        string options[] = {"New Game", "Load Game", "Leaderboard", "Exit"};
        const int numBtn = 4;

        Button buttons[numBtn] = {
            Button(220, 90, 150, 40, font, options[0]),
            Button(220, 140, 150, 40, font, options[1]),
            Button(220, 190, 150, 40, font, options[2]),
            Button(220, 240, 150, 40, font, options[3])};

        sf::Font title;
        title.loadFromFile("assets/tl.ttf");
        sf::Text gameTitle("Magical Pets Kingdom", title);
        gameTitle.setPosition(90, 20);
        gameTitle.setOutlineColor(sf::Color::White);
        gameTitle.setOutlineThickness(4);
        gameTitle.setCharacterSize(50);
        gameTitle.setFillColor(sf::Color(10, 20, 60, 255));

        int choice = 0;
        bool menuActive = true;
        sf::Clock clock;

        while (window.isOpen() && menuActive)
        {
            sf::Event event;
            float dTime = clock.restart().asSeconds();
        //hover effect
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            while (window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    window.close();


                if(event.type == sf::Event::KeyPressed)
                {

                    if(event.key.code == sf::Keyboard::Up)
                    {
                        sound.play();
                        choice = (choice - 1 + numBtn) % numBtn;
                    }
                    else if(event.key.code == sf::Keyboard::Down)
                    {
                        sound.play();
                        choice = (choice + 1) % numBtn;
                    }
                    else if(event.key.code == sf::Keyboard::Enter)
                    {
                        sound.play();
                        menuActive = false;
                        break;
                    }
                }


                if(event.type == sf::Event::MouseButtonPressed)
                {
                    if(event.mouseButton.button == sf::Mouse::Left)
                    {

                        for (int i = 0; i < numBtn; i++)
                        {
                            if(buttons[i].getShape().getGlobalBounds().contains(mousePos))
                            {
                                choice = i;
                                menuActive = false;
                                break;
                            }
                        }
                    }
                }
            }
            for (int i = 0; i < numBtn; i++)
            {
                if(i == choice)
                {

                    buttons[i].getShape().setFillColor(sf::Color(10, 20, 60, 255)); 
                    buttons[i].getText().setFillColor(sf::Color::Cyan);
                }
                else
                {
                    buttons[i].update(mousePos);
                    buttons[i].getText().setFillColor(sf::Color::White);
                }
            }

            // Draw everything
            bg.update(dTime);
            window.clear();
            bg.draw(window);

            window.draw(gameTitle);
            for (int i = 0; i < numBtn; i++)
            {
                buttons[i].render(window);
            }

            window.display();
        }

        // selected choice after menu loop exits
        switch (choice + 1)
        {
        case 1:
            createNewPlayer(window, font);
            mainGameLoop(window, font, sound);
            break;

        case 2:
            if(loadGame(window, font))
            {
                mainGameLoop(window, font, sound);
            }
            else
            {
                start(window, font, sound);
            }
            break;

        case 3:
            showLeaderboard(window, font);
            start(window, font, sound);
            break;
        case 4:
            cout<<"Thank you for playing Magical Pets Kingdom!"<<endl;
            window.close();
            return;

        default:
            cout<<"Invalid choice. Exiting game."<<endl;
            window.close();
            return;
        }
    }
    void mainGameLoop(sf::RenderWindow &window, sf::Font &font, sf::Sound &sound)
    {
        while (gameRunning)
        {
            //all the buttons for the menu 
            Button buttons[8] = {Button(80, 50, 150, 50, font, "View Pets"),
                                 Button(80, 120, 150, 50, font, "Battle Arena"),
                                 Button(80, 190, 150, 50, font, "Training Camp"),
                                 Button(80, 260, 150, 50, font, "Guild Management"),
                                 Button(280, 50, 150, 50, font, "Shop"),
                                 Button(280, 120, 150, 50, font, "Inventory"),
                                 Button(280, 190, 150, 50, font, "Save Game"),
                                 Button(280, 260, 150, 50, font, "Exit")};


    //loaded background layers
            ParallaxBackground bg(window.getSize().x, window.getSize().y);
            bg.addLayer("assets/BattleBG/1.png", 20.f);
            bg.addLayer("assets/BattleBG/2.png", 20.f);
            bg.addLayer("assets/BattleBG/3.png", 20.f);
            bg.addLayer("assets/BattleBG/4.png", 20.f);
            bg.addLayer("assets/BattleBG/5.png", 20.f);
            bg.addLayer("assets/BattleBG/6.png", 20.f);
            bg.addLayer("assets/BattleBG/7.png", 20.f);

            bool menuActive = true;
            int choice = 0;
            sf::Clock clock;
            const int numMenu = 8;

            while (window.isOpen() && menuActive)
            {
                sf::Event event;
                float dTime = clock.restart().asSeconds();
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                while (window.pollEvent(event))
                {
                    if(event.type == sf::Event::Closed)
                        window.close();
//keyboard input 
                    if(event.type == sf::Event::KeyPressed)
                    {
                        if(event.key.code == sf::Keyboard::Up)
                        {
                            sound.play();
                            choice = (choice - 1 + numMenu) % numMenu;
                        }
                        else if(event.key.code == sf::Keyboard::Down)
                        {
                            sound.play();
                            choice = (choice + 1) % numMenu;
                        }
                        else if(event.key.code == sf::Keyboard::Enter)
                        {
                            sound.play();
                            menuActive = false;
                            break;
                        }
                    }
                    if(event.type == sf::Event::MouseButtonPressed)
                    {
                        if(event.mouseButton.button == sf::Mouse::Left)
                        {

                            for (int i = 0; i < numMenu; i++)
                            {
                                if(buttons[i].getShape().getGlobalBounds().contains(mousePos))
                                {
                                    choice = i;
                                    menuActive = false;
                                    break;
                                }
                            }
                        }
                    }
                }
                for (int i = 0; i < numMenu; i++)
                {
                    if(i == choice)
                    {

                        buttons[i].getShape().setFillColor(sf::Color(10, 20, 60, 255)); 
                        buttons[i].getText().setFillColor(sf::Color::Cyan);
                    }
                    else
                    {
                        buttons[i].update(mousePos);
                        buttons[i].getText().setFillColor(sf::Color::White);
                    }
                }
                // Draw everything
                bg.update(dTime);
                window.clear();
                bg.draw(window);
                for (int i = 0; i < numMenu; i++)
                {
                    buttons[i].render(window);
                }

                window.display();
            }
            switch (choice + 1)
            {
            case 1:
                petMenu(window, font);

                break;

            case 2:
                battleMenu(window, font, sound);
                break;

            case 3:
                trainingMenu(window, font, sound);
                break;

            case 4:
                guildMenu(window, font, sound);
                break;

            case 5:
                shopMenu(window, font, sound);
                break;

            case 6:
                inventoryMenu(window, font);
                break;

            case 7:
                savePlayers();
            break;

            case 8:
                cout<<"Thank you for playing Magical Pets Kingdom!"<<endl;
                gameRunning = false;
                break;

            default:
                cout<<"Invalid choice!"<<endl;
                break;
            }
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(576, 324), "Magical Pet Kingdom");
    sf::ContextSettings setting;
    setting.antialiasingLevel = 8;

    window.setFramerateLimit(60);
//font and sound loading
    sf::Font font;
    sf::SoundBuffer buffer;

    if(!buffer.loadFromFile("assets/sound/1.wav"))
        return -1;
    sf::Sound sound(buffer);
    sound.play();

    if(!font.loadFromFile("assets/title.ttf"))
    {
        cerr << "Failed to load font\n";
        return -1;
    }
    font.setSmooth(true);

    gameController game;
    game.start(window, font, sound);


    return 0;
}
