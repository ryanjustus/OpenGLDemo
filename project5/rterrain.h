#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
//TSIZE should be 2^n
#define TSIZE 96
#define T_MAX 50
#define TILT 0.0
float terrain[TSIZE+1][TSIZE+1];

/* Gausssian Filter Kernel */
GLfloat gfk[5] = {
	1.0/38.0,
	9.0/38.0,
	18.0/38.0,
	9.0/38.0,
	1.0/38.0};
	
/* Mean Filter Kernel */
GLfloat mfk[3][3] = {
	1.0/9.0,
	1.0/9.0,
	1.0/9.0,
	1.0/9.0,
	1.0/9.0,
	1.0/9.0,
	1.0/9.0,
	1.0/9.0,
	1.0/9.0};

void buildTerrain(int startx, int endx, int startz, int endz);
float getGausRand(int mag);
void printTerrain();
void gaussianFilter();
void meanFilter();

void initTerrain(){

	for(int i=0;i<=TSIZE;i++){
		for(int j=0;j<=TSIZE;j++){
			terrain[i][j] = 0.0f;
		}
	}	
	
	//printf("init terrain\n");
	//initialize four corners
	terrain[0][0] = 60.0f;
	//printf("terrain[0][0]=%.1f\n",terrain[0][0]);
	terrain[0][TSIZE] = 40.0f;
	//printf("terrain[0][TSIZE]=%.1f\n",terrain[0][TSIZE]);
	terrain[TSIZE][0] = 40.0f;
	//printf("terrain[TSIZE][0]=%.1f\n",terrain[TSIZE][0]);
	terrain[TSIZE][TSIZE] = 0.0f;
	//printf("terrain[TSIZE][TSIZE]=%.1f\n",terrain[TSIZE][TSIZE]);
	//terrain[TSIZE/2][TSIZE/2] = 30/1.0f;
	
	buildTerrain(0, TSIZE, 0, TSIZE);

	meanFilter();
	//meanFilter();
	//meanFilter();
	for(int i=0;i<2;i++){
		gaussianFilter();
	}	
	meanFilter();
}

float getHeight(float x, float z){
	x+=TSIZE/2;
	z+=TSIZE/2;
	if(x>TSIZE || z>TSIZE || x<0 || z<0){
		return -10.0f;
	}else{
		return terrain[(int)floor(x)][(int)floor(z)];
	}
}

void buildTerrain(int startx, int endx, int startz, int endz){
	int mid = (endx-startx)/2;
	if(mid<1)
		return;		
	terrain[startx+mid][startz] = (terrain[startx][startz] + terrain[endx][startz])/2.0f + getGausRand(T_MAX)*mid/1.0f/TSIZE;//top
	terrain[startx][startz+mid] = (terrain[startx][startz] + terrain[startx][endz])/2.0f + getGausRand(T_MAX)*mid/1.0f/TSIZE;	//left	
	terrain[endx][startz+mid] = (terrain[endx][startz] + terrain[endx][endz])/2.0f + getGausRand(T_MAX)*mid/1.0f/TSIZE; //right	
	terrain[startx+mid][endz] = (terrain[startx][endz] + terrain[endx][endz])/2.0f + getGausRand(T_MAX)*mid/1.0f/TSIZE; //bottom
	
	if(terrain[startx+mid][startz+mid]==0.0f){
		terrain[startx+mid][startz+mid] = (terrain[startx][startz] + terrain[startx][endz] + terrain[endx][startz] + terrain[endx][endz])/4.0f 
		+ getGausRand(T_MAX/2); //middle
	}
	
	buildTerrain(startx, startx+mid, startz, startz+mid);
	buildTerrain(startx+mid, endx, startz, startz+mid);
	buildTerrain(startx+mid, endx, startz+mid, endz);
	buildTerrain(startx, startx+mid, startz+mid, endz);
}


void colorTerrain(GLfloat h) {
	float hMax = 80;
	h+=20;
	if(h<0.0){
		glColor3f(green.r,green.g,green.b);
		return;
	}

	float hue = 200.0 - (180*h/hMax);
	float sat = 0.2 + 0.5*h/hMax;
	float v = 0.4;
	hsv chsv;
	chsv.h = hue;
	chsv.s = sat;
	chsv.v = v;
	rgb c = hsv2rgb(chsv);
	glColor3f(c.r, c.g, c.b);
}

