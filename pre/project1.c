#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "colors.h"



// Global Vars
const float PI = 3.14159265358979f;
const float g = 0.05; //Acceleration of the circle

static GLfloat spinX = 0.0;    // X rotation
static GLfloat spinZ = 0.0;    // Z rotation
static int xScreen,yScreen;		//current screen size

static int AniOn = 0;          // Animation Flag 
                               //    0 - Animation Off
                               //    1 - Animation On

//Values updated in mouse
static float startX, startY; //Actual location of the circle
static float endX,endY; //Target location of circle

//Values updated in animate
//Circle values
static float xPos, yPos; //Actual location of the circle
static float xVel,yVel;
static float time = 0.0; //Start moving the circle time
static int phaseShift; //Phase shift of circle colors
//triangle values
static float triangleZ;
//octo values
static float octoX;

//Function prototypes
void drawCircle(float x, float y, int sides, float xRadius, float yRadius, int phaseShift);
void drawOcto();
void drawTriangle();


// Generate the scene to Display
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and z-buffer
  drawOcto();
  drawTriangle();
  drawCircle(xPos, yPos, 50,0.2+xVel,0.2+yVel,phaseShift); 
  glutSwapBuffers();                          // Swap Buffers   
  glFlush();                                  // Force GL Command to complete
}

// A rather standard init taken for exising code
void init() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  /*
  glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);        // Orthographic Perspective
                                                   //    left   -1.0  -x
                                                   //    right   1.0  +x
                                                   //    bottom -1.0  -y
                                                   //    top     1.0  +y
                                                   //    near   -2.0  -z
                                                   //    far     2.0  +z

  //glClearColor(yellow.r, yellow.g, yellow.b, 0.0); // Background Color
  glShadeModel(GL_SMOOTH);                         // Smooth Shading Model
  */
  gluPerspective(45.0f,800.0f/600.0f,1.0f,500.0f);
  glMatrixMode(GL_MODELVIEW);
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);


}

float getOGLX(int xWin){
	xWin=xWin-xScreen/2;
	float newX = xWin/(float)xScreen*2.0f;
	return newX;	
}

float getOGLY(int yWin){
	yWin=(yScreen-yWin)-yScreen/2;
	float newY = yWin/(float)yScreen*2.0f;
	return newY;
}

void drawOcto(){
	glPushMatrix();
	glRotatef(octoX, 2.0, 0.0, 0.0);	
	glTranslatef(-0.4f,0.6f,0.0f);	
	glPolygonMode(GL_BACK, GL_LINE);
	glPolygonMode(GL_FRONT, GL_FILL);	
	glBegin(GL_TRIANGLE_FAN); {
		//draw the center point			
		glColor3f(neonOrange.r,neonOrange.g,neonOrange.b);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(neonGreen.r,neonGreen.g,neonGreen.b);
		glVertex3f(0.15f, 0.05f, 0.0);
		glColor3f(neonBlue.r,neonBlue.g,neonBlue.b);
		glVertex3f(0.4f, -0.15f, 0.0); 
		glVertex3f(0.15f, -0.2f, 0.0); 
		glColor3f(neonOrange.r,neonOrange.g,neonOrange.b);
		glVertex3f(-0.15f, -0.4f, 0.0); 
		glVertex3f(-0.2f, -0.15f, 0.0);
		glColor3f(neonPurple.r,neonPurple.g,neonPurple.b);
		glVertex3f(-0.4f, 0.15f, 0.0);
		glVertex3f(-0.15f, 0.4f, 0.0);
	} glEnd();
	glPopMatrix();
}

void drawTriangle(){
	glPushMatrix();	
	glRotatef(triangleZ, 0.0, 0.0, 1.0);
	glTranslatef(0.4f,-0.4f,0.0f);	
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_FILL);
	//glPolygonMode(GL_BACK, GL_LINE);
	glBegin(GL_TRIANGLE_FAN); {                     

		//draw the center point			
		glColor3f(neonGreen.r,neonGreen.g,neonGreen.b);
		glVertex3f(0.0, 0.0, 0.0);
		glColor3f(neonBlue.r,neonBlue.g,neonBlue.b);		
		glVertex3f(0.4f, -0.4f, 0.0); 
		glColor3f(neonOrange.r,neonOrange.g,neonOrange.b);
		glVertex3f(0.4f, 0.4f, 0.0);
	} glEnd();
	glPopMatrix();
}

