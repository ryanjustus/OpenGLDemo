#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>

/*
  Hidden Surface Removal Steps:
  1) Add GLUT_DETH to glutInitDisplayMode
  2) Call glEnable(GL_DEPTH_TEST) after window is created
  3) Add GL_DEPTH_BUFFER_BIT to glClear

  Lighting Steps:
  1) Define normal vectors
     glutSolidSphere does this for us!
  2) Create, and position light sources
     glLight*
     glEnable
  3) Create and select lighting model
     
  4) Define material properties
*/

typedef struct {
  GLfloat r;
  GLfloat g;
  GLfloat b;
} rgb;


static rgb green = {0.0, 1.0, 0.0};
static rgb violet = {238.0/255.0, 130.0/255.0, 238.0/255.0};
static rgb olive = {192.0/255.0, 1.0, 62.0/255.0};
static rgb khaki = {1.0, 246.0/255.0, 143.0/255.0};
static rgb gold = {1.0, 215.0/255.0, 0.0};
static rgb black = {0.0, 0.0, 0.0};
static rgb white = {1.0, 1.0, 1.0};
static rgb yellow = {1.0, 1.0, 0.0};
static rgb red = {1.0, 0.0, 0.0};     // Sun
static rgb paleGreen = {0.6, 0.9, 0.6};
static rgb deepSkyBlue = {0.0, 0.74, 1.0};
static rgb lightSkyBlue = {100.0/255.0, 226.0/255.0, 1.0};
static rgb offWhite = {0.87, 0.87, 0.87};
static rgb antiqueWhite = {1.0, 0.93, 0.83};
static rgb peachPuff = {0.93, 0.81, 0.678};
static rgb azure = {0.94, 0.93, 0.93};
static rgb coral = {1.0, 0.5, 0.3};
static rgb purple = {0.62, 0.125, 0.94};
static rgb aqua = {127.0/255.0, 1.0, 212.0/255.0};
static rgb seaGreen = {0.329, 1.0, 0.6};
static rgb lavenderBlush = {1.0, 240.0/255.0, 245.0/255.0};
static rgb honeydew = {0.941, 1.0, 0.941};


