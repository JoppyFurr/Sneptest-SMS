/*
 * Sneptest SMS
 *
 * This tool allows testing various features of the Sega Master System.
 * Its purpose is to allow comparisons of how different feauters behave
 * across emulators and Sega hardware.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SMSlib.h"

const uint8_t patterns[] = {

    /* Public-domain 8x8 font from https://github.com/dhepper/font8x8 */

    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* U+0020 (space) */
    0x18, 0x3c, 0x3c, 0x18, 0x18, 0x00, 0x18, 0x00,  /* U+0021 (!) */
    0x6c, 0x6c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,  /* U+0022 (") */
    0x6c, 0x6c, 0xfe, 0x6c, 0xfe, 0x6c, 0x6c, 0x00,  /* U+0023 (#) */
    0x30, 0x7c, 0xc0, 0x78, 0x0c, 0xf8, 0x30, 0x00,  /* U+0024 ($) */
    0x00, 0xc6, 0xcc, 0x18, 0x30, 0x66, 0xc6, 0x00,  /* U+0025 (%) */
    0x38, 0x6c, 0x38, 0x76, 0xdc, 0xcc, 0x76, 0x00,  /* U+0026 (&) */
    0x60, 0x60, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,  /* U+0027 (') */
    0x18, 0x30, 0x60, 0x60, 0x60, 0x30, 0x18, 0x00,  /* U+0028 (() */
    0x60, 0x30, 0x18, 0x18, 0x18, 0x30, 0x60, 0x00,  /* U+0029 ()) */
    0x00, 0x66, 0x3c, 0xff, 0x3c, 0x66, 0x00, 0x00,  /* U+002A (*) */
    0x00, 0x30, 0x30, 0xfc, 0x30, 0x30, 0x00, 0x00,  /* U+002B (+) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x60,  /* U+002C (,) */
    0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00,  /* U+002D (-) */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x00,  /* U+002E (.) */
    0x06, 0x0c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00,  /* U+002F (/) */
    0x7c, 0xc6, 0xce, 0xde, 0xf6, 0xe6, 0x7c, 0x00,  /* U+0030 (0) */
    0x30, 0x70, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00,  /* U+0031 (1) */
    0x78, 0xcc, 0x0c, 0x38, 0x60, 0xcc, 0xfc, 0x00,  /* U+0032 (2) */
    0x78, 0xcc, 0x0c, 0x38, 0x0c, 0xcc, 0x78, 0x00,  /* U+0033 (3) */
    0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x1e, 0x00,  /* U+0034 (4) */
    0xfc, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00,  /* U+0035 (5) */
    0x38, 0x60, 0xc0, 0xf8, 0xcc, 0xcc, 0x78, 0x00,  /* U+0036 (6) */
    0xfc, 0xcc, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00,  /* U+0037 (7) */
    0x78, 0xcc, 0xcc, 0x78, 0xcc, 0xcc, 0x78, 0x00,  /* U+0038 (8) */
    0x78, 0xcc, 0xcc, 0x7c, 0x0c, 0x18, 0x70, 0x00,  /* U+0039 (9) */
    0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x00,  /* U+003A (:) */
    0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30, 0x60,  /* U+003B (;) */
    0x18, 0x30, 0x60, 0xc0, 0x60, 0x30, 0x18, 0x00,  /* U+003C (<) */
    0x00, 0x00, 0xfc, 0x00, 0x00, 0xfc, 0x00, 0x00,  /* U+003D (=) */
    0x60, 0x30, 0x18, 0x0c, 0x18, 0x30, 0x60, 0x00,  /* U+003E (>) */
    0x78, 0xcc, 0x0c, 0x18, 0x30, 0x00, 0x30, 0x00,  /* U+003F (?) */
    0x7c, 0xc6, 0xde, 0xde, 0xde, 0xc0, 0x78, 0x00,  /* U+0040 (@) */
    0x30, 0x78, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0x00,  /* U+0041 (A) */
    0xfc, 0x66, 0x66, 0x7c, 0x66, 0x66, 0xfc, 0x00,  /* U+0042 (B) */
    0x3c, 0x66, 0xc0, 0xc0, 0xc0, 0x66, 0x3c, 0x00,  /* U+0043 (C) */
    0xf8, 0x6c, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00,  /* U+0044 (D) */
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x62, 0xfe, 0x00,  /* U+0045 (E) */
    0xfe, 0x62, 0x68, 0x78, 0x68, 0x60, 0xf0, 0x00,  /* U+0046 (F) */
    0x3c, 0x66, 0xc0, 0xc0, 0xce, 0x66, 0x3e, 0x00,  /* U+0047 (G) */
    0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00,  /* U+0048 (H) */
    0x78, 0x30, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,  /* U+0049 (I) */
    0x1e, 0x0c, 0x0c, 0x0c, 0xcc, 0xcc, 0x78, 0x00,  /* U+004A (J) */
    0xe6, 0x66, 0x6c, 0x78, 0x6c, 0x66, 0xe6, 0x00,  /* U+004B (K) */
    0xf0, 0x60, 0x60, 0x60, 0x62, 0x66, 0xfe, 0x00,  /* U+004C (L) */
    0xc6, 0xee, 0xfe, 0xfe, 0xd6, 0xc6, 0xc6, 0x00,  /* U+004D (M) */
    0xc6, 0xe6, 0xf6, 0xde, 0xce, 0xc6, 0xc6, 0x00,  /* U+004E (N) */
    0x38, 0x6c, 0xc6, 0xc6, 0xc6, 0x6c, 0x38, 0x00,  /* U+004F (O) */
    0xfc, 0x66, 0x66, 0x7c, 0x60, 0x60, 0xf0, 0x00,  /* U+0050 (P) */
    0x78, 0xcc, 0xcc, 0xcc, 0xdc, 0x78, 0x1c, 0x00,  /* U+0051 (Q) */
    0xfc, 0x66, 0x66, 0x7c, 0x6c, 0x66, 0xe6, 0x00,  /* U+0052 (R) */
    0x78, 0xcc, 0xe0, 0x70, 0x1c, 0xcc, 0x78, 0x00,  /* U+0053 (S) */
    0xfc, 0xb4, 0x30, 0x30, 0x30, 0x30, 0x78, 0x00,  /* U+0054 (T) */
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0xfc, 0x00,  /* U+0055 (U) */
    0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x00,  /* U+0056 (V) */
    0xc6, 0xc6, 0xc6, 0xd6, 0xfe, 0xee, 0xc6, 0x00,  /* U+0057 (W) */
    0xc6, 0xc6, 0x6c, 0x38, 0x38, 0x6c, 0xc6, 0x00,  /* U+0058 (X) */
    0xcc, 0xcc, 0xcc, 0x78, 0x30, 0x30, 0x78, 0x00,  /* U+0059 (Y) */
    0xfe, 0xc6, 0x8c, 0x18, 0x32, 0x66, 0xfe, 0x00,  /* U+005A (Z) */

    /* Box drawing */
    0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,  /* Box h-line */
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10,  /* Box v-line */
    0x00, 0x00, 0x00, 0xc0, 0x20, 0x10, 0x10, 0x10,  /* Box tr-corner */
    0x10, 0x10, 0x20, 0xc0, 0x00, 0x00, 0x00, 0x00,  /* Box br-corner */
};

