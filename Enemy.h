#pragma once
#include "GameObject.h"

class Enemy : public GameObject {
protected:
    bool alive;
public:
    Enemy(int x = 0, int y = 0, char symbol = 'V', COLORS color = RED);
    virtual int getScoreValue() const = 0;
    void update() override {}
    bool isAlive() const { return alive; }
    void kill() { alive = false; }
    void moveX(int dx) { x += dx; }
    void moveY(int dy) { y += dy; }
};

class EnemyType1 : public Enemy {
public:
    EnemyType1(int x, int y) : Enemy(x, y, 'W', LIGHT_GREEN) {}
    int getScoreValue() const override { return 10; }
};

class EnemyType2 : public Enemy {
public:
    EnemyType2(int x, int y) : Enemy(x, y, 'M', LIGHT_CYAN) {}
    int getScoreValue() const override { return 20; }
};

class EnemyType3 : public Enemy {
public:
    EnemyType3(int x, int y) : Enemy(x, y, 'X', PURPLE) {}
    int getScoreValue() const override { return 30; }
};

class EnemyType4 : public Enemy {
public:
    EnemyType4(int x, int y) : Enemy(x, y, 'Z', YELLOW) {}
    int getScoreValue() const override { return 40; }
};