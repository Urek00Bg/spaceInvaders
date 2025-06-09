#include <iostream>
#include <vector>
#include <list>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <algorithm>
#include <thread>

using namespace std;

const int WIDTH = 40;
const int HEIGHT = 20;

enum COLORS {
    BLACK = 0,
    BLUE = FOREGROUND_BLUE,
    CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
    GREEN = FOREGROUND_GREEN,
    RED = FOREGROUND_RED,
    BROWN = FOREGROUND_RED | FOREGROUND_GREEN,
    PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
    LIGHT_GREY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    GREY = FOREGROUND_INTENSITY,
    LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHT_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
    YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    PINK = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    WHITE = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY
};

HANDLE hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);

void draw_char(char ch, int y, int x, COLORS foreground_color, COLORS background_color = BLACK) {
    CHAR_INFO ch_info;
    ch_info.Char.AsciiChar = ch;
    ch_info.Attributes = foreground_color | (background_color << 4);

    COORD buf_size = {1, 1};
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = {(SHORT)x, (SHORT)y, (SHORT)(x+1), (SHORT)(y+1)};
    ::WriteConsoleOutput(hConsoleOutput, &ch_info, buf_size, buf_coord, &screen_pos);
}

class GameObject {
protected:
    int x, y;
    char symbol;
    COLORS color;
public:
    GameObject(int x = 0, int y = 0, char symbol = ' ', COLORS color = WHITE)
        : x(x), y(y), symbol(symbol), color(color) {}

    virtual ~GameObject() {}

    virtual void update() = 0;

    void render() {
        draw_char(symbol, y, x, color);
    }

    int getX() const { return x; }
    int getY() const { return y; }
    char getSymbol() const { return symbol; }
};

class Player : public GameObject {
private:
    int lives;
    int score;
public:
    Player(int x = 0, int y = 0, int lives = 3, int score = 0)
        : GameObject(x, y, 'A', LIGHT_GREEN), lives(lives), score(score) {}

    Player(const Player& other)
        : GameObject(other.x, other.y, other.symbol, other.color), lives(other.lives), score(other.score) {}

    Player(Player&& other) noexcept
        : GameObject(other.x, other.y, other.symbol, other.color), lives(other.lives), score(other.score) {
        other.lives = 0; other.score = 0;
    }

    Player& operator=(const Player& other) {
        if (this != &other) {
            x = other.x; y = other.y; symbol = other.symbol; color = other.color;
            lives = other.lives; score = other.score;
        }
        return *this;
    }

    Player& operator=(Player&& other) noexcept {
        if (this != &other) {
            x = other.x; y = other.y; symbol = other.symbol; color = other.color;
            lives = other.lives; score = other.score;
            other.lives = 0; other.score = 0;
        }
        return *this;
    }

    ~Player() override {}

    Player operator+(int points) const {
        return Player(x, y, lives, score + points);
    }

    Player operator-(int points) const {
        return Player(x, y, lives, score - points);
    }

    friend ostream& operator<<(ostream& out, const Player& p) {
        out << "Player(Lives: " << p.lives << ", Score: " << p.score << ")";
        return out;
    }

    friend istream& operator>>(istream& in, Player& p) {
        in >> p.lives >> p.score;
        return in;
    }

    void moveLeft() { if (x > 0) x--; }
    void moveRight() { if (x < WIDTH - 1) x++; }
    void update() override {}

    int getLives() const { return lives; }
    int getScore() const { return score; }
    void addScore(int s) { score += s; }
    void loseLife() { lives--; }
    void gainLife() { lives++; }
};

class Bullet : public GameObject {
private:
    int direction;
    bool active;
public:
    Bullet(int x = 0, int y = 0, int direction = 1, COLORS color = WHITE)
        : GameObject(x, y, '|', color), direction(direction), active(true) {}