void drawTerrain(float bx, float by, float bz) {
	GLfloat v1[3], v2[3], v3[3], v4[4],nor[3];
	GLfloat xstep, hstep;
	int x, z;

	xstep = 1.0;
	hstep = 1.0;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
	glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
	glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
	glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);
	glMaterialfv(GL_BACK, GL_AMBIENT, goldMat.ambient);         
	glMaterialfv(GL_BACK, GL_DIFFUSE, goldMat.diffuse);         
	glMaterialfv(GL_BACK, GL_SPECULAR, goldMat.specular);         
	glMaterialf(GL_BACK, GL_SHININESS, goldMat.shininess);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);

	glPushMatrix();
	glTranslatef(bx, by, bz);
	glRotatef(TILT, 1.0, 0.0, 0.0);
	glRotatef(0.0, 0.0, 1.0, 0.0);	
	

	
	for (z = 0; z < TSIZE; z++) {
		for (x = 0; x < TSIZE; x++) {
			glBegin(GL_TRIANGLE_STRIP);
			
			//0,0
			v1[0] = bx+(x*xstep);
			v1[1] = by+(terrain[x][z]*hstep);
			v1[2] = bz+(z*xstep);

			//0,1
			v2[0] = bx+(x*xstep);
			v2[1] = by+(terrain[x][z+1]*hstep);
			v2[2] = bz+((z+1)*xstep);

			//1,0
			v3[0] = bx+((x+1)*xstep);
			v3[1] = by+(terrain[(x+1)][(z)]*hstep);
			v3[2] = bz+(z*xstep);
			
			//1,1
			v4[0] = bx+((x+1)*xstep);
			v4[1] = by+(terrain[(x+1)][(z+1)]*hstep);
			v4[2] = bz+((z+1)*xstep);			
			
			findNormal3Points(v1, v2, v3, nor);
			glNormal3fv(nor);
			colorTerrain(terrain[x][z]);
			glVertex3fv(v1);
			colorTerrain(terrain[x][z+1]);
			glVertex3fv(v2);
			colorTerrain(terrain[x+1][z+1]);
			glVertex3fv(v3);
			
			findNormal3Points(v2, v4, v3, nor);
			glNormal3fv(nor);
			glVertex3fv(v4);
			
			glEnd();
		}
	}

	
	glPopMatrix();
}

float getGausRand(int mag){
	return ((rand()%mag - mag/2.0f) + (rand()%mag - mag/2.0f))/2.0f;
}

//From Dr. Wellborn @ uvu
void meanFilter() {
	int x, xn, xp, z, zn, zp;
	GLfloat I[TSIZE+1][TSIZE+1];
	
	// Convolve in the X Direction
	for (z = 0; z <= TSIZE; z++) {
		for (x = 0; x <= TSIZE; x++) {

			xn = x-1;
			if (xn < 0) xn = x;

			xp = x+1;
			if (xp > TSIZE) xp = x;

			zn = z-1;
			if (zn < 0) zn = z;

			zp = z+1;
			if (zp > TSIZE) zp = z;

			I[x][z] = 
			terrain[xn][zn]*mfk[0][0] +
			terrain[xn][z]*mfk[0][1] +
			terrain[xn][zp]*mfk[0][2] +
			terrain[x][zn]*mfk[1][0] +
			terrain[x][z]*mfk[1][1] + 
			terrain[x][zp]*mfk[1][2] + 
			terrain[xp][zn]*mfk[2][0] + 
			terrain[xp][z]*mfk[2][1] + 
			terrain[xp][zp]*mfk[2][2];
		}
	}

	for (z = 0; z <= TSIZE; z++) 
		for (x = 0; x <= TSIZE; x++) 
			terrain[x][z] = I[x][z];
}

//From Dr. Wellborn @ uvu
void gaussianFilter() {
	int x, xn, x2n, xp, x2p, z, zn, z2n, zp, z2p;
	GLfloat I[TSIZE+1][TSIZE+1];

	for (z = 0; z <= TSIZE; z++) {
		for (x = 0; x <= TSIZE; x++) {

			xn = x-1;
			if (xn < 0) xn = x;

			x2n = x-2;
			if (x2n < 0) x2n = x;

			xp = x+1;
			if (xp > TSIZE) xp = x;

			x2p = x+2;
			if (x2p > TSIZE) x2p = x;

			I[x][z] = 
			terrain[x2n][z]*gfk[0] +
			terrain[xn][z]*gfk[1] +
			terrain[x][z]*gfk[2] +
			terrain[xp][z]*gfk[3] +
			terrain[x2p][z]*gfk[4];
		}
	}

	// Convolve in the Z Direction
	for (x = 0; x < TSIZE; x++) {
		for (z = 0; z < TSIZE; z++) {

			zn = z-1;
			if (zn < 0) zn = z;

			z2n = z-2;
			if (z2n < 0) z2n = z;

			zp = z+1;
			if (zp > TSIZE-1) zp = z;

			z2p = z+2;
			if (z2p > TSIZE-1) z2p = z;

			I[x][z] += 
			terrain[x][z2n]*gfk[0] +
			terrain[x][zn]*gfk[1] +
			terrain[x][z]*gfk[2] + 
			terrain[x][zp]*gfk[3] + 
			terrain[x][z2p]*gfk[4];
		}
	}

	for (z = 0; z < TSIZE; z++) 
		for (x = 0; x < TSIZE; x++) 
			terrain[x][z] = I[x][z]/2.0;
}

void printTerrain(){
	printf("printing terrain\n");
	for(int i=0;i<=TSIZE;i++){
		for(int j=0;j<=TSIZE;j++){
			printf("%.1f |",terrain[i][j]);
		}
		printf("\n");
	}	
}

