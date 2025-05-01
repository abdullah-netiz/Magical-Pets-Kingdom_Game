//Abdullah
//24I-3164
//SE-C
#ifndef BATTLE_H
#define BATTLE_H

#include <string>
#include "Pet.h"
#include "Player.h"

// Constants
const int MAX_BATTLE_PETS = 4; // For 2v2 battles
const int MAX_BATTLE_TURNS = 50; // Limit to prevent infinite battles

const int BATTLE_TYPE_ONE_VS_ONE = 0;
const int BATTLE_TYPE_TWO_VS_TWO = 1;
const int BATTLE_TYPE_GUILD_WAR = 2;

const int BATTLE_RESULT_PLAYER_WIN = 0;
const int BATTLE_RESULT_OPPONENT_WIN = 1;
const int BATTLE_RESULT_DRAW = 2;
const int BATTLE_RESULT_ONGOING = 3;

const int MAX_ARENAS = 5;
const int MAX_OBSTACLES = 10;
const int MAX_POWER_UPS = 5;

const int ARENA_TYPE_BEGINNER = 0;
const int ARENA_TYPE_INTERMEDIATE = 1;
const int ARENA_TYPE_ADVANCED = 2;
const int ARENA_TYPE_EXPERT = 3;
const int ARENA_TYPE_GUILD = 4;

const int OBSTACLE_TYPE_WALL = 0;
const int OBSTACLE_TYPE_TRAP = 1;
const int OBSTACLE_TYPE_WATER = 2;
const int OBSTACLE_TYPE_FIRE = 3;
const int OBSTACLE_TYPE_NONE = 4;


const int POWERUP_TYPE_HEALTH_BOOST = 0;
const int POWERUP_TYPE_ATTACK_BOOST = 1;
const int POWERUP_TYPE_DEFENSE_BOOST = 2;
const int POWERUP_TYPE_SPEED_BOOST = 3;
const int POWERUP_TYPE_MANA_RESTORE = 4;
const int POWERUP_TYPE_NONE = 5;

const int PET_ROLE_TANK = 0;
const int PET_ROLE_HEALER = 1;
const int PET_ROLE_DAMAGE_DEALER = 2;
const int PET_ROLE_SUPPORT = 3;
const int PET_ROLE_NONE = 4;


class Battle {
private:
      int type; // Using battle type constants
    Player* player;
    Pet* playerPets[MAX_BATTLE_PETS];
    int playerPetCount;
    
    // For AI opponent
      string opponentName;
    Pet* opponentPets[MAX_BATTLE_PETS];
    int opponentPetCount;
    
    // Battle state
    int currentTurn;
    int result; // Using battle result constants
    bool isPlayerTurn;
    
    // Battle log
    static const int MAX_LOG_ENTRIES = 20;
      string battleLog[MAX_LOG_ENTRIES];
    int logCount;
    
    // Private methods
    void initializeBattle();
    void determineTurnOrder();
    void aiTurn();
    void checkBattleStatus();
    Pet* selectTargetForAI(Pet* attacker);
    
public:
    // Constructor and Destructor
    Battle(int battleType, Player* player);
    ~Battle();
    
    // Battle setup
    bool addPlayerPet(Pet* pet);
    bool addOpponentPet(Pet* pet);
    void setOpponentName(const   string& name);
    
    // Battle flow
    void startBattle();
    int executeTurn(int playerAction, int targetIndex, int abilityIndex = -1);
    void endBattle();
    
    // Getters
    int getType() const;
    int getCurrentTurn() const;
    int getResult() const;
    bool getIsPlayerTurn() const;
    Pet* getPlayerPet(int index) const;
    Pet* getOpponentPet(int index) const;
    int getPlayerPetCount() const;
    int getOpponentPetCount() const;
    
    // Battle log
    void addToLog(const   string& entry);
    void displayBattleLog() const;
    void clearLog();
    
    // Display functions
    void displayBattleStatus() const;
    void displayBattleResult() const;
    void displayAvailableActions() const;
    
    // Battle actions
    bool executeAttack(Pet* attacker, Pet* target);
    bool executeAbility(Pet* attacker, Pet* target, int abilityIndex);
    bool useItem(const   string& itemName, Pet* target);
    
    // Utility functions
    static int calculateDamage(Pet* attacker, Pet* defender);
    static bool isKnockedOut(Pet* pet);
    
    // Arena obstacles and power-ups (to be expanded in Arena class)
    void spawnPowerUp();
    void activatePowerUp(Pet* pet);
};


struct Obstacle {
    int type; // Using obstacle type constants
      string name;
      string description;
    int effect; // For example, damage for traps
    
    Obstacle() : type(OBSTACLE_TYPE_NONE), name(""), description(""), effect(0) {}
    Obstacle(int t, const   string& n, const   string& d, int e) 
        : type(t), name(n), description(d), effect(e) {}
};

// Power-up structure
struct PowerUp {
    int type; // Using power-up type constants
      string name;
      string description;
    int effect; // Amount of boost
    int duration; // Number of turns
    
    PowerUp() : type(POWERUP_TYPE_NONE), name(""), description(""), effect(0), duration(0) {}
    PowerUp(int t, const   string& n, const   string& d, int e, int dur) 
        : type(t), name(n), description(d), effect(e), duration(dur) {}
};

class Arena {
private:
      string name;
    int type; // Using arena type constants
    int minLevel; // Minimum pet level required
    int maxLevel; // Maximum pet level allowed (for balanced play)
    
    // Obstacles in this arena
    Obstacle obstacles[MAX_OBSTACLES];
    int obstacleCount;
    
    // Available power-ups
    PowerUp availablePowerUps[MAX_POWER_UPS];
    int powerUpCount;
    
    // Arena stats
    int totalBattles;
    int totalPoints;
    Player* topPlayer;
    Guild* topGuild;
    
public:
    // Constructor and Destructor
    Arena(const string& arenaName, int arenaType, int minLvl, int maxLvl);
    ~Arena();
    
    // Getters
    string getName() const;
    int getType() const;
    int getMinLevel() const;
    int getMaxLevel() const;
    int getTotalBattles() const;
    int getTotalPoints() const;
    Player* getTopPlayer() const;
    Guild* getTopGuild() const;
    
    // Setters
    void setName(const   string& newName);
    void setType(int newType);
    void setLevelRange(int minLvl, int maxLvl);
    void setTopPlayer(Player* player);
    void setTopGuild(Guild* guild);
    
    // Obstacle management
    bool addObstacle(int type, const   string& name, const   string& description, int effect);
    void removeObstacle(int index);
    Obstacle* getObstacle(int index);
    void displayObstacles() const;
    
    // Power-up management
    bool addPowerUp(int type, const   string& name, const   string& description, int effect, int duration);
    void removePowerUp(int index);
    PowerUp* getPowerUp(int index);
    PowerUp* getRandomPowerUp();
    void displayPowerUps() const;
    
    // Battle mechanics
    Battle* createBattle(int battleType, Player* player);
    void recordBattleResult(Battle* battle);
    void updateArenaStats(int result, Player* player, Guild* guild);
    
    // Arena validation
    bool canPetParticipate(Pet* pet) const;
    bool isValidForBattle(Player* player, int battleType) const;
    
    // Display functions
    void displayArenaInfo() const;
    void displayArenaStats() const;
    void displayArenaRules() const;
    
    // Static arena management
    static Arena* arenas[MAX_ARENAS];
    static int arenaCount;
    static bool initializeDefaultArenas();
    static Arena* getArenaByName(const   string& name);
    static Arena* getArenaByType(int type);
    static void displayAllArenas();
};

#endif 
