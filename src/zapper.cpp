#include "zapper.h"
#include "main.h"

Zapper::Zapper(float x, float y, color_t color, float rot) {
    this->position = glm::vec3(x, y, 0);
    this->rotation = rot;
    this->speed_y = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    GLfloat vertex_buffer_data[] = {
        -0.05f,-1.0f, 0.1f, // triangle 1 : begin
        0.05f, 1.0f, 0.1f,
        -0.05f, 1.0f, 0.1f, // triangle 1 : end
        -0.05f,-1.0f, 0.1f, // triangle 2 : begin
        0.05f, 1.0f, 0.1f,
        0.05f, -1.0f, 0.1f // triangle 2 : end
    };

    this->object = create3DObject(GL_TRIANGLES, 2*3, vertex_buffer_data, color, GL_FILL);

    int i, n = 50;

	float angle = (M_PI*2)/(float)(n);
	GLfloat vertex_buffer_data1[9*(n)];
    GLfloat cx = 0.0, cy = 1.0;
	GLfloat x1 = 0.0f, y1 = 1.2f;
	GLfloat x2 = cos(angle)*(x1-cx) - sin(angle)*(y1-cy) + cx, y2 = sin(angle)*(x1-cx) + cos(angle)*(y1-cy) + cy; 

	for(i=0 ; i<n ; i++){
		int j = 9*i;
		vertex_buffer_data1[j] = cx;
		vertex_buffer_data1[j+1] = cy;
		vertex_buffer_data1[j+2] = 0.1f;
		vertex_buffer_data1[j+3] = x1;
		vertex_buffer_data1[j+4] = y1;
		vertex_buffer_data1[j+5] = 0.1f;
		vertex_buffer_data1[j+6] = x2;
		vertex_buffer_data1[j+7] = y2;
		vertex_buffer_data1[j+8] = 0.1f;
		x1 = x2;
		y1 = y2;
		GLfloat z = cos(angle)*(x2-cx)-sin(angle)*(y2-cy);
		y2 = sin(angle)*(x2-cx) + cos(angle)*(y2-cy) + cy;
		x2 = z + cx; 
	}

    this->cir1 = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data1, COLOR_RED, GL_FILL);

    cx = 0.0, cy = -1.0;
	x1 = 0.0f, y1 = -1.2f;
	x2 = cos(angle)*(x1-cx) - sin(angle)*(y1-cy) + cx, y2 = sin(angle)*(x1-cx) + cos(angle)*(y1-cy) + cy; 

	for(i=0 ; i<n ; i++){
		int j = 9*i;
		vertex_buffer_data1[j] = cx;
		vertex_buffer_data1[j+1] = cy;
		vertex_buffer_data1[j+2] = 0.1f;
		vertex_buffer_data1[j+3] = x1;
		vertex_buffer_data1[j+4] = y1;
		vertex_buffer_data1[j+5] = 0.1f;
		vertex_buffer_data1[j+6] = x2;
		vertex_buffer_data1[j+7] = y2;
		vertex_buffer_data1[j+8] = 0.1f;
		x1 = x2;
		y1 = y2;
		GLfloat z = cos(angle)*(x2-cx)-sin(angle)*(y2-cy);
		y2 = sin(angle)*(x2-cx) + cos(angle)*(y2-cy) + cy;
		x2 = z + cx; 
	}
    
    this->cir2 = create3DObject(GL_TRIANGLES, n*3, vertex_buffer_data1, COLOR_RED, GL_FILL);
}

void Zapper::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->cir1);
    draw3DObject(this->cir2);
}

void Zapper::set_position(float x, float y) {
    this->position = glm::vec3(x, y, 0);
}

bounding_box_t Zapper::bbox() {
    bounding_box_t a;
    a.x = this->position.x;
    a.y = this->position.y;
    a.height = 0.1;
    a.width = 2;
    a.rot = this->rotation;
    return a;
}