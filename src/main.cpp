#include "main.h"
#include "timer.h"
#include "score.h"
#include "ball.h"
#include "zapper.h"
#include "platform.h"
#include "coin.h"
#include "ring.h"
#include "enemy2.h"
#include "propulsion.h"
#include "flying_obj.h"
#include "fake_flying_obj.h"
#include "magnet.h"
#include "boomerang.h"
#include "water_balloon.h"
#include "dragon.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
 * Customizable functions *
 **************************/

void tick_zapper(); void tick_enemy2(); void tick_coins(); void tick_ring(); void tick_magnet(); void tick_boomerang(); void tick_wb();
void move_space(); void move_left(); void move_right(); void move_up(); void move_down(); void tick_powerup(); void tick_create();
void tick_ib(); void tick_fake_powerup(); void move_pan_left(); void move_pan_right();

Ball ball1; FObj powerup; FFObj fake_powerup; Platform platform, ceiling; BRang br; Magnet mag; Dragon dragon;
Score sc[5][7], li[7];
vector<Zapper> zp; vector<Coin> coin; vector<Enemy> en; vector<Propulsion> prop; vector<Ring> ring; vector<WBalloon> wb, ice_balls;
int died, magnet_counter, br_counter, score = 0, stage = 1,lives = 3, in_r = -1, wb_counter, ib_counter;
float msx, msy, safe_dist = 0, mangle[] = { 0, 180 }, my[] = { -3.6, 3.6 }, angle[] = { 0, 30, 45, 60, 90, -60, -45, -30 }, prop_pos[] = { -0.15, -0.05, 0.05, 0.15 };
float rangle = -(M_PI*2)/(float)(100), speed = 0.06;

bool in_ring = false, magnet_present = false, br_present = false, dragon_present = false;

float screen_zoom = 106, screen_center_x = 0, screen_center_y = 0, camera_rotation_angle = 0;
float cam_x = 0, cam_y = 0, cam_z = 3;

Timer t60(1.0 / 60);

