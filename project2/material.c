#include <windows.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
  GLfloat r;
  GLfloat g;
  GLfloat b;
} rgb;


static rgb green = {0.0, 1.0, 0.0};
static rgb violet = {238.0/255.0, 130.0/255.0, 238.0/255.0};
static rgb olive = {192.0/255.0, 1.0, 62.0/255.0};
static rgb khaki = {1.0, 246.0/255.0, 143.0/255.0};
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


typedef struct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} material;

material silver = {0.19225, 0.19225, 0.19225, 1.0,
		   0.50754, 0.50754, 0.50754, 1.0,
		   0.508273, 0.508273, 0.508273, 1.0,
		   51.2};

material blackRubber = {0.02, 0.02, 0.02,  1.0, 
			0.01, 0.01, 0.01, 1.0,
			0.40, 0.40, 0.40, 1.0,
			10.0};

material blackPlastic = {0, 0, 0, 1.0,
			 0.01, 0.01, 0.01, 1.0,
			 0.5, 0.5, 0.5, 1.0,
			 32.0};

material ruby = {0.1745, 0.01175, 0.01175, 1.0,
		 0.61424, 0.04136, 0.04136, 1.0,
		 0.727811, 0.626959, 0.626959, 1.0,
		 76.8};


material brass = {0.329412, 0.223529, 0.027451, 1.0,
		  0.780392, 0.568627, 0.113725, 1.0,
		  0.992157, 0.941176, 0.807843, 1.0,
		  27.8974 };

material emerald = {0.0215, 0.1745, 0.0215, 1.0,
		    0.07568, 0.61424,  0.07568, 1.0, 
		    0.633, 0.727811, 0.633, 1.0,
		    76.8};

material whitePlastic = {0.0, 0.0, 0.0, 1.0,
			 0.55, 0.55, 0.55, 1.0,
			 0.70, 0.70, 0.70, 1.0,
			 32.0};

material pearl = {0.25, 0.20725, 0.20725, 1.0,
		  1.0, 0.829, 0.829, 1.0,
		  0.296648, 0.296648, 0.296648, 1.0,
		  11.264};


material jade = {0.135, 0.2225, 0.1575, 1.0,
		 0.135, 0.2225, 0.1575, 1.0,
		 0.316228, 0.316228, 0.316228, 1.0,
		 12.18};

material gold = {0.24725, 0.1995, 0.0745, 1.0,
		 0.75164, 0.60648, 0.22648, 1.0, 
		 0.628281, 0.555802, 0.366065, 1.0,
		 51.2};

material pewter = {0.105882, 0.058824, 0.113725, 1.0,
		   0.427451, 0.470588, 0.541176, 1.0,
		   0.333333, 0.333333, 0.521569, 1.0,
		   9.84615};


material chrome = {0.25, 0.25, 0.25, 1.0,
		   0.4, 0.4, 0.4, 1.0,
		   0.774597, 0.774597, 0.774597, 1.0,
		   76.8};

material copper = {0.19125, 0.0735, 0.0225, 1.0,
		   0.7038, 0.27048, 0.0828, 1.0,
		   0.256777, 0.137622, 0.086014, 1.0,
		   12.8};



GLfloat lightPosition0[] = {0.0, 0.0, 2.0, 1.0}; 
GLfloat lightAmbient[] =  {0.0, 0.0, 0.0, 1.0}; 
GLfloat lightDiffuse[] =  {1.0, 1.0, 1.0, 1.0}; 
GLfloat lightSpecular[] = {1.0, 1.0, 1.0, 1.0}; 
GLfloat lightEmission[] = {1.0, 1.0, 1.0, 1.0};   
GLfloat globalAmbientLight[] = {0.2, 0.2, 0.2, 1.0};

GLfloat prop[] = {0.0, 0.0, 0.0, 1.0};

GLfloat ry = 0.0;
GLfloat rry = 0.0;
GLfloat rz = 0.0;
#define PR_MAX 100
#define MAX_PR 0.08
GLfloat pr[PR_MAX];

void setProp(rgb* color) {
  prop[0] = color->r;
  prop[1] = color->g;
  prop[2] = color->b;
}

void init() {
  int i;
  //
  for (i = 0; i < PR_MAX; i++) {  // Random radi
    pr[i] = rand();         // random #
    pr[i] = (pr[i]/RAND_MAX); // 0.0...1.0 = (rand()/(RAND_MAX) - limit range
    pr[i] = pr[i] * MAX_PR;     // 0.0...0.06 = ((0.0 ... 1.0) * 0.06) - scale
  }

  glClearColor(offWhite.r, offWhite.g, offWhite.b, 1.0);
  glShadeModel(GL_SMOOTH);

  // Position, Ambient, Diffuse & Specular Componen  
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbientLight);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);


    // Enable Lighting and Hidden Surface Removal
    glEnable(GL_LIGHTING); 
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);  // Must be after window init & create
}