GLfloat lightPosition0[] = {-0.75, 1.0, 2.0, 1.0}; 
GLfloat lightAmbient0[] =  {0.0, 0.0, 0.0, 1.0}; // Default
GLfloat lightDiffuse0[] =  {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat lightSpecular0[] = {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat spot_direction[] = {0.0, 0.0, -1.0};
int SPOT_LIGHT = 0;

GLfloat globalAmbientLight[] = {0.15, 0.15, 0.15, 1.0}; 


GLfloat emit[] = {0.0, 0.0, 0.0, 1.0};
GLfloat emitDefault[] = {0.0, 0.0, 0.0, 1.0};


static GLfloat p1 = 0.0;   // Position of planet 1
static GLfloat p2 = 0.0;
static GLfloat p3 = 0.0;
static GLfloat p4 = 0.0;
static GLfloat p5 = 0.0;
static GLfloat p6 = 0.0;   // Postion of planet 6


static double d2r = 3.14159/180.0;  // Degree to radian conversion

void init() {
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

}

GLfloat speed = 2.0;    // Global speed control
/*
  Function to move planets.
  Planets always move around the Sun from
  0 to 360 degrees.
  
  Each planet has as a unique speed at which is move.
  p# = p# + rateOfSpeed
*/
void move(int i) {
  p1 = (p1 + 1.1*speed);
  if (p1 > 360.0)
    p1 = p1 - 360.0;

  p2 = (p2 + 0.81*speed);
  if (p2 > 360.0)
    p2 = p2 - 360.0;

  p3 = (p3 + 0.51*speed);
  if (p3 > 360.0)
    p3 = p3 - 360.0;


  p4 = (p4 + 0.31*speed);
  if (p4 > 360.0)
    p4 = p4 - 360.0;

  p5 = (p5 + 0.25*speed);
  if (p5 > 360.0)
    p5 = p5 - 360.0;

  p6 = (p6 + 0.17*speed);
  if (p6 > 360.0)
    p6 = p6 - 360.0;

  glutPostRedisplay();      
}


void display() {
  GLfloat cx, cy, cz = 0.0;      // X, Y, Z position of planet
  int i;

  glClear(GL_COLOR_BUFFER_BIT | 
	  GL_DEPTH_BUFFER_BIT);  // clear color and depth buffer


  glEnable(GL_COLOR_MATERIAL);   // Using glColorMaterial allows planets
                                 // to just use the color set by
                                 // glColor as a material setting

  glPushMatrix();                   // Sun
  emit[0] = red.r;
  emit[1] = red.g;
  emit[2] = red.b;
  glMaterialfv(GL_FRONT_AND_BACK, 
	       GL_EMISSION, emit);
  glutSolidSphere(0.20, 20, 16); 
  emit[0] = 0.0;
  emit[1] = 0.0;
  emit[2] = 0.0;
  glMaterialfv(GL_FRONT_AND_BACK, 
	       GL_EMISSION, emit); // Reset Emission
  glPopMatrix();


  glPushMatrix();                       // Planet 1

                                        // Compute position along an ellipse as:
                                        // x = xc + (a * cos(degree))
                                        // y = yc + (b * sin(degree))

                                        // xc, xy are center of ellipse on
                                        //        both x and y axis
                                        // a is radius along x axis of ellipse
                                        // b is radius along y axis of ellipse 
                                        // degree is 0...360, 

  cx = 0.0 + (0.5 * cos(p1*d2r));       // x postion of planet
  cy = 0.0 + (1.0 * sin(p1*d2r));       // y position of planet
  glTranslatef(cx, cy, cz);             // move to location of planet
  glColor3f(lightSkyBlue.r,             // Color the planet
	    lightSkyBlue.g, 
	    lightSkyBlue.b);
  glutSolidSphere(0.13, 20, 16);        // Make the planet
                                        // 0.13 = radius
                                        // 20 = slices (vertical)
                                        // 16 = stackes (horizontal)
  glPopMatrix();

  glPushMatrix();                       // Planet 2
  cx = 0.0 + (1.0 * cos(p2*d2r));
  cy = 0.0 + (1.5 * sin(p2*d2r));
  glTranslatef(cx, cy, cz);
  glColor3f(green.r, green.g, green.b); 
  glutSolidSphere(0.15, 20, 16); 
  glPopMatrix();

  glPushMatrix();                       // Planet 3
  cx = 0.0 + (1.5 * cos(p3*d2r));
  cy = 0.0 + (2.0 * sin(p3*d2r));
  glTranslatef(cx, cy, cz);
  glColor3f(white.r, white.g, white.b); 
  glutSolidSphere(0.14, 20, 16); 
  glPopMatrix();

  glPushMatrix();                       // Planet 4
  cx = 0.0 + (2.0 * cos(p4*d2r));
  cy = 0.0 + (2.5 * sin(p4*d2r));
  glTranslatef(cx, cy, cz);
  glColor3f(yellow.r, yellow.g, yellow.b);
  glutSolidSphere(0.14, 20, 16); 
  glPopMatrix();


  glPushMatrix();                       // Planet 5
  cx = 0.0 + (2.5 * cos(p5*d2r));
  cy = 0.0 + (3.0 * sin(p5*d2r));
  glTranslatef(cx, cy, cz);
  glColor3f(violet.r, violet.g, violet.b);
  glutSolidSphere(0.15, 20, 16); 
  glPopMatrix();


  glPushMatrix();                       // Planet 6
  cx = 0.0 + (3.0 * cos(p6*d2r));
  cy = 0.0 + (3.5 * sin(p6*d2r));
  glTranslatef(cx, cy, cz);
  glColor3f(coral.r, coral.g, coral.b);
  glutSolidSphere(0.19, 20, 16); 
  glPopMatrix();


  if (SPOT_LIGHT) {
    glPushMatrix();                       // Spot Light Direction
    glTranslatef(spot_direction[0], spot_direction[1], spot_direction[2]);
    glColor3f(0.0, 1.0, 0.0);
    glutSolidSphere(0.45, 20, 16); 
    glPopMatrix();

  }



  glDisable(GL_COLOR_MATERIAL);         // Turn off glColorMaterial

  glutSwapBuffers();
  glutTimerFunc(70, move, 0);
}



void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, (GLfloat)w/(GLfloat)h, 0.5, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void special(int key, int x, int y) {

  switch(key) {
  case GLUT_KEY_UP:
    spot_direction[1] += 0.1;
    break;
  case GLUT_KEY_DOWN:
    spot_direction[1] -= 0.1;
    break;
  case GLUT_KEY_LEFT:
    spot_direction[0] -= 0.1;
    break;
  case GLUT_KEY_RIGHT:
    spot_direction[0] += 0.1;
    break;
  default:
    break;
  }

  /*  glLoadIdentity();
      gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); */
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
  printf("Spot Light at (%f, %f, %f)\n", 
	 spot_direction[0], 
	 spot_direction[1],
	 spot_direction[2]);  
	 

}

void keyboard(unsigned char key, int x, int y) {
  switch(key) {
  case 'l':
    // Position, Ambient, Diffuse & Specular Components are Set
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, 
		   globalAmbientLight);           // Ambient Light in Scene

    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 
		  GL_TRUE);                       // Gives more realistic result
                                                  // but is slower in computation
                                                  
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 
		  GL_FALSE);                       // Only light front faces
                                                   // We use all spheres so 
                                                   // this is OK

    // Enable Lighting and Hidden Surface Removal
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);  // Must be after window init & create

    //    glutPostRedisplay();
    break;
  case 'L':

    // Disable Lighting and Hidden Surface Removal
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);

    //    glutPostRedisplay();
    break;
  case 'A':
    // Change the Color of Global Ambient Light from black (0,0,0) to white (1,1,1)
    if (globalAmbientLight[0] >= 1.0) break;
    globalAmbientLight[0] += 0.2;
    globalAmbientLight[1] += 0.2;
    globalAmbientLight[2] += 0.2;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, 
		   globalAmbientLight);           // Ambient Light in Scene

    //    glutPostRedisplay();
    break;
  case 'a':
    if (globalAmbientLight[0] <= 0.0) break;
    globalAmbientLight[0] -= 0.2;
    globalAmbientLight[1] -= 0.2;
    globalAmbientLight[2] -= 0.2;
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, 
		   globalAmbientLight);           // Ambient Light in Scene
    //    glutPostRedisplay();
    break;
  case 'D':
    // Change the Color of Diffuse light
    if (lightDiffuse0[0] >= 1.0) break;
    lightDiffuse0[0] += 0.2;
    lightDiffuse0[0] += 0.2;
    lightDiffuse0[0] += 0.2;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    //    glutPostRedisplay();
    break;
  case 'd':
    if (lightDiffuse0[0] <= 0.0) break;
    lightDiffuse0[0] -= 0.2;
    lightDiffuse0[0] -= 0.2;
    lightDiffuse0[0] -= 0.2;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    //    glutPostRedisplay();
    break;
  case 'S':
    // Change the Color of Specular Light
    if (lightSpecular0[0] >= 1.0) break;
    lightSpecular0[0] += 0.2;
    lightSpecular0[1] += 0.2;
    lightSpecular0[2] += 0.2;
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
    //    glutPostRedisplay();
    break;
  case 's':
    if (lightSpecular0[0] <= 0.0) break;
    lightSpecular0[0] -= 0.2;
    lightSpecular0[1] -= 0.2;
    lightSpecular0[2] -= 0.2;
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);
    //    glutPostRedisplay();
    break;
  case 'b':
  case 'B':
    SPOT_LIGHT = 1;
    /*    glLoadIdentity();
	  gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); */
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 10.0);
    lightPosition0[0] = 0.0; //x
    lightPosition0[1] = 0.0; //y
    lightPosition0[2] = 1.0; //z
    //    lightPosition0[3] = 0.0; // make it a spot light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);

    break;
  default:
    break;
  }
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1150, 850);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Sun");

  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special);
  glutTimerFunc(70, move, 0);
  glutMainLoop();
  return 0;
}
