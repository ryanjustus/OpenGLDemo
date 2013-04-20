#include <math.h>
#define N2 10000
#define P 100
#include <stdlib.h>
#include <time.h>

//GLfloat lightSpecular1[] = {1.0, 1.0, 1.0, 1.0}; // Default
typedef struct{
	int active;	
	float t;//tempurature
	//Position
	float x;
	float y;
	float z;
	//Velocity
	float dx;
	float dy;
	float dz;
}Particle;

float speed = 5.0f;
float life = 2.0f;

float rho[P][P][P];  //Vector field containing pressure
float atm = 10.01f;
float g = 0.00f; //gravity
Particle pp[N2]; //Array to hole particles
//GLuint  texture[0];         // Storage For Our Particle texture
float scale = N2/(4*log(N2));
//float scale = N*3/(log(N)-3);
int t=0;

/**
* Initial pressure from outside forming a boundary around the edge pointing inward
*/
void initPressure(){
	int i,j,k;
	for(i=0;i<P;i++){
		for(j=0;j<P;j++){
			if(i>0 && j>0 && i<P-1 && j<P-1){
				for(k=1;k<P-1;k++){
					if(j<P/4)
						rho[i][j][k]=0.0f;
					else
						rho[i][j][k]=0.0f;
				}
			}
			rho[P-1][i][j] = atm; //initialize right		
			rho[0][i][j] = atm; //initialize left
			
			rho[i][P-1][j] = atm; //initialize top
			rho[i][0][j] = atm; //initialize bottom
			
			rho[i][j][P-1] = atm;//initialize front
			rho[i][j][0] = atm;//initialize back
			
		}
	}
}

void logParticle(int n){
	printf("p[%d]: loc=(%f,%f,%f) v=(%f,%f,%f)\n"	
	,n, pp[n].x, pp[n].y, pp[n].z, pp[n].dx ,pp[n].dy ,pp[n].dz);
	printf("\tactive=%i temp=%f\n",pp[n].active, pp[n].t);
	fflush(stdout);
}