void draw() {
	// clear the color and depth in the frame buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// use the loaded shader program
	// Don't change unless you know what you are doing
	glUseProgram (programID);

	// Eye - Location of camera. Don't change unless you are sure!!
	glm::vec3 eye ( cam_x, cam_y, cam_z );
	// Target - Where is the camera looking at.  Don't change unless you are sure!!
	glm::vec3 target ( cam_x, cam_y, 0);
	// Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
	glm::vec3 up (0, 1, 0);

	// Compute Camera matrix (view)
	Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
	// Don't change unless you are sure!!
	// Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

	// Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
	// Don't change unless you are sure!!
	glm::mat4 VP = Matrices.projection * Matrices.view;

	// Send our transformation to the currently bound shader, in the "MVP" uniform
	// For each model you render, since the MVP will be different (at least the M part)
	// Don't change unless you are sure!!
	glm::mat4 MVP;  // MVP = Projection * View * Model

	// Scene render
	ball1.draw(VP);
	if(dragon_present) dragon.draw(VP);
	powerup.draw(VP);
	fake_powerup.draw(VP);
	for(vector<Propulsion>::size_type i = 0; i != prop.size(); i++) {
		prop[i].draw(VP);
	}
    for(vector<WBalloon>::size_type i = 0; i != wb.size(); i++) {
		wb[i].draw(VP);
	}
	for(vector<Zapper>::size_type i = 0; i != zp.size(); i++) {
		zp[i].draw(VP);
	}
	for(vector<Enemy>::size_type i = 0; i != en.size(); i++) {
		en[i].draw(VP);
	}   
	for(vector<Coin>::size_type i = 0; i != coin.size(); i++) {
		coin[i].draw(VP);
	}
	for(vector<WBalloon>::size_type k = 0; k != ice_balls.size(); k++) {
		ice_balls[k].draw(VP);
	}
	platform.draw(VP);
	ceiling.draw(VP);
	if(magnet_present) mag.draw(VP);
	if(br_present) br.draw(VP);
	for(vector<Ring>::size_type i = 0; i != ring.size(); i++) {
		ring[i].draw(VP);
	}
	int tmp_score = score;
	int v;
	for(int i = 4 ; i>=0 ; --i) {
		v = tmp_score%10;
		tmp_score = tmp_score/10;
		if(v == 0){
			sc[i][0].draw(VP);
			sc[i][1].draw(VP);
			sc[i][2].draw(VP);
			sc[i][4].draw(VP);
			sc[i][5].draw(VP);
			sc[i][6].draw(VP);
		}
		else if(v == 1){
			sc[i][2].draw(VP);
			sc[i][5].draw(VP);
		}
		else if(v == 2){
			sc[i][1].draw(VP);
			sc[i][2].draw(VP);
			sc[i][3].draw(VP);
			sc[i][4].draw(VP);
			sc[i][6].draw(VP);
		}
		else if(v == 3){
			sc[i][1].draw(VP);
			sc[i][2].draw(VP);
			sc[i][3].draw(VP);
			sc[i][5].draw(VP);
			sc[i][6].draw(VP);
		}
		else if(v == 4){
			sc[i][0].draw(VP);
			sc[i][2].draw(VP);
			sc[i][3].draw(VP);
			sc[i][5].draw(VP);
		}
		else if(v == 5){
			sc[i][0].draw(VP);
			sc[i][1].draw(VP);
			sc[i][3].draw(VP);
			sc[i][5].draw(VP);
			sc[i][6].draw(VP);
		}
		else if(v == 6){
			sc[i][0].draw(VP);
			sc[i][1].draw(VP);
			sc[i][3].draw(VP);
			sc[i][4].draw(VP);
			sc[i][5].draw(VP);
			sc[i][6].draw(VP);
		}
		else if(v == 7){
			sc[i][1].draw(VP);
			sc[i][2].draw(VP);
			sc[i][5].draw(VP);
		}
		else if(v == 8){
			for( int j = 0 ; j < 7 ; ++j) sc[i][j].draw(VP);
		}
		else if(v == 9){
			sc[i][0].draw(VP);
			sc[i][1].draw(VP);
			sc[i][2].draw(VP);
			sc[i][3].draw(VP);
			sc[i][5].draw(VP);
			sc[i][6].draw(VP);
		}
	}
	v = lives;
	{
		if(v == 0){
			li[0].draw(VP);
			li[1].draw(VP);
			li[2].draw(VP);
			li[4].draw(VP);
			li[5].draw(VP);
			li[6].draw(VP);
		}
		else if(v == 1){
			li[2].draw(VP);
			li[5].draw(VP);
		}
		else if(v == 2){
			li[1].draw(VP);
			li[2].draw(VP);
			li[3].draw(VP);
			li[4].draw(VP);
			li[6].draw(VP);
		}
		else if(v == 3){
			li[1].draw(VP);
			li[2].draw(VP);
			li[3].draw(VP);
			li[5].draw(VP);
			li[6].draw(VP);
		}
		else if(v == 4){
			li[0].draw(VP);
			li[2].draw(VP);
			li[3].draw(VP);
			li[5].draw(VP);
		}
		else if(v == 5){
			li[0].draw(VP);
			li[1].draw(VP);
			li[3].draw(VP);
			li[5].draw(VP);
			li[6].draw(VP);
		}
		else if(v == 6){
			li[0].draw(VP);
			li[1].draw(VP);
			li[3].draw(VP);
			li[4].draw(VP);
			li[5].draw(VP);
			li[6].draw(VP);
		}
		else if(v == 7){
			li[1].draw(VP);
			li[2].draw(VP);
			li[5].draw(VP);
		}
		else if(v == 8){
			for( int j = 0 ; j < 7 ; ++j) li[j].draw(VP);
		}
		else if(v == 9){
			li[0].draw(VP);
			li[1].draw(VP);
			li[2].draw(VP);
			li[3].draw(VP);
			li[5].draw(VP);
			li[6].draw(VP);
		}
	}
}

