#include <stdio.h>
#include <GL/gl.h>
#include <math.h>
#include <stdio.h>
#include "colors.h"

#define PI 3.141592653589793f

#define PHI 1.6180339887f
#define K 60
static double d2r = 3.14159/180.0;  // Degree to radian conversion

//Coordinates for buckyball
double coords[K][3] = {
	{0.0f, 1.0f, 3*PHI},	{0.0f, 1.0f, -3*PHI},	{0.0f, -1.0f, 3*PHI},	{0.0f, -1.0f, -3*PHI},
	
	{1.0f, 3*PHI, 0},	{1.0f, -3*PHI, 0},	{-1.0f, 3*PHI, 0},	{-1.0f, -3*PHI, 0},
	
	{3*PHI, 0, 1},	{3*PHI, 0, -1},	{-3*PHI, 0, 1},	{-3*PHI, 0, -1},	
	
	{2, (1+2*PHI), PHI},	{2, (1+2*PHI), -PHI},	{2, -(1+2*PHI), PHI},	{2, -(1+2*PHI), -PHI},	{-2, (1+2*PHI), PHI},
	{-2, (1+2*PHI), -PHI},	{-2, -(1+2*PHI), PHI},	{-2, -(1+2*PHI), -PHI},	{(1+2*PHI), PHI, 2},	{(1+2*PHI), PHI, -2},
	
	{(1+2*PHI), -PHI, 2},	{(1+2*PHI), -PHI, -2},	{-(1+2*PHI), PHI, 2},	
	{-(1+2*PHI), PHI, -2},	{-(1+2*PHI), -PHI, 2},	{-(1+2*PHI), -PHI, -2},
	
	{PHI, 2, (1+2*PHI)},	{PHI, 2, -(1+2*PHI)},	{PHI, -2, (1+2*PHI)},	{PHI, -2, -(1+2*PHI)},
	{-PHI, 2, (1+2*PHI)},	{-PHI, 2, -(1+2*PHI)},	{-PHI, -2, (1+2*PHI)},	{-PHI, -2, -(1+2*PHI)},
	
	{1, (2+PHI), 2*PHI},	{1, (2+PHI), -2*PHI},	{1, -(2+PHI), 2*PHI},	{1, -(2+PHI), -2*PHI},
	{-1, (2+PHI), 2*PHI},	{-1, (2+PHI), -2*PHI},	{-1, -(2+PHI), 2*PHI},	{-1, -(2+PHI), -2*PHI},
	
	{(2+PHI), 2*PHI,1},	{(2+PHI), 2*PHI,-1},	{(2+PHI), -2*PHI,1},	{(2+PHI), -2*PHI,-1},
	{-(2+PHI), 2*PHI,1},	{-(2+PHI), 2*PHI,-1},	{-(2+PHI), -2*PHI,1},	{-(2+PHI), -2*PHI,-1},
	
	{2*PHI,1,(2+PHI)},	{2*PHI,1,-(2+PHI)},	{2*PHI,-1,(2+PHI)},	{2*PHI,-1,-(2+PHI)},
	{-2*PHI,1,(2+PHI)},	{-2*PHI,1,-(2+PHI)},	{-2*PHI,-1,(2+PHI)},	{-2*PHI,-1,-(2+PHI)}
};
float buckyRotateX=0.0f;
float buckyRotateY=0.0f;
float buckyRotateZ=0.0f;
float buckyRadius=0;

void drawSphere(GLfloat radius,int cuts,int stacks, rgb color1, rgb color2)
{
  float dTheta = PI/cuts;      // Cuts are long.
  float dPhi = PI/stacks;  // Stacks are lat.
 
  glPushMatrix();
  //glPolygonMode( GL_FRONT , GL_LINE); //For Wireframe sphere
  //glPolygonMode( GL_BACK, GL_LINE);

  glPolygonMode( GL_FRONT , GL_FILL);
  //glPolygonMode( GL_BACK, GL_FILL);
  
	//use a triangle fan for the cap
	
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(color1.r,color1.g,color1.b);
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
		j%2 ? glColor3f(color1.r,color1.g,color1.b) : glColor3f(color2.r,color2.g,color2.b);
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
		  for(i=0;i<=cuts*2;i++){
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
		glColor3f(color2.r,color2.g,color2.b);
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
			theta-=dTheta;
			glNormal3f(xNormal,yNormal,zNormal);
			glVertex3f(radius*xNormal,radius*yNormal,zRadius);			
		}	
		
	}glEnd();
	
	glPopMatrix();
}

