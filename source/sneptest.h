
#define REPEAT_RATE 20

void clear_screen (void);
void draw_string (int x, int y, char *string);
void reference_draw (char *text);
void title_draw (char *title);

/* Menu API */
void menu_new (char *title);
void menu_item_add (char *name, void (*func) (void));
void menu_item_add_value (char *name, uint16_t max, void (*func) (uint16_t));
void menu_run (void (*menu_func) (void));
