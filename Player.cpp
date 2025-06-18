#include "Player.h"

Player::Player(int x, int y, int lives, int score)
    : GameObject(x, y, 'A', LIGHT_GREEN), lives(lives), score(score) {}

Player::Player(const Player& other)
    : GameObject(other.x, other.y, other.symbol, other.color),
      lives(other.lives), score(other.score) {}

Player::Player(Player&& other) noexcept
    : GameObject(other.x, other.y, other.symbol, other.color),
      lives(other.lives), score(other.score) {
    other.lives = 0;
    other.score = 0;
}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        x = other.x; y = other.y; symbol = other.symbol; color = other.color;
        lives = other.lives; score = other.score;
    }
    return *this;
}

Player& Player::operator=(Player&& other) noexcept {
    if (this != &other) {
        x = other.x; y = other.y; symbol = other.symbol; color = other.color;
        lives = other.lives; score = other.score;
        other.lives = 0; other.score = 0;
    }
    return *this;
}

Player Player::operator+(int points) const {
    return Player(x, y, lives, score + points);
}

Player Player::operator-(int points) const {
    return Player(x, y, lives, score - points);
}

std::ostream& operator<<(std::ostream& out, const Player& p) {
    out << "Player(Lives: " << p.lives << ", Score: " << p.score << ")";
    return out;
}

std::istream& operator>>(std::istream& in, Player& p) {
    in >> p.lives >> p.score;
    return in;
}

void Player::moveLeft() { if (x > 0) x--; }
void Player::moveRight() { if (x < 40 - 1) x++; }