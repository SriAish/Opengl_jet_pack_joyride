#include "main.h"

#ifndef ZAPPER_H
#define ZAPPER_H


class Zapper {
public:
    Zapper() {}
    Zapper(float x, float y, color_t color, float rot);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double speed_y;
    bounding_box_t bbox();
private:
    VAO *object;
    VAO *cir1;
    VAO *cir2;
};

#endif // ZAPPER_H
