#define VIDEO_MEMORY 0xB8000
#define COL_SIZE 80
#define ROW_SIZE 25
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5
#define DEFAULT_STR_COLOR 0x07

void print_at(char* str, int col, int row);
void print_cursor(char* str);
void clear_row();
void clear_screen();


