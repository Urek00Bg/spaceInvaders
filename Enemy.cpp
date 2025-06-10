#include "Enemy.h"

Enemy::Enemy(int x, int y, char symbol, COLORS color)
    : GameObject(x, y, symbol, color), alive(true) {}