#pragma once
#include "GameObject.h"

class Enemy : public GameObject {
protected:
    bool alive;
    wchar_t altSymbol = ' ';
    mutable int animCounter = 0;

public:
    Enemy(int x = 0, int y = 0, wchar_t symbol = 'V', COLORS color = RED);
    virtual int getScoreValue() const = 0;
    virtual void update() override {}
    bool isAlive() const { return alive; }
    void kill() { alive = false; }
    void moveX(int dx) { x += dx; }
    void moveY(int dy) { y += dy; }
    void setAltSymbol(wchar_t alt) { altSymbol = alt; }

    void render() const override;
};

class EnemyType1 : public Enemy {
public:
    EnemyType1(int x, int y) : Enemy(x, y, L'W', LIGHT_GREEN) {}
    int getScoreValue() const override { return 10; }
};

class EnemyType2 : public Enemy {
private:
    int moveTimer = 0;
    int moveDir = 1;
public:
    EnemyType2(int x, int y);
    int getScoreValue() const override;
    void update() override;
};


class EnemyType3 : public Enemy {
private:
    int tick = 0;
public:
    EnemyType3(int x, int y);
    int getScoreValue() const override;
    void update() override;
};


class EnemyType4 : public Enemy {
public:
    EnemyType4(int x, int y) : Enemy(x, y, L'Z', YELLOW) {}
    int getScoreValue() const override { return 40; }
};