void drawSphereAlpha(GLfloat radius,int cuts,int stacks, rgb color1, rgb color2, float alpha)
{
  float dTheta = PI/cuts;      // Cuts are long.
  float dPhi = PI/stacks;  // Stacks are lat.
 
  glPushMatrix();
  //glPolygonMode( GL_FRONT , GL_LINE); //For Wireframe sphere
  //glPolygonMode( GL_BACK, GL_LINE);

  glPolygonMode( GL_FRONT , GL_FILL);
  //glPolygonMode( GL_BACK, GL_FILL);
  
	//use a triangle fan for the cap
	
	glBegin(GL_TRIANGLE_FAN);{
		glColor4f(color1.r,color1.g,color1.b,alpha);
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
		j%2 ? glColor4f(color1.r,color1.g,color1.b,alpha) : glColor4f(color2.r,color2.g,color2.b,alpha);
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
		  for(i=0;i<=cuts*2;i++){
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
		glColor4f(color2.r,color2.g,color2.b,alpha);
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
			theta-=dTheta;
			glNormal3f(xNormal,yNormal,zNormal);
			glVertex3f(radius*xNormal,radius*yNormal,zRadius);			
		}	
		
	}glEnd();
	
	glPopMatrix();
}


void drawSpiral(float a, float b, float c, float k, float thetaMax){
	GLfloat theta;
	GLfloat dTheta =  PI/10;
	for(theta=0;theta<thetaMax;theta+=dTheta){
		glPushMatrix();
		float x = a*exp(-k*theta)*cos(theta);
		float y = -c*theta;
		float z = b*exp(-k*theta)*sin(theta);
		glTranslatef(x,y,z);
		drawSphere(a/10,2,2, yellow, red);
		glPopMatrix();
	}
}


void drawStaticBucky(GLfloat buckyRad, GLfloat sphereRad, rgb color1, rgb color2){

	glRotatef(buckyRotateX, 1.0, 0.0, 0.0);
	glRotatef(buckyRotateY, 0.0, 1.0, 0.0);
	glRotatef(buckyRotateZ, 0.0, 0.0, 1.0);   
	int i=0;
	for(i=0;i<K;i++){
		glPushMatrix();
		glTranslatef(buckyRad*coords[i][0], buckyRad*coords[i][1], buckyRad*coords[i][2]);
		drawSphere(sphereRad,10,10, color1, color2);		    
		glPopMatrix();
	}
}


void drawBuckyball(GLfloat buckyRad, GLfloat sphereRad){
	buckyRotateX+=0.1f;
	if(buckyRotateX>360){
		buckyRotateX=0;
	}

	buckyRotateY+=0.5f;
	if(buckyRotateY>360){
		buckyRotateY=0;
	}

	buckyRotateZ+=0.9f;
	if(buckyRotateZ>360){
		buckyRotateZ=0;
	}
	
	buckyRadius+=0.1f;
	if(buckyRadius>360){
		buckyRadius=0;
	}
	
	float radius = buckyRad*sin(buckyRadius/(PI))+buckyRad;
	float sphereRadius = sphereRad*pow(cos(buckyRadius/(PI)),2)+sphereRad;
	
	glRotatef(buckyRotateX, 1.0, 0.0, 0.0);
	glRotatef(buckyRotateY, 0.0, 1.0, 0.0);
	glRotatef(buckyRotateZ, 0.0, 0.0, 1.0);   
	int i=0;
	for(i=0;i<K;i++){
		glPushMatrix();
		glTranslatef(radius*coords[i][0], radius*coords[i][1], radius*coords[i][2]);
		glRotatef(-1.0f*buckyRotateX, 1.0, 0.0, 0.0);
	    glRotatef(1.0f*buckyRotateY, 0.0, 1.0, 0.0);
	    glRotatef(-1.0f*buckyRotateZ, 0.0, 0.0, 1.0);
		drawSphere(sphereRadius,12,12, gold, lavenderBlush);		    
		glPopMatrix();
	}
}

