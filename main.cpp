#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Joystick.hpp>

#define DIV 15
#define DIV_SLOW 40
#define SPEED 1
#define INPUT_SLEEP 200
#define MODIFIER_INPUT_SLEEP 400
#define VOL "10"

#define getPos Joystick::getAxisPosition
#define butPress Joystick::isButtonPressed

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

void input(char cmd[], char R1[], char R2[], char L1[], char L2[], unsigned short int joystickId = 0);
char getJoystick(unsigned char connected);

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
            system("xdotool mouseup 1 mouseup 2 mouseup 3 "
                   "keyup Super ");
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
    if(running)
    {
        system("onboard");
    }
    while(running)
    {
        Joystick::update();
        sleep(milliseconds(1000/80));

        //Unpause
        if( butPress(connected, joyButton::R1) &&
            butPress(connected, joyButton::Select))
        {
            paused = false;
            cout << "Paused = false\n";
            sleep(milliseconds(500));
        }

        if(!paused)
        {
            //Pausing
            if(butPress(connected, joyButton::R1) &&
               butPress(connected, joyButton::Start))
            {
                paused = true;
                cout << "Paused = true\n";
                sleep(milliseconds(500));
            }

            //Normal buttons
            else if(butPress(connected, joyButton::Triangle))
            {
                input("xdotool key BackSpace","xdotool key space" , "", "", "", connected);
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
                input("xdotool key Super", "", "", "", "", connected);
                sleep(sf::milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Start))
            {
                input("xdotool key Return", "", "", "", "", connected);
                sleep(sf::milliseconds(INPUT_SLEEP));
            }

            //Joystick presses
            else if(butPress(connected, joyButton::AnaLeft))
            {
                input("banshee --toggle-playing", "", "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::AnaRight))
            {
                input("banshee --stop", "", "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }

            //Modifier Keys
            else if(butPress(connected, joyButton::L1))
            {
                if(!shift)
                {
                    system("xdotool keydown Shift");
                    shift = true;
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                }
                else
                {
                    system("xdotool keyup Shift");
                    shift = false;
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                }
            }
            else if(butPress(connected, joyButton::L2))
            {
                if(!ctrl)
                {
                    system("xdotool keydown Ctrl");
                    ctrl = true;
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                }
                else
                {
                    system("xdotool keyup Ctrl");
                    ctrl = false;
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                }
            }
            else if(butPress(connected, joyButton::R2))
            {
                if(!alt)
                {
                    system("xdotool keydown Alt");
                    alt = true;
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                }
                else
                {
                    system("xdotool keyup Alt");
                    alt = false;
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                }
            }

            //Right analog
            if(Joystick::getAxisPosition(connected, Joystick::Axis::R) < 0)
            {
                input("banshee --set-volume=+" VOL, "", "", "", "", connected);
            }
            else if(Joystick::getAxisPosition(connected, Joystick::Axis::R) > 0)
            {
                input("banshee --set-volume=-" VOL, "", "", "", "", connected);
            }
            else if(Joystick::getAxisPosition(connected, Joystick::Axis::U) > 0)
            {
                input("banshee --next", "", "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(Joystick::getAxisPosition(connected, Joystick::Axis::U) < 0)
            {
                input("banshee --restart-or-previous", "", "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }

            //Left analog
            if(Joystick::getAxisPosition(connected, Joystick::Axis::X) != 0)
            {
                if(!butPress(connected, joyButton::R1))
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(getPos(connected, Joystick::Axis::X)/DIV * SPEED, 0));
                else
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(getPos(connected, Joystick::Axis::X)/DIV_SLOW * SPEED, 0));
            }

            if(Joystick::getAxisPosition(connected, Joystick::Axis::Y) != 0)
            {
                if(!butPress(connected, joyButton::R1))
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(0, getPos(connected, Joystick::Axis::Y)/DIV * SPEED));
                else
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(0, getPos(connected, Joystick::Axis::Y)/DIV_SLOW * SPEED));
            }
        }
        //Close
        if(butPress(connected, joyButton::Start) &&
           butPress(connected, joyButton::Select) &&
           butPress(connected, joyButton::R1))
        {
            running = false;
        }
    }
    //cin.ignore();
    cout << "Goodbye!\n";
    return 0;
}




















