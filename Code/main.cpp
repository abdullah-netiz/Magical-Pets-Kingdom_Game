// Abdullah
// 24I-3164
// SE-C
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <ctime>
#include "player.h"
#include "shop.h"
#include "training.h"
#include "battle.h"

using namespace std;

const int NUM_ITEMS = 4;
void Player::setName(const string &newName)
{
  name = newName;
}
string Player::getName() const{
  return name;
}

Button::Button(float x, float y, float width, float height,sf::Font& font, std::string buttonText, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor) : idleColor(idleColor), hoverColor(hoverColor), activeColor(activeColor), isHovered(false)
{
  shape.setPosition(x, y);
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(idleColor);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);


   text.setFont(font);
    text.setString(buttonText);
    text.setCharacterSize(18);
    text.setFillColor(sf::Color::White);


    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, 
                  textRect.top + textRect.height/2.0f);
    text.setPosition(x + width/2.0f, y + height/2.0f); 
}

void Button::update(const sf::Vector2f& mousePos) 
{
    isHovered = shape.getGlobalBounds().contains(mousePos);
    
    if (isHovered) 
    {
        shape.setFillColor(hoverColor);
        shape.setOutlineColor(sf::Color(255, 255, 255, 255));
        text.setScale(0.8f, 0.8f); 
    } 
    else 
    {
        shape.setFillColor(idleColor);
        shape.setOutlineColor(sf::Color(255, 255, 255, 200));
        text.setScale(1.f, 1.f);
    }
}

void Button::render(sf::RenderTarget& target) 
{
    target.draw(shape);
    target.draw(text);
}
bool Button::isPressed() const 
{
    return isHovered && sf::Mouse::isButtonPressed(sf::Mouse::Left);
}


string getPlayerName(sf::RenderWindow &window, sf::Font &font)
{
  string playerName;
  bool showPlaceholder = true;

  sf::Text promptText("Enter your name: ", font, 18);
  promptText.setPosition(window.getSize().x/5.f, 50 + 70);
  promptText.setFillColor(sf::Color::White);

  sf::Text nameText("", font, 18);
  sf::Text placeholderText("Your name...", font, 18);
  nameText.setPosition(window.getSize().x/2.f, 50 + 70);
  nameText.setFillColor(sf::Color::Yellow);
  placeholderText.setPosition(window.getSize().x/2.f, 50 + 70);
  placeholderText.setFillColor(sf::Color(150, 150, 150));
  sf::Texture bgtext;
  if(!bgtext.loadFromFile("../Assets/bg.png"))
  {
    cout<<"Could not load image!"<<endl;
  }
  sf::Sprite bg(bgtext);
  sf::Text Level("Level: ", font,18);
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
        return "";
      }

      if (event.type == sf::Event::TextEntered)
      {
        if (event.text.unicode == '\b')
        {
          if (!playerName.empty())
          {
            playerName.pop_back();

            showPlaceholder = playerName.empty();
          }
        }
        else if (event.text.unicode == '\r' || event.text.unicode == '\n')
        { // Enter key
          if (!playerName.empty())
          {
            return playerName;
          }
        }
        else if (event.text.unicode < 128 && event.text.unicode != '\b')
        {

          if (showPlaceholder)
          {
            showPlaceholder = false;
            playerName = "";
          }
          playerName += static_cast<char>(event.text.unicode);
        }
      }
    }

    nameText.setString(playerName);

    window.clear(sf::Color::Black);
    window.draw(bg);
    window.draw(promptText);

    if (showPlaceholder && playerName.empty())
    {
      window.draw(placeholderText);
    }
    else
    {
      window.draw(nameText);
    }

    window.display();
  }

  return playerName;
}


