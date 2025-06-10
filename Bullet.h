#pragma once
#include "GameObject.h"

class Bullet : public GameObject {
private:
    int direction;
    bool active;
public:
    Bullet(int x = 0, int y = 0, int direction = 1, COLORS color = WHITE);
    void update() override;
    bool isActive() const { return active; }
    void deactivate() { active = false; }
};