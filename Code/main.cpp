//Abdullah
//24I-3164
//SE-C
#include <SFML/Graphics.hpp>
#include<iostream>
#include<string>
#include<cstdlib>
#include<fstream>
#include<ctime>
#include "player.h"
#include "shop.h"
#include"training.h"
#include "battle.h"

using namespace std;




const int NUM_ITEMS = 4;
<<<<<<< HEAD
=======
void playGame(sf::RenderWindow &window, sf::Font &font) {
    sf::Text text;
    text.setFont(font);
    text.setString("Game Started!");
    text.setCharacterSize(40);
    text.setFillColor(sf::Color::Green);
    text.setPosition(100, 100);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                return;  // Go back to menu
        }

        window.clear(sf::Color::Black);
        window.draw(text);
        window.display();
    }
}


>>>>>>> main

void runMenu(sf::RenderWindow &window, sf::Font &font) 
{
  sf::Text buttons[NUM_ITEMS];
  string labels[NUM_ITEMS] = { "Play Game", "View Pets", "View Guilds" ,"Exit"};
  int selectedIndex = 0;
  sf::Text title;
  title.setFont(font);
  title.setString("Magical Pets Kingdom");
  title.setCharacterSize(35);
  title.setFillColor(sf::Color::Cyan);
  title.setPosition(150,20);
  sf::FloatRect titleBounds = title.getLocalBounds();
  title.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
  title.setPosition(window.getSize().x / 2.0f, 60); 

<<<<<<< HEAD
=======
    sf::Texture bgTexture;
    if(!bgTexture.loadFromFile("../Assets/menu.jpg)"))
    {
      cout << "Failed to load background!" << endl;
    }
    sf::Sprite background(bgTexture);
>>>>>>> main
    
    for(int i = 0; i < NUM_ITEMS; ++i) 
    {
        buttons[i].setFont(font);
        buttons[i].setString(labels[i]);
        buttons[i].setCharacterSize(30);
        buttons[i].setPosition(200, 100 + i * 70);
        sf::FloatRect bounds = buttons[i].getLocalBounds();
        buttons[i].setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
        buttons[i].setPosition(window.getSize().x / 2.0f, 150 + i * 70);
        buttons[i].setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
    }

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedIndex = (selectedIndex - 1 + NUM_ITEMS) % NUM_ITEMS;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedIndex = (selectedIndex + 1) % NUM_ITEMS;
                }
<<<<<<< HEAD
                else if (event.key.code == sf::Keyboard::Enter) {
                    if (selectedIndex == 0) {
                        
=======
                else if (event.key.code == sf::Keyboard::Enter) 
                {
                    if (selectedIndex == 0) 
                    {
                      playGame(window, font);    
>>>>>>> main
                    } else if (selectedIndex == 1) {
                        
                    } else if (selectedIndex == 2) {
                        window.close(); // Exit
                    }
                    else if (selectedIndex ==3)
                    {
                      window.close();
                    }
                }

              
                for (int i = 0; i < NUM_ITEMS; ++i) 
                {
                buttons[i].setFillColor(i == selectedIndex ? sf::Color::Yellow : sf::Color::White);
                }
            }
        }

        window.clear();
<<<<<<< HEAD
=======
        window.draw(background);
>>>>>>> main
        window.draw(title);
        for (int i = 0; i < NUM_ITEMS; ++i) 
        {

            window.draw(buttons[i]);
        }
        window.display();
    }
}




<<<<<<< HEAD

int main()
{
  sf::ContextSettings settings;
  settings.antialiasingLevel= 8;
=======
int main()
{
  sf::ContextSettings settings;
  
>>>>>>> main
  sf::RenderWindow window(sf::VideoMode(800,600), "Magical Pets Kingdom", sf::Style::Default,  settings);
  window.setFramerateLimit(30);
   sf::Font font;

<<<<<<< HEAD
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }
  
  sf::CircleShape ball(50.f);
  ball.setFillColor(sf::Color::Yellow);
  ball.setPosition(0.f, 0.f);

  sf::Vector2f velocity(5.f, 5.f);
   


  while(window.isOpen())
  {
    sf::Event event;
    while(window.pollEvent(event))
    {
    if(event.type == sf::Event::Closed)
    window.close();
    }

  window.clear(sf::Color::Black);
  
  runMenu(window, font);
  
  window.display();
  }
  
  
=======
    if(!font.loadFromFile("arial.ttf")) 
    {
      return -1;
    }
   
  runMenu(window, font);
  
>>>>>>> main
  return 0;
}