    void update() override {
        y -= direction;
        if (y < 0 || y >= HEIGHT)
            active = false;
    }
    bool isActive() const { return active; }
    void deactivate() { active = false; }
};

class Enemy : public GameObject {
protected:
    bool alive;
public:
    Enemy(int x = 0, int y = 0, char symbol = 'V', COLORS color = RED)
        : GameObject(x, y, symbol, color), alive(true) {}

    void update() override {}

    bool isAlive() const { return alive; }
    void kill() { alive = false; }
    void moveX(int dx) { x += dx; }
    void moveY(int dy) { y += dy; }
    virtual int getScoreValue() const = 0;
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

class Game {
private:
    Player player;
    vector<shared_ptr<Enemy>> enemies;
    list<Bullet> bullets;
    list<Bullet> enemyBullets;
    int level;
    bool running;
    int enemySpeed;
    int enemyShootChance;

public:
    Game() : player(WIDTH / 2, HEIGHT - 2), level(1), running(true), enemySpeed(30), enemyShootChance(10) {}

    void initEnemies() {
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

    void updateLevel() {
        if (player.getScore() >= 500)
            level = 3;
        else if (player.getScore() >= 200)
            level = 2;
        else
            level = 1;

        if (level == 1) {
            enemySpeed = 30;
            enemyShootChance = 10;
        }
        else if (level == 2) {
            enemySpeed = 20;
            enemyShootChance = 20;
            if (player.getScore() >= 300 && player.getLives() < 5)
                player.gainLife();
        }
        else if (level == 3) {
            enemySpeed = 10;
            enemyShootChance = 40;
        }
    }

    void input() {
        if (GetAsyncKeyState('A') & 0x8000) player.moveLeft();
        if (GetAsyncKeyState('D') & 0x8000) player.moveRight();
        if (GetAsyncKeyState(' ') & 0x8000) bullets.emplace_back(player.getX(), player.getY() - 1, 1);
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) running = false;
    }

    void updateEnemies() {
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

    void updateBullets() {
        for (auto& b : bullets) b.update();
        bullets.remove_if([](const Bullet& b) { return !b.isActive(); });

        for (auto& b : enemyBullets) b.update();
        enemyBullets.remove_if([](const Bullet& b) { return !b.isActive(); });
    }

    void checkCollisions() {
        for (auto& b : bullets) {
            for (auto& e : enemies) {
                if (!e->isAlive()) continue;
                if (abs(b.getX() - e->getX()) <= 0 && abs(b.getY() - e->getY()) <= 0) {
                    e->kill();
                    b.deactivate();
                    player.addScore(e->getScoreValue());
                }
            }
        }

        for (auto& b : enemyBullets) {
            if (abs(b.getX() - player.getX()) <= 0 && abs(b.getY() - player.getY()) <= 0) {
                b.deactivate();
                player.loseLife();
                if (player.getLives() <= 0)
                    running = false;
            }
        }
    }

    void render() {
        system("cls");
        cout << "Score: " << player.getScore() << "  Lives: " << player.getLives() << "  Level: " << level << endl;

        for (auto& e : enemies) if (e->isAlive()) e->render();
        for (auto& b : bullets) if (b.isActive()) b.render();
        for (auto& b : enemyBullets) if (b.isActive()) b.render();
        player.render();
    }

    void run() {
        srand(static_cast<unsigned int>(time(0)));
        initEnemies();

        while (running) {
            input();
            updateLevel();
            updateEnemies();
            updateBullets();
            checkCollisions();
            render();

            bool allDead = all_of(enemies.begin(), enemies.end(), [](shared_ptr<Enemy> e) { return !e->isAlive(); });

            if (allDead) {
                if (level == 3)
                    running = false;
                else
                    initEnemies();
            }

            this_thread::sleep_for(50ms);
        }

        system("cls");
        if (player.getLives() <= 0)
            cout << "GAME OVER!" << endl;
        else
            cout << "YOU WON!" << endl;

        system("pause");
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
