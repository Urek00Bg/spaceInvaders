#include "Enemy.h"

Enemy::Enemy(int x, int y, wchar_t symbol, COLORS color)
    : GameObject(x, y, symbol, color), alive(true) {}

void Enemy::render() const {
    animCounter++;
    wchar_t s = (animCounter % 10 < 5) ? symbol : altSymbol;
    draw_char(s, y, x, color);
}


EnemyType2::EnemyType2(int x, int y)
    : Enemy(x, y, L'M', LIGHT_CYAN) {
}

int EnemyType2::getScoreValue() const { return 20; }

void EnemyType2::update() {
    moveTimer++;
    if (moveTimer % 8 == 0) {
        x += moveDir;
        if (x <= 1 || x >= 78) moveDir *= -1;
    }
}

EnemyType3::EnemyType3(int x, int y)
    : Enemy(x, y, L'X', PURPLE) {
}

int EnemyType3::getScoreValue() const { return 30; }

void EnemyType3::update() {
    tick++;
    if (tick % 4 == 0) {
        y++;
    }
}



