/*
 * Sneptest SMS - VDP Tests
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "SMSlib.h"
#include "sneptest.h"

static uint16_t line_interrupt_count = 0;

/*
 * Test for VDP scrolling behaviour.
 */
static void vdp_scroll_test (void)
{
    uint16_t pressed = 0;
    uint8_t scroll_x = 0;
    uint8_t scroll_y = 0;
    uint8_t repeat = 0;
    char string_buf[3] = { '\0' };

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
        sprintf (string_buf, "%02X", scroll_x);
        draw_string (20, 12, string_buf);
        sprintf (string_buf, "%02X", scroll_y);
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


/*
 * Test for sprite behaviour.
 */
static void vdp_sprite_test (void)
{
    char string_buf[3] = { '\0' };
    uint8_t sprite_x = 128;
    uint8_t sprite_y = 96;
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
        sprintf (string_buf, "%02X", sprite_x);
        draw_string (22, 12, string_buf);
        sprintf (string_buf, "%02X", sprite_y);
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




/*
 * Increment the line interrupt counter.
 */
static void vdp_interrupt_test_handler (void)
{
    line_interrupt_count++;
}


static uint16_t vdp_line_interrupt_count_get (void)
{
    uint16_t ret = line_interrupt_count;
    line_interrupt_count = 0;
    return ret;
}


static void vdp_line_interrupt_reload_set (uint16_t value)
{
    SMS_setLineCounter (value);
}


/*
 * Test the line interrupt behaviour.
 *
 * Note: Responding to user input can keep us busy enough
 *       to miss a vblank. This can cause two frames worth
 *       of interrupts to be shown while actively scrolling.
 *
 *       To get a more reliable value, perhaps we need a
 *       custom frame interrupt handler...
 */
static void vdp_line_interrupt_menu (void)
{
    menu_new ("VDP LINE INTERRUPT");
    menu_item_add_value ("COUNTER RELOAD", 0x80, 0xff, vdp_line_interrupt_reload_set);
    menu_item_add_show_uint ("INTERRUPTS PER FRAME", vdp_line_interrupt_count_get);
}
static void vdp_line_interrupt_test (void)
{
    SMS_setLineInterruptHandler (vdp_interrupt_test_handler);
    SMS_setLineCounter (0x80);
    SMS_enableLineInterrupt();

    menu_run (vdp_line_interrupt_menu);

    SMS_disableLineInterrupt();
    SMS_setBackdropColor (0);
}


static void vdp_background_backdrop_set (uint16_t value)
{
    SMS_setBackdropColor (value);
}


static void vdp_background_blank_set (uint16_t value)
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
 * Test for the background and backdrop behaviour.
 */
static void vdp_background_menu (void)
{
    menu_new ("VDP BACKGROUND");
    menu_item_add_value ("BACKDROP", 0x00, 0x0f, vdp_background_backdrop_set);
    menu_item_add_value ("BLANKING", 0x00, 0x01, vdp_background_blank_set);

    /* TODO: draw_string_priority (10, y,  "PRIORITY STRING"); */
}
static void vdp_background_test (void)
{
    menu_run (vdp_background_menu);
    SMS_setBackdropColor (0);
    SMS_displayOn ();
}


/*
 * VDP test submenu
 */
static void vdp_menu (void)
{
    menu_new ("VDP TESTS");
    menu_item_add ("VDP BACKGROUND", vdp_background_test);
    menu_item_add ("VDP LINE INTERRUPTS", vdp_line_interrupt_test);
    menu_item_add ("VDP SCROLLING", vdp_scroll_test);
    menu_item_add ("VDP SPRITES", vdp_sprite_test);
}
void vdp_menu_run (void)
{
    menu_run (vdp_menu);
}