void move_score(float mv){
	for(int i = 0 ; i<5 ; ++i) for(int j = 0 ; j<7 ; ++j) sc[i][j].position.x += mv; 
}

void move_lives(float mv){
	for(int j = 0 ; j<7 ; ++j) li[j].position.x += mv; 
}

void move_left(){
	if (!in_ring && ball1.position.x > -3.5) {
		cam_x -= speed;
		ball1.position.x -= speed;
		move_score(-speed);
		move_lives(-speed);
	}
	else if(in_ring) {
		float x=ball1.position.x, y=ball1.position.y;
		ball1.position.x = cos(-rangle)*(x - ring[in_r].position.x) - sin(-rangle)*(y - ring[in_r].position.y) + ring[in_r].position.x;
		ball1.position.y = sin(-rangle)*(x - ring[in_r].position.x) + cos(-rangle)*(y - ring[in_r].position.y) + ring[in_r].position.y;
		cam_x += ball1.position.x - x;
		move_score(ball1.position.x - x);
		move_lives(ball1.position.x - x);
		if(ball1.position.x <= ring[in_r].position.x - 1.9){ 
			in_ring = false;
			ball1.position.y -= 0.4;
		}
	}
}

void move_right(){
	if (!in_ring) {
		cam_x += speed;
		ball1.position.x += speed;
		move_score(speed);
		move_lives(speed);
	}
	else {
		float x = ball1.position.x, y = ball1.position.y;
		ball1.position.x = cos(rangle)*(x - ring[in_r].position.x) - sin(rangle)*(y - ring[in_r].position.y) + ring[in_r].position.x;
		ball1.position.y = sin(rangle)*(x - ring[in_r].position.x) + cos(rangle)*(y - ring[in_r].position.y) + ring[in_r].position.y;
		cam_x += ball1.position.x - x;
		move_score(ball1.position.x - x);
		move_lives(ball1.position.x - x);
		if(ball1.position.x >= ring[in_r].position.x + 1.9){ 
			in_ring = false;
			ball1.position.y -= 0.4;
		}
	}
}

void move_up(){
	cam_y += 0.01;
}

void move_down(){
	cam_y -= 0.01;
}

void move_pan_left(){
	cam_x -= 0.01;

}

void move_pan_right(){
	cam_x += 0.01;
}

void move(){
	if(!in_ring){    
		cam_x += msx;
		ball1.position.x += msx;
		move_score(msx);
		move_lives(msx);
		ball1.position.y += msy;
	}
	if(abs(ball1.position.y) > 2.7) ball1.position.y -= msy;
}

void tick_input(GLFWwindow *window) {
    wb_counter--;
	int left  = glfwGetKey(window, GLFW_KEY_LEFT);
	int right = glfwGetKey(window, GLFW_KEY_RIGHT);
	int pan_up = glfwGetKey(window, GLFW_KEY_W);
	int pan_down = glfwGetKey(window, GLFW_KEY_S);
	int pan_right = glfwGetKey(window, GLFW_KEY_D);
	int pan_left = glfwGetKey(window, GLFW_KEY_A);
	int space = glfwGetKey(window, GLFW_KEY_SPACE);
	int x = glfwGetKey(window, GLFW_KEY_X);

    if (x && wb_counter<0){
        wb_counter = 30; 
        wb.push_back(WBalloon(ball1.position.x+0.1, ball1.position.y, -0.005, 0.1, COLOR_WATER));
    }
	move();
	if (left) move_left(); 
	if (right) move_right();
	if (pan_up) move_up();
	if (pan_down) move_down();
	if (pan_right) move_pan_right();
	if (pan_left) move_pan_left();
	if (space) move_space();
}

