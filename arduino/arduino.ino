#include <M5Dial.h>

#define ENABLE_MOUSE 1

#ifdef ENABLE_MOUSE
#include "USB.h"
#include "USBHIDMouse.h"
#endif

#if ENABLE_MOUSE
USBHIDMouse Mouse;
#endif

void setup()
{
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);
#if ENABLE_MOUSE
    Mouse.begin();
    USB.begin();
#endif
}

long oldPosition = -999;

int prev_x = -1;
int prev_y = -1;
static m5::touch_state_t prev_state;

int alpha_count = 0;

bool touched = false;
bool first_move = false;

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
        if (t.state == m5::touch_state_t::touch)
        {
            touched = true;
            first_move = true;
            prev_x = t.x;
            prev_y = t.y;
            Serial.println("TOUCH X:" + String(t.x) + " / " + "Y:" + String(t.y));
        }
        if (t.state == m5::touch_state_t::touch_end)
        {
            Serial.println("LEFT CLICK!!!");
#if ENABLE_MOUSE
            Mouse.click(MOUSE_LEFT);
#endif
        }
        if (t.state == m5::touch_state_t::none)
        {
            touched = false;
            M5Dial.Display.fillRect(0, 0, 240, 240, BLACK);
        }
    }
    if (touched && (prev_x != t.x || prev_y != t.y))
    {
        int8_t dx = (int16_t)t.x - (int16_t)prev_x;
        int8_t dy = (int16_t)t.y - (int16_t)prev_y;

        if (first_move)
        {
            first_move = false;
            Serial.println("FIRST MOVE");
        }
        else
        {
            Serial.println("MOVE  X:" + String(t.x) + " / " + "Y:" + String(t.y) + " / " + "DX:" + String(dx) + " / " + "DY:" + String(dy));
#if ENABLE_MOUSE
            Mouse.move(dx * 5, dy * 5, 0, 0);
#endif
        }
        prev_x = t.x;
        prev_y = t.y;
        M5Dial.Display.drawCircle(t.x, t.y, 5, RED);
    }

    if (M5Dial.BtnA.wasPressed())
    {
        Serial.println("RIGHT CLICK");
#if ENABLE_MOUSE
        Mouse.click(MOUSE_RIGHT);
#endif
    }

    long newPosition = M5Dial.Encoder.read();
    if (newPosition != oldPosition)
    {
        int8_t dw = newPosition - oldPosition;
        Serial.println("W:" + String(newPosition) + " / " + "DW:" + String(dw));
        oldPosition = newPosition;
#if ENABLE_MOUSE
        Mouse.move(0, 0, dw, 0);
#endif
    }
}
