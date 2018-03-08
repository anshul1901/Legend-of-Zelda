#include "main.h"
#include "timer.h"
#include "ball.h"
#include "boat.h"
#include "math.h"
#include "aim.h"
#include "monster.h"
#include "barrel.h"

using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

// Ball boat;

float random_number(float min, float max) {
  return ((float(rand()) / float(RAND_MAX)) * (max - min)) + min;
}

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0;
glm::vec3 eye (8, 2, 0);
glm::vec3 target (0, 0, 0);
glm::vec3 up (0, 1, 0);
extern int view;
extern int f;
extern int cursor_x1_press, cursor_y1_press;
int is_wind = 0;
double cursor_x, cursor_y;
double old_x, old_y;
const int number_of_rocks = 100, number_of_monsters = 10, number_of_barrels = 500;
int monster_count = 0;
time_t start = 0, total_time;
int old_view = 0, temp_view = 0;
int aim_flag = 0;
int boss_flag = 0;
int is_sail = 0;
int score = 0;
float boost_flag = 1;

ao_device *device;
ao_sample_format format;
int default_driver;
char *bufferN, *bufferH, *bufferL;
int buf_size, sounder;
int sampleN, sampleH, sampleL;
float freq = 440.0;
unsigned long long int back_counter = 0;

Ball rocks[number_of_rocks];
Ball water;
Boat boat;
Aim aim;
Monster monsters[number_of_monsters];
Monster boss;
Ball booster;
Barrel barrels[number_of_barrels];

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    // glm::vec3 eye ( 5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f) );
    // eye = camera_position;
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    // target = camera_target;
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    // glm::vec3 up (0, 1, 0);
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
    water.draw(VP, glm::vec3 (0, 1, 0));
    boat.draw(VP, is_sail);
    for (int i = 0; i < number_of_rocks; i++) {
      rocks[i].draw(VP, glm::vec3(0, 1, 0));
    }
    if (aim_flag == 1) {
      aim.draw(VP);
    }
    for (int i = 0; i < number_of_monsters; i++) {
      monsters[i].draw(VP, glm::vec3 (0, 1, 0));
    }
    // if (monster_count % 3 == 0 && boss_flag && monster_count != 0) {
      boss.draw(VP, glm::vec3 (0, 1, 0));
      // cout<<"BOSS from draw\n";
    // }
    for (int i = 0; i < number_of_barrels; i++) {
      barrels[i].draw(VP, glm::vec3 (0, 1, 0));
    }
    booster.draw(VP, glm::vec3 (0, 0, 1));
}

void tick_input(GLFWwindow *window) {
    int up  = glfwGetKey(window, GLFW_KEY_UP);
    int down  = glfwGetKey(window, GLFW_KEY_DOWN);
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int space = glfwGetKey(window, GLFW_KEY_SPACE);
    int z = glfwGetKey(window, GLFW_KEY_Z);

    if (left) {
      if (down) boat.rotation -= 2;
      else boat.rotation += 2;
    }
    if (right) {
      if (down) boat.rotation += 2;
      else boat.rotation -= 2;
    }
    if (up) {
      boat.position.x -= boost_flag*0.4*cos(boat.rotation * M_PI / 180.0f);
      boat.position.z += boost_flag*0.4*sin(boat.rotation * M_PI / 180.0f);
    }
    if (down) {
      boat.position.x += boost_flag*0.4*cos(boat.rotation * M_PI / 180.0f);
      boat.position.z -= boost_flag*0.4*sin(boat.rotation * M_PI / 180.0f);
    }
    if (space && boat.position.y <= -1.68) {
      boat.speed.y += 1.2;
    }
    if (z) {
      is_wind = 0;
      if(boat.fireball.position.x == boat.position.x)
        boat.fireball.shoot(boat.rotation, 1.5);
    }
}
int rock = 0;

