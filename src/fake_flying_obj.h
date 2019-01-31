#include "main.h"

#ifndef FFOBJ_H
#define FFOBJ_H


class FFObj {
public:
    FFObj() {}
    FFObj(float x, float y, float acc, int typ);
    glm::vec3 position;
    float rotation;
    float center;
    int score;
    int life;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    void tick();
    double acc_y;
    double speed_y;
    bounding_box_t bbox();
private:
    VAO *object;
    VAO *object1;
};

#endif // FFOBJ_H