#define BOX_LINE_H    59
#define BOX_LINE_V    60
#define BOX_CORNER_TR 61
#define BOX_CORNER_BR 62


void draw_string (int x, int y, char *string)
{
    uint16_t name_table[32] = { 0 };
    int count = 0;

    while (string[count])
    {
        if      (string[count] >= ' ' && string[count] <= 'Z')
        {
            name_table[count] = string[count] - ' ';
        }
        /* Default to space */
        else
        {
            name_table[count] = 0;
        }

        count++;
    }

    SMS_loadTileMapArea (x, y, name_table, count, 1);
}


void clear_screen (void)
{
    uint16_t name_table[32] = { 0 };
    for (int i = 0; i < 28; i++)
    {
        SMS_loadTileMapArea (0, i, name_table, 32, 1);
    }
}


/* Menu state */

typedef struct menu_item_s {
    uint8_t type;
    char *name;
    void (*func) (void);
    uint16_t value;
    uint16_t value_max;
    void (*value_func) (uint16_t);
} menu_item;

#define MENU_LEN_MAX 8

static char *menu_title = NULL;
static menu_item menu [MENU_LEN_MAX];
static uint8_t menu_len = 0;
static uint8_t menu_cursor = 0;

/*
 * Start a new menu.
 */
void menu_new (char *title)
{
    menu_title = title;
    menu_len = 0;
    menu_cursor = 0;
}

#define MENU_ITEM_FUNCTION  0
#define MENU_ITEM_VALUE     1

/*
 * Add a selectable menu item.
 */
void menu_item_add (char *name, void (*func) (void))
{
    menu [menu_len].type = MENU_ITEM_FUNCTION;
    menu [menu_len].name = name;
    menu [menu_len].func = func;
    menu_len++;
}


void menu_item_add_value (char *name, uint16_t max, void (*func) (uint16_t))
{
    menu [menu_len].type = MENU_ITEM_VALUE;
    menu [menu_len].name = name;
    menu [menu_len].value = 0;
    menu [menu_len].value_max = max;
    menu [menu_len].value_func = func;
    menu_len++;
}


