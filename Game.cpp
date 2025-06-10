#include "Game.h"
#include <iostream>
#include <thread>
#include <windows.h>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <chrono>


using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

Game::Game()
    : player(WIDTH / 2, HEIGHT - 2), level(1), running(true),
      enemySpeed(30), enemyShootChance(10) {}

void Game::initEnemies() {
    enemies.clear();
    for (int i = 0; i < 8; i++) {
        int enemyType = rand() % 4;
        shared_ptr<Enemy> e;
        switch (enemyType) {
            case 0: e = make_shared<EnemyType1>(3 + i * 4, 2); break;
            case 1: e = make_shared<EnemyType2>(3 + i * 4, 4); break;
            case 2: e = make_shared<EnemyType3>(3 + i * 4, 6); break;
            case 3: e = make_shared<EnemyType4>(3 + i * 4, 8); break;
        }
        enemies.push_back(e);
    }
}

void Game::updateLevel() {
    if (player.getScore() >= 500)
        level = 3;
    else if (player.getScore() >= 200)
        level = 2;
    else
        level = 1;

    if (level == 1) {
        enemySpeed = 30;
        enemyShootChance = 10;
    } else if (level == 2) {
        enemySpeed = 20;
        enemyShootChance = 20;
        if (player.getScore() >= 300 && player.getLives() < 5)
            player.gainLife();
    } else if (level == 3) {
        enemySpeed = 10;
        enemyShootChance = 40;
    }
}

void Game::input() {
    if (GetAsyncKeyState('A') & 0x8000) player.moveLeft();
    if (GetAsyncKeyState('D') & 0x8000) player.moveRight();
    if (GetAsyncKeyState(' ') & 0x8000) bullets.emplace_back(player.getX(), player.getY() - 1, 1);
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;
}

void Game::updateEnemies() {
    static int counter = 0;
    counter++;
    if (counter < enemySpeed) return;
    counter = 0;

    bool reverse = false;
    for (auto& e : enemies) {
        if (!e->isAlive()) continue;
        e->moveX(1);
        if (e->getX() >= WIDTH - 1) reverse = true;
    }

    if (reverse) {
        for (auto& e : enemies) {
            if (!e->isAlive()) continue;
            e->moveX(-1);
            e->moveY(1);
        }
    }

    for (auto& e : enemies) {
        if (!e->isAlive()) continue;
        int chance = rand() % 100;
        if (chance < enemyShootChance)
            enemyBullets.emplace_back(e->getX(), e->getY() + 1, -1, RED);
    }
}

void Game::updateBullets() {
    for (auto& b : bullets) b.update();
    bullets.remove_if([](const Bullet& b) { return !b.isActive(); });

    for (auto& b : enemyBullets) b.update();
    enemyBullets.remove_if([](const Bullet& b) { return !b.isActive(); });
}

void Game::checkCollisions() {
    for (auto& b : bullets) {
        for (auto& e : enemies) {
            if (!e->isAlive()) continue;
            if (b.getX() == e->getX() && b.getY() == e->getY()) {
                e->kill();
                b.deactivate();
                player.addScore(e->getScoreValue());
            }
        }
    }

    for (auto& b : enemyBullets) {
        if (b.getX() == player.getX() && b.getY() == player.getY()) {
            b.deactivate();
            player.loseLife();
            if (player.getLives() <= 0)
                running = false;
        }
    }
}

void Game::render() {
    system("cls");
    cout << "Score: " << player.getScore() << "  Lives: " << player.getLives() << "  Level: " << level << endl;

    for (auto& e : enemies) if (e->isAlive()) e->render();
    for (auto& b : bullets) if (b.isActive()) b.render();
    for (auto& b : enemyBullets) if (b.isActive()) b.render();
    player.render();
}

void Game::run() {
    srand(static_cast<unsigned int>(time(0)));
    initEnemies();

    while (running) {
        input();
        updateLevel();
        updateEnemies();
        updateBullets();
        checkCollisions();
        render();

        bool allDead = all_of(enemies.begin(), enemies.end(),
                               [](shared_ptr<Enemy> e) { return !e->isAlive(); });

        if (allDead) {
            if (level == 3)
                running = false;
            else
                initEnemies();
        }

        this_thread::sleep_for(std::chrono::milliseconds(50));
        
    }

    system("cls");
    if (player.getLives() <= 0)
        cout << "GAME OVER!" << endl;
    else
        cout << "YOU WON!" << endl;

    system("pause");
}