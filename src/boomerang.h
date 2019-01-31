#include "main.h"

#ifndef BRANG_H
#define BRANG_H


class BRang {
public:
    BRang() {}
    BRang(float x, float y);
    glm::vec3 position;
    glm::vec3 center;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double acc_y;
    double speed_y;
    double speed_x;
    bounding_box_t bbox();
private:
    VAO *object;
};

#endif // BRANG_H
