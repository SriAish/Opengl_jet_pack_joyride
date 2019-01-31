#include "main.h"

#ifndef RING_H
#define RING_H


class Ring {
public:
    Ring() {}
    Ring(float x, float y, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double acc_y;
    double speed_y;
    bounding_box_t bbox();
private:
    VAO *object;
    VAO *in_object;
};

#endif // RING_H