void drawCylinder(float radius, float height, rgb color1, rgb color2){
	//
	glPushMatrix();
	glPolygonMode( GL_FRONT , GL_FILL);
	glPolygonMode( GL_BACK , GL_FILL);
	int i=0;
	//draw top
	int sides =12;
	float dTheta = 2*PI/sides;
	float theta=0;
	glColor3f(color2.r,color2.g,color2.b);
	glBegin(GL_TRIANGLE_FAN);{	
		glNormal3f(0.0f,0.0f, -1.0f);
		float theta=0;		
		float y = height;
		
		glVertex3f(0.0,y,0.0);
		int i=0;
		for(i=0;i<=sides;i++){					
			glVertex3f(radius*cosf(theta),y,radius*sinf(theta));
			theta+=dTheta;			
		}		
	}glEnd();
	
	theta=0;
	//draw middle
	glBegin(GL_TRIANGLE_STRIP);{ 
	  int i;
	  for(i=0;i<=sides;i++){
		int even = i%2;
		even ? glColor3f(color2.r,color2.g,color2.b) : glColor3f(color1.r,color1.g,color1.b);
		float z = sin(theta);
		float x = cos(theta);
		glNormal3f(x,0,z);	
		glVertex3f(radius*x,0,radius*z);		
		glVertex3f(radius*x,height,radius*z);			
		theta+=dTheta;
	  }
	}glEnd();	
	
	//draw bottom
	theta=0;
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(color1.r,color1.g,color1.b);
		glNormal3f(0.0f,-1.0f, 0.0f);
		float theta=0;		
		float y = 0;
		glVertex3f(0.0,y,0.0);
		int i=0;
		for(i=0;i<=sides;i++){		
			glVertex3f(radius*cosf(theta),y,radius*sinf(theta));
			theta+=dTheta;			
		}		
	}glEnd();	
	glPopMatrix();
}

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

float DEG2RAD = 3.141592653589793/180;
#define X 0
#define Y 1
#define Z 2

void ellipsoid(GLfloat xr, GLfloat yr, GLfloat zr, int cuts, int stacks) {
  //Equ. for a Ellipsoid (3D Ellipse) at the origin
  //x = xr*cos(longitude)*cos(latitude) 
  //y = yr*cos(longitude)*sin(latitude)
  //z = zr*sin(longitude) 

  int i,j;
  int numCut = 360/cuts;
  int numSlice = 180/stacks;

  float azimuth; // aka longitude in 360-degrees
  float zenith; // aka cozenith in 180-degrees
  GLfloat nor[3] = { 0.0 , 0.0 , 0.0};
  GLfloat vector1[3],vector2[3],vector3[3],vector4[3],
    normal[3] = {0.0 , 0.0 , 0.0};

  glPushMatrix();
  glPolygonMode( GL_FRONT , GL_FILL);
  glPolygonMode( GL_BACK, GL_FILL);
	

  for(j=0;j<=360;j = numCut + j) {  // asimuth
    for(i=0;i<=180;i = numSlice + i) { // zenith
			
      azimuth = j*DEG2RAD;
      zenith  = i*DEG2RAD;
	
      vector1[X] = xr*(cosf(azimuth)*sinf(zenith));
      vector1[Y] = yr*(sinf(azimuth)*sinf(zenith));
      vector1[Z] = zr*(cosf(zenith));
			
      azimuth	= (j+numCut)*DEG2RAD;  
      vector2[X] = xr*(cosf(azimuth)*sinf(zenith));
      vector2[Y] = yr*(sinf(azimuth)*sinf(zenith));
      vector2[Z] = zr*(cosf(zenith));

			
      zenith = (i+numSlice)*DEG2RAD;
      vector3[X] = xr*(cosf(azimuth)*sinf(zenith));
      vector3[Y] = yr*(sinf(azimuth)*sinf(zenith));
      vector3[Z] = zr*(cosf(zenith));

      azimuth	= j*DEG2RAD;
      vector4[X] = xr*(cosf(azimuth)*sinf(zenith));
      vector4[Y] = yr*(sinf(azimuth)*sinf(zenith));
      vector4[Z] = zr*(cosf(zenith));

      findNormal3Points(vector1,vector2,vector3,nor);
				
      glBegin(GL_TRIANGLES); {
				
	glNormal3fv(nor);
	glVertex3fv(vector1);
				
	glNormal3fv(nor);
	glVertex3fv(vector2);

	glNormal3fv(nor);
	glVertex3fv(vector3);
      }
      glEnd();
				
      findNormal3Points(vector3,vector2,vector4,nor);

      glBegin(GL_TRIANGLES); {
	glNormal3fv(nor);
	glVertex3fv(vector1);
				
	glNormal3fv(nor);
	glVertex3fv(vector4);

	glNormal3fv(nor);
	glVertex3fv(vector3);
      } glEnd();
    }
  }
  glPopMatrix();
}
