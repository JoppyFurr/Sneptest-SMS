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

#include "sneptest.h"
#include "input_tests.h"
#include "vdp_tests.h"

SMS_EMBED_SEGA_ROM_HEADER (9999, 0);

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


/*
 * Fully clear the name table.
 */
void clear_name_table (void)
{
    uint16_t name_table[32] = { 0 };
    for (int i = 0; i < 28; i++)
    {
        SMS_loadTileMapArea (0, i, name_table, 32, 1);
    }
}


/*
 * Clear the active area of the menu.
 * (area above the help-text rule)
 */
void clear_screen (void)
{
    uint16_t name_table[32] = { 0 };
    for (int i = 0; i < 22; i++)
    {
        SMS_loadTileMapArea (0, i, name_table, 32, 1);
    }
}


/* Menu state */
typedef struct menu_item_s {
    uint8_t type;
    char *name;
    uint16_t value;
    uint16_t value_max;
    union {
        void (*func) (void);
        void (*value_func) (uint16_t);
        uint16_t (*show_func) (void);
    };
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
#define MENU_ITEM_SHOW_UINT 2

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


void menu_item_add_value (char *name, uint16_t start, uint16_t max, void (*func) (uint16_t))
{
    menu [menu_len].type = MENU_ITEM_VALUE;
    menu [menu_len].name = name;
    menu [menu_len].value = start;
    menu [menu_len].value_max = max;
    menu [menu_len].value_func = func;
    menu_len++;
}


void menu_item_add_show_uint (char *name, uint16_t (*func) (void))
{
    menu [menu_len].type = MENU_ITEM_SHOW_UINT;
    menu [menu_len].name = name;
    menu [menu_len].show_func = func;
    menu_len++;
}


/*
 * Update the VDP with the current menu state.
 */
void menu_update (bool cursor_change)
{
    uint8_t len;
    uint8_t str_buf [32];

    if (cursor_change)
    {
        /* Redraw the cursor */
        for (uint8_t i = 0; i < menu_len; i++)
        {
            draw_string (1, 4 + (2 * i), (i == menu_cursor) ? "->" : "  ");
        }

        /* Update the reference text */
        if (menu [menu_cursor].type == MENU_ITEM_VALUE)
        {
            reference_draw ("   1: FAST-SCROLL     2: BACK   ");
        }
        else
        {
            reference_draw ("     1: SELECT      2: BACK     ");
        }
    }

    /* If the cursor is on a value chooser, redraw the number */
    if (menu [menu_cursor].type == MENU_ITEM_VALUE)
    {
        len = strlen (menu [menu_cursor].name);
        sprintf (str_buf, "%02X", menu [menu_cursor].value);
        draw_string (4 + len + 2, 4 + (2 * menu_cursor), str_buf);
    }

    /* Redraw any shown values */
    for (uint8_t i = 0; i < menu_len; i++)
    {
        if (menu [i].type == MENU_ITEM_SHOW_UINT)
        {
            /* Trailing spaces to clear previous value */
            sprintf (str_buf, "%d  ", menu [i].show_func ());
            len = strlen (menu [i].name);
            draw_string (4 + len + 2, 4 + 2 * i, str_buf);
        }
    }
}


/*
 * Draw a bordered title for the current screen.
 */
void title_draw (char *title)
{
    uint8_t title_len;
    uint8_t str_buf [32] = "";
    uint16_t name_table [32];

    title_len = sprintf (str_buf, "%s", title);
    draw_string (1, 1, str_buf);

    for (uint8_t i = 0; i < title_len + 2; i++)
    {
        name_table [i] = BOX_LINE_H;
    }

    /* Border */
    name_table [title_len + 2] = BOX_CORNER_TR;
    SMS_loadTileMapArea (0, 0, name_table, title_len + 3, 1);

    name_table [title_len + 2] = BOX_LINE_V;
    SMS_loadTileMapArea (title_len + 2, 1, &name_table [title_len + 2], 1, 1);

    name_table [title_len + 2] = BOX_CORNER_BR;
    SMS_loadTileMapArea (0, 2, name_table, title_len + 3, 1);
}


/*
 * Draw a line of help text below a rule at the bottom of the screen.
 */
void reference_draw (char *text)
{
    uint16_t name_table [32];

    /* Bottom-rule */
    for (uint8_t i = 0; i < 32; i++)
    {
        name_table [i] = BOX_LINE_H;
    }
    SMS_loadTileMapArea (0, 22, name_table, 32, 1);

    /* Text */
    draw_string (0, 23, text);
}


/*
 * Initial draw of the menu to the VDP.
 */
void menu_draw (void)
{
    uint8_t str_buf [32] = "";

    clear_screen ();
    title_draw (menu_title);

    /* Menu items */
    for (uint8_t i = 0; i < menu_len; i++)
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
        else if (menu [i].type == MENU_ITEM_SHOW_UINT)
        {
            sprintf (str_buf, "%s:", menu [i].name);
            draw_string (4, 4 + (2 * i), str_buf);
        }
    }

    menu_update (true);
}


