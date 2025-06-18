#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    int direction;
    bool active;
    int frameCounter = 0;

public:
    Bullet(int x = 0, int y = 0, int direction = 1, COLORS color = WHITE, wchar_t symbol = L'█');

    void update() override;
    void render() const override;
    bool isActive() const { return active; }
    void deactivate() { active = false; }
};