#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Joystick.hpp>

using namespace std;
using namespace sf;

bool holding = false;

enum joyButton
{
    Triangle = 0, Circle, Cross, Square,
    Up, Right, Down, Left,
    Select, Start,
    R1, R2,
    L1, L2,
    AnaLeft, AnaRight
};

void update();
void input(char cmd[], char R1[], char R2[], char L1[], char L2[], unsigned short int joystickId = 1);

void update()
{
    Joystick::update();
}

void input(char cmd[], char R1[], char R2[], char L1[], char L2[], unsigned short int joystickId)
{
    if(Joystick::isButtonPressed(joystickId, joyButton::R1))
    {
        if(!holding)
        {
            system(R1);
            holding = true;
        }
        else
        {
            system("xdotool mouseup 1 mouseup 2 mouseup 3");
            holding = false;
        }
    }
    //else if(Joystick::isButtonPressed(joystickId, joyButton::R2));
        //Do something
    //else if(Joystick::isButtonPressed(joystickId, joyButton::L1));
        //Do something
    //else if(Joystick::isButtonPressed(joystickId, joyButton::L2));
        //Do something
    else
        system(cmd);

    sleep(sf::milliseconds(200));
}

int main()
{
    RenderWindow wnd(VideoMode(200, 200), "Some random window!");
    bool running = true;
    while(running)
    {
        Thread updater(update);
        updater.launch();
        //sleep(milliseconds(1000/30));
        if(Keyboard::isKeyPressed(Keyboard::Return) || Joystick::isButtonPressed(1, 9))
            running = false;
        else if(Joystick::getAxisPosition(1, Joystick::Axis::X) > 0)
        {
            Mouse::setPosition(Mouse::getPosition() + Vector2i(5, 0));
        }
        else if(Joystick::getAxisPosition(1, Joystick::Axis::Y) > 0)
        {
            Mouse::setPosition(Mouse::getPosition() + Vector2i(0, 5));
        }
        else if(Joystick::getAxisPosition(1, Joystick::Axis::X) < 0)
        {
            Mouse::setPosition(Mouse::getPosition() + Vector2i(-5, 0));
        }
        else if(Joystick::getAxisPosition(1, Joystick::Axis::Y) < 0)
        {
            Mouse::setPosition(Mouse::getPosition() + Vector2i(0, -5));
        }
        else if(Joystick::isButtonPressed(1, joyButton::Triangle))
        {
            input("xdotool key Return", "", "", "", "");
        }
        else if(Joystick::isButtonPressed(1, joyButton::Circle))
        {
            input("xdotool click 3", "xdotool mousedown 3", "", "", "");
        }
        else if(Joystick::isButtonPressed(1, joyButton::Cross))
        {
            input("xdotool click 1", "xdotool mousedown 1", "", "", "");
        }
        else if(Joystick::isButtonPressed(1, joyButton::Square))
        {
            input("xdotool click 2", "xdotool mousedown 2", "", "", "");
        }

        wnd.setVisible(false);
    }
    //cin.ignore();
    wnd.close();
    cout << "Goodbye!\n";
    return 0;
}




















