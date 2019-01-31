#include "dragon.h"
#include "main.h"

Dragon::Dragon(float x, float y, color_t color, float acc) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->lives = 3;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        // start head part 1
        0.0f, 0.4f, 0.1f, 
        0.6f, 0.4f, 0.1f, 
        0.0f, 0.0f, 0.1f, 
        0.0f, 0.0f, 0.1f, 
        0.6f, 0.4f, 0.1f, 
        0.6f, 0.0f, 0.1f, 
        // end head part 1
        // start head part 2
        0.0f,-0.1f, 0.1f, 
        0.6f,-0.1f, 0.1f, 
        0.0f,-0.3f, 0.1f, 
        0.6f,-0.1f, 0.1f, 
        0.0f,-0.3f, 0.1f, 
        0.6f,-0.3f, 0.1f, 
        // end head part 2
        // start body mid
        0.6f, 0.2f, 0.1f, 
        2.0f, 0.2f, 0.1f, 
        0.6f,-0.2f, 0.1f, 
        2.0f, 0.2f, 0.1f, 
        0.6f,-0.2f, 0.1f, 
        2.0f,-0.2f, 0.1f, 
        // end body mid
        // start body upper
        0.8f, 0.2f, 0.1f, 
        1.8f, 0.2f, 0.1f, 
        1.8f, 0.6f, 0.1f, 
        0.8f, 0.2f, 0.1f, 
        1.8f, 0.6f, 0.1f, 
        0.8f, 0.6f, 0.1f, 
        // end body upper
        // start body lower
        0.8f,-0.2f, 0.1f, 
        1.8f,-0.2f, 0.1f, 
        1.8f,-0.6f, 0.1f, 
        0.8f,-0.2f, 0.1f, 
        1.8f,-0.6f, 0.1f, 
        0.8f,-0.6f, 0.1f, 
        // end body lower
        // start wing
        1.3f, 0.6f, 0.1f,
        1.3f, 1.0f, 0.1f,
        2.5f, 1.0f, 0.1f,
        // end wing
    };

    this->object = create3DObject(GL_TRIANGLES, 11*3, vertex_buffer_data, color, GL_FILL);
}

void Dragon::draw(glm::mat4 VP) {
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

void Dragon::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Dragon::tick() {
    // this->rotation += speed_r;
}

bounding_box_t Dragon::bbox() {
    bounding_box_t a;
    a.x = this->position.x + 1;
    a.y = this->position.y;
    a.height = 1.2;
    a.width = 2.0;
    a.rot = this->rotation;
    return a;
}