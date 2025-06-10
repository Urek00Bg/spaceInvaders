#pragma once

#include <windows.h>

enum COLORS {
    BLACK = 0,
    BLUE = FOREGROUND_BLUE,
    CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
    GREEN = FOREGROUND_GREEN,
    RED = FOREGROUND_RED,
    BROWN = FOREGROUND_RED | FOREGROUND_GREEN,
    PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
    LIGHT_GREY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GREY = FOREGROUND_INTENSITY,
    LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHT_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
    YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    PINK = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

extern HANDLE hConsoleOutput;

void draw_char(char ch, int y, int x, COLORS foreground_color, COLORS background_color = BLACK);

class GameObject {
protected:
    int x, y;
    char symbol;
    COLORS color;
public:
    GameObject(int x = 0, int y = 0, char symbol = ' ', COLORS color = WHITE);
    virtual ~GameObject() {}
    virtual void update() = 0;
    void render();
    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }
};