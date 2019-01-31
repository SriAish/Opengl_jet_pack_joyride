#include "ring.h"
#include "main.h"

Ring::Ring(float x, float y, color_t color) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = 0;
    this->speed_y = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int i, n = 50;

	float angle = (M_PI*2)/(float)(n);
	GLfloat vertex_buffer_data[9*(n/2)];
	GLfloat x1 = 2.0f, y1 = 0.0f;
	GLfloat x2 = cos(angle)*x1 - sin(angle)*y1, y2 = sin(angle)*x1 + cos(angle)*y1; 

	for(i=0 ; i<n/2 ; i++){
		int j = 9*i;
		vertex_buffer_data[j] = 0.0f;
		vertex_buffer_data[j+1] = 0.0f;
		vertex_buffer_data[j+2] = -0.01f;
		vertex_buffer_data[j+3] = x1;
		vertex_buffer_data[j+4] = y1;
		vertex_buffer_data[j+5] = -0.01f;
		vertex_buffer_data[j+6] = x2;
		vertex_buffer_data[j+7] = y2;
		vertex_buffer_data[j+8] = -0.01f;
		x1 = x2;
		y1 = y2;
		GLfloat z = cos(angle)*x2-sin(angle)*y2;
		y2 = sin(angle)*x2 + cos(angle)*y2;
		x2 = z; 
	}

    this->object = create3DObject(GL_TRIANGLES, (n/2)*3, vertex_buffer_data, color, GL_FILL);

    x1 = 1.9f, y1 = 0.0f;
	x2 = cos(angle)*x1 - sin(angle)*y1, y2 = sin(angle)*x1 + cos(angle)*y1; 

	for(i=0 ; i<n/2 ; i++){
		int j = 9*i;
		vertex_buffer_data[j] = 0.0f;
		vertex_buffer_data[j+1] = 0.0f;
		vertex_buffer_data[j+2] = 0.0f;
		vertex_buffer_data[j+3] = x1;
		vertex_buffer_data[j+4] = y1;
		vertex_buffer_data[j+5] = 0.0f;
		vertex_buffer_data[j+6] = x2;
		vertex_buffer_data[j+7] = y2;
		vertex_buffer_data[j+8] = 0.0f;
		x1 = x2;
		y1 = y2;
		GLfloat z = cos(angle)*x2-sin(angle)*y2;
		y2 = sin(angle)*x2 + cos(angle)*y2;
		x2 = z; 
	}

    this->in_object = create3DObject(GL_TRIANGLES, (n/2)*3, vertex_buffer_data, COLOR_BACKGROUND, GL_FILL);
}

void Ring::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->in_object);
}

void Ring::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

void Ring::tick() {
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

bounding_box_t Ring::bbox() {
    bounding_box_t a;
    a.x = this->position.x;
    a.y = this->position.y;
    a.height = 0.6;
    a.width = 0.6;
    a.rot = this->rotation;
    return a;
}