void playGame(sf::RenderWindow &window, sf::Font &font, Player &players)
{

  sf::Texture bgtext;
  sf::Text petSelect("Select Any Pet From below", font,18);
  
  
  if (!bgtext.loadFromFile("../Assets/bg.png"))
  {
    cout << "Could not load Image!" << endl;
  }
  sf::Sprite bgSprite(bgtext);

  // Taking Player Name
  players.setName(getPlayerName(window, font));
  cout<<"Player Name is : "<<players.getName()<<endl;
  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        return; // Go back to menu
    }

    window.clear(sf::Color::Black);
    window.draw(bgSprite);
    window.draw(petSelect);
    window.display();
  }
}
void petMenu(sf::RenderWindow &window, sf::Font &font, Player &players)
{
  
  ParallaxBackground bg(window.getSize().x, window.getSize().y);
  bg.addLayer("../Assets/bgSprite/1.png", 20.f);
  bg.addLayer("../Assets/bgSprite/2.png", 0.f);
  bg.addLayer("../Assets/bgSprite/3.png", 0.f);
  bg.addLayer("../Assets/bgSprite/4.png",0.f);
  

  sf::Clock clock;
   sf::Clock animationClock; 
  sf::Texture DragonTxt;
  sf::Sprite DragonSprite;
  int currentFrame = 0;
    const int frameWidth = 191;
    const int frameHeight = 161;
    const int animationRow = 2; // Y index (row 2)
    const int numFrames = 3;    // Frames (0,2) to (2,2)
    const float animationSpeed = 0.3f; // Time between frames in seconds
    
    if(DragonTxt.loadFromFile("../Assets/Dragon/sheet.png")) {
        cout << "Dragon Loaded" << endl;
        DragonSprite.setTexture(DragonTxt);
        DragonSprite.setTextureRect(sf::IntRect(0, animationRow * frameHeight, frameWidth, frameHeight));
        DragonSprite.setScale(1.f, 1.f);
        DragonSprite.setPosition(window.getSize().x/4.f, window.getSize().y/2.f - frameHeight/2);
    }
  
    while (window.isOpen())
    {
      float dTime = clock.restart().asSeconds();
      if (animationClock.getElapsedTime().asSeconds() > animationSpeed) 
      {
      currentFrame = (currentFrame + 1) % numFrames;
      DragonSprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, animationRow * frameHeight, frameWidth, frameHeight));
      animationClock.restart();
      }
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
      if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        return; 
      if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
      return;
    }
    bg.update(dTime);

    window.clear(sf::Color::Black);
    bg.draw(window);
    window.draw(DragonSprite);
    window.display();
  }

}
void runMenu(sf::RenderWindow &window, sf::Font &font)
{
  Player players;
  sf::Text title;
  Button buttons[NUM_BUTTONS] = {
    Button(210, 50, 150, 50, font, "Play Game"),
    Button(210, 120, 150, 50, font, "View Pets"),
    Button(210, 190, 150, 50, font, "View Guilds"),
    Button(210, 270, 150, 50, font, "Exit")
};
  int selectedIndex = 0;
  ParallaxBackground bg(window.getSize().x, window.getSize().y);
  bg.addLayer("../Assets/bgMain/1.png", 0.f);
  bg.addLayer("../Assets/bgMain/2.png", 20.f);
  bg.addLayer("../Assets/bgMain/3.png", 0.f);
  bg.addLayer("../Assets/bgMain/4.png",0.f);
  

  

  
  sf::Clock clock;
 while (window.isOpen()) {
  float dTime = clock.restart().asSeconds();
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
            
        // Handle button clicks
        if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                for (int i = 0; i < NUM_BUTTONS; i++) {
                    if (buttons[i].getShape().getGlobalBounds().contains(
                        static_cast<float>(event.mouseButton.x), 
                        static_cast<float>(event.mouseButton.y))) {
                        
                        // Handle button actions
                        if (i == 0) { // Play Game
                            playGame(window, font, players);
                        }
                        else if (i == 1) { // View Pets
                            petMenu(window, font, players);
                        }
                        else if (i == 2) { // View Guilds
                            // guildMenu(window, font, *players);
                        }
                        else if (i == 3) { // Exit
                            window.close();
                        }
                    }
                }
            }
        }
    }
    
    // Update buttons with mouse position
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].update(mousePos);
    }
    
    // Drawing
    window.clear();
    bg.update(dTime);
    // Draw background
    bg.draw(window);
    
    // Draw buttons
    for (int i = 0; i < NUM_BUTTONS; i++) {
        buttons[i].render(window);
    }
    
    window.display();
}
  
}

int main()
{
  sf::ContextSettings settings;
  settings.antialiasingLevel =8;
  sf::RenderWindow window(sf::VideoMode(576, 324), "Magical Pets Kingdom", sf::Style::Default, settings);
  window.setFramerateLimit(60);
  sf::Font font;
  if (!font.loadFromFile("arial.ttf"))
  {
    return -1;
  }

  runMenu(window, font);

  return 0;
}