void printRho(){
	for(int i=0;i<P;i++){
		for(int j=0;j<P;j++){
			printf("%0.2f ",rho[P/2][i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


/**
* add the current locations of the particles to the pressure
*/
void updatePressure(){
/*
	initPressure();
	int n;
	int d = P/2;
	for(n=0;n<N;n++){
		int x = (int)(pp[n].x*4+d);
		int y = (int)(pp[n].y*4+d);
		int z = (int)(pp[n].z*4+d);
		//If it is in the box update the density
		//printf("position (%d,%d,%d)\n",x,y,z);
		int d = P/2;
		if(x<P && y<P && z<P && x>=0 && y>=0 && z>=0){
			//Add a pressure proportional to the temperature to rho			
			rho[x][y][z]+=pp[n].t*10; 
			//printf("update p %d",n);
		}else{			
			pp[n].active=0; //Mark the particle as dead
		}
	}
	*/
	//printRho();
}

/**
* Create an initial velocity based on tempurature, t
*/
void initParticle(int n, float xi, float yi, float zi){
	if(n>=N2/2){
		xi*=-1;
		yi*=-1;
		zi*=-1;
	pp[n].active=1;
	}
	
	float s = speed;
	float l = life;
	if(tb>750){
		s*=3;
		l*=1.1;
	}
	pp[n].x=((rand()%100/100.0f-0.5f)+(rand()%100/100.0f-0.5f))*s+xi;
	pp[n].z=((rand()%100/100.0f-0.5f)+(rand()%100/100.0f-0.5f))*s+zi;
	pp[n].y=((rand()%100/100.0f-0.5f)+(rand()%100/100.0f-0.5f))*s+yi;
	pp[n].t = (rand()%100/100+0.5f)*l;
	pp[n].dx = ((rand()%100/100.0f) - 0.5f)*50.0f;
	pp[n].dy = ((rand()%100/100.0f) - 0.5f)*50.0f;
	pp[n].dz = ((rand()%100/100.0f) - 0.5f)*50.0f;
}


void updateParticle(int n){
	
	//update the velocity based on current pressure field	
	pp[n].x+=pp[n].dx/(scale*2);
	pp[n].y+=(pp[n].dy)/(scale*2) - g;
	pp[n].z+=pp[n].dz/(scale*2);	
	float d = (float)P/2;	
	int x = (int)(pp[n].x*2+d);
	int y = (int)(pp[n].y*2+d/2);
	int z = (int)(pp[n].z*2+d);
	
	//if(x<P-1 && x>0 && y<P-1 && y>0 && z<P-1 && z>0 && pp[n].t>0.1f){
	if(pp[n].t>0.1f){
	/*
		float dv = rho[x][y][z];
		float dx = (rho[x-1][y][z] - rho[x+1][y][z]);
		float dy = (rho[x][y-1][z] - rho[x][y+1][z]);
		float dz = (rho[x][y][z-1] - rho[x][y][z+1]);
		pp[n].dx+=dx;
		pp[n].dy+=dy;
		pp[n].dz+=dz;
		*/
		
		float dt = rand()%100/100.0f*.008f;
		//printf("dt: %f\n",dt);
		pp[n].t-=dt; //reduce the temperature (this would be from conduction)
	}else{
		
		float xi = 2.0f*cosf(t/100.0f);
		float zi = 2.0f*sinf(t/90.0f);;
		initParticle(n, 0.0f, 0.0f, 0.0f);
	}
	

}

void initParticles(){
	int n;
	for(n=0;n<N2;n++){
		initParticle(n,0.0f,0.0f, 0.0f);
		//logParticle(n);
	}
}

void updateParticles(){
	int n;
	//printf("update particles\n");
	//for each particle
	for(n=0;n<N2;n++){		
		updateParticle(n);
	}
}


int LoadGLtextures(char* file, int textureNo)							// Load Bitmap And Convert to A texture
{
	int Status;
	GLubyte *tex = new GLubyte[32 * 32 * 3];
	FILE *tf;

	tf = fopen ( file, "rb" );
	fread ( tex, 1, 32 * 32 * 3, tf );
	fclose ( tf );

	// do stuff
	Status=1;						// Set the Status to tRUE
	glGenTextures(1, &texture[0]);				// Create One texture

	glBindTexture(GL_TEXTURE_2D, texture[textureNo]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 32,
		0, GL_RGB, GL_UNSIGNED_BYTE, tex);

	delete [] tex;

	return Status;							// Return the Status
}


rgb getColor(float t){
	hsv c = {60.0 * t, 0.50, 0.50};
	return hsv2rgb(c);
}

void drawParticles(){
	//glClear(GL_DEPTH_BUFFER_BIT);	
	glPushMatrix();	
	glClearDepth(1.0f);						// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);					// Disable Depth testing
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);						// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);				// type Of Blending to Perform
	glEnable(GL_TEXTURE_2D);					// Enable texture Mapping
	glBindTexture(GL_TEXTURE_2D,texture[0]);			// Select Our texture
	glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
	glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
	glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
	glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);	
	glTranslatef(0.0f, 25.0f, 0.0f);
	int n;
	for(n=0;n<N2;n++){
		glClearDepth(1.0f);	
		glPixelTransferf(GL_RED_SCALE,0.299);
		glPixelTransferf(GL_BLUE_SCALE,0.299);
		glPixelTransferf(GL_GREEN_SCALE,0.299);
		rgb color = getColor(pp[n].t);
		float size = (1.1f - pp[n].t)*5000.0f/scale;
		if(size>1.0f){
			size=1.0f;
		}
		//printf("p[%d] color (%f,%f,%f)\n",n,color.r,color.b,color.g);
		//printf("p[%d] pos (%f,%f,%f), size %f\n",n,pp[n].x,pp[n].y,pp[n].z, size);
		glColor4f(color.r,color.g,color.b, pp[n].t);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2d(1,1); 
			glVertex3f(pp[n].x+size,pp[n].y+size,pp[n].z); // top Right
			glTexCoord2d(0,1);
			glVertex3f(pp[n].x,pp[n].y+size,pp[n].z); // top Left
			glTexCoord2d(1,0); 
			glVertex3f(pp[n].x+size,pp[n].y,pp[n].z); // Bottom Right
			glTexCoord2d(0,0); 
			glVertex3f(pp[n].x,pp[n].y,pp[n].z); // Bottom Left
		glEnd();
	}
	fflush(stdout);
	GLfloat emitReset[] = {0.0f, 0.0f, 0.0f, 0.0f};	
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emitReset);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);	
	glPopMatrix();
	
}

void update(){
	t++;
	if(t>1885){
		t=0;
	}
	fflush(stdout);
	updatePressure();
	fflush(stdout);
	updateParticles();	
}

void initialize(){
	t=0;
	printf("initializing");
	fflush(stdout);
	if(!LoadGLtextures("Particle.bmp",0)){
		printf("UNABLE TO LOAD TEXTURE");
	}
	printf("init pressure");
	fflush(stdout);
	initPressure();
	printf("init particles");
	fflush(stdout);
	initParticles();
	int i;
	
	for(i=0;i<N2/2;i++){
	  updateParticles();
	}
	
}

