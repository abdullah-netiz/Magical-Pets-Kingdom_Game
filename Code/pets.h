//Abdullah
//24I-3164
//SE-C
#ifndef PETS_H
#define PETS_H
#include<iostream>
using namespace std;
#include<string>
class Ability{
    string name;
    string description;
    int manaCost;
    int coolDownTimer;
    int currentTimer;
    public:
    Ability(string n, string desc, int cost, int cd) 
            : name(n), description(desc), manaCost(cost), cooldownTimer(cd), currentTimer(0) {}
    

};
class Pet{
    protected:
    string name;
    string type;
    string role;
    Ability *abilities; 
    int level;
    int xp;
    int health;
    int maxhealth;
    int attackPower;
    int defense;
    int speed;
    int abilityPoints;
    int coolDownCounter;
    bool isAlive;
    bool specialAbility;
    
    int maxMana;
    int currentMana;
    int trainingPoints;
    public:
    Pet(string name, string type): this->name(name), this->type(type){}
    
//getter setter
    void setName(const string& newName);
    void setType(const string& newType);
    void setLevel(int lvl);
    void setExperience(int xp);
    void setHealth(int hp);
    void setMaxHealth(int maxHp);
    void setAttackPower(int ap);
    void setDefense(int def);
    void setSpeed(int spd);
    void setAbilityPoints(int ap);
    void setIsAlive(bool status);
    void setSpecialUsed(bool used);
    
    string getName() const;
    string getType() const;
    int getLevel() const;
    int getExperience() const;
    int getHealth() const;
    int getMaxHealth() const;
    int getAttackPower() const;
    int getDefense() const;
    int getSpeed() const;
    int getAbilityPoints() const;
    bool getIsAlive() const;
    bool getSpecialUsed() const;
    int getMaxMana() const;
    int getCurrentMana() const;
    int getAvailableTrainingPoints() const;
    
    
    void levelUp();
    void heal(int amount);
    void takeDamage(int amount);
    void restoreMana(int amount);
    void useMana(int amount);
    void gainExperience(int amount);
    void addTrainingPoints(int points);
    bool useTrainingPoint(const string& attribute);
    int calculateDamage() const;
    bool useAbility(int abilityIndex, Pet& target);
    void updateCoolDownTimer();
    void displayStats() const;
    
    bool operator==(const Pet& other) const;
    bool operator!=(const Pet& other) const;
    Pet& operator+=(int expPoints);
    virtual void specialAbility() = 0;
    
};
// Dragon.h
// Name: [Your Name]
// Student ID: [Your ID]
// Assignment: OOP Project


class Dragon : public Pet {
private:
    int fireBreathPower;
    bool roarAvailable;
    
public:
    // Constructor
    Dragon(const string& name);
    
    
    // Override virtual functions
    virtual void levelUp() override;
    virtual int calculateDamage() const override;
    virtual void displayStats() const override;
    
    // Special abilities
    virtual void specialAbility() override;
    void fireballAttack(Pet& target);
    void dragonsRoar(vector<Pet*>& enemies);
    
    // Getters and setters specific to Dragon
    int getFireBreathPower() const;
    void setFireBreathPower(int power);
    bool isRoarAvailable() const;
    void setRoarAvailable(bool available);
};
class Phoenix : public Pet {
private:
    int healingPower;
    bool rebirthAvailable;
    
public:
    // Constructor
    Phoenix(const std::string& name);
    
    // Destructor
    virtual ~Phoenix();
    
    // Override virtual functions
    virtual void levelUp() override;
    virtual int calculateDamage() const override;
    virtual void displayStats() const override;
    
    // Special abilities
    virtual void specialAbility() override;
    void flameHealing(Pet& target);
    bool rebirth();
    
    // Getters and setters specific to Phoenix
    int getHealingPower() const;
    void setHealingPower(int power);
    bool isRebirthAvailable() const;
    void setRebirthAvailable(bool available);
};

class Unicorn : public Pet {
private:
    int magicPower;
    int shieldStrength;
    bool speedBoostActive;
    int speedBoostDuration;
    
public:
    // Constructor
    Unicorn(const std::string& name);
    
    // Destructor
    virtual ~Unicorn();
    
    // Override virtual functions
    virtual void levelUp() override;
    virtual int calculateDamage() const override;
    virtual void displayStats() const override;
    virtual void updateCooldowns() override;
    
    // Special abilities
    virtual void specialAbility() override;
    void speedBurst();
    void magicShield();
    
    // Getters and setters specific to Unicorn
    int getMagicPower() const;
    void setMagicPower(int power);
    int getShieldStrength() const;
    void setShieldStrength(int strength);
    bool isSpeedBoostActive() const;
    void setSpeedBoostActive(bool active);
    int getSpeedBoostDuration() const;
    void setSpeedBoostDuration(int duration);
};

class Griffin : public Pet {
private:
    int clawStrength;
    int aerialPower;
    bool diveBombReady;
    
public:
    // Constructor
    Griffin(const std::string& name);
    
    // Destructor
    virtual ~Griffin();
    
    // Override virtual functions
    virtual void levelUp() override;
    virtual int calculateDamage() const override;
    virtual void displayStats() const override;
    
    // Special abilities
    virtual void specialAbility() override;
    void clawStrike(Pet& target);
    void diveBomb(Pet& target);
    
    // Getters and setters specific to Griffin
    int getClawStrength() const;
    void setClawStrength(int strength);
    int getAerialPower() const;
    void setAerialPower(int power);
    bool isDiveBombReady() const;
    void setDiveBombReady(bool ready);
};

#endif
