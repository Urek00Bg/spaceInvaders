#pragma once
#include "GameObject.h"
#include <iostream>

class Player : public GameObject {
private:
    int lives;
    int score;
public:
    Player(int x = 0, int y = 0, int lives = 3, int score = 0);
    Player(const Player& other);
    Player(Player&& other) noexcept;
    Player& operator=(const Player& other);
    Player& operator=(Player&& other) noexcept;
    ~Player() override {}

    Player operator+(int points) const;
    Player operator-(int points) const;

    friend std::ostream& operator<<(std::ostream& out, const Player& p);
    friend std::istream& operator>>(std::istream& in, Player& p);

    void moveLeft();
    void moveRight();
    void update() override {}

    int getLives() const { return lives; }
    int getScore() const { return score; }
    void addScore(int s) { score += s; }
    void loseLife() { lives--; }
    void gainLife() { lives++; }
};