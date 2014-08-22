#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window/Joystick.hpp>
#include "uInputLib.h"



using namespace std;
using namespace sf;



char getJoystick(unsigned char connected);

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
    if (setup_uinput_device() < 0)
    {
        printf("Unable to find uinput device\n");
        return -1;
    }
    bool running = true;
    bool paused = false;
    Joystick::update();
    connected = getJoystick();
    if(connected == -1)
    {
        running = false;
        cout << "Failed to detect any joystick!\n";
    }
    while(running)
    {
        sleep(milliseconds(1000/80));
        Joystick::update();

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
                buttons(KEY_BACKSPACE, KEY_SPACE, KEY_KP7, numLock());
            }
            else if(butPress(connected, joyButton::Circle))
            {
                mouseButtons(BTN_RIGHT, KEY_KP3, KEY_NUMLOCK);
            }
            else if(butPress(connected, joyButton::Cross))
            {
                if(butPress(connected, joyButton::L1))
                {
                    keyPress(KEY_KP1);
                }
                else if(butPress(connected, joyButton::R1))
                {
                    keyDown(BTN_LEFT);
                }
                else
                {
                    keyPress(BTN_LEFT);
                }
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Square))
            {
                if(butPress(connected, joyButton::L1))
                {
                    keyPress(KEY_KP5);
                }
                else if(butPress(connected, joyButton::R1))
                {
                    keyDown(BTN_MIDDLE);
                }
                else
                {
                    keyPress(BTN_MIDDLE);
                }
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Select))
            {
                if(butPress(connected, joyButton::R1))
                {
                    keyPress(KEY_ESC);
                }
                else
                {
                    keyPress(KEY_LEFTMETA);
                }
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::Start))
            {
                if(butPress(connected, joyButton::R1))
                {
                    keyPress(KEY_TAB);
                }
                else
                {
                    keyPress(KEY_ENTER);
                }
                sleep(milliseconds(INPUT_SLEEP));
            }

            //Joystick presses
            else if(butPress(connected, joyButton::AnaLeft))
            {
                system("banshee --toggle-playing &");
                sleep(milliseconds(INPUT_SLEEP));
            }
            else if(butPress(connected, joyButton::AnaRight))
            {
                system("banshee --stop &");
                sleep(milliseconds(INPUT_SLEEP));
            }

            //Modifier Keys
            else if(butPress(connected, joyButton::L1))
            {
                if(butPress(connected, joyButton::R1))
                {
                    if(!shift)
                    {
                        keyUp(KEY_LEFTSHIFT);
                        shift = true;
                    }
                    else
                    {
                        keyUp(KEY_LEFTSHIFT);
                        shift = false;
                    }
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
                }
            }
            else if(butPress(connected, joyButton::L2))
            {
                if(butPress(connected, joyButton::R1))
                {
                    if(!ctrl)
                    {
                        keyUp(KEY_LEFTCTRL);
                        ctrl = true;
                    }
                    else
                    {
                        keyUp(KEY_LEFTCTRL);
                        ctrl = false;
                    }
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
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
                        keyDown(KEY_LEFTALT);
                        alt = true;
                    }
                    else
                    {
                        keyUp(KEY_LEFTALT);
                        alt = false;
                    }
                    sleep(milliseconds(MODIFIER_INPUT_SLEEP));
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
                        keyPress(KEY_UP);
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else if(axisPos > 0)
                    {
                        keyPress(KEY_DOWN);
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
                else    //No modifier is pressed
                {
                    if(axisPos < 0)
                    {
                        relative(REL_WHEEL, 1);
                        sleep(milliseconds(-(10/axisPos)*INPUT_SLEEP));
                    }
                    else
                    {
                        relative(REL_WHEEL, -1);
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
                        keyPress(KEY_RIGHT);
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else
                    {
                        keyPress(KEY_LEFT);
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
                else
                {
                    if(axisPos > 0)
                    {
                        relative(REL_HWHEEL, axisPos/40);
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                    else if(axisPos < 0)
                    {
                        relative(REL_HWHEEL, axisPos/40);
                        sleep(milliseconds(INPUT_SLEEP));
                    }
                }
            }

            //Left analog
            if(Joystick::getAxisPosition(connected, Joystick::Axis::X) != 0)
            {
                float axisPos = Joystick::getAxisPosition(connected, Joystick::Axis::X);
                if(butPress(connected, joyButton::R1))
                    relative(REL_X, axisPos/DIV_SLOW * SPEED);
                else if(butPress(connected, joyButton::L1))
                    relative(REL_X, axisPos/DIV * SPEED_FAST);
                else
                    relative(REL_X, axisPos/DIV * SPEED);
            }

            if(Joystick::getAxisPosition(connected, Joystick::Axis::Y) != 0)
            {
                float axisPos = Joystick::getAxisPosition(connected, Joystick::Axis::Y);

                if(butPress(connected, joyButton::R1))
                    relative(REL_Y, axisPos/DIV_SLOW * SPEED);
                else if(butPress(connected, joyButton::L1))
                    relative(REL_Y, axisPos/DIV * SPEED_FAST);
                else
                    relative(REL_Y, axisPos/DIV * SPEED);
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






















