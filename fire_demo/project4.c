#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include "colors.h"
#include "shapes.h"
#include "materials.h"
#include "particle_fire.h"

GLfloat lightPosition0[] = {50, 50.0, 50.0, 1.0}; 
GLfloat lightAmbient0[] =  {0.0, 0.0, 0.0, 1.0}; // Default
GLfloat lightDiffuse0[] =  {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat lightSpecular0[] = {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat globalAmbientLight[] = {0.0, 0.0, 0.0, 1.0}; 


GLfloat emit[] = {0.0, 0.0, 0.0, 1.0};
GLfloat emitDefault[] = {0.0, 0.0, 0.0, 1.0};

void init() {
  glShadeModel(GL_SMOOTH);
	glClearColor(0.0f,0.0f,0.0f,0.0f);                  // Black Background
	glClearDepth(1.0f);                         // Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);                       // Disables Depth Testing
	glEnable(GL_BLEND);                         // Enable Blending
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);                   // Type Of Blending To Perform                
	//glBindTexture(GL_TEXTURE_2D,texture[0]);    
                                                  
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
	initialize();
	fflush(stdout);
}

void move(int i) {
  glutPostRedisplay();      
}

void moveCamera(){
}

void display() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // clear color and depth buffer
  glEnable(GL_COLOR_MATERIAL);   // Using glColorMaterial allows planets
                                 // to just use the color set by
                                 // to just use the color set by
                                 // glColor as a material setting
  
  glPushMatrix();
  moveCamera();
  glPopMatrix();  
  update();
  drawParticles();
  fflush(stdout);
  glDisable(GL_COLOR_MATERIAL);         // Turn off glColorMaterial
  glutSwapBuffers();
  glutTimerFunc(30, move, 0);

}

void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(50.0, (GLfloat)w/(GLfloat)h, 0.5, 200.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 15.0, 15.0, 0.0, 5.0, 0.0, 0.0, 1.0, 0.0);
}

void keyboard(unsigned char key, int x, int y) {

}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1500, 700);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Random Stuff");

  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(16, move, 0);
  glutMainLoop();
  return 0;
}

