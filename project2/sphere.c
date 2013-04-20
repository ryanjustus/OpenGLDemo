#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <math.h>


int WINX = 800.0; //window size
int WINY = 600.0;

static GLdouble zFAR = 20;

GLfloat zoom = 8.0; //to control camera movements
GLfloat xMovement = 0.0;
GLfloat yMovement = 0.0;

typedef struct {
  GLfloat r;
  GLfloat g;
  GLfloat b;
} rgb;

static rgb red = {255.0/255.0, 0.0, 0.0};

GLfloat lightPosition0[] = {-0.75, 1.0, 2.0, 1.0}; 
GLfloat lightAmbient0[] =  {0.0, 0.0, 0.0, 1.0}; // Default
GLfloat lightDiffuse0[] =  {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat lightSpecular0[] = {1.0, 1.0, 1.0, 1.0}; // Default

GLfloat globalAmbientLight[] = {0.2, 0.2, 0.2, 1.0}; 

void normalise(GLfloat x,GLfloat y,GLfloat z,GLfloat normal[3]) //to normalize a vector
{
  GLfloat mag;
  int thing;

  mag = sqrtf((x*x)+(y*y)+(z*z));

  thing = (int)ceil(mag);

  if(thing != 0)
    {
      normal[0] = x / mag;
      normal[1] = y / mag;
      normal[2] = z / mag;
    }
  else
    {
      normal[0] = 0.0;
      normal[1] = 0.0;
      normal[2] = 1.0;
    }

}

void findNormal3Points(GLfloat vector1[3],GLfloat vector2[3], GLfloat vector3[3], GLfloat normal[3])
{
  GLfloat tvector1[3],tvector2[3];
	
  //finding normal for polygon with 3 points
  //[v1-v2]X[v2-v3] -- X is the Cross Product

  //[v1-v2]
  tvector1[0] = vector1[0]-vector2[0];
  tvector1[1] = vector1[1]-vector2[1];
  tvector1[2] = vector1[2]-vector2[2];

  //[v2-v3]
  tvector2[0] = vector2[0]-vector3[0];
  tvector2[1] = vector2[1]-vector3[1];
  tvector2[2] = vector2[2]-vector3[2];

  normal[0] = (tvector1[1]*tvector2[2])-(tvector2[1]*tvector1[2]);
  normal[1] = (tvector2[0]*tvector1[2])-(tvector1[0]*tvector2[2]);
  normal[2] = (tvector1[0]*tvector2[1])-(tvector2[0]*tvector1[1]);

  normalise(normal[0],normal[1],normal[2],normal);

}

void drawSphere(GLfloat radius,int cuts,int stacks)
{
  //Equ. for a Sphere
  //x = x_0 + r*cos(phi)*sin(theta) 
  //y = y_0 + r*sin(phi)*sin(theta) 0 <= phi <= 2*pi , 0 <= theta <= pi )
  //z = z_0 + r*cos(theta) 

  int i,j;
  float DEG2RAD = 3.141592653589793/180;
  int numCut = 360/cuts;      // Cuts are long.
  int numSlice = 180/stacks;  // Stacks are lat.

  glPushMatrix();
  //glPolygonMode( GL_FRONT , GL_LINE); //For Wireframe sphere
  //glPolygonMode( GL_BACK, GL_LINE);

  glPolygonMode( GL_FRONT , GL_FILL);
  glPolygonMode( GL_BACK, GL_FILL);
	

  for(j=0;j<=360;j = numCut + j)
    {
      for(i=0;i<=180;i = numSlice + i)
	{
			
	  float theta = i*DEG2RAD;
	  float phi	= j*DEG2RAD;
	  GLfloat nor[3] = { 0.0 , 0.0 , 0.0};
	  GLfloat vector1[3],vector2[3],vector3[3],vector4[3],normal[3] = {0.0 , 0.0 , 0.0};
				
	  vector1[0] = radius*(cosf(phi)*sinf(theta));
	  vector1[1] = radius*(sinf(phi)*sinf(theta));
	  vector1[2] = radius*(cosf(theta));
			
	  phi	= (j+numCut)*DEG2RAD;  
	  vector2[0] = radius*(cosf(phi)*sinf(theta));
	  vector2[1] = radius*(sinf(phi)*sinf(theta));
	  vector2[2] = radius*(cosf(theta));
			
	  theta = (i+numSlice)*DEG2RAD;

	  vector3[0] = radius*(cosf(phi)*sinf(theta));
	  vector3[1] = radius*(sinf(phi)*sinf(theta));
	  vector3[2] = radius*(cosf(theta));

	  phi	= j*DEG2RAD;
	  vector4[0] = radius*(cosf(phi)*sinf(theta));
	  vector4[1] = radius*(sinf(phi)*sinf(theta));
	  vector4[2] = radius*(cosf(theta));
			
	  findNormal3Points(vector1,vector2,vector3,nor);
				
	  glBegin(GL_TRIANGLES);
	  {
				
	    glNormal3fv(nor);
	    glVertex3fv(vector1);
				
	    glNormal3fv(nor);
	    glVertex3fv(vector2);

	    glNormal3fv(nor);
	    glVertex3fv(vector3);
	  }
	  glEnd();
				
	  findNormal3Points(vector3,vector2,vector4,nor);

	  glBegin(GL_TRIANGLES);
	  {
				
	    glNormal3fv(nor);
	    glVertex3fv(vector1);
				
	    glNormal3fv(nor);
	    glVertex3fv(vector4);

	    glNormal3fv(nor);
	    glVertex3fv(vector3);
	  }

	  glEnd();
	}
    }
  glPopMatrix();
}

void display(void) 
{

  glClear(GL_COLOR_BUFFER_BIT | 
	  GL_DEPTH_BUFFER_BIT);  // clear color and depth buffer

  glEnable(GL_COLOR_MATERIAL);
	
  glPushMatrix();
	
  glColor3f(red.r,red.g,red.b);
  glTranslatef(0.0,0.0,0.0);
  drawSphere(4.0 , 45 , 45 );  
		
  glPopMatrix();
	
  glutSwapBuffers();
}

void reshape(int w, int h) 
{
  WINX = w;
  WINY = h;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, (GLfloat)w/(GLfloat)h, 0.5, zFAR);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(xMovement, yMovement, zoom, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void specialKeys(int key, int x, int y)
{
  switch(key)
    {
    case GLUT_KEY_LEFT:
      {
	xMovement = xMovement - .1;
	reshape(WINX,WINY);
	break;
      }
    case GLUT_KEY_RIGHT:
      {
	xMovement = xMovement + .1;
	reshape(WINX,WINY);
	break;
      }
    case GLUT_KEY_UP:
      {
	yMovement = yMovement + .1;
	reshape(WINX,WINY);
	break;
      }
    case GLUT_KEY_DOWN:
      {
	yMovement = yMovement - .1;
	reshape(WINX,WINY);
	break;
      }
    }
}
void keyboard(unsigned char key, int x, int y) 
{
	
  switch(key)
    {
    case 27:
      {
	exit(0);
	break;
      }
    case '=':
      {
	zoom = zoom + 0.1;
	if(zoom >= zFAR )
	  zoom = zFAR;
	reshape(WINX,WINY);
	break;
      }
    case '-':
      {
	zoom = zoom - 0.1;
	if(zoom <= 0.0 )
	  zoom = 0.0;
	reshape(WINX,WINY);
	break;
      }
    case 'a':
      {
	xMovement = xMovement - .1;
	reshape(WINX,WINY);
	break;
      }
    case 'd':
      {
	xMovement = xMovement + .1;
	reshape(WINX,WINY);
	break;
      }
    case 'w':
      {
	yMovement = yMovement + .1;
	reshape(WINX,WINY);
	break;
      }
    case 's':
      {
	yMovement = yMovement - .1;
	reshape(WINX,WINY);
	break;
      }
    case ' ':
      {
	yMovement = 0.0; //recenter the camera
	xMovement = 0.0;
	zoom = 20.0;
	reshape(WINX,WINY);
	break;
      }
    }

  glutPostRedisplay();
}

void lighting()
{
  // Position, Ambient, Diffuse & Specular Components are Set
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);

  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
	
  // Enable Lighting and Hidden Surface Removal
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);  // Must be after window init & create


  glutPostRedisplay();
}
void init() 
{

  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_SMOOTH);

}




int main(int argc,char ** argv)
{	
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(WINX,WINY);
  glutInitWindowPosition(20, 50);
  glutCreateWindow("Sphere Program by Ryan Dutson");

  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  lighting();
  glutMainLoop();
  return 0;
}
