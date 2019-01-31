#include "boomerang.h"
#include "main.h"

float brangle = (M_PI*2)/(float)(150);

BRang::BRang(float x, float y) {
    this->center = glm::vec3(x, y, 0);
    this->position = glm::vec3(x + 3, 2, 0);
    this->rotation = 0;
    this->speed_y = 0;
    this->speed_x = -0.15;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -0.3f,-0.05f, 0.1f, // triangle 1 : begin
        0.3f, 0.05f, 0.1f,
        -0.3f, 0.05f, 0.1f, // triangle 1 : end
        -0.3f,-0.05f, 0.1f, // triangle 2 : begin
        0.3f, 0.05f, 0.1f,
        0.3f, -0.05f, 0.1f // triangle 2 : end
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, COLOR_PINK, GL_FILL);
}

void BRang::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void BRang::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void BRang::tick() {
    if(this->position.x > this->center.x && this->position.y > this->center.y) this->position.x += this->speed_x;
    else if(this->position.x > this->center.x && this->position.y < this->center.y) this->position.x -= this->speed_x;
    else{
        float x=this->position.x, y=this->position.y;
        this->position.x = cos(brangle)*(x - this->center.x) - sin(brangle)*(y - this->center.y) + this->center.x;
        this->position.y = sin(brangle)*(x - this->center.x) + cos(brangle)*(y - this->center.y) + this->center.y;
    }
    
}

bounding_box_t BRang::bbox() {
    bounding_box_t a;
    a.x = this->position.x;
    a.y = this->position.y;
    a.height = 0.1;
    a.width = 0.6;
    a.rot = this->rotation;
    return a;
}