#include "Bullet.h"

Bullet::Bullet(int x, int y, int direction, COLORS color)
    : GameObject(x, y, '|', color), direction(direction), active(true) {}

void Bullet::update() {
    y -= direction;
    if (y < 0 || y >= 20)
        active = false;
}