void tick_elements() {
	ib_counter--;
	if(!in_ring){
		ball1.tick();
	}
	bool p_nex = true;
	for(vector<Propulsion>::size_type k = 0; k != prop.size(); k++) {
		prop[k].tick();
		if(prop[k].position.y > -3.1) p_nex = false;
	}

	if(p_nex) prop.clear();

	if(dragon_present && (dragon.position.x - ball1.position.x < 7 && ib_counter < 0)){ 
		ib_counter = 30;
		float xspeed = rand()%100 + 1;
		xspeed/=1000;
        ice_balls.push_back(WBalloon(dragon.position.x-0.1, dragon.position.y, -0.005, -xspeed, COLOR_ICE));
	}
	tick_ib();
    tick_wb();
	tick_create();
	tick_powerup();
	tick_fake_powerup();
	tick_enemy2();
	tick_zapper();
	tick_coins();
	tick_ring();
	tick_magnet();
	tick_boomerang();
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initScore(){
	for(int i = 0 ; i < 5 ; ++i){
		sc[i][0] = Score( -3.7 + i*0.3, 2.9, 0, COLOR_WHITE);
		sc[i][1] = Score( -3.6 + i*0.3, 2.98, 90, COLOR_WHITE);
		sc[i][2] = Score( -3.5 + i*0.3, 2.9, 0, COLOR_WHITE);
		sc[i][3] = Score( -3.6 + i*0.3, 2.8, 90, COLOR_WHITE);
		sc[i][4] = Score( -3.7 + i*0.3, 2.7, 0, COLOR_WHITE);
		sc[i][5] = Score( -3.5 + i*0.3, 2.7, 0, COLOR_WHITE);
		sc[i][6] = Score( -3.6 + i*0.3, 2.62, 90, COLOR_WHITE);
	}
}

void initLife(){
	li[0] = Score( 2.3 + 1.2, 2.9, 0, COLOR_BLOOD);
	li[1] = Score( 2.4 + 1.2, 2.98, 90, COLOR_BLOOD);
	li[2] = Score( 2.5 + 1.2, 2.9, 0, COLOR_BLOOD);
	li[3] = Score( 2.4 + 1.2, 2.8, 90, COLOR_BLOOD);
	li[4] = Score( 2.3 + 1.2, 2.7, 0, COLOR_BLOOD);
	li[5] = Score( 2.5 + 1.2, 2.7, 0, COLOR_BLOOD);
	li[6] = Score( 2.4 + 1.2, 2.62, 90, COLOR_BLOOD);
}

void initGL(GLFWwindow *window, int width, int height) {
	/* Objects should be created before any other gl function and shaders */
	// Create the models
	int i = rand()%8;
	initScore();
	initLife();
	ball1 = Ball(-3, 0, COLOR_SKIN, -0.0005);
	en.push_back(Enemy(4, 0, COLOR_RED));
	en.push_back(Enemy(4, -0.42, COLOR_RED));
	platform = Platform(110, -5.5, COLOR_PLATFORM);
	ceiling = Platform(110, 5.5, COLOR_PLATFORM);
	ring.push_back(Ring(3, 0, COLOR_RING));
	powerup = FObj(8, 0, 0, 1);
	fake_powerup = FFObj(8, -3, -0.005, 1);
	// Create and compile our GLSL program from the shaders
	programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
	// Get a handle for our "MVP" uniform
	Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


	reshapeWindow (window, width, height);

	// Background color of the scene
	glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
	glClearDepth (1.0f);

	glEnable (GL_DEPTH_TEST);
	glDepthFunc (GL_LEQUAL);

	cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
	cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
	cout << "VERSION: " << glGetString(GL_VERSION) << endl;
	cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

int main(int argc, char **argv) {
	srand(time(0));
	int width  = 600;
	int height = 600;

	window = initGLFW(width, height);

	initGL (window, width, height);
	Matrices.projection = glm::perspective(glm::radians(screen_zoom), (float)600 / (float)600, 0.1f, 100.0f);
	char scstr[50];
	/* Draw in loop */
	while (!glfwWindowShouldClose(window)) {
		// Process timers
		if(lives <= 0){
			printf("You dead\nScore: %d\n", score);
			exit(0);
		}
		if (t60.processTick()) {
			// 60 fps
			// OpenGL Draw commands
			if(died == 50)died--;
			else if(died){
				died--;
				continue;
			}
			draw();
			// Swap Frame Buffer in double buffering
			glfwSwapBuffers(window);
			sprintf(scstr, "Stage: %d Score: %d Lives: %d", stage, score, lives);
			glfwSetWindowTitle(window, scstr);
			tick_elements();
			tick_input(window);
		}

		// Poll for Keyboard and mouse events
		glfwPollEvents();
	}

	quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
	float l = sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));

	if(l>1.4) return false;

	float pt_x = a.x + a.height/2, pt_y = a.y + a.width/2;
	float tn = (tan((b.rot)* M_PI / 180.0f));
	float dist_x = (pt_y-b.y)*tn;

	// printf("1\n");
	if(dist_x + 0.05 > b.x - pt_x && dist_x - 0.05 < b.x - pt_x) return true;

	pt_y = a.y - a.width/2;
	dist_x = (pt_y-b.y)*tn;

	// printf("2\n");
	if(dist_x + 0.05 > b.x - pt_x && dist_x - 0.05 < b.x - pt_x) return true;

	pt_x = a.x - a.height/2;

	// printf("3\n");
	if(dist_x + 0.05 > b.x - pt_x && dist_x - 0.05 < b.x - pt_x) return true;

	pt_y = a.y + a.width/2;
	dist_x = (pt_y-b.y)*tn;

	// printf("4\n");
	if(dist_x + 0.05 > b.x - pt_x && dist_x - 0.05 < b.x - pt_x) return true;

	// printf("5\n");
	if(b.rot == 0 && b.x < a.x+a.width/2 && b.x > a.x-a.width/2) return true;
	if(b.rot == 90 && b.y < a.y+a.height/2 && b.y > a.y-a.height/2) return true;
	return false; 
}

