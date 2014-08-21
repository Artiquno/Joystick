#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Joystick.hpp>
#define __USE_GNU
#include <sched.h>

#define DIV 20
#define DIV_SLOW 40

#define SPEED 1
#define SPEED_FAST 3

#define INPUT_SLEEP 200
#define MODIFIER_INPUT_SLEEP 400

#define VOL "5"

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

void input(char cmd[], char R1[], char L1[], char R2[], char L2[], unsigned short int joystickId = 0);
char getJoystick(unsigned char connected);

void input(char cmd[], char R1[], char L1[], char R2[], char L2[], unsigned short int joystickId)
{
    if(holding)
    {
        system("xdotool mouseup 1 mouseup 2 mouseup 3 "
               "keyup Super ");
        holding = false;
    }
    else
    {
        if(butPress(joystickId, joyButton::R1))
        {
            system(R1);
            holding = true;
        }
        else if(butPress(joystickId, joyButton::L1))
        {
            system(L1);
        }
        //else if(butPress(joystickId, joyButton::L1));
            //Do something
        //else if(butPress(joystickId, joyButton::L2));
            //Do something
        else
            system(cmd);
    }
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

    int abc = sched_setaffinity(0, sizeof(cpu_set_t), 0);
    bool running = true;
    bool paused = false;
    Joystick::update();
    char connected = getJoystick();
    if(connected == -1)
    {
        running = false;
        cout << "Failed to detect any joystick!\n";
    }
    while(running)
    {
        Joystick::update();
        sleep(milliseconds(1000/80));

        //Unpause
        if( butPress(connected, joyButton::L1) &&
            butPress(connected, joyButton::Select))
        {
            if(paused)
            {
                paused = false;
                cout << "Paused = false\n";
                sleep(milliseconds(500));
            }
            else
            {
                paused = true;
                cout << "Paused = true\n";
                sleep(milliseconds(500));
            }
        }

        if(!paused)
        {
            //Normal buttons
            if(butPress(connected, joyButton::Triangle))
            {
                input("xdotool key BackSpace",
                      "xdotool key space" ,
                      "xdotool key KP_7",
                      "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Circle))
            {
                input("xdotool click 3",
                      "xdotool mousedown 3",
                      "xdotool key KP_3",
                      "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Cross))
            {
                input("xdotool click 1",
                      "xdotool mousedown 1",
                      "xdotool key KP_1",
                      "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Square))
            {
                input("xdotool click 2",
                      "xdotool mousedown 2",
                      "xdotool key KP_5",
                      "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Select))
            {
                input("xdotool key Super",
                      "xdotool key Escape",
                      "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Start))
            {
                input("xdotool key Return",
                      "xdotool key Tab",
                      "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }

            //Joystick presses
            else if(butPress(connected, joyButton::AnaLeft))
            {
                input("banshee --toggle-playing &", "", "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::AnaRight))
            {
                input("banshee --stop &", "", "", "", "", connected);
                sleep(milliseconds(INPUT_SLEEP));
            }

            //Modifier Keys
            else if(butPress(connected, joyButton::L1))
            {
                if(butPress(connected, joyButton::R1))
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
            }
            else if(butPress(connected, joyButton::L2))
            {
                if(butPress(connected, joyButton::R1))
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
                else
                {
                    system("onboard &");
                    sleep(milliseconds(INPUT_SLEEP));
                }
            }
            else if(butPress(connected, joyButton::R2))
            {
                if(butPress(connected, joyButton::R1))
                {
                    if(!alt)
                    {
                        input("", "xdotool keydown Alt", "", "", "", connected);
                        alt = true;
                        sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                    }
                    else
                    {
                        input("", "xdotool keyup Alt", "", "", "", connected);
                        alt = false;
                        sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                    }
                }
                else
                {
                    system("killall onboard");
                    sleep(milliseconds(INPUT_SLEEP));
                }
            }

            //Right analog
            if(Joystick::getAxisPosition(connected, Joystick::Axis::R) != 0)
            {
                float axisPos = Joystick::getAxisPosition(connected, Joystick::Axis::R);
                if(butPress(connected, joyButton::R1))  //R1 is pressed
                {
                    if(axisPos < 0)
                    {
                        system("banshee --set-volume=+" VOL " &");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else if(axisPos > 0)
                    {
                        system("banshee --set-volume=-" VOL " &");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
                else if(butPress(connected, joyButton::L1)) //L1 is pressed
                {
                    if(axisPos < 0)
                    {
                        system("xdotool key Up");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else if(axisPos > 0)
                    {
                        system("xdotool key Down");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
                else    //No modifier is pressed
                {
                    if(axisPos < 0)
                    {
                        system("xdotool click 4");
                        sleep(milliseconds(-(10/axisPos)*INPUT_SLEEP));
                    }
                    else
                    {
                        system("xdotool click 5");
                        sleep(milliseconds((10/axisPos)*INPUT_SLEEP));
                    }
                }
            }   //End Axis::R

            else if(Joystick::getAxisPosition(connected, Joystick::Axis::U) != 0)
            {
                float axisPos = Joystick::getAxisPosition(connected, Joystick::Axis::U);
                if(butPress(connected, joyButton::R1))
                {
                    if(axisPos > 0)
                    {
                        system("banshee --next &");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else
                    {
                        system("banshee --restart-or-previous &");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
                else if(butPress(connected, joyButton::L1))
                {
                    if(axisPos > 0)
                    {
                        system("xdotool key Right");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else
                    {
                        system("xdotool key Left");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
                else
                {
                    if(axisPos > 0)
                    {
                        system("amixer -c 0 set Master " VOL "%+ ");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else if(axisPos < 0)
                    {
                        system("amixer -c 0 set Master " VOL "%- ");
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
            }

            //Left analog
            if(Joystick::getAxisPosition(connected, Joystick::Axis::X) != 0)
            {
                if(butPress(connected, joyButton::R1))
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(getPos(connected, Joystick::Axis::X)/DIV_SLOW * SPEED, 0));
                else if(butPress(connected, joyButton::L1))
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(getPos(connected, Joystick::Axis::X)/DIV * SPEED_FAST, 0));
                else
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(getPos(connected, Joystick::Axis::X)/DIV * SPEED, 0));
            }

            if(Joystick::getAxisPosition(connected, Joystick::Axis::Y) != 0)
            {
                if(butPress(connected, joyButton::R1))
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(0, getPos(connected, Joystick::Axis::Y)/DIV_SLOW * SPEED));
                else if(butPress(connected, joyButton::L1))
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(0, getPos(connected, Joystick::Axis::Y)/DIV * SPEED_FAST));
                else
                    Mouse::setPosition(Mouse::getPosition() + Vector2i(0, getPos(connected, Joystick::Axis::Y)/DIV * SPEED));
            }
        }
        //Closer
        if(butPress(connected, joyButton::Start) &&
           butPress(connected, joyButton::Select) &&
           butPress(connected, joyButton::L1))
        {
            running = false;
        }
    }
    //cin.ignore();
    cout << "Goodbye!\n";
    return 0;
}




















