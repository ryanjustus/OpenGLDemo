#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "colors.h"
#include "shapes.h"
typedef struct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} material;
material goldMat = {0.24725, 0.1995, 0.0745, 1.0,
		 0.75164, 0.60648, 0.22648, 1.0, 
		 0.628281, 0.555802, 0.366065, 1.0,
		 51.2};
		 
material blackRubber = {0.02, 0.02, 0.02,  1.0, 
			0.01, 0.01, 0.01, 1.0,
			0.40, 0.40, 0.40, 1.0,
			10.0};

GLfloat lightPosition0[] = {50, 50.0, 50.0, 1.0}; 
GLfloat lightAmbient0[] =  {0.0, 0.0, 0.0, 1.0}; // Default
GLfloat lightDiffuse0[] =  {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat lightSpecular0[] = {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat spot_direction[] = {0.0, 0.0, -1.0};

GLfloat lightPosition1[] = {0, 0, 0, 1.0}; 
GLfloat lightAmbient1[] =  {0.0, 0.0, 0.0, 1.0}; // Default
GLfloat lightDiffuse1[] =  {0.1, 0.1, 0.1, 1.0}; // Default
GLfloat lightSpecular1[] = {0.5, 0.5, 0.5, 1.0}; // Default

int SPOT_LIGHT = 0;

GLfloat globalAmbientLight[] = {0.0, 0.0, 0.0, 1.0}; 


GLfloat emit[] = {0.0, 0.0, 0.0, 1.0};
GLfloat emitDefault[] = {0.0, 0.0, 0.0, 1.0};
float cylRotate = 0.0f;
float cylRotateDelta = 1.0f;

float o2Orbit = 2.0f;
float co2Orbit = 50.0f;
float hOrbit = 120.0f;

float cameraPhi=0;
float cameraPhiDelta=0.1f;
float cameraZoom=1;
float cameraZoomDelta=0.001;


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
                                                  
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 
		  GL_FALSE);                       // Only light front faces
                                                   // We use all spheres so 
                                                   // this is OK

    // Enable Lighting and Hidden Surface Removal
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);  // Must be after window init & create
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 
		  GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, 
		   globalAmbientLight);
		   
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);

  // Enable Second Light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);  // Must be after window init & create
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 
		  GL_TRUE);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, 
		   globalAmbientLight);
	glColor3f ( red.r, red.g, red.b ) ;	   
    glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lightAmbient1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse1);
    glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);	

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
  glutPostRedisplay();      
}

void moveCamera(){
	glLoadIdentity();
	cameraPhi+=cameraPhiDelta;
	if(cameraPhi>100 || cameraPhi<0){
		cameraPhiDelta*=-1;
	}
	cameraZoom+=cameraZoomDelta;
	if(cameraZoom<0.10 || cameraZoom>1.0){
		cameraZoomDelta*=-1;
	}
	gluLookAt(cameraZoom*(cameraPhi-50), cameraZoom*cameraPhi, cameraZoom*(cameraPhi-100), 0.0, 0.0, 0.0, 0.0,0.0 , 1);
}