void drawCircle(float xCenter, float yCenter, int sides, float xRadius, float yRadius, int phaseShift){
	glPushMatrix();  
	glTranslatef(xCenter,yCenter,0.0f);
	glPolygonMode(GL_FRONT, GL_FILL);
	//glPolygonMode(GL_BACK, GL_LINE);
	glBegin(GL_TRIANGLE_FAN); {                     

		//draw the center point
	glColor3f(black.r,black.g,black.b);
	glVertex3f(0.0, 0.0, 0.125f);
	int i;
	for(i=0;i<=sides;i++){		
		float angleDeg = (360.0f*i)/((float)sides);
		//get the hsv color angle
		float angleRad = angleDeg*PI/180;
		angleDeg+=phaseShift;
		while(angleDeg>360.0f){
			angleDeg-=360.0f;
		}
		//printf("SIDE: %d, ANGLE %f ANGLERAD %f\n",i,angleDeg,angleRad);
		hsv colorHsv = {angleDeg,0.8f,0.8f};
		rgb colorRgb = hsv2rgb(colorHsv);
		//printf("Color %d,%d,%d\n", colorRgb.r, colorRgb.g, colorRgb.b);
		glColor3f(colorRgb.r, colorRgb.g, colorRgb.b);
		
		float x = xRadius*cos(angleRad)*yScreen/(float)xScreen;
		float y = yRadius*sin(angleRad);
		glVertex3f(x, y, 0.125); 		
	}
	
  } glEnd();
  glPopMatrix();
  
}

// Callback used when resizing window
void reshape(int w, int h) {
  xScreen=w;
  yScreen=h;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(-1.0, 1.0, -1.0, 1.0, -2.0, 2.0);  // Orthographic Perspective
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}


//Draw the updated circle
void animateTimer(int p) {
  //calculate new position
  if(xPos!=endX || yPos!=endY){
	  //Move in the direction of endY;
	  float dx=g;
	  if(endX<startX){
		  dx*=-1;
	  }

	  float dy=g;
	  if(endY<startY){
		  dy*=-1;
	  }
	  if((endX+g)>xPos && (endX-g)<xPos){
		  xPos = endX;
		  xVel=0;
	  }else{
		xVel=-0.1;
		xPos+=dx;
	  }
	  if((endY+g)>yPos && (endY-g)<yPos){
		  yPos = endY;
		  yVel=0;
	  }else{
		if(xVel==0){
			yVel=-0.1;
		}
		yPos+=dy;
	  }
	  //increment the time
	  time++;	  
  }
  //make the circle colors spin
  phaseShift+=5;

  //do rotations
  octoX+=1.0f;
  if(octoX>360){
	octoX=0;
  }

  triangleZ+=1.5f;
  if(triangleZ>360){
	triangleZ=0;
  }

  glutPostRedisplay(); 
  glutTimerFunc(17, animateTimer,0);
  fflush(stdout);
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
  case GLUT_LEFT_BUTTON:{
    printf(" LEFT (%d,%d)\n",x,y);
	time=0.0f;
	startX = xPos;
	startY = yPos;
	endX = getOGLX(x);
	endY = getOGLY(y);
	glutPostRedisplay();
  }break;

  case GLUT_RIGHT_BUTTON:{
	//Ignore the right button
  }break;

  case GLUT_MIDDLE_BUTTON:{                    // Reset the scene
    if (state == GLUT_DOWN) {
      xPos =0;
	  yPos=0;
      glutPostRedisplay();
    }

	}break;
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
  xScreen=800;
  yScreen=800;
  glutInitWindowSize(xScreen, yScreen);

  // Set the position in pixels; x the y
  // The origin of the window is at the upper left corner.
  // Use glutFullScreen() to make the window full screen
  glutInitWindowPosition(50, 50);

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

  // Set a callback function for when a mouse button is 
  // pressed or released.
  // See the callback function mouse
  glutMouseFunc(mouse);

  // Set a callback function for the timer
  // pressed or released.
  // See the callback function mouse
  glutTimerFunc(17, animateTimer,0);

  // Invoke an event loop
  // The event loop will run until the program is terminated.
  // The event loop handle envoking callback function as necessary
  // based upon events it receives.
  glutMainLoop();
  return 0;
}
