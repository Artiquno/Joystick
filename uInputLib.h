#include <string.h>
#include <fcntl.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <unistd.h>

using namespace std;
using namespace sf;

#define DIV 20
#define DIV_SLOW 50

#define WHEEL_FAST 50
#define WHEEL_DIV 3000
#define WHEEL_DIV_SLOW 7000

#define SPEED 1
#define SPEED_FAST 3

#define INPUT_SLEEP 200
#define MODIFIER_INPUT_SLEEP 400

#define VOL "5"

#define getPos Joystick::getAxisPosition
#define butPress Joystick::isButtonPressed

static char connected;
bool alt, shift, ctrl, sysRq = false;

void keyDown(int key);
void keyUp(int key);
void keyPress(int key);
void mouseWheel(int type, int code, int val);

//After i understand how to light up the LEDs
//int numLock();

int modifierEvent(bool& mod, int key);

bool r1(int R1, bool hold = false);
bool l1(int L1, bool hold = false);
bool l2(int L2, bool hold = false);
bool r2(int R2, bool hold = false);

void buttons(int regular, int L1 = 0, int R1 = 0, int L2 = 0, int R2 = 0);
void mouseButtons(int mouseButton, int L1 = 0, int R2 = 0, int L2 = 0);

int axisR1(int positive = 0, int negative = 0, float axisPos = 0);
int axisL1(int positive = 0, int negative = 0, float axisPos = 0);
int axisL2(int positive = 0, int negative = 0, float axisPos = 0);
int axisR2(int positive = 0, int negative = 0, float axisPos = 0);

bool axisEvent(float axisPos, int regular, int R1 = 0, int L1 = 0, int L2 = 0, int R2 = 0);

enum joyButton
{
    Triangle = 0, Circle, Cross, Square,
    L1, R1,
    L2, R2,
    Select, Start,
    AnaLeft, AnaRight
};

/* Globals */
static int uinp_fd = -1;
struct uinput_user_dev uinp;

// uInput device structure
struct input_event event; // Input device structure

/* Setup the uinput device */
int setup_uinput_device()
{
    // Open the input device
    uinp_fd = open("/dev/uinput", O_WRONLY | O_NDELAY);
    if (uinp_fd < 0)
    {
        printf("Unable to open /dev/uinput\n");
        return -1;
    }
    memset(&uinp,0,sizeof(uinp)); // Intialize the uInput device to NULL
    strncpy(uinp.name, "Joystick Controller", UINPUT_MAX_NAME_SIZE);
    uinp.id.version = 4;
    uinp.id.bustype = BUS_USB;

    // Setup the uinput device
    ioctl(uinp_fd, UI_SET_EVBIT, EV_KEY);
    ioctl(uinp_fd, UI_SET_EVBIT, EV_REL);
    ioctl(uinp_fd, UI_SET_EVBIT, EV_LED);

    ioctl(uinp_fd, UI_SET_RELBIT, REL_X);
    ioctl(uinp_fd, UI_SET_RELBIT, REL_Y);
    ioctl(uinp_fd, UI_SET_RELBIT, REL_HWHEEL);
    ioctl(uinp_fd, UI_SET_RELBIT, REL_WHEEL);

    ioctl(uinp_fd, UI_SET_LEDBIT, LED_CAPSL);
    ioctl(uinp_fd, UI_SET_LEDBIT, LED_NUML);
    ioctl(uinp_fd, UI_SET_LEDBIT, LED_SCROLLL);
    //ioctl(uinp_fd, UI_SET_LEDBIT, LED_);


    for (int i = 0; i < 256; i++) {
        ioctl(uinp_fd, UI_SET_KEYBIT, i);
    }

    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MOUSE);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_LEFT);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_RIGHT);
    ioctl(uinp_fd, UI_SET_KEYBIT, BTN_MIDDLE);

    /* Create input device into input sub-system */
    write(uinp_fd, &uinp, sizeof(uinp));
    if (ioctl(uinp_fd, UI_DEV_CREATE))
    {
        printf("Unable to create UINPUT device.\n");
        return -1;
    }
    return 1;
}

