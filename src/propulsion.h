#include "main.h"

#ifndef PROPULSION_H
#define PROPULSION_H


class Propulsion {
public:
    Propulsion() {}
    Propulsion(float x, float y, color_t color, float acc);
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
};

#endif // PROPULSION_H
