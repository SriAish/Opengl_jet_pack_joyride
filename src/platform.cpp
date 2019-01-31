#include "platform.h"
#include "main.h"

Platform::Platform(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed_y = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -304.0f,-2.5f, 0.12f, // triangle 1 : begin
        300.0f, 2.5f, 0.12f,
        -304.0f, 2.5f, 0.12f, // triangle 1 : end
        -304.0f,-2.5f, 0.12f, // triangle 2 : begin
        300.0f, 2.5f, 0.12f,
        300.0f, -2.5f, 0.12f // triangle 2 : end
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);
}

void Platform::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Platform::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Platform::tick() {
    // this->rotation += speed_r;
    // if(this->position.y > -2.4){
    //     this->speed_y += acc_y;
    // }
    // this->position.y += this->speed_y;
    
    // if(this->position.y >= 2.5 || this->position.y <= -2.5){
    //     this->position.y -= this->speed_y;
    //     this->speed_y = 0;
    // }
}

bounding_box_t Platform::bbox() {
    bounding_box_t a;
    a.x = this->position.x;
    a.y = this->position.y;
    a.height = 0.4;
    a.width = 0.4;

    return a;
}