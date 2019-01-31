#include "fake_flying_obj.h"
#include "main.h"

FFObj::FFObj(float x, float y, float acc, int typ) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed_y = 0.2;
    this->life = 0;
    this->score = 0;
    center = this->position.y;
    color_t color = COLOR_GREEN;
    if(typ==1){ 
        this->life = 1;
        color = COLOR_BLUE;
    }
    else this->score = 100;
    acc_y = acc;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -0.18f,-0.18f, 0.101f, // triangle 1 : begin
        0.18f, 0.18f, 0.101f,
        -0.18f, 0.18f, 0.101f, // triangle 1 : end
        -0.18f,-0.18f, 0.101f, // triangle 2 : begin
        0.18f, 0.18f, 0.101f,
        0.18f, -0.18f, 0.101f // triangle 2 : end
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);

    static const GLfloat vertex_buffer_data1[] = {
        -0.2f,-0.2f, 0.1f, // triangle 1 : begin
        0.2f, 0.2f, 0.1f,
        -0.2f, 0.2f, 0.1f, // triangle 1 : end
        -0.2f,-0.2f, 0.1f, // triangle 2 : begin
        0.2f, 0.2f, 0.1f,
        0.2f, -0.2f, 0.1f // triangle 2 : end
    };

    this->object1 = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data1, COLOR_WHITE, GL_FILL);   
}

void FFObj::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
}

void FFObj::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void FFObj::tick() {
    // this->rotation += speed_r;
    this->position.x -= 0.055;
    this->speed_y += acc_y;
    
    if(this->position.y < -3.5) this->position.y = -20;
    this->position.y += this->speed_y;
}

bounding_box_t FFObj::bbox() {
    bounding_box_t a;
    a.x = this->position.x;
    a.y = this->position.y;
    a.height = 0.4;
    a.width = 0.4;
    a.rot = this->rotation;
    return a;
}