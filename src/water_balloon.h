#include "main.h"

#ifndef WB_H
#define WB_H


class WBalloon {
public:
    WBalloon() {}
    WBalloon(float x, float y, float acc, double sx, color_t color);
    glm::vec3 position;
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

#endif // WB_H
