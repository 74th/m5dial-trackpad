#include <M5Dial.h>
#include "USB.h"
#include "USBHIDMouse.h"
#define ENABLE_MOUSE 0

#ifdef ENABLE_MOUSE
USBHIDMouse Mouse;
#endif

void setup()
{
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);
#ifdef ENABLE_MOUSE
    Mouse.begin();
    USB.begin();
#endif
}

long oldPosition = -999;

int prev_x = -1;
int prev_y = -1;
static m5::touch_state_t prev_state;

int alpha_count = 0;

void loop()
{
    M5Dial.update();
    auto t = M5Dial.Touch.getDetail();

    if (prev_state != t.state)
    {
        prev_state = t.state;
        static constexpr const char *state_name[16] = {
            "none", "touch", "touch_end", "touch_begin",
            "___", "hold", "hold_end", "hold_begin",
            "___", "flick", "flick_end", "flick_begin",
            "___", "drag", "drag_end", "drag_begin"};
        Serial.println(state_name[t.state]);
        if (t.state == m5::touch_state_t::none)
        {
            M5Dial.Display.fillRect(0, 0, 240, 240, BLACK);
        }
        if (t.state == m5::touch_state_t::touch)
        {
            prev_x = t.x;
            prev_y = t.y;
        }
    }
    if (prev_x != t.x || prev_y != t.y)
    {
        Serial.println("X:" + String(t.x) + " / " + "Y:" + String(t.y));
#ifdef ENABLE_MOUSE
        Mouse.move(t.x - prev_x, t.y - prev_y, 0, 0);
#endif
        prev_x = t.x;
        prev_y = t.y;
        M5Dial.Display.drawCircle(t.x, t.y, 5, RED);
    }

    long newPosition = M5Dial.Encoder.read();
    if (newPosition != oldPosition)
    {
        oldPosition = newPosition;
        Serial.println(newPosition);
    }
}
