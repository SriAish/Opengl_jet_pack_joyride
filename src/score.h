#include "main.h"

#ifndef SCORE_H
#define SCORE_H


class Score {
public:
    Score() {}
    Score(float x, float y, float rot, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y);
    double speed_y;
    bounding_box_t bbox();
private:
    VAO *object;
};

#endif // SCORE_H