unsigned long long int j = 0;
unsigned long long int time_count = 0;
void tick_elements() {
    glfwGetCursorPos(window, &cursor_x, &cursor_y);
    booster.rotation += 1;
    boat.tick();
    boss.tick();
    for (int i = 0; i < number_of_barrels; i++) {
      barrels[i].tick();
    }
    booster.tick();
    boat.fireball.rotation += 100;
    for (int i = 0 ; i < number_of_monsters; i++) {
      monsters[i].tick();
    }
    if (boat.fireball.position.y > -3.2 && boat.fireball.speed.y != 0) {
      boat.fireball.speed.y -= 0.06;
    } else {
      boat.fireball.position = boat.position;
      boat.fireball.speed = glm::vec3 (0, 0, 0);
    }
    if ((time_count++) % 1000 == 0) {
      is_wind = (int)random_number(0, 4);
    }
    if (is_wind == 0) {                                     // No wind
      float angle = boat.rotation + 75;
      if (boat.sail1.rotation > angle) {
        if (boat.sail1.rotation != angle) {
          boat.sail1.rotation -= 1;
          boat.sail2.rotation -= 1;
        }
      }
      else {
        if (boat.sail1.rotation != angle) {
            boat.sail1.rotation += 1;
            boat.sail2.rotation += 1;
          }
      }
    }
    else if (is_wind == 1 || is_wind == 2) {
      if (boat.sail1.rotation > 105) {
        if (boat.sail1.rotation != 105) {
          boat.sail1.rotation -= 1;
          boat.sail2.rotation -= 1;
        }
      }
      else {
        if (boat.sail1.rotation != 105) {
            boat.sail1.rotation += 1;
            boat.sail2.rotation += 1;
          }
      }
      if (is_wind == 1)
        boat.position.x += 0.01;
      else boat.position.x -= 0.01;
    }

    else if (is_wind == 3 || is_wind == 4) {
      if (boat.sail1.rotation > 15) {
        if (boat.sail1.rotation != 15) {
          boat.sail1.rotation -= 1;
          boat.sail2.rotation -= 1;
        }
      }
      else {
        if (boat.sail1.rotation != 15) {
            boat.sail1.rotation += 1;
            boat.sail2.rotation += 1;
        }
      }
      if (is_wind == 3)
        boat.position.z -= 0.01;
      else boat.position.z += 0.01;
    }
    is_sail = 1;
    if (view == 0) {
      up = glm::vec3(0, 1, 0);                                              //follow_view
      target.x = boat.position.x;
      target.z = boat.position.z;
      eye.x = boat.position.x + 10*cos(boat.rotation * M_PI / 180.0f) ;
      eye.y = 2;
      eye.z = boat.position.z - 10*sin(boat.rotation * M_PI / 180.0f);
    }
    else if (view == 1) {
      is_sail = 0;
      up = glm::vec3(0, 1, 0);                                              //boat_view
      eye = boat.position - glm::vec3(0, -0.5, 0);
      target = boat.position - glm::vec3 (0, -0.5, 0);
      target.x = boat.position.x - 10*cos(boat.rotation * M_PI / 180.0f);
      target.z = boat.position.z + 10*sin(boat.rotation * M_PI / 180.0f);
      eye.x = boat.position.x - 0.5*cos(boat.rotation * M_PI / 180.0f);
      eye.z = boat.position.z + 0.5*sin(boat.rotation * M_PI / 180.0f);
    }
    else if (view == 2) {                                                //top_view
      eye = boat.position + glm::vec3(0, 20, 0);
      target = boat.position;
      up.x = -cos(boat.rotation * M_PI / 180.0f);
      up.z = sin(boat.rotation * M_PI / 180.0f);
    }
    else if (view == 3) {
      up = glm::vec3(0, 1, 0);                                              //tower_view
      eye = glm::vec3 (0, 10, 10);
      target.x = boat.position.x;
      target.z = boat.position.z;
    }
    else if (view == 4) {
      up = glm::vec3(0, 1, 0);                                              //tower_view
      eye.x = 10;
      // eye.y = 10;
      eye.z = 10;
      // target = glm::vec3(100, 100, 100);
      int mouse_left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
      // target.y += (cursor_y1_press - cursor_y)*2;
      // target.z -= (cursor_x1_press - cursor_x)/1;

      // float temp_y = cursor_y1_press - cursor_y;
      // float temp_z = cursor_x1_press - cursor_x;
      // float theta = M_PI - atan2(temp_z, temp_y);
      // float target_y = 0, target_z = 0;
      // target.y += cursor_y/100;
      // target.z -= cursor_x/100;
      cursor_x -= 960;
      cursor_x1_press -= 960;
      cursor_y -= 504;
      cursor_y1_press -= 504;

      if (mouse_left) {
        // cursor_x -= 960;
        // cursor_x1_press -= 960;
        // cursor_y -= 504;
        // cursor_y1_press -= 504;
        target.y = (cursor_y)/100;
        target.z = (cursor_x)/100;
      }
      // target.y = target_y;
      // target.z = target_z;
    }
    if (!glfwGetKey(window, GLFW_KEY_F)) {
      old_view = view;
      aim_flag = 0;
    } else {
      is_wind = 0;
      aim_flag = 0;
      // cursor_x -= cursor_x;
      // cursor_y -= cursor_y;
      cursor_x -= 960;
      cursor_y -= 504;
      target.y += cursor_y/100;
      target.z -= cursor_x/100;
      // cout<<cursor_x<<" "<<cursor_y<<endl;
      float x_dist = target.x - eye.x;
      float z_dist = target.z - eye.z;
      float theta = 180.0f - atan2(z_dist,x_dist)*180.0f/M_PI;
      int mouse_left = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
      if (mouse_left && boat.position.x == boat.fireball.position.x) {
        boat.fireball.set_position(eye.x, eye.y, eye.z);
      // aim.set_position(target.x - boat.position.x - 0.001, target.y - boat.position.y + 0.001, target.z - boat.position.z + 0.001);
        // cursor_y = -cursor_y;
        cursor_y += 504;
        boat.fireball.shoot(theta, abs(cursor_y)/500);
      }
    }
    if ( boat.health > 0) {
      if (boat.position.y > -1.68) {
        boat.speed.y -= 0.1;
      }
      else {
        boat.position.y += 0.02*sin((++j)/7);
        boat.speed.y = 0;
      }
    } else {
      boat.speed.y -= 0.1;
    }
    if (boat.position.y < -4) {
      cout<<endl<<endl<<"Final score: "<< score<<endl<<endl;
      quit(window);
    }
    for (int i = 0; i < number_of_rocks; i++) {
      if (detect_collision(boat.bounding_box(), rocks[i].bounding_box())) {
        boat.position.x += 2*cos(boat.rotation*M_PI / 180.0f);
        boat.position.z -= 2*sin(boat.rotation*M_PI / 180.0f);
        boat.health -= 1;
      }
    }

    for (int i = 0; i < number_of_monsters; i++) {
      if(detect_collision(boat.fireball.bounding_box(), monsters[i].bounding_box())) {
        monsters[i].health -= 20;
        boat.fireball.set_position(boat.position.x, boat.position.y, boat.position.z);
        // cout<<"YES\n";
        if (monsters[i].health == 0) {
          for (int j = 0; j < monsters[i].number_of_gifts; j++) {
            float temp = random_number(0, 360);
            monsters[i].gifts[j].shoot(temp, 0.7);
          }
          monster_count += 1;
          score += 20;
        }
      }
    }

    for (int i = 0; i < number_of_monsters; i++) {
      for (int j = 0; j < monsters[i].number_of_gifts; j++) {
        if(detect_collision(boat.bounding_box(), monsters[i].gifts[j].bounding_box())) {
            boat.health += 2;
            monsters[i].gifts[j].position = monsters[i].position;
        }
      }
    }

    if (monster_count % 3 == 0 && !boss_flag && monster_count != 0) {
      boss.set_position(random_number(boat.position.x - 50, boat.position.x -25 ), -5, random_number(boat.position.z -50, boat.position.z-25));
      boss.speed.y = 1;
      boss_flag = 1;
    }
    if (boss.position.y >= -1.7) {
      boss.speed.y = 0;
      boss.position.y += 0.1*cos((j/7));
    }
    if (boss_flag && detect_collision(boat.fireball.bounding_box(), boss.bounding_box())) {
      // cout<<"BOSS\n";
      boss.health -= 20;
      if (boss.health == 0) {
        // boss.speed.y = -0.5;
        booster.set_position(boss.position.x, boss.position.y, boss.position.z);
        monster_count += 1;
        boss.speed.y = -0.3;
        for (int i = 0; i < boss.number_of_gifts; i++) {
          float temp = random_number(0, 360);
          boss.gifts[i].shoot(temp, 0.7);
        }
        boss_flag = 0;
      }
    }
    unsigned long long boost_counter = 0;
    if (detect_collision(boat.bounding_box(), booster.bounding_box())) {
      // cout<<"start\n";
      boost_flag = 4;
      boost_counter = j;
      booster.set_position(100, -8, 100);
    }
    if (j - boost_counter == 1000) {
      boost_flag = 1;
    }
    if (boss_flag) {
      float x_dist = -boss.position.x + boat.position.x;
      float z_dist = -boss.position.z + boat.position.z;
      float temp_dist = sqrt(x_dist*x_dist + z_dist*z_dist);
      float theta = 180.0f - atan2(z_dist,x_dist)*180.0f/M_PI;
      // boss.speed.x += 0.2*cos(theta * M_PI / 180);
      // boss.speed.z += 0.2*sin(theta * M_PI / 180);
      boss.rotation = theta;
      // cout<<sin(2*theta*M_PI/180)<<endl;
      for (int k = 0; k < boss.number_of_fireballs; k++) {
        if (boss.fireballs[k].position.x == boss.position.x && temp_dist < 60)
          boss.fireballs[k].shoot(theta, temp_dist/30);
        if (boss.fireballs[k].position.y > -3.2 && boss.fireballs[k].speed.y != 0) {
          boss.fireballs[k].speed.y -= 0.1;
        } else {
          boss.fireballs[k].position = boss.position;
          boss.fireballs[k].speed = glm::vec3 (0, 0, 0);
        }
        if (detect_collision(boss.fireballs[k].bounding_box(), boat.bounding_box())) {
          boat.health -= 1;
          boss.fireballs[k].set_position(boss.position.x, boss.position.y, boss.position.z);
        }
      }
    }

    for (int i = 0; i < number_of_monsters && monsters[i].health != 0; i++) {
      if (j % 2 == 0) {
        float x_dist = -monsters[i].position.x + boat.position.x;
        float z_dist = -monsters[i].position.z + boat.position.z;
        float temp_dist = sqrt(x_dist*x_dist + z_dist*z_dist);
        float theta = 180.0f - atan2(z_dist,x_dist)*180.0f/M_PI;
        monsters[i].rotation = theta;
        // cout<<sin(2*theta*M_PI/180)<<endl;
        for (int k = 0; k < monsters[i].number_of_fireballs; k++) {
          if (monsters[i].fireballs[k].position.x == monsters[i].position.x && temp_dist < 60)
            monsters[i].fireballs[k].shoot(theta, temp_dist/30);
          if (monsters[i].fireballs[k].position.y > -3.2 && monsters[i].fireballs[k].speed.y != 0) {
            monsters[i].fireballs[k].speed.y -= 0.1;
          } else {
            monsters[i].fireballs[k].position = monsters[i].position;
            monsters[i].fireballs[k].speed = glm::vec3 (0, 0, 0);
          }
          if (detect_collision(monsters[i].fireballs[k].bounding_box(), boat.bounding_box())) {
            boat.health -= 1;
            monsters[i].fireballs[k].set_position(monsters[i].position.x, monsters[i].position.y, monsters[i].position.z);
          }
        }
      }
    }
    for (int i = 0; i < number_of_barrels; i++) {
      if (detect_collision(boat.bounding_box(), barrels[i].bounding_box())) {
        score += 2;
        barrels[i].set_position(random_number(-500, 500), -1.7, random_number(-500, 500));
        // cout<<"barrel\n";
      }
    }

    // cout<<boost_flag<<endl;

    char s[100];
    total_time = time(0) - start;
    sprintf(s, "Health: %d | Score: %d | Time: %d", boat.health, score, (int)total_time);
    glfwSetWindowTitle(window, s);

    color_t blue = { 185, 245, 246 }, brown = { 0, 16, 0 };
  if (((back_counter++)/1200)%2 == 0) {
    if(COLOR_BACKGROUND.r != brown.r) {
      COLOR_BACKGROUND.r--;
    }
    if(COLOR_BACKGROUND.g != brown.g) {
      COLOR_BACKGROUND.g--;
    }
    if(COLOR_BACKGROUND.b != brown.b) {
      COLOR_BACKGROUND.b--;
    }
  } else {
    if(COLOR_BACKGROUND.r != blue.r) {
      COLOR_BACKGROUND.r++;
    }
    if(COLOR_BACKGROUND.g != blue.g) {
      COLOR_BACKGROUND.g++;
    }
    if(COLOR_BACKGROUND.b != blue.b) {
      COLOR_BACKGROUND.b++;
    }
  }
  glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f);


}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    // boat       = Ball(1, -1.7, 0, 0.7, 0.2, 0.5, COLOR_RED);
    boat       = Boat(0, -1.7, 0);
    water       = Ball(0, -3, 0, 1000, 1, 1000, COLOR_BLUE);
    int count_rocks = 0;
    for (int i = 0; i < number_of_rocks; i++) {
        float x = random_number(-500, 500);
        float z = random_number(-500, 500);
        rocks[count_rocks++] = Ball(x, -1.7, z, 0.5, 0.5, 0.5, COLOR_BLACK);
    }
    aim = Aim(0, 0, 0);

    for (int i = 0; i < number_of_barrels; i++) {
      barrels[i] = Barrel(random_number(-1000, 1000), -1.7, random_number(-1000, 1000));
    }

    for (int i = 0; i < number_of_monsters; i++) {
      float x = random_number(-100, 100);
      // float y = random_number(-100, 100);
      float z = random_number(-100, 100);
      if (i % 2 == 0){
        monsters[i] = Monster(x, -0.2, z, 0.5, 0);
      } else {
        monsters[i] = Monster(x, -0.2, z, 0.7, 1);
      }
    }

    boss = Monster(2, -20, 2, 1, 2);
    booster = Ball (100, -5, 1000, 0.6, 0.6, 0.6, COLOR_YELLOW);


    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    // glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
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
    int width  = 1920;
    int height = 1080;

    ao_initialize();
    default_driver = ao_default_driver_id();
    memset(&format, 0, sizeof(format));
    format.bits = 16;
    format.channels = 2;
    format.rate = 44100;
    format.byte_format = AO_FMT_LITTLE;
    device = ao_open_live(default_driver, &format, NULL /* no options */);
    if (device == NULL) {
  		fprintf(stderr, "Error opening device.\n");
  		return 1;
  	}
    window = initGLFW(width, height);
    buf_size = format.bits/8 * format.channels * format.rate;
    bufferN = (char*)calloc(buf_size,
        sizeof(char));
    bufferH = (char*)calloc(buf_size,
        sizeof(char));
    bufferL = (char*)calloc(buf_size,
        sizeof(char));

    for (int i = 0; i < format.rate; i++) {
      sampleN = (int)(0.45 * 32768.0 *
        sin(2 * M_PI * freq * ((float) i/format.rate)));
      sampleH = (int)(0.75 * 32768.0 *
        sin(2 * M_PI * freq*2 * ((float) i/format.rate)));
      sampleL = (int)(0.75 * 32768.0 *
        sin(2 * M_PI * freq*(0.5) * ((float) i/format.rate)));

      /* Put the same stuff in left and right channel */
      bufferN[4*i] = bufferN[4*i+2] = sampleN & 0xff;
      bufferN[4*i+1] = bufferN[4*i+3] = (sampleN >> 8) & 0xff;

      bufferH[4*i] = bufferH[4*i+2] = sampleH & 0xff;
      bufferH[4*i+1] = bufferH[4*i+3] = (sampleH >> 8) & 0xff;

      bufferL[4*i] = bufferL[4*i+2] = sampleL & 0xff;
      bufferL[4*i+1] = bufferL[4*i+3] = (sampleL >> 8) & 0xff;
    }

    window = initGLFW(width, height);

    initGL (window, width, height);

    /* Draw in loop */
    start = time(0);
    ao_play(device, bufferN + (sounder++)%buf_size, 1);
    while (!glfwWindowShouldClose(window)) {
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.length + b.length)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height)) &&
           (abs(a.z - b.z) * 2 < (a.width + b.width));
}

void reset_screen() {
    float near = 0.01;
    float far = 500;
    float angle = M_PI/2;
    float aspect_ratio = float(16/9);
    float top    = screen_center_y + 4 / screen_zoom;
    float bottom = screen_center_y - 4 / screen_zoom;
    float left   = screen_center_x - 4 / screen_zoom;
    float right  = screen_center_x + 4 / screen_zoom;

    // float aspect_ratio = 1.0f;
    if (aim_flag)
      Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);
    else
      Matrices.projection = glm::perspective(angle, aspect_ratio, near, far);
}
