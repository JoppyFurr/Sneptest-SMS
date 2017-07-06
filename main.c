#include <stdbool.h>
#include <stdint.h>
#include "SMSlib.h"


void main (void)
{
    unsigned char patterns[] = { 0x00, 0x00, 0x00, 0x00, /* White square */
                                 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00,

                                 0xff, 0x00, 0x00, 0x00, /* Black square */
                                 0xff, 0x00, 0x00, 0x00,
                                 0xff, 0x00, 0x00, 0x00,
                                 0xff, 0x00, 0x00, 0x00,
                                 0xff, 0x00, 0x00, 0x00,
                                 0xff, 0x00, 0x00, 0x00,
                                 0xff, 0x00, 0x00, 0x00,
                                 0xff, 0x00, 0x00, 0x00,
    };

    uint16_t name_table[] = {  0x0000, 0x0001, 0x0000, 0x0001,
                               0x0001, 0x0000, 0x0001, 0x0000,
                               0x0000, 0x0001, 0x0000, 0x0001,
                               0x0001, 0x0000, 0x0001, 0x0000,
    };


    SMS_setBGPaletteColor (0, 0x3f); /* White at index 0 */
    SMS_setBGPaletteColor (1, 0x00); /* Black at index 1 */
    SMS_setBackdropColor (0);        /* Backdrop is white */
    SMS_displayOn ();

    SMS_loadTiles (patterns, 0, sizeof (patterns));
    SMS_loadTileMapArea (10, 10, name_table, 4, 4);

    while (true)
    {
        SMS_waitForVBlank ();
    }
}
