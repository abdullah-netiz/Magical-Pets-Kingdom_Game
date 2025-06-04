![Game](https://github.com/user-attachments/assets/5a2609f4-899b-4b9d-ad05-39b97ea92076)# 🐉 Magical Pets Kingdom

Welcome to **Magical Pets Kingdom**, a second-semester OOP game project built with **C++** and **SFML**! In this fantasy game, players raise mythical pets, train them, and engage in epic battles. This project highlights OOP principles and SFML graphics magic. Let's dive in!

## 🎮 Gameplay Overview

![Game](https://github.com/user-attachments/assets/b4a0361c-a9d6-4137-8795-fb70d61e0606)
<br>
![Menu](https://github.com/user-attachments/assets/2cf4172d-00e2-46c3-a787-e782dc5d3cc7)
<br>
![gm](https://github.com/user-attachments/assets/3ba7f33d-a2e2-4342-bded-42c2223bef47)
<br>
![ss](https://github.com/user-attachments/assets/36858e04-5f07-4535-b9e0-4fe39f15dad3)
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>
<br>











































* **Choose Your Pet**: Dragon, Phoenix, Unicorn, or Griffin – each with unique stats and animations.
* **Train**: Level up your pet in the Training Camp to boost their stats.
* **Shop**: Purchase items like Healing Potions, Mana Potions, and Shields to aid your journey.
* **Battle**: Challenge enemy pets in battles and prove your kingdom's might.
* **Guild System**: Join a guild to unlock new missions and collective power.
* **Animated Graphics & Background Music**: Smooth animations and immersive music using SFML.

## 🏗️ Project Structure

The game's code is neatly split into these **OOP-based classes**:

* **Pet**: Base class for pets (Dragon, Phoenix, Unicorn, Griffin) with stats and attack/defense logic.
* **Player**: Manages the player's name, chosen pet, and inventory.
* **Battle**: Handles combat mechanics and outcomes.
* **Guild**: Guild joining logic and related bonuses.
* **TrainingCamp**: Stat-boosting training sessions.
* **Shop**: Buy items to boost your pet's performance.
* **Item**: Base class for items (HealingPotion, ManaPotion, Shield).
* **PetAnimation**: Loads and animates pet spritesheets.
* **MusicPlayer**: Plays background music tracks and manages volume.
* **ParallaxBackground**: Dynamic background visuals for depth.

The game loop and main menu system are orchestrated in **main.cpp** with real-time rendering and user input handling.

## 🔧 Installation & Building

Here's how to get the game running:

### 1️⃣ **Clone the repository**:
```bash
git clone https://github.com/abdullah-netiz/Magical-Pets-Kingdom_Game.git
cd Magical-Pets-Kingdom_Game
```

### 2️⃣ **Install SFML**:
Follow the SFML download and setup guide for your platform.

### 3️⃣ **Build the game** (example for g++ on Linux/macOS):
```bash
g++ main.cpp -o MagicalPetsKingdom -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

For **Windows** with Visual Studio, set up the SFML include and lib paths in project settings.

### 4️⃣ **Run the game**:
```bash
./MagicalPetsKingdom
```

## ⚡ How to Play

* Launch the game and **choose your pet**.
* **Navigate** using keyboard inputs (like arrow keys or number keys depending on your setup).
* **Train** in the Training Camp to level up your pet's attack and defense.
* **Shop** for items that boost your performance.
* **Battle** enemies to earn rewards and prove your worth.
* Enjoy the **animated graphics** and dynamic **music**!

## 🌟 Key Features

✅ **Fully OOP**: Encapsulation, inheritance, polymorphism in action.  
✅ **Interactive Menus**: Guilds, training, shop, and battle scenes.  
✅ **Pet Animations**: Each pet has unique animation frames loaded from spritesheets.  
✅ **Audio Integration**: Background music and sound effects powered by SFML.  
✅ **Dynamic Background**: Parallax scrolling for an immersive feel.  
✅ **File Handling**: Save and load pet stats and game progress (via files).

## 📁 Assets & Resources

* **Assets Folder**: Contains pet spritesheets and music tracks.
* **Spritesheets**: Preloaded animations for each pet (`assets/Dragon/sheet.png`, etc.).
* **Music**: In-game music to boost the fantasy vibe.

## 👨‍💻 Project Highlights & Learning Points

### ✨ **Solid OOP Practices**:
* Inheritance for pets and items
* Composition for player and pet relationships
* Clear class responsibilities

### ✨ **SFML Integration**:
* Graphics rendering: Sprites, textures, scaling
* Audio playback with volume control
* Event handling for real-time user input

### ✨ **Gameplay Mechanics**:
* Stat-based combat
* Shop economy and item effects
* Training system for progression

## 🗂️ Project File Breakdown

* **main.cpp**: Core game loop and all class implementations in one file (can be split for bigger projects).
* **assets/**: All images and sounds needed for the game.
* **README.md**: (You're reading it!).

## 🚀 Future Improvements

💡 Modularize the code (split classes into `.cpp` and `.h` files).  
💡 Add more pets, items, and battle scenarios.  
💡 Enhance AI for enemy pets.  
💡 Implement save/load feature with JSON or XML.  
💡 Add visual feedback for battles (damage numbers, health bars).

## 🤝 Contributing

This project is a **learning-based solo endeavor**. Feel free to fork it and experiment! If you want to tweak gameplay, add new features, or clean up the code structure, go for it! Contributions are welcome.

## 📜 License

This project is released for **educational and non-commercial use**. All assets are either placeholders or self-created. If you reuse the code, please credit the original creator (Abdullah).

## 🙌 Final Words

Thanks for checking out **Magical Pets Kingdom**! It's been a fun and challenging project, combining **OOP principles** with **real-time graphics** and **gameplay mechanics**. Feel free to fork, play around, or simply enjoy battling mythical creatures!

🔥 **Happy coding!**
