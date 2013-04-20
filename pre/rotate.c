#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

// Global Vars
static GLfloat spinX = 0.0;    // X rotation
static GLfloat spinZ = 0.0;    // Z rotation

static int AniOn = 0;          // Animation Flag 
                               //    0 - Animation Off
                               //    1 - Animation On

// Color Structure
typedef struct {
  GLfloat r;     // Red
  GLfloat g;     // Green
  GLfloat b;     // Blue
} rgb;


// Colors
static rgb skyBlue = {0.529, .807, 1.0};
static rgb offWhite = {0.87, 0.87, 0.87};
static rgb yellow = {1.0, 1.0, 0.};
static rgb aqua = {127.0/255.0, 1.0, 212.0/255.0};
static rgb seaGreen = {0.329, 1.0, 0.6};
static rgb lavenderBlush = {1.0, 240.0/255.0, 245.0/255.0};
static rgb honeydew = {0.941, 1.0, 0.941};
static rgb coral = {1.0, 114.0/255.0, 86.0/255.0};
static rgb magenta = {1.0, 0.0, 1.0};
static rgb navajoWhite = {1.0, 222.0/255.0, 173.0/255.0};
static rgb blue = {0.0, 0.0, 1.0};
static rgb cadetBlue = {152.0/255.0, 245.0/255.0, 1.0};
static rgb green = {0.0, 1.0, 0.0};
static rgb olive = {192.0/255.0, 1.0, 62.0/255.0};
static rgb khaki = {1.0, 246.0/255.0, 143.0/255.0};
static rgb gold = {1.0, 215.0/255.0, 0.0};


// Generate the scene to Display
void display() {
  glClear(GL_COLOR_BUFFER_BIT);               // Clear the Background

  glPushMatrix();                             // Save Matrixes
  glRotatef(spinX, 1.0, 0.0, 0.0);
  glRotatef(spinZ, 0.0, 0.0, 1.0);

  glPolygonMode(GL_FRONT, GL_FILL);
  glPolygonMode(GL_BACK, GL_LINE);

  glBegin(GL_POLYGON); {                        // Create a 5 sided polygon
                                                // Each vertex has a
                                                // different color

                                               // OpenGL uses the convention that a 
                                               // polygon generated in counterclockwise 
                                               // order in oriented such that it is 
                                               // facing you.

    glColor3f(skyBlue.r, skyBlue.g, skyBlue.b);
    glVertex3f(-0.5, +0.95, 0.0); 
    glColor3f(blue.r, blue.g, blue.b);
    glVertex3f(-0.2, +0.75, 0.0); 
    glColor3f(seaGreen.r, seaGreen.g, seaGreen.b);
    glVertex3f(-0.05, +0.35, 0.0); 
    glColor3f(aqua.r, aqua.g, aqua.b);
    glVertex3f(-0.45, +0.05, 0.0); 
    glColor3f(coral.r, coral.g, coral.b);
    glVertex3f(-0.85, +0.4, 0.0); 
    glColor3f(magenta.r, magenta.g, magenta.b);
    glVertex3f(-0.95, +0.85, 0.0); 
  } glEnd();

  glPopMatrix();                              // Restore Matrixes
  glutSwapBuffers();                          // Swap Buffers
  glFlush();                                  // Force GL Command to complete
}

// A rather standard init taken for exising code
void init() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);        // Orthographic Perspective
                                                   //    left   -1.0  -x
                                                   //    right   1.0  +x
                                                   //    bottom -1.0  -y
                                                   //    top     1.0  +y
                                                   //    near   -2.0  -z
                                                   //    far     2.0  +z

  glClearColor(yellow.r, yellow.g, yellow.b, 0.0); // Background Color
  glShadeModel(GL_SMOOTH);                         // Smooth Shading Model
}

// Callback used when resizing window
void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);  // Orthographic Perspective
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}
 

void spinZDisplay() {
  spinZ = spinZ + 0.8;             // Z rotation by 0.8 degrees
  if (spinZ > 360.0)
    spinZ = spinZ - 360.0;

  glutPostRedisplay();             // Redisplay the scene
}

void spinTZDisplay(int k) {
  if (k == 0) return;              // Skip it if k is 0

  spinZ = spinZ + 1.0;
  if (spinZ > 360.0)
    spinZ = spinZ - 360.0;
  glutTimerFunc(100, spinTZDisplay, AniOn);  // Set Timer 100msec
  glutPostRedisplay();
}

