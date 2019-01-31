#include "main.h"

#ifndef DRAGON_H
#define DRAGON_H


class Dragon {
public:
    Dragon() {}
    Dragon(float x, float y, color_t color, float acc);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    int lives;
    bounding_box_t bbox();
private:
    VAO *object;
};

#endif // DRAGON_H
