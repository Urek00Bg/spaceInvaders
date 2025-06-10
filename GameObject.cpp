#include "GameObject.h"
#include <windows.h>

HANDLE hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);

void draw_char(char ch, int y, int x, COLORS foreground_color, COLORS background_color) {
    CHAR_INFO ch_info;
    ch_info.Char.AsciiChar = ch;
    ch_info.Attributes = foreground_color | (background_color << 4);

    COORD buf_size = {1, 1};
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = { (SHORT)x, (SHORT)y, (SHORT)(x+1), (SHORT)(y+1) };
    ::WriteConsoleOutput(hConsoleOutput, &ch_info, buf_size, buf_coord, &screen_pos);
}

GameObject::GameObject(int x, int y, char symbol, COLORS color)
    : x(x), y(y), symbol(symbol), color(color) {}

void GameObject::render() {
    draw_char(symbol, y, x, color);
}