void display() {
  GLfloat cx, cy, cz = 0.0;      // X, Y, Z position of planet
  int i;

  glClear(GL_COLOR_BUFFER_BIT | 
	  GL_DEPTH_BUFFER_BIT);  // clear color and depth buffer


  glEnable(GL_COLOR_MATERIAL);   // Using glColorMaterial allows planets
                                 // to just use the color set by
                                 // to just use the color set by
                                 // glColor as a material setting
  
  glPushMatrix();
  moveCamera();
  
  glPushMatrix();                   // Sun  
  
  emit[0] = red.r;
  emit[1] = red.g;
  emit[2] = red.b;
  //glutSolidSphere(0.20, 20, 16);
  glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);    
  drawBuckyball(1.0f,2.0f);	//this is my sun
  glTranslatef(0.0f,-10.0f,0.0f);	//Move coords from middle to offset sun

  emit[0] = 0.0;
  emit[1] = 0.0;
  emit[2] = 0.0;
  glMaterialfv(GL_FRONT_AND_BACK, 
	       GL_EMISSION, emit); // Reset Emission
  glPopMatrix();

  glPushMatrix();{//Draw planets 
	    
	    //draw o2
		glPushMatrix();{  
		  o2Orbit+=0.007f;
			if(o2Orbit>360){
				o2Orbit=0;
			}
		  
		  glTranslatef(20.0f*sin(o2Orbit/2*PI),0.0f, 15.0f*cos(o2Orbit/2*PI));
			cylRotate+=cylRotateDelta;
			if(cylRotate>360){
				cylRotate=0;
			}	
		  glRotatef(3.0f*cylRotate, 0.0, 1.0, 1.0);
		  float cylHeight  = 5.0f;
		  
		  drawCylinder(0.2f, 4.0f, neonGreen, neonBlue);
		  drawSphere(1.0f,10,12, blue, green);	
		  glPushMatrix();
			glTranslatef(0.0f, cylHeight, 0.0f);
			drawSphere(1.0f,10,12, green, blue);
		  glPopMatrix();
		  
		  glPushMatrix(); //Draw the moon
			glTranslatef(0.0f, 0.5f*cylHeight, 0.0f);
			glTranslatef(4.0f*sin(10*o2Orbit/(2*PI)),4*cos(10*o2Orbit/(2*PI)), 2.0f*cos(20*o2Orbit/(2*PI)));
			drawSphere(0.8, 2, 2, purple,white);	
		  glPopMatrix(); 
		
		}glPopMatrix();
		
		
		glPushMatrix();{ //draw c02
		
			co2Orbit-=0.005f;
			if(co2Orbit>360){
				co2Orbit=0;
			}	  
			glTranslatef(46.0f*sin(co2Orbit/2*PI),0.0f, 25.0f*cos(co2Orbit/2*PI));
			glRotatef(150*co2Orbit,0,1,0);
			glRotatef(100*co2Orbit,1,0,0);
			float cylHeight  = 5.0f;

			drawSphere(1.0f,8,8, yellow, aqua);	//Center sphere
							
			glRotatef(50, 0.0, 0.0, 1.0);
			drawCylinder(0.2f, cylHeight, aqua, yellow);		
			glPushMatrix();
				glTranslatef(0.0f, cylHeight, 0.0f);
				drawSphere(1.0f,8,8, neonGreen, neonBlue);
					//draw a moon
					glPushMatrix();{
						glRotatef(500*co2Orbit,0,1,0);
						glTranslatef(1.0f, -3.0f, 1.0f);
						glRotatef(500*co2Orbit,1,1,0);
						drawCylinder(0.1f, cylHeight/2, coral, seaGreen);
						drawSphere(0.5f,8,8, coral, seaGreen);	
						glPushMatrix();
						glTranslatef(0.0f, cylHeight/2, 0.0f);
						drawSphere(0.5f,8,8, coral, seaGreen);
						glPopMatrix();
					}glPopMatrix();					
			glPopMatrix();
			glRotatef(-100, 0.0, 0.0, 1.0);
			drawCylinder(0.2f, 4.0f, aqua, yellow);
			glPushMatrix();
				glTranslatef(0.0f, cylHeight, 0.0f);
				//draw a moon
				glPushMatrix();{					
					glRotatef(-500*co2Orbit,0,1,0);
					glTranslatef(3.0f, -3.0f, 3.0f);					
					glRotatef(-500*co2Orbit,1,1,0);
					drawCylinder(0.1f, cylHeight/2, red, coral);
					drawSphere(0.5f,3,4, offWhite, red);	
					glPushMatrix();
					glTranslatef(0.0f, cylHeight/2, 0.0f);
					drawSphere(0.5f,3,4, coral, red);
					glPopMatrix();
				}glPopMatrix();	
				drawSphere(1.0f,8,8, neonBlue, neonGreen);
			glPopMatrix();
		
		}glPopMatrix();
			
		glPushMatrix();{ //draw h
			  glMaterialfv(GL_FRONT, GL_AMBIENT, blackRubber.ambient);         
			  glMaterialfv(GL_FRONT, GL_DIFFUSE, blackRubber.diffuse);         
			  glMaterialfv(GL_FRONT, GL_SPECULAR, blackRubber.specular);         
			  glMaterialf(GL_FRONT, GL_SHININESS, blackRubber.shininess);  		
			hOrbit+=0.06f;
			if(hOrbit>360){
				hOrbit=0;
			}	
			glRotatef(40.0f,0,0,1); //tilt the orbital plane of this planet			
			glTranslatef(70.0f*sin(hOrbit/(2*PI)),0.0f, 60.0f*cos(hOrbit/(2*PI)));	
			glPushMatrix();
				glRotatef(50.0f*hOrbit, 0.0, 1.0, 0.0);	
				drawSphere(7.0f,14,14, neonPurple, neonGreen);	//Center sphere					
			glPopMatrix();
			
			glPushMatrix();
				glTranslatef(10.0f*sin(hOrbit*0.001/(2*PI)), 0.0f, 12.0*cos(hOrbit*0.001/(2*PI)));
				glRotatef(hOrbit, 1.0, 0.0, 0.0);	//rotate the moon
				drawStaticBucky(0.4, 0.4, azure, violet);
			glPopMatrix();
			glPushMatrix();
				//glTranslatef(20.0f*sin(hOrbit*2/(2*PI)), 0.0f, 20.0*cos(hOrbit*2/(2*PI)));
				//glRotatef(-0.01*hOrbit, 0.0, 1.0, 0.0);	//rotate the moon
				drawStaticBucky(3.0, 1.0, black, black);
			glPopMatrix();
		}glPopMatrix();
		
	}glPopMatrix(); //End draw planets
  glPopMatrix();
  
  glDisable(GL_COLOR_MATERIAL);         // Turn off glColorMaterial

  glutSwapBuffers();
  glutTimerFunc(21, move, 0);
}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, (GLfloat)w/(GLfloat)h, 0.5, 200.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 50.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
}

void keyboard(unsigned char key, int x, int y) {

}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1500, 700);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Sun");

  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(16, move, 0);
  glutMainLoop();
  return 0;
}
