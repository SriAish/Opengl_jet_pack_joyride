#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, color_t color, float acc) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed_y = 0;
    acc_y = acc;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -0.26f,-0.26f, 0.101f, // triangle 1 : begin
        0.26f, 0.26f, 0.101f,
        -0.26f, 0.26f, 0.101f, // triangle 1 : end
        -0.26f,-0.26f, 0.101f, // triangle 2 : begin
        0.26f, 0.26f, 0.101f,
        0.26f, -0.26f, 0.101f // triangle 2 : end
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);

    static const GLfloat vertex_buffer_data1[] = {
        -0.3f,-0.3f, 0.1f, // triangle 1 : begin
        0.3f, 0.3f, 0.1f,
        -0.3f, 0.3f, 0.1f, // triangle 1 : end
        -0.3f,-0.3f, 0.1f, // triangle 2 : begin
        0.3f, 0.3f, 0.1f,
        0.3f, -0.3f, 0.1f, // triangle 2 : end
        // eyes
        0.1f, 0.1f, 0.1012,
        0.18f, 0.1f, 0.1012,
        0.14f, 0.05f, 0.1012,
        -0.1f, 0.1f, 0.1012,
        -0.18f, 0.1f, 0.1012,
        -0.14f, 0.05f, 0.1012,
        0.1f, 0.1f, 0.1012,
        0.18f, 0.1f, 0.1012,
        0.14f, 0.15f, 0.1012,
        -0.1f, 0.1f, 0.1012,
        -0.18f, 0.1f, 0.1012,
        -0.14f, 0.15f, 0.1012,
        // eyes
        // nose
        -0.04f, 0.0f, 0.1012,
        0.04f, 0.0f, 0.1012,
        0.0f, -0.05f, 0.1012,
        -0.04f, 0.0f, 0.1012,
        0.04f, 0.0f, 0.1012,
        0.0f, 0.05f, 0.1012,
        // nose
        //smile
        0.15f, -0.1f, 0.1012,
        -0.15f, -0.1f, 0.1012,
        0.0f, -0.2f, 0.1012,
        //smile
    };

    this->object_out = create3DObject(GL_TRIANGLES, 9*3, vertex_buffer_data1, COLOR_BLACK, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object_out);
    draw3DObject(this->object);
}

void Ball::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ball::tick() {
    // this->rotation += speed_r;
    if(this->position.y > -2.6){
        this->speed_y += acc_y;
    }
    this->position.y += this->speed_y;
    
    if(this->position.y >= 2.7 || this->position.y <= -2.7){
        this->position.y -= this->speed_y;
        this->speed_y = 0;
    }
}

bounding_box_t Ball::bbox() {
    bounding_box_t a;
    a.x = this->position.x;
    a.y = this->position.y;
    a.height = 0.6;
    a.width = 0.6;
    a.rot = this->rotation;
    return a;
}