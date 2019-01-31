#include "main.h"

#ifndef COIN_H
#define COIN_H


class Coin {
public:
    Coin() {}
    Coin(float x, float y, int points);
    glm::vec3 position;
    float rotation;
    float speed_r;
    int points;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    bounding_box_t bbox();
private:
    VAO *object;
};

#endif // COIN_H