/*
 * Update the VDP with the current menu state.
 */
void menu_update (void)
{
    uint8_t str_buf [32] = "";

    /* Re-draw the cursor */
    for (uint8_t i = 0; i < menu_len; i++)
    {
        draw_string (1, 4 + (2 * i), (i == menu_cursor) ? "->" : "  ");
    }

    /* If the cursor is on a value chooser, redraw the number */
    if (menu [menu_cursor].type == MENU_ITEM_VALUE)
    {
        sprintf (str_buf, "%s: %02X", menu [menu_cursor].name, menu [menu_cursor].value);
        draw_string (4, 4 + (2 * menu_cursor), str_buf);
    }
}


/*
 * Initial draw of the menu to the VDP.
 */
void menu_draw (void)
{
    uint8_t title_len;
    uint8_t str_buf [32] = "";
    uint16_t name_table [32];
    uint8_t i;

    clear_screen ();

    /* Title */
    title_len = sprintf (str_buf, "%s", menu_title);
    draw_string (1, 1, str_buf);

    /* Bottom-rule */
    for (i = 0; i < 32; i++)
    {
        name_table [i] = BOX_LINE_H;
    }
    SMS_loadTileMapArea (0, 22, name_table, 32, 1);

    /* Title border */
    name_table [title_len + 2] = BOX_CORNER_TR;
    SMS_loadTileMapArea (0, 0, name_table, title_len + 3, 1);

    name_table [title_len + 2] = BOX_LINE_V;
    SMS_loadTileMapArea (title_len + 2, 1, &name_table [title_len + 2], 1, 1);

    name_table [title_len + 2] = BOX_CORNER_BR;
    SMS_loadTileMapArea (0, 2, name_table, title_len + 3, 1);

    /* Menu items */
    for (i = 0; i < menu_len; i++)
    {
        if (menu [i].type == MENU_ITEM_FUNCTION)
        {
            draw_string (4, 4 + (2 * i), menu [i].name);
        }
        else if (menu [i].type == MENU_ITEM_VALUE)
        {
            sprintf (str_buf, "%s: %02X", menu [i].name, menu [i].value);
            draw_string (4, 4 + (2 * i), str_buf);
        }
    }

    /* Reference */
    draw_string (0, 23, "      1: SELECT     2: BACK     ");

    menu_update ();
}


/*
 * Run a menu.
 */
void menu_run (void (*menu_func) (void))
{
    unsigned int pressed;
    uint8_t cursor_store;

    menu_func ();
    SMS_waitForVBlank ();
    menu_draw ();

    while (true)
    {
        SMS_waitForVBlank ();
        pressed = SMS_getKeysPressed ();

        if (pressed & PORT_A_KEY_UP)
        {
            menu_cursor--;
            if (menu_cursor > menu_len)
            {
                menu_cursor = 0;
            }
        }
        else if (pressed & PORT_A_KEY_DOWN)
        {
            menu_cursor++;
            if (menu_cursor >= menu_len)
            {
                menu_cursor = menu_len - 1;
            }
        }
        else if (menu [menu_cursor].type == MENU_ITEM_VALUE && pressed & PORT_A_KEY_LEFT)
        {
            menu [menu_cursor].value--;
            if (menu [menu_cursor].value > menu [menu_cursor].value_max)
            {
                menu [menu_cursor].value = 0;
            }
            if (menu [menu_cursor].value_func)
            {
                menu [menu_cursor].value_func (menu [menu_cursor].value);
            }
        }
        else if (menu [menu_cursor].type == MENU_ITEM_VALUE && pressed & PORT_A_KEY_RIGHT)
        {
            menu [menu_cursor].value++;
            if (menu [menu_cursor].value > menu [menu_cursor].value_max)
            {
                menu [menu_cursor].value = menu [menu_cursor].value_max;
            }
            if (menu [menu_cursor].value_func)
            {
                menu [menu_cursor].value_func (menu [menu_cursor].value);
            }
        }
        else if (pressed & PORT_A_KEY_1)
        {
            if (menu [menu_cursor].type == MENU_ITEM_FUNCTION && menu [menu_cursor].func)
            {
                cursor_store = menu_cursor;
                menu [menu_cursor].func ();
                menu_func ();
                menu_draw ();
                menu_cursor = cursor_store;
            }
        }
        else if (pressed & PORT_A_KEY_2)
        {
            return;
        }
        menu_update ();
    }
}