void keyDown(int key)
{
    event.type = EV_KEY;
    event.code = key;
    event.value = 1;
    write(uinp_fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
}

void keyUp(int key)
{
    event.type = EV_KEY;
    event.code = key;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
}

void keyPress(int key)
{
    keyDown(key);
    keyUp(key);
}

void relative(int code, int val)
{
    event.type = EV_REL;
    event.code = code;
    event.value = val;
    write(uinp_fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));


    event.type = EV_REL;
    event.code = code;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));

    event.type = EV_SYN;
    event.code = SYN_REPORT;
    event.value = 0;
    write(uinp_fd, &event, sizeof(event));
}

bool r1(int R1, bool hold)
{
    if(butPress(connected, joyButton::R1))
    {
        if(!hold)
            keyPress(R1);
        else
            keyDown(R1);
        return true;
    }
    return false;
}

bool l1(int L1, bool hold)
{
    if(butPress(connected, joyButton::L1))
    {
        if(!hold)
            keyPress(L1);
        else
            keyDown(L1);
        return true;
    }
    return false;
}

bool r2(int R2, bool hold)
{
    if(butPress(connected, joyButton::R2))
    {
        if(!hold)
            keyPress(R2);
        else
            keyDown(R2);
        return true;
    }
    return false;
}

bool l2(int L2, bool hold)
{
    if(butPress(connected, joyButton::L2))
    {
        if(!hold)
            keyPress(L2);
        else
            keyDown(L2);
        return true;
    }
    return false;
}

void buttons(int regular, int R1, int L1, int L2, int R2)
{
    if(r1(R1));         //Frequently used
    else if(l1(L1));    //Less frequent used
    else if(l2(L2));    //Sometimes used
    else if(r2(R2));    //Rarely used
    else keyPress(regular); //Free press

    sleep(milliseconds(INPUT_SLEEP));
}

void mouseButtons(int mouseButton, int L1, int L2, int R2)
{
    if(r1(mouseButton, true));  //Frequent
    else if(l1(L1));        //Less frequent
    else if(l2(L2));        //Modifiers
    else if(r2(R2));        //Rarely
    else keyPress(mouseButton); //Free press

    sleep(milliseconds(INPUT_SLEEP));
}

int modifierEvent(bool& mod, int key)
{
    mod = !mod;
    l2(key, mod);
    return 0;
}

int axisR1(int positive, int negative, float axisPos)
{
    if(!positive)
        return 0;
    if(butPress(connected, joyButton::R1))
    {
        if(axisPos > 0)
            keyPress(positive);
        else
            keyPress(negative);
        return positive;
    }
    return false;
}

int axisL1(int positive, int negative, float axisPos)
{
    if(!positive)
        return 0;
    if(butPress(connected, joyButton::L1))
    {
        if(axisPos > 0)
            keyPress(positive);
        else
            keyPress(negative);
        return positive;
    }
    return false;
}

int axisL2(int positive, int negative, float axisPos)
{
    if(!positive)
        return 0;
    if(butPress(connected, joyButton::L2))
    {
        if(axisPos > 0)
            keyPress(positive);
        else
            keyPress(negative);
        return positive;
    }
    return false;
}

int axisR2(int positive, int negative, float axisPos)
{
    if(!positive)
        return 0;
    if(butPress(connected, joyButton::R2))
    {
        if(axisPos > 0)
            keyPress(positive);
        else
            keyPress(negative);
        return positive;
    }
    return false;
}

bool axisEvent(float axisPos, int regular, int R1, int L1, int L2, int R2)
{
    if(axisR1(R1) || axisL1(L1) || axisL2(L2) || axisR2(R2))
    {
        sleep(milliseconds(INPUT_SLEEP));
        return true;
    }
    else
        return false;
}


























