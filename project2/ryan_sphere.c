#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include <GL/glut.h>



int WINX = 800.0; //window size
int WINY = 600.0;

static GLdouble zFAR = 20;
static float PI = 3.141592653589793f;

GLfloat zoom = 8.0; //to control camera movements
GLfloat xMovement = 0.0;
GLfloat yMovement = 0.0;

typedef struct {
  GLfloat r;
  GLfloat g;
  GLfloat b;
} rgb;

static rgb red = {1.0, 0.0, 0.0};
static rgb blue = {0.0, 0.0, 1.0};

GLfloat lightPosition0[] = {-0.75, 1.0, 2.0, 1.0}; 
GLfloat lightAmbient0[] =  {0.0, 0.0, 0.0, 1.0}; // Default
GLfloat lightDiffuse0[] =  {1.0, 1.0, 1.0, 1.0}; // Default
GLfloat lightSpecular0[] = {1.0, 1.0, 1.0, 1.0}; // Default

GLfloat globalAmbientLight[] = {0.2, 0.2, 0.2, 1.0}; 

void normalise(GLfloat x,GLfloat y,GLfloat z,GLfloat normal[3]) //to normalize a vector
{
  GLfloat mag;
  if(0==z && 0==x && 0==y){
	normal[0] = 0.0;
    normal[1] = 0.0;
    normal[2] = 1.0;
	return;
  }
  mag = sqrtf((x*x)+(y*y)+(z*z));
  int thing = (int)ceil(mag);
  normal[0] = x / mag;
  normal[1] = y / mag;
  normal[2] = z / mag;
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

  float dTheta = PI/cuts;      // Cuts are long.
  float dPhi = PI/stacks;  // Stacks are lat.
 
  glPushMatrix();
  //glPolygonMode( GL_FRONT , GL_LINE); //For Wireframe sphere
  //glPolygonMode( GL_BACK, GL_LINE);

  glPolygonMode( GL_FRONT , GL_FILL);
  glPolygonMode( GL_BACK, GL_LINE);
  
	//use a triangle fan for the cap
	glBegin(GL_TRIANGLE_FAN);{
		//glNormal3f(0.0f, 0.0f, 1.0f);
		glNormal3f(0.0f,0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 1.0*radius);
		float zNormal = cosf(dPhi); //get z component of normal
		float zRadius = zNormal*radius;
		float xyFactor = sinf(dPhi); //same as sin(PI/stacks)
		float theta=0;		
		int i=0;
		for(i=0;i<=cuts*2;i++){
			float yNormal = xyFactor*sinf(theta);
			float xNormal = xyFactor*cosf(theta);
			//glNormal3f(xyFactor*xNormal,xyFactor*yNormal,zNormal);
			theta+=dTheta;
			glNormal3f(xNormal,yNormal,zNormal);
			glVertex3f(radius*xNormal,radius*yNormal,zRadius);			
		}	
		
	}glEnd();

  float phi=dPhi;
  int j;
  for(j=1;j<stacks-1;j++){
		int even = j%2;
		even ? glColor3f(red.r,red.g,red.b) : glColor3f(blue.r,blue.g,blue.b);
		float z1 = cosf(phi);
		float z2 = cosf(phi+dPhi);
		float zRadius1 = radius*z1;
		float zRadius2 = radius*z2;
		float xyFactor1 = sinf(phi);
		float xyFactor2 = sinf(phi+dPhi);
		float theta;

     //Now use triangle strip going around the (latitude)
	  glBegin(GL_TRIANGLE_STRIP);{ 
		  int i;
		  for(i=0;i<=cuts*2-1;i++){
			int even = i%2;
			//Draw four points		
			float x1 = xyFactor1*cosf(theta);
			float x2 = xyFactor2*cosf(theta);
			float y1 = xyFactor1*sinf(theta);
			float y2 = xyFactor2*sinf(theta);
			glNormal3f(x1,y1,z1);
			glVertex3f(radius*x1,radius*y1,zRadius1);
			glNormal3f(x2,y2,z2);
			glVertex3f(radius*x2,radius*y2,zRadius2);	
			
			//glVertex3f(x2,y2,z1);
			//glVertex3f(x2,y2,z2);
			theta+=dTheta;
		  }
	  }glEnd();
	  phi+=dPhi;
	}
	
	//use a triangle fan for the cap
	glBegin(GL_TRIANGLE_FAN);{
		//glNormal3f(0.0f, 0.0f, 1.0f);
		glNormal3f(0.0f,0.0f, -1.0f);
		glVertex3f(0.0f, 0.0f, -radius);
		float zNormal = -cosf(dPhi); //get z component of normal
		float zRadius = radius*zNormal;
		float xyFactor = -sinf(dPhi); //same as sin(PI/stacks)
		float theta=0;		
		int i=0;
		for(i=0;i<=cuts*2;i++){
			float yNormal = xyFactor*sinf(theta);
			float xNormal = xyFactor*cosf(theta);
			//glNormal3f(xyFactor*xNormal,xyFactor*yNormal,zNormal);
			theta+=dTheta;
			glNormal3f(xNormal,yNormal,zNormal);
			glVertex3f(radius*xNormal,radius*yNormal,zRadius);			
		}	
		
	}glEnd();
	
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
  drawSphere(4.0 , 10 , 10 );  
		
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