void draw_string_priority (int x, int y, char *string)
{
    uint16_t name_table[32] = { 0 };
    int count = 0;

    while (string[count])
    {
        if      (string[count] >= ' ' && string[count] <= 'Z')
        {
            name_table[count] =  0x1000 | (string[count] - ' ');
        }
        /* Default to space */
        else
        {
            name_table[count] = 0;
        }

        count++;
    }

    SMS_loadTileMapArea (x, y, name_table, count, 1);
}


/*
 * TODO: Remove when remaining code is converted to new menu framework.
 */
void uint8_to_string (char *string, uint8_t value)
{
    /* Most-significant nibble */
    if (value < 0xa0)
        string[0] = '0' + (value >> 4);
    else
        string[0] = 'A' + (value >> 4) - 10;

    /* Least-significant nibble */
    if ((value & 0x0f) < 0x0a)
        string[1] = '0' + (value & 0x0f);
    else
        string[1] = 'A' + (value & 0x0f) - 10;
}


/*
 * Test for VDP scrolling behaviour.
 */
void scroll_test (void)
{
    char string_buf[3] = { '\0' };
    unsigned int scroll_x = 0;
    unsigned int scroll_y = 0;

    clear_screen ();
    draw_string (7, 23, "1: SLOW, 2: BACK");

    draw_string (10, 12,  "SCROLL X:");
    draw_string (10, 14,  "SCROLL Y:");

    while (true)
    {
        unsigned int pressed;
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
        if (pressed & PORT_A_KEY_1)
            pressed = SMS_getKeysPressed ();

        if (pressed & PORT_A_KEY_UP)    scroll_y++;
        if (pressed & PORT_A_KEY_DOWN)  scroll_y--;
        if (pressed & PORT_A_KEY_LEFT)  scroll_x--;
        if (pressed & PORT_A_KEY_RIGHT) scroll_x++;
        if (pressed & PORT_A_KEY_2)     break;
    }

    SMS_setBGScrollX (0);
    SMS_setBGScrollY (0);
}


/*
 * Test for SMS gamepad behaviour.
 */
void input_test (void)
{
    char string_buf[3] = { '\0' };
    unsigned int scroll_x = 0;
    unsigned int scroll_y = 0;
    uint8_t line;

    clear_screen ();
    draw_string (10, 23, "1 + 2: BACK");

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

uint8_t line_interrupt_count = 0;

void vdp_interrupt_test_handler (void)
{
    line_interrupt_count++;
}


/*
 * Test for VDP line-interrupt behaviour.
 */
void vdp_interrupt_test (void)
{
    char string_buf[3] = { '\0' };
    unsigned int counter_reload = 128;
    unsigned int count_last_frame = 0;

    /* Configure the interrupt */
    SMS_setLineInterruptHandler (vdp_interrupt_test_handler);
    SMS_enableLineInterrupt();

    clear_screen ();
    draw_string (7, 23, "1: SLOW, 2: BACK");

    draw_string (10, 12,  "RELOAD:");
    draw_string (10, 14,  "SEEN:");

    while (true)
    {
        unsigned int pressed;
        SMS_waitForVBlank ();

        /* Update and reset counter */
        count_last_frame = line_interrupt_count;
        line_interrupt_count = 0;

        /* Render during vblank */
        uint8_to_string (string_buf, counter_reload);
        draw_string (20, 12, string_buf);
        uint8_to_string (string_buf, count_last_frame);
        draw_string (20, 14, string_buf);

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
    draw_string (7, 23, "1: SLOW, 2: BACK");

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
void vdp_background (void)
{
    menu_run (vdp_background_menu);
    SMS_setBackdropColor (0);
    SMS_displayOn ();
}


/*
 * Main menu, shown to the user at startup.
 */
void main_menu (void)
{
    menu_new ("SNEPTEST SMS");
    menu_item_add ("INPUT", input_test);
    menu_item_add ("VDP SCROLLING", scroll_test);
    menu_item_add ("VDP INTERRUPTS", vdp_interrupt_test);
    menu_item_add ("VDP SPRITES", vdp_sprite_test);
    menu_item_add ("VDP BACKGROUND", vdp_background);
}
void main (void)
{
    /* Initial setup */
    SMS_setBackdropColor (0);
    SMS_setSpritePaletteColor (0, 0x01);    /* Sprite 0: Dark red (backdrop) */
    SMS_setSpritePaletteColor (1, 0x0f);    /* Sprite 1: Yellow (text as sprite) */
    SMS_setBGPaletteColor (0, 0x01);        /* Background 0: Dark red */
    SMS_setBGPaletteColor (1, 0x3f);        /* Background 1: White (text) */

    SMS_load1bppTiles (patterns, 0, sizeof (patterns), 0, 1);

    SMS_displayOn ();

    while (true)
    {
        menu_run (main_menu);
    }
}
