#include "main.h"

#ifndef ENEMY_H
#define ENEMY_H


class Enemy {
public:
    Enemy() {}
    Enemy(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double speed_y;
    float mid;
    bounding_box_t bbox();
private:
    VAO *object;
    VAO *cir1;
    VAO *cir2;
};

#endif // ENEMY_H
