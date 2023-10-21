#include <M5Dial.h>

void setup()
{
    auto cfg = M5.config();
    M5Dial.begin(cfg, true, false);
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
    }
    if (prev_x != t.x || prev_y != t.y)
    {
        Serial.println("X:" + String(t.x) + " / " + "Y:" + String(t.y));
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