bool coin_collect(bounding_box_t a, bounding_box_t b) {
	return (abs(a.x - b.x) * 2 < (a.width + b.width)) &&
		(abs(a.y - b.y) * 2 < (a.height + b.height));
}

bool detect_ring(bounding_box_t a){
	for(vector<Ring>::size_type i = 0; i != ring.size(); i++) {

		float x = ring[i].position.x, y = ring[i].position.y;

		if(a.y < y - 0.29) return false;

		float yd1 = (a.y + 0.3 - y)*(a.y + 0.3 - y), yd2 = (a.y - 0.3 - y)*(a.y - 0.3 - y);
		float xd1 = (a.x - 0.3 - x)*(a.x - 0.3 - x), xd2 = (a.x + 0.3 - x)*(a.x + 0.3 - x);

		float d1 = sqrt(xd1 + yd1), d2 = sqrt(xd2 + yd1), d3 = sqrt(xd1 + yd2), d4 = sqrt(xd2 + yd2);

		if((d1 <= 2.0 && d1 >= 1.9) || 
				(d2 <= 2.0 && d2 >= 1.9) || 
				(d3 <= 2.0 && d3 >= 1.9) || 
				(d4 <= 2.0 && d4 >= 1.9)){ in_r = i ; return true;}
		if((d1 > 1.95 && d2 < 1.95) || (d3 > 1.95 && d4 < 1.95) || (d2 > 1.95 && d1 < 1.95) || (d4 > 1.95 && d3 < 1.95)){in_r = i ;  return true;}
	}
	return false;
}

void reset_screen() {
	float top    = screen_center_y + 4 / screen_zoom;
	float bottom = screen_center_y - 4 / screen_zoom;
	float left   = screen_center_x - 4 / screen_zoom;
	float right  = screen_center_x + 4 / screen_zoom;
	Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
}

void make_coin(float y, float x){
	for(float i = x ; i <= x + 1.8 ; i+=0.3){
		for(float j = y ; j <= y + 0.9 ; j+=0.3){
			int r = rand()%4;
			if(r<3) coin.push_back(Coin(i, j, 1));
			else coin.push_back(Coin(i, j, 5));
		}
	}
}

