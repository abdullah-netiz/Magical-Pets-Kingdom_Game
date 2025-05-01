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





void mainMenu();
int main()
{
  sf::ContextSettings settings;
  settings.antialiasingLevel= 8;
  sf::RenderWindow window(sf::VideoMode(800,600), "Magical Pets Kingdom", sf::Style::Default,  settings);
  window.setFramerateLimit(30);
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
  ball.move(velocity);
  sf::Vector2f pos = ball.getPosition();
  if(pos.x <=0 || pos.x + ball.getRadius()*2 >= 800)
  velocity.x *= -1;
  if(pos.y <=0 || pos.y + ball.getRadius()*2 >=600)
  velocity.y *=-1;
  
  window.draw(ball);
  window.display();
  }
  
  
  return 0;
}







void mainMenu()
{
  int choice;
  cout<<"Welcome to the Magical Pets Kingdom Game\n";
  cout<<"==========================================\n";
  cout<<"1.Play Game\n";
  cout<<"2.Save Game\n";
  cout<<"3.Load Game\n";
  cout<<"4.Exit game\n";
  cout<<"===========================================\n";
  do{
  cin >> choice;
  if(choice <1 || choice >4)
  cout<<"Enter again Invalid Choice\n";
  }while(choice <1 || choice >4);
}
