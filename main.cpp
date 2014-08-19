#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Joystick.hpp>

#define DIV 15
#define SPEED 1
#define INPUT_SLEEP 200

#define getPos Joystick::getAxisPosition
#define butPress Joystick::isButtonPressed

//Note: L1 = Shift
//      L2 = Ctrl
//      R2 = Alt

using namespace std;
using namespace sf;

bool alt, shift, ctrl, holding = false;

enum joyButton
{
    Triangle = 0, Circle, Cross, Square,
    L1, R1,
    L2, R2,
    Select, Start,
    AnaLeft, AnaRight
};

void update();
void input(char cmd[], char R1[], char R2[], char L1[], char L2[], unsigned short int joystickId = 0);
char getJoystick(unsigned char connected);

void update()
{
    Joystick::update();
}

void input(char cmd[], char R1[], char R2[], char L1[], char L2[], unsigned short int joystickId)
{
    if(butPress(joystickId, joyButton::R1))
    {
        if(!holding)
        {
            system(R1);
            holding = true;
        }
        else
        {
            system("xdotool mouseup 1 mouseup 2 mouseup 3 keyup Super");
            holding = false;
        }
    }
    //else if(butPress(joystickId, joyButton::R2));
        //Do something
    //else if(butPress(joystickId, joyButton::L1));
        //Do something
    //else if(butPress(joystickId, joyButton::L2));
        //Do something
    else
        system(cmd);
}

char getJoystick()
{
    for(int i = 0; i < 10; ++i)
    {
        if(Joystick::isConnected(i))
            return i;
    }
    return -1;
}

int main()
{
    bool running = true;
    bool paused = false;
    Joystick::update();
    char connected = getJoystick();
    if(connected == -1)
    {
        running = false;
        cout << "Failed to detect any joystick!\n";
    }
    system("onboard");
    while(running)
    {
        Thread updater(update);
        updater.launch();
        sleep(milliseconds(1000/80));
        if(!paused)
        {
//            if(butPress(connected, joyButton::Select))
//                running = false;
//            else
            if(butPress(connected, joyButton::Triangle))
            {
                input("xdotool key BackSpace", "", "", "", "", connected);
                sleep(sf::milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Circle))
            {
                input("xdotool click 3", "xdotool mousedown 3", "", "", "", connected);
                sleep(sf::milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Cross))
            {
                input("xdotool click 1", "xdotool mousedown 1", "", "", "", connected);
                sleep(sf::milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Square))
            {
                input("xdotool click 2", "xdotool mousedown 2", "", "", "", connected);
                sleep(sf::milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Select))
            {
                input("xdotool key Super", "xdotool keydown Super", "", "", "", connected);
                sleep(sf::milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Start))
            {
                input("xdotool key Return", "xdotool keydown Return", "", "", "", connected);
            }
            else if(butPress(connected, joyButton::L1) &&
                    butPress(connected, joyButton::R1))
            {
                paused = true;
                cout << "Paused = true\n";
                sleep(milliseconds(500));
            }

            if(Joystick::getAxisPosition(connected, Joystick::Axis::X) > 0)
            {
                Mouse::setPosition(Mouse::getPosition() + Vector2i(getPos(connected, Joystick::Axis::X)/DIV * SPEED, 0));
            }
            else if(Joystick::getAxisPosition(connected, Joystick::Axis::X) < 0)
            {
                Mouse::setPosition(Mouse::getPosition() + Vector2i(getPos(connected, Joystick::Axis::X)/DIV * SPEED, 0));
            }

            if(Joystick::getAxisPosition(connected, Joystick::Axis::Y) > 0)
            {
                Mouse::setPosition(Mouse::getPosition() + Vector2i(0, getPos(connected, Joystick::Axis::Y)/DIV * SPEED));
            }
            else if(Joystick::getAxisPosition(connected, Joystick::Axis::Y) < 0)
            {
                Mouse::setPosition(Mouse::getPosition() + Vector2i(0, getPos(connected, Joystick::Axis::Y)/DIV * SPEED));
            }
        }
        if( butPress(connected, joyButton::R1) &&
            butPress(connected, joyButton::R2))
        {
            paused = false;
            cout << "Paused = false\n";
            sleep(milliseconds(500));
        }
    }
    //cin.ignore();
    cout << "Goodbye!\n";
    return 0;
}




















