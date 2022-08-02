/*
 * Sneptest SMS - Input tests
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "SMSlib.h"
#include "sneptest.h"

/*
 * Test for 2-button SMS gamepad behaviour.
 */
static void input_test_2_button (void)
{
    char string_buf[3] = { '\0' };
    unsigned int scroll_x = 0;
    unsigned int scroll_y = 0;
    uint8_t line;

    clear_screen ();
    title_draw ("2 BUTTON GAMEPAD");
    reference_draw ("          1 + 2: BACK           ");

    draw_string (4, line  = 4,  "PLAYER 1");
    draw_string (4, line += 3,  "UP");
    draw_string (4, line += 2,  "DOWN");
    draw_string (4, line += 2,  "LEFT");
    draw_string (4, line += 2,  "RIGHT");
    draw_string (4, line += 2,  "BUTTON 1");
    draw_string (4, line += 2,  "BUTTON 2");

    draw_string (20, line  = 4,  "PLAYER 2");
    draw_string (20, line += 3,  "UP");
    draw_string (20, line += 2,  "DOWN");
    draw_string (20, line += 2,  "LEFT");
    draw_string (20, line += 2,  "RIGHT");
    draw_string (20, line += 2,  "BUTTON 1");
    draw_string (20, line += 2,  "BUTTON 2");

    while (true)
    {
        unsigned int pressed;

        SMS_waitForVBlank ();
        pressed = SMS_getKeysStatus ();

        if (pressed & PORT_A_KEY_UP)    draw_string (1,   7,  "->"); else draw_string (1,   7,  "  ");
        if (pressed & PORT_A_KEY_DOWN)  draw_string (1,   9,  "->"); else draw_string (1,   9,  "  ");
        if (pressed & PORT_A_KEY_LEFT)  draw_string (1,  11,  "->"); else draw_string (1,  11,  "  ");
        if (pressed & PORT_A_KEY_RIGHT) draw_string (1,  13,  "->"); else draw_string (1,  13,  "  ");
        if (pressed & PORT_A_KEY_1)     draw_string (1,  15,  "->"); else draw_string (1,  15,  "  ");
        if (pressed & PORT_A_KEY_2)     draw_string (1,  17,  "->"); else draw_string (1,  17,  "  ");

        if (pressed & PORT_B_KEY_UP)    draw_string (17,  7,  "->"); else draw_string (17,  7,  "  ");
        if (pressed & PORT_B_KEY_DOWN)  draw_string (17,  9,  "->"); else draw_string (17,  9,  "  ");
        if (pressed & PORT_B_KEY_LEFT)  draw_string (17, 11,  "->"); else draw_string (17, 11,  "  ");
        if (pressed & PORT_B_KEY_RIGHT) draw_string (17, 13,  "->"); else draw_string (17, 13,  "  ");
        if (pressed & PORT_B_KEY_1)     draw_string (17, 15,  "->"); else draw_string (17, 15,  "  ");
        if (pressed & PORT_B_KEY_2)     draw_string (17, 17,  "->"); else draw_string (17, 17,  "  ");

        if ((pressed & PORT_A_KEY_1) && (pressed & PORT_A_KEY_2)) break;
    }
}


/*
 * Test the pause and reset buttons.
 */
static void input_test_pause_reset (void)
{
    uint8_t pressed = 0;
    char string_buf[20] = { '\0' };

    uint8_t pause_counter = 0;

    clear_screen ();
    title_draw ("PAUSE & RESET");
    reference_draw ("             2: BACK            ");

    draw_string (4, 9,  "PAUSE COUNTER:");
    draw_string (4, 13, "RESET:");

    while (!(pressed & PORT_A_KEY_2))
    {
        SMS_waitForVBlank ();

        pressed = SMS_getKeysStatus ();
        if (SMS_queryPauseRequested ())
        {
            SMS_resetPauseRequest ();
            pause_counter++;
        }

        sprintf (string_buf, "%d", pause_counter);
        draw_string (19, 9, string_buf);

        draw_string (11, 13, pressed & RESET_KEY ? "PRESSED    " : "NOT PRESSED");
    }
}


/*
 * Input test submenu
 */
static void input_menu (void)
{
    menu_new ("INPUT TESTS");
    menu_item_add ("SMS 2-BUTTON GAMEPAD", input_test_2_button);
    menu_item_add ("PAUSE & RESET", input_test_pause_reset);
}
void input_menu_run (void)
{
    menu_run (input_menu);
}