void tick_create(){
	if(ball1.position.x < safe_dist) return;
	float prob = rand()%1000;
	prob = prob/1000;

	if( 60.0 <= safe_dist && safe_dist <= 64.2 ){
		dragon = Dragon(ball1.position.x + 8, 0, COLOR_BLUE, 0);
		dragon_present = true;
	}
	else if (prob<0.3) {
		int i = rand()%8;

		float y = rand()%60;
		y -= 30;
		y /= 10; 

		zp.push_back(Zapper(ball1.position.x+8, y, COLOR_YELLOW, angle[i]));
	}
	else if (prob >= 0.3 && prob < 0.5){
		float y = rand()%40;
		y -= 20;
		y /= 10; 

		en.push_back(Enemy(ball1.position.x+8, y, COLOR_RED));
		en.push_back(Enemy(ball1.position.x+8, y-0.42, COLOR_RED));
	}
	else if (prob >= 0.5 && prob < 0.8){
		float y = rand()%40;
		y -= 20;
		y /= 10;

		make_coin(y, ball1.position.x+8);
		make_coin(y, ball1.position.x+10.2);
	}
	else if (prob >= 0.8 && prob < 0.9) ring.push_back(Ring(ball1.position.x + 8, 0, COLOR_RING));

	safe_dist = safe_dist + 4.2;


}

void tick_zapper(){

	if(!in_ring){
		for(vector<Zapper>::size_type k = 0; k != zp.size(); k++) {
			if (detect_collision(ball1.bbox(), zp[k].bbox())){
				zp[k].position.x = -20;
				lives--;
				died = 50;
			}
		}
	}
}

void tick_enemy2(){
	if(!in_ring){
		for(vector<Enemy>::size_type k = 0; k != en.size(); k++) {
			if (detect_collision(ball1.bbox(), en[k].bbox())){
				en[k].position.x = -20;
				lives--;
				died = 50;
			}
		}
	}
	for(vector<Enemy>::size_type k = 0; k != en.size(); k++) {
		en[k].tick();
	}
}

void tick_coins(){
	for(vector<Coin>::size_type k = 0; k != coin.size(); k++) {
		coin[k].tick();
	}
	for(vector<Coin>::size_type k = 0; k != coin.size(); k++) {
		if (coin_collect(ball1.bbox(), coin[k].bbox())){
			coin[k].position.x = -20;
			score += coin[k].points;
		}
	}
}

void tick_ring(){
	if (!in_ring && detect_ring(ball1.bbox())){
		in_ring = true;
		ball1.speed_y = 0;
		if(ball1.position.y <= ring[in_r].position.y && ball1.position.x < ring[in_r].position.x){
			ball1.position.y = ring[in_r].position.y;
			ball1.position.x = ring[in_r].position.x - 1.95;
		}
		else if(ball1.position.y <= ring[in_r].position.y && ball1.position.x > ring[in_r].position.x){
			ball1.position.y = ring[in_r].position.y;
			ball1.position.x = ring[in_r].position.x + 1.95;
		}
		else ball1.position.y = sqrt(abs((1.95*1.95)-((ball1.position.x -ring[in_r].position.x)*(ball1.position.x -ring[in_r].position.x)))) + ring[in_r].position.y;
		float x = sqrt(abs((1.95*1.95)-(ball1.position.y*ball1.position.y))) + ring[in_r].position.x;
		if(abs(ball1.position.x) > x){
			if(ball1.position.x < 0)  ball1.position.x = -x;
			else ball1.position.x = x;
		}
	}
}

void tick_magnet(){
	magnet_counter++;
	if(!magnet_present && magnet_counter==600){
		int r = rand()%2;
		mag=Magnet(ball1.position.x+7, my[r], mangle[r]);
		magnet_present = true;
		magnet_counter = 0;
		msx = msy = 0;
	}
	else if(magnet_present && magnet_counter==200){
		magnet_present = false;
		magnet_counter = 0;
		msx = msy = 0;
	}
	else if(magnet_present){
		float bm_dist = sqrt( (ball1.position.x - mag.position.x)*(ball1.position.x - mag.position.x) +
				(ball1.position.y - mag.position.y)*(ball1.position.y - mag.position.y) );
		msx-=((ball1.position.x - mag.position.x)/bm_dist)*0.0002;
		msy-=((ball1.position.y - mag.position.y)/bm_dist)*0.0002;
	}
}