void spinXDisplay() {
  spinX = spinX + 0.8;              // X rotation by 0.8 degrees
  if (spinX > 360.0)
    spinX = spinX - 360.0;
  glutPostRedisplay();              // Redisplay the scene
}

void spinTXDisplay(int k) {
  if (k == 0) return;

  spinX = spinX + 1.0;
  if (spinX > 360.0)
    spinX = spinX - 360.0;
  glutTimerFunc(100, spinTXDisplay, AniOn);
  glutPostRedisplay();
}


// Callback used when pressing or releasing a mouse button
// button is one of
//        GLUT_LEFT_BUTTON
//        GLUT_MIDDLE_BUTTON
//        GLUT_RIGHT_BUTTON
// state is one of
//        GLUT_UP
//        GLUT_DOWN
// x,y give the position on the screen
void mouse(int button, int state, int x, int y) {
  printf("Mouse %d", AniOn);

  switch(button) {
  case GLUT_LEFT_BUTTON:
    printf(" LEFT \n");
                                                   // if Animation Off
                                                   // Then
    if ((AniOn==0) && (state == GLUT_DOWN)) {
      AniOn = 1;                                   // Animation On

      // glutIdleFunc(spinXDisplay);               // call spinXDisplay
                                                   //  when idle

      glutTimerFunc(100, spinTXDisplay, AniOn);    // call spinTXDisplay
                                                   //  in 100 msec
                                                   //  with Animation On
    }
    else                                           // Else
                                                   // if Left Button Up

      if (state == GLUT_UP) {
	AniOn = 0;                                 // Animation Off

	// glutIdleFunc(NULL);                     // No idle function

	glutTimerFunc(100, spinTXDisplay, AniOn); // call spinTXDisply
	                                          //  in 100 msec
                                                  //  with Animation Off
      }
    break;
  case GLUT_RIGHT_BUTTON:
    printf(" RIGHT \n");
    if ((AniOn==0) && (state == GLUT_DOWN)) {
      AniOn = 1;
      // glutIdleFunc(spinZDisplay);
       glutTimerFunc(100, spinTZDisplay, AniOn);
    }
    else
      if (state == GLUT_UP) {
	AniOn = 0;
	// glutIdleFunc(NULL);
	 glutTimerFunc(100, spinTZDisplay, AniOn);
      }
    break;
  case GLUT_MIDDLE_BUTTON:                    // Reset the scene
    if (state == GLUT_DOWN) {
      spinX = 0.0;
      spinZ = 0.0;
      glutPostRedisplay();
    }

    break;
  default:
    break;
  }
  fflush(stdout);                             // Force output to stdout

}

int main(int argc, char** argv) {
  glutInit(&argc, argv);                  // Init the GLUT Library
  // Set the mode for the window to be opened
  // Modes
  //   GLUT_RGB same as GLUT_RGBA
  //   GLUT_RGBA use RGBA mode for color vs using index color mode
  //   GLUT_SINGLE use a single color buffer
  //   GLUT_DOUBLE use two color buffers for double buffering
  //   GLUT_DEPTH  setup a depth buffer for hidden line removal
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);  

  // Set the size of the window in pixels; width then height
  glutInitWindowSize(350, 350);

  // Set the position in pixels; x the y
  // The origin of the window is at the upper left corner.
  // Use glutFullScreen() to make the window full screen
  glutInitWindowPosition(100, 100);

  // Create a top-level window, the name should be placed on the
  // title bar of the window
  glutCreateWindow("Rotate - The Adventure Continues");

  // Init function written by developer
  init();

  // Set a callback function to draw what will be displayed on the screen
  // The display function must redraw the scene from scratch each time it
  // is envoked.
  glutDisplayFunc(display);

  // Set a callback function for when the window is resized.
  glutReshapeFunc(reshape);

  // Set a callback fucntion for when a mouse button is 
  // pressed or released.
  // See the callback function mouse
  glutMouseFunc(mouse);

  // Invoke an event loop
  // The event loop will run until the program is terminated.
  // The event loop handle envoking callback function as necessary
  // based upon events it receives.
  glutMainLoop();
  return 0;
}