/*
 * Run a menu.
 */
void menu_run (void (*menu_func) (void))
{
    uint16_t keys_pressed = 0;
    uint16_t keys_status = 0;
    uint8_t repeat = 0;
    uint8_t cursor_store;

    menu_func ();
    menu_draw ();

    while (true)
    {
        bool cursor_change = false;

        SMS_waitForVBlank ();
        keys_pressed = SMS_getKeysPressed ();
        keys_status = SMS_getKeysStatus ();

        /* Common menu controls: Up, Down, & Back */
        if (keys_pressed & PORT_A_KEY_UP)
        {
            menu_cursor--;
            if (menu_cursor > menu_len)
            {
                menu_cursor = 0;
            }
            else
            {
                cursor_change = true;
            }
        }
        else if (keys_pressed & PORT_A_KEY_DOWN)
        {
            menu_cursor++;
            if (menu_cursor >= menu_len)
            {
                menu_cursor = menu_len - 1;
            }
            {
                cursor_change = true;
            }
        }
        else if (keys_pressed & PORT_A_KEY_2)
        {
            return;
        }

        /* Function items */
        else if (menu [menu_cursor].type == MENU_ITEM_FUNCTION)
        {
            if ((keys_pressed & PORT_A_KEY_1) && menu [menu_cursor].func)
            {
                cursor_store = menu_cursor;
                menu [menu_cursor].func ();
                menu_func ();
                menu_draw ();
                menu_cursor = cursor_store;
            }
        }

        /* Value items */
        else if (menu [menu_cursor].type == MENU_ITEM_VALUE)
        {
            bool value_change = false;

            /* Change the value if one of the following are true:
             * - Left/Right were just pressed
             * - The repeat-rate timer has elapsed
             * - Button '1' is being held (fast-scroll)
             */
            if ((keys_pressed & (PORT_A_KEY_LEFT | PORT_A_KEY_RIGHT)) ||
                (repeat >= REPEAT_RATE) ||
                (keys_status & PORT_A_KEY_1))
            {
                value_change = true;
                repeat = 0;
            }
            repeat++;

            if (value_change)
            {
                if (keys_status & PORT_A_KEY_LEFT)
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
                else if (keys_status & PORT_A_KEY_RIGHT)
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
            }
        }

        menu_update (cursor_change);
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
 * Main menu, shown to the user at startup.
 */
void main_menu (void)
{
    menu_new ("SNEPTEST SMS");
    menu_item_add ("INPUT TESTS", input_menu_run);
    menu_item_add ("VDP TESTS", vdp_menu_run);
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
    clear_name_table ();

    SMS_waitForVBlank ();
    SMS_displayOn ();

    while (true)
    {
        menu_run (main_menu);
    }
}
