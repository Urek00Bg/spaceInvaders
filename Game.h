#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <vector>
#include <list>
#include <memory>

class Game {
private:
    Player player;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::list<Bullet> bullets;
    std::list<Bullet> enemyBullets;
    int level;
    bool running;
    int enemySpeed;
    int enemyShootChance;
    int bulletCooldown = 0;
    const int bulletCooldownMax = 10;


    void initEnemies();
    void updateLevel();
    void input();
    void updateEnemies();
    void updateBullets();
    void checkCollisions();
    void render();

public:
    Game();
    void run();
};