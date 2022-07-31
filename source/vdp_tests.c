/*
 * Sneptest SMS - VDP Tests
 */

#include <stdint.h>
#include <stdbool.h>
#include "SMSlib.h"
#include "sneptest.h"

/*
 * Test for VDP scrolling behaviour.
 */
void vdp_scroll_test (void)
{
    char string_buf[3] = { '\0' };
    uint16_t pressed = 0;
    uint16_t scroll_x = 0;
    uint16_t scroll_y = 0;
    uint8_t repeat = 0;

    clear_screen ();
    title_draw ("VDP SCROLLING");
    reference_draw ("    1: FAST-SCROLL    2: BACK   ");

    draw_string (10, 12,  "SCROLL X:");
    draw_string (10, 14,  "SCROLL Y:");

    while (!(pressed & PORT_A_KEY_2))
    {
        bool move = false;
        SMS_waitForVBlank ();

        SMS_setBGScrollX (scroll_x);
        SMS_setBGScrollY (scroll_y);

        /* Render during vblank */
        uint8_to_string (string_buf, scroll_x);
        draw_string (20, 12, string_buf);
        uint8_to_string (string_buf, scroll_y);
        draw_string (20, 14, string_buf);

        /* Input handling */
        pressed = SMS_getKeysStatus ();

        /* Respond instantly when the direction first goes down */
        if (SMS_getKeysPressed () & (PORT_A_KEY_UP | PORT_A_KEY_DOWN | PORT_A_KEY_LEFT | PORT_A_KEY_RIGHT))
        {
            move = true;
        }

        /* Rate-limited repeat if the button is held down */
        if (repeat >= REPEAT_RATE)
        {
            move = true;
        }

        /* Fast move if button 1 is held down */
        if (pressed & PORT_A_KEY_1)
        {
            move = true;
        }

        if (move)
        {
            repeat = 0;
            if (pressed & PORT_A_KEY_UP)    scroll_y++;
            if (pressed & PORT_A_KEY_DOWN)  scroll_y--;
            if (pressed & PORT_A_KEY_LEFT)  scroll_x--;
            if (pressed & PORT_A_KEY_RIGHT) scroll_x++;
        }

        repeat++;
    }

    SMS_setBGScrollX (0);
    SMS_setBGScrollY (0);
}


uint8_t line_interrupt_count = 0;

void vdp_interrupt_test_handler (void)
{
    line_interrupt_count++;
}


/*
 * Test for VDP line-interrupt behaviour.
 * TODO: Option to change background in interrupt handler.
 */
void vdp_line_interrupt_test (void)
{
    char string_buf[3] = { '\0' };
    unsigned int counter_reload = 128;
    unsigned int count_last_frame = 0;

    /* Configure the interrupt */
    SMS_setLineInterruptHandler (vdp_interrupt_test_handler);
    SMS_enableLineInterrupt();

    clear_screen ();
    title_draw ("VDP LINE INTERRUPTS");
    reference_draw ("      1: SLOW       2: BACK     ");

    draw_string (6, 10,  "COUNTER RELOAD:");

    draw_string (6, 12,  "INTERRUPTS");
    draw_string (6, 13,  "THIS FRAME:");

    while (true)
    {
        unsigned int pressed;
        SMS_waitForVBlank ();

        /* Update and reset counter */
        count_last_frame = line_interrupt_count;
        line_interrupt_count = 0;

        /* Render during vblank */
        uint8_to_string (string_buf, counter_reload);
        draw_string (22, 10, string_buf);
        uint8_to_string (string_buf, count_last_frame);
        draw_string (22, 13, string_buf);

        /* Input handling */
        pressed = SMS_getKeysStatus ();
        if (pressed & PORT_A_KEY_1)
            pressed = SMS_getKeysPressed ();

        if (pressed & PORT_A_KEY_UP)
        {
            counter_reload++;
            SMS_setLineCounter (counter_reload);
        }
        if (pressed & PORT_A_KEY_DOWN)
        {
            counter_reload--;
            SMS_setLineCounter (counter_reload);
        }
        if (pressed & PORT_A_KEY_2)     break;
    }

    SMS_disableLineInterrupt();
}


/*
 * Test for sprite behaviour.
 */
void vdp_sprite_test (void)
{
    char string_buf[3] = { '\0' };
    unsigned int sprite_x = 128;
    unsigned int sprite_y = 96;
    signed char sprite_index = 0;

    clear_screen ();
    title_draw ("VDP SPRITES");
    reference_draw ("      1: SLOW       2: BACK     ");

    draw_string (10, 12,  "SPRITE X:");
    draw_string (10, 14,  "SPRITE Y:");

    SMS_useFirstHalfTilesforSprites (true);

    SMS_initSprites();
    sprite_index = SMS_addSprite (sprite_x, sprite_y, '#' - ' ');
    SMS_finalizeSprites();

    while (true)
    {
        unsigned int pressed;
        SMS_waitForVBlank ();

        /* Render during vblank */
        uint8_to_string (string_buf, sprite_x);
        draw_string (22, 12, string_buf);
        uint8_to_string (string_buf, sprite_y);
        draw_string (22, 14, string_buf);
        SMS_updateSpritePosition (sprite_index, sprite_x, sprite_y);
        SMS_copySpritestoSAT ();

        /* Input handling */
        pressed = SMS_getKeysStatus ();
        if (pressed & PORT_A_KEY_1)
            pressed = SMS_getKeysPressed ();

        if (pressed & PORT_A_KEY_UP)
        {
            sprite_y--;
        }
        if (pressed & PORT_A_KEY_DOWN)
        {
            sprite_y++;
        }
        if (pressed & PORT_A_KEY_LEFT)
        {
            sprite_x--;
        }
        if (pressed & PORT_A_KEY_RIGHT)
        {
            sprite_x++;
        }
        if (pressed & PORT_A_KEY_2)     break;
    }

    SMS_initSprites ();
    SMS_finalizeSprites ();
    SMS_copySpritestoSAT ();
}


void vdp_background_backdrop_set (uint16_t value)
{
    SMS_setBackdropColor (value);
}


void vdp_background_blank_set (uint16_t value)
{
    if (value)
    {
        SMS_displayOff ();
    }
    else
    {
        SMS_displayOn ();
    }
}


/*
 * Test for the background and backtrop behaviour.
 */
void vdp_background_menu (void)
{
    menu_new ("VDP BACKGROUND");
    menu_item_add_value ("BACKDROP", 0x0f, vdp_background_backdrop_set);
    menu_item_add_value ("BLANKING", 0x01, vdp_background_blank_set);

    /* TODO: draw_string_priority (10, y,  "PRIORITY STRING"); */
}
void vdp_background_test (void)
{
    menu_run (vdp_background_menu);
    SMS_setBackdropColor (0);
    SMS_displayOn ();
}

