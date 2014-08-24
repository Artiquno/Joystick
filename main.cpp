#include <iostream>
#include <SFML/System/Sleep.hpp>
#include <SFML/Window/Joystick.hpp>

#ifdef __unix__
    #include "uInputLib.h"
#elif defined _WIN32
    #include "winInputLib.h"
#endif // defined

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
    #ifdef __unix__
    if (setup_uinput_device() < 0)
    {
        printf("Unable to find uinput device\n");
        return -1;
    }
    #endif
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

        //Pause/Unpause
        if( butPress(connected, joyButton::R2) &&
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
                buttons(KEY_BACKSPACE, KEY_SPACE, KEY_KP7, KEY_NUMLOCK, KEY_DELETE);
            }
            else if(butPress(connected, joyButton::Circle))
            {
                mouseButtons(BTN_RIGHT, KEY_KP3, modifierEvent(shift, KEY_LEFTSHIFT));
            }
            else if(butPress(connected, joyButton::Cross))
            {
                mouseButtons(BTN_LEFT, KEY_KP1, modifierEvent(ctrl, KEY_LEFTCTRL));
            }
            else if(butPress(connected, joyButton::Square))
            {
                mouseButtons(BTN_MIDDLE, KEY_KP5, modifierEvent(alt, KEY_LEFTALT));
            }
            else if(butPress(connected, joyButton::Select))
            {
                buttons(KEY_LEFTMETA, KEY_ESC, KEY_WLAN);
            }
            else if(butPress(connected, joyButton::Start))
            {
                buttons(KEY_ENTER, KEY_TAB, KEY_SYSRQ);
            }

            //KEY_BASSBOOST is assigned for opening onboard (on-screen keybard)
            //Joystick presses
            else if(butPress(connected, joyButton::AnaLeft))
            {
                buttons(KEY_PLAYPAUSE, KEY_BASSBOOST, KEY_PREVIOUSSONG);
            }
            else if(butPress(connected, joyButton::AnaRight))
            {
                buttons(KEY_STOPCD, KEY_MUTE, KEY_NEXTSONG);
            }

            //Right Y Axis
            if(Joystick::getAxisPosition(connected, Joystick::Axis::R) != 0)
            {
                float axisPos = -Joystick::getAxisPosition(connected, Joystick::Axis::R);
                if(axisEvent(axisPos, 0, 0, 0,
                             axisL2(KEY_UP, KEY_DOWN, axisPos),
                             axisR2(KEY_VOLUMEUP, KEY_VOLUMEDOWN, axisPos)));
                else    //No modifier is pressed
                {
                    float temp = (axisPos > 0 ? axisPos : -axisPos);
                    axisPos = (axisPos > 0 ? 1 : -1);
                    if(butPress(connected, joyButton::R1))
                    {
                        relative(REL_WHEEL, axisPos);
                        sleep(milliseconds(WHEEL_DIV_SLOW/temp));
                    }
                    else if(butPress(connected, joyButton::L1))
                    {
                        relative(REL_WHEEL, axisPos);
                        sleep(milliseconds(WHEEL_DIV/temp / WHEEL_FAST));
                    }
                    else
                    {
                        relative(REL_WHEEL, axisPos);
                        sleep(milliseconds(WHEEL_DIV/temp));
                    }
                }
            }

            //Right X Axis
            else if(Joystick::getAxisPosition(connected, Joystick::Axis::U) != 0)
            {
                float axisPos = Joystick::getAxisPosition(connected, Joystick::Axis::U);
                if(axisEvent(axisPos, 0, 0, 0,
                             axisL2(KEY_RIGHT, KEY_LEFT, axisPos),
                             axisR2(KEY_FASTFORWARD, KEY_REWIND, axisPos)));
                else    //No modifier is pressed
                {
                    float temp = (axisPos > 0 ? axisPos : -axisPos);
                    axisPos = (axisPos > 0 ? 1 : -1);
                    if(butPress(connected, joyButton::R1))
                    {
                        relative(REL_HWHEEL, axisPos);
                        sleep(milliseconds(WHEEL_DIV_SLOW/temp));
                    }
                    else if(butPress(connected, joyButton::L1))
                    {
                        relative(REL_HWHEEL, axisPos);
                        sleep(milliseconds(WHEEL_DIV/temp / WHEEL_FAST));
                    }
                    else
                    {
                        relative(REL_HWHEEL, axisPos);
                        sleep(milliseconds(WHEEL_DIV/temp));
                    }
                }
            }

            //Left X Axis
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

            //Left Y Axis
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
           butPress(connected, joyButton::R2))
        {
            running = false;
        }
    }
    //cin.ignore();
    cout << "Goodbye!\n";
    return 0;
}






