void tick_boomerang(){
	br_counter++;
	if(!br_present && br_counter==800){
		br=BRang(cam_x - 1, 0);
		br_present = true;
		br_counter = 0;
	}
	else if( br_present && br.position.x > cam_x + 4){
		br_present = false;
		br_counter = 0;
	}
	else if( !in_ring && br_present && coin_collect(ball1.bbox(), br.bbox()) ){
		lives--;
		died = 50;
		br_present = false;
	}
	else if(br_present){
		br.tick();
	}
}

void tick_powerup(){
	powerup.tick();
	if(coin_collect(ball1.bbox(), powerup.bbox()) || powerup.position.x < ball1.position.x - 8){
		if(powerup.position.x >= ball1.position.x - 8){
			score+=powerup.score;
			lives+=powerup.life;
		}
		int a = rand()%100;

		if(a<80) powerup = FObj(powerup.position.x+15, 0, 0, 0);
		else powerup = FObj(powerup.position.x+15, 0, 0, 1); 
	}
}

void tick_fake_powerup(){
	fake_powerup.tick();
	if(coin_collect(ball1.bbox(), fake_powerup.bbox()) || fake_powerup.position.x < ball1.position.x - 40){
		if(fake_powerup.position.x >= ball1.position.x - 8){
			score+=fake_powerup.score;
			lives+=fake_powerup.life;
		}
		int a = rand()%100;

		if(a<80) fake_powerup = FFObj(ball1.position.x+8, -3, -0.005, 0);
		else fake_powerup = FFObj(ball1.position.x+8, -3, -0.005, 1); 
	}
}

void tick_wb(){
	bool wb_nex = true;
    for(vector<WBalloon>::size_type k = 0; k != wb.size(); k++) {
		wb[k].tick();
		if(wb[k].position.y > -3.5) wb_nex = false;
	}

	if(wb_nex) wb.clear();

    for(vector<WBalloon>::size_type i = 0; i != wb.size(); i++) {
		for(vector<Zapper>::size_type k = 0; k != zp.size(); k++) {
			if(zp[k].position.x > -4 && detect_collision(wb[i].bbox(), zp[k].bbox())){ 
                zp[k].position.x = -20;
                wb[i].position.y = -20;
            }
		}
		for(vector<Enemy>::size_type k = 0; k != en.size(); k++) {
			if(en[k].position.x > -4 && detect_collision(wb[i].bbox(), en[k].bbox())){ 
                en[k].position.x = -20;
                wb[i].position.y = -20;
            }        
        }

		if(dragon_present && coin_collect(wb[i].bbox(), dragon.bbox())){ 
            wb[i].position.y = -20;
			--dragon.lives;
		}

		if(dragon_present && dragon.lives == 0){ 
			dragon_present = false;
			stage++;
			score += 200;
		}

	}
}

void tick_ib(){

    for(vector<WBalloon>::size_type k = 0; k != ice_balls.size(); k++) {
		ice_balls[k].tick();
	}
	if(in_ring) return;
    for(vector<WBalloon>::size_type i = 0; i != ice_balls.size(); i++) {
		if(detect_collision(ice_balls[i].bbox(), ball1.bbox())){ 
            ice_balls[i].position.y = -20;
			lives--;
			died = 50;
        }
	}
}

void move_space(){
	if (ball1.position.y < 2.6) {
		int cur_prop_pos = rand()%8;
		if(cur_prop_pos<4) prop.push_back(Propulsion(ball1.position.x + prop_pos[cur_prop_pos], ball1.position.y - 0.3, COLOR_BLOOD, -0.0005));
		ball1.speed_y+=0.001;
	}
}
