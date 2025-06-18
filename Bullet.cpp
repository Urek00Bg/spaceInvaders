#include "Bullet.h"

Bullet::Bullet(int x, int y, int direction, COLORS color, wchar_t symbol)
    : GameObject(x, y, symbol, color), direction(direction), active(true) {
}


void Bullet::update() {
    frameCounter++;


    if (frameCounter >= 2) {
        y -= direction;
        frameCounter = 0;
    }

    if (y < 0 || y >= 20)
        active = false;
}


void Bullet::render() const {
    draw_char(symbol, y, x, color);
}