void move(int i) {
  GLfloat t;
  ry += 2.5;
  if (ry > 360.0) ry -= 360.0;
  rry += 4.75;
  if (rry > 360.0) ry -= 360.0;

  for (i = 0; i < PR_MAX; i++) {           // Grow and Shrink radi
                                           // A positive radi is growing
                                           // A negative radi is shrinking
    t = rand();         // random #
    t = (t/RAND_MAX);   // 0.0...1.0 = (rand()/(RAND_MAX) - limit range
    t = t * 0.004;   // 0.0...0.004 = ((0.0 ... 1.0) * 0.004) - scale
      pr[i] += t;
    if (pr[i] > MAX_PR) 
      pr[i] = pr[i] - (MAX_PR*2);  // Goes negative
  }
  
  glutPostRedisplay();      
}

void display() {
  GLfloat cx = -1.0;
  GLfloat cy =  1.0;
  GLfloat cz =  0.0;  
  int r, c;

  glClear(GL_COLOR_BUFFER_BIT | 
	  GL_DEPTH_BUFFER_BIT);  // clear color and depth buffer
  //glEnable(GL_COLOR_MATERIAL);   // Using glColorMaterial defaults

  glPushMatrix(); // Row 1
  cx = -1.0;
  cy =  1.0;
  cz =  0.0;  
  setProp(&red);
  // Dull Red Ball with Black Shading
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, prop);  // Red
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);             // Black
  
  glMaterialf(GL_FRONT, GL_SHININESS, 0.0);              // None
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_EMISSION, prop);             // Black

  // #1
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 
  cx = 0.5;
  cy = 0.0;

  // #2
  setProp(&deepSkyBlue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);            
  glMaterialf(GL_FRONT, GL_SHININESS, 25.0);    
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 


  // #3
  setProp(&green);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);         
  glMaterialf(GL_FRONT, GL_SHININESS, 0.5);    // Set value to 0.0 and 0.5 to see the difference?
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  // #4
  glMaterialfv(GL_FRONT, GL_AMBIENT, ruby.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, ruby.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, ruby.shininess);         

  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  // #5
  glMaterialfv(GL_FRONT, GL_AMBIENT, blackRubber.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blackRubber.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, blackRubber.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, blackRubber.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  glPopMatrix();
  
  glPushMatrix(); // Row 2
  cx = -1.0;
  cy =  0.5;
  cz =  0.0;  
  setProp(&seaGreen);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, prop);  
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);             // Black
  
  glMaterialf(GL_FRONT, GL_SHININESS, 0.0);              // None
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_EMISSION, prop);             // Black

  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  cx = 0.5;
  cy = 0.0;

  // #2
  setProp(&lightSkyBlue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);         
  glMaterialf(GL_FRONT, GL_SHININESS, 50.0);    
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 


  // #3
  setProp(&yellow);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);         
  glMaterialf(GL_FRONT, GL_SHININESS, 50.0);    
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  // #4 
  glMaterialfv(GL_FRONT, GL_AMBIENT, blackPlastic.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blackPlastic.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, blackPlastic.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, blackPlastic.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  // #5
  glMaterialfv(GL_FRONT, GL_AMBIENT, emerald.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, emerald.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, emerald.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, emerald.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  glPopMatrix();

  glPushMatrix(); // Row 3
  cx = -1.0;
  cy =  0.0;
  cz =  0.0;  
  setProp(&yellow);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, prop);  // Red
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);             // Black
  
  glMaterialf(GL_FRONT, GL_SHININESS, 0.0);              // None
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_EMISSION, prop);             // Black


  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  cx = 0.5;
  cy = 0.0;

  // #2
  setProp(&deepSkyBlue);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);         
  glMaterialf(GL_FRONT, GL_SHININESS, 25.0);    
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 


  // #3

  glMaterialfv(GL_FRONT, GL_AMBIENT, pewter.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, pewter.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, pewter.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, pewter.shininess);         
  glTranslatef(cx, cy, cz);
  glPushMatrix();
  glRotatef(ry, 0.0, 1.0, 0.0);
  glFrontFace(GL_CW); 
  glutSolidTeapot(0.20); 
  glFrontFace(GL_CCW);
  glPopMatrix();

  // #4
  glMaterialfv(GL_FRONT, GL_AMBIENT, pewter.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, pewter.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, pewter.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, pewter.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  // #5
  glMaterialfv(GL_FRONT, GL_AMBIENT, silver.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, silver.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, silver.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, silver.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  
  glPopMatrix();

  glPushMatrix(); // Row 4
  cx = -1.0;
  cy =  -0.5;
  cz =  0.0;  
  setProp(&lightSkyBlue);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, prop);  // Red
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);             // Black
  
  glMaterialf(GL_FRONT, GL_SHININESS, 0.0);              // None
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_EMISSION, prop);             // Black

  setProp(&lightSkyBlue);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, prop);

  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  cx = 0.5;
  cy = 0.0;

  // #2
  setProp(&red);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);         
  glMaterialf(GL_FRONT, GL_SHININESS, 80.0);    
  glTranslatef(cx, cy, cz);
  glPushMatrix();
  glRotatef(-rry, 0.0, 1.0, 0.0);
  glutSolidTorus(0.07, 0.18, 25, 20); 


  glPushMatrix(); 
    glRotatef(rz, 0.0, 0.0, 1.0);          // rotate within the ceter of the torus
    rz += 1.5;
    glTranslatef(0.18, cy, cz);            // translate to the ring of the torus
    glRotatef(-rry*2.0, 0.0, 1.0, 0.0);    // rotate around the ring
    glTranslatef(0.18, cy, cz);            // translate out away from the ring
    glutSolidSphere(0.07, 20, 16);         // draw the ball
  glPopMatrix();                         // Move back to the center of the torus

    glRotatef(-rz, 0.0, 0.0, 1.0);         // rotate within the ceter of the torus
    rz += 1.5;
    glTranslatef(0.18, cy, cz);           // translate to the ring of the torus
    glRotatef(rry*2.5, 0.0, 1.0, 0.0);    // rotate around the ring
    glTranslatef(0.18, cy, cz);           // translate out away from the ring
    glutSolidSphere(0.07, 20, 16);        // draw the ball

  glPopMatrix();


  // #3
  setProp(&yellow);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);         
  glMaterialf(GL_FRONT, GL_SHININESS, 100.0);    
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 


  glMaterialfv(GL_FRONT, GL_AMBIENT, copper.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, copper.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, copper.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, copper.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  glMaterialfv(GL_FRONT, GL_AMBIENT, brass.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, brass.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, brass.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, brass.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 
  glPopMatrix();

  glPushMatrix(); // Row 5
  cx = -1.0;
  cy =  -1.0;
  cz =  0.0;  
  setProp(&peachPuff);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, prop);  // Red
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_SPECULAR, prop);             // Black
  
  glMaterialf(GL_FRONT, GL_SHININESS, 0.0);              // None
  setProp(&black);
  glMaterialfv(GL_FRONT, GL_EMISSION, prop);             // Black

  // #1
  glTranslatef(cx, cy, cz);
  glMaterialf(GL_FRONT, GL_SHININESS, 1.0);
  glutSolidSphere(0.10, 20, 16); 

  cx = 0.5;
  cy = 0.0;

  // #2
  glMaterialfv(GL_FRONT, GL_AMBIENT, chrome.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, chrome.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, chrome.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, chrome.shininess);         

  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 


  // #3
  glMaterialfv(GL_FRONT, GL_AMBIENT, jade.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, jade.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, jade.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, jade.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 


  glMaterialfv(GL_FRONT, GL_AMBIENT, pearl.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, pearl.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, pearl.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, pearl.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 

  glMaterialfv(GL_FRONT, GL_AMBIENT, gold.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, gold.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, gold.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, gold.shininess);         
  glTranslatef(cx, cy, cz);
  glutSolidSphere(0.10, 20, 16); 
  glPopMatrix();
  

  // Composite Objects
  // Rotating Composit
  glPushMatrix();
  cx = -1.5;
  cy = 0.0;
  cz = 0.0;
  glTranslatef(cx, cy, cz);
  glRotatef(55.0, 1.0, 0.0, 0.0);
  glRotatef(25.0, 0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ruby.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, ruby.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, ruby.shininess);         
  glRotatef(rry, 1.0, 0.0, 1.0);                          // Cube +rry rotation
  glutSolidCube(0.38);
  glMaterialfv(GL_FRONT, GL_AMBIENT, copper.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, copper.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, copper.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, copper.shininess);         
  glRotatef(-rry, 0.0, 1.0, 1.0);                        // Sphere -rry rotation
  glutSolidSphere(0.25, 12, 12);
  glPopMatrix();

  // Pulsing Composite #1
  glPushMatrix();
  glTranslatef(1.5, 0.5, 0.0);
  glMaterialfv(GL_FRONT, GL_AMBIENT, ruby.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, ruby.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, ruby.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, ruby.shininess);         
  glutSolidSphere(0.15, 20, 16);                         // Sphere doesn't move
  GLfloat yy, xx, len; 
                                                        // Create spikes every 45 degrees
  for (yy = 15.0; yy < 375.0; yy += 45.0) {             // along a Y rotation
                                                        // Note how first cone is offset
                                                        // to start at 15 and end at
                                                        // 375 vs. 0 and 360
    glPushMatrix();
    glRotatef(yy, 0.0, 1.0, 0.0);
    for (xx = 0.0; xx < 360.0; xx += 45.0) {            // Also move 45 degrees in an
                                                        // X rotation
      glPushMatrix();
      glRotatef(xx, 1.0, 0.0, 0.0);
      glMaterialfv(GL_FRONT, GL_AMBIENT, copper.ambient);         
      glMaterialfv(GL_FRONT, GL_DIFFUSE, copper.diffuse);         
      glMaterialfv(GL_FRONT, GL_SPECULAR, copper.specular);         
      glMaterialf(GL_FRONT, GL_SHININESS, copper.shininess);         

      // 0.0 ... 1.0 = (rand()/(RAND_MAX) - limit range
      // 0.0 ... 0.49 = ((0.0 ... 1.0) * 0.49) - scale
      // 0.01 ... 0.5 = ((0.0 ... 0.49) + 0.01) - set min & max
      len = rand();
      len = (len/RAND_MAX);
      len = len * 0.49;
      len = len + 0.01;
      glutSolidCone(0.08, len, 20, 25);
      glPopMatrix();
    }
    glPopMatrix();
  }
  glPopMatrix();


  // Pulsing Composite #2
  glPushMatrix();
  glTranslatef(1.65, -0.5, 0.0);                   // Move to WC postion
  glMaterialfv(GL_FRONT, GL_AMBIENT, 
	       emerald.ambient);         
  glMaterialfv(GL_FRONT, GL_DIFFUSE, 
	       emerald.diffuse);         
  glMaterialfv(GL_FRONT, GL_SPECULAR, 
	       emerald.specular);         
  glMaterialf(GL_FRONT, GL_SHININESS, 
	      emerald.shininess);         
  glutSolidSphere(0.20, 20, 16);                  // Draw Sphere
  GLfloat rad;
  int i;
  i = 0;
  for (yy = 0.0; yy < 360.0; yy += 30.0) {
    glPushMatrix();
    glRotatef(yy, 0.0, 1.0, 0.0);                // Rotate y-axis
    for (xx = 0.0; xx < 360.0; xx += 30.0) {
      glPushMatrix();
      glRotatef(xx, 1.0, 0.0, 0.0);              // Rotate x-axis
      glTranslatef(0.0, 0.0, 0.20);
      glMaterialfv(GL_FRONT, GL_AMBIENT, blackPlastic.ambient);         
      glMaterialfv(GL_FRONT, GL_DIFFUSE, blackPlastic.diffuse);         
      glMaterialfv(GL_FRONT, GL_SPECULAR, blackPlastic.specular);         
      glMaterialf(GL_FRONT, GL_SHININESS, blackPlastic.shininess);         
      
      if (pr[i] < 0.0)
	glutSolidSphere(-pr[i], 20, 25);  // Shrinking radi are negative
      else
	glutSolidSphere(pr[i], 20, 25);   // Growing radi is positive
      glPopMatrix();
      i++;
    }
    glPopMatrix();
    i++;
  }
  glPopMatrix();

  glutTimerFunc(150, move, 0);
  glutSwapBuffers();

}



void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(100.0, (GLfloat)w/(GLfloat)h, 0.5, 20.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 1.25, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}


void keyboard(unsigned char key, int x, int y) {
  switch(key) {
  case 'A':
    if (lightAmbient[0] == 1.0) break;
    lightAmbient[0] += 0.2;
    lightAmbient[0] += 0.2;
    lightAmbient[0] += 0.2;
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glutPostRedisplay();
    break;
  case 'a':
    if (lightAmbient[0] == 0.0) break;
    lightAmbient[0] -= 0.2;
    lightAmbient[0] -= 0.2;
    lightAmbient[0] -= 0.2;
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glutPostRedisplay();
    break;
  case 'D':
    if (lightDiffuse[0] == 1.0) break;
    lightDiffuse[0] += 0.2;
    lightDiffuse[0] += 0.2;
    lightDiffuse[0] += 0.2;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glutPostRedisplay();
    break;
  case 'd':
    if (lightDiffuse[0] == 0.0) break;
    lightDiffuse[0] -= 0.2;
    lightDiffuse[0] -= 0.2;
    lightDiffuse[0] -= 0.2;
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glutPostRedisplay();
    break;
  case 'S':
    if (lightSpecular[0] == 1.0) break;
    lightSpecular[0] += 0.2;
    lightSpecular[0] += 0.2;
    lightSpecular[0] += 0.2;
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glutPostRedisplay();
    break;
  case 's':
    if (lightSpecular[0] == 0.0) break;
    lightSpecular[0] -= 0.2;
    lightSpecular[0] -= 0.2;
    lightSpecular[0] -= 0.2;
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glutPostRedisplay();
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
  glutCreateWindow("Material");

  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(150, move, 0);
  glutMainLoop();
  return 0;
}
