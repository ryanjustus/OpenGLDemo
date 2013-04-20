#define N 3500
#define P 60


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
}FirePart;


float rho[P][P][P];  //Vector field containing pressure
float atm = 10.01f;
float g = 0.1f; //gravity
FirePart pp_fire[N]; //Array to hole particles
GLuint  texture[0];         // Storage For Our Particle texture
float scale = N*1/(log(N));
//float scale = N*3/(log(N)-3);
int t_fire;

/**
* Initial pressure from outside forming a boundary around the edge pointing inward
*/
void initPressure(){
	int i,j,k;
	for(i=0;i<P;i++){
		for(j=0;j<P;j++){
			if(i>0 && j>0 && i<P-1 && j<P-1){
				for(k=1;k<P-1;k++){
					if(j<P/2)
						rho[i][j][k]=14.0f;
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
	,n, pp_fire[n].x, pp_fire[n].y, pp_fire[n].z, pp_fire[n].dx ,pp_fire[n].dy ,pp_fire[n].dz);
	printf("\tactive=%i temp=%f\n",pp_fire[n].active, pp_fire[n].t);
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
	initPressure();
	int n;
	int d = P/2;
	for(n=0;n<N;n++){
		int x = (int)(pp_fire[n].x*6+d);
		int y = (int)(pp_fire[n].y*6+d);
		int z = (int)(pp_fire[n].z*6+d);
		//If it is in the box update the density
		//printf("position (%d,%d,%d)\n",x,y,z);
		int d = P/2;
		if(x<P && y<P && z<P && x>=0 && y>=0 && z>=0){
			//Add a pressure proportional to the temperature to rho			
			rho[x][y][z]+=pp_fire[n].t*10; 
			//printf("update p %d",n);
		}else{			
			pp_fire[n].active=0; //Mark the particle as dead
		}
	}
	//printRho();
}

/**
* Create an initial velocity based on tempurature, t
*/
void initFireParticle(int n, float xi, float yi, float zi){
	if(n>=N/2){
		xi*=-1;
		yi*=-1;
		zi*=-1;
	pp_fire[n].active=1;
	}
	pp_fire[n].x=(rand()%100/100.0f-0.5f)*0.5f+xi;
	pp_fire[n].z=(rand()%100/100.0f-0.5f)*0.5f+zi;
	pp_fire[n].y=(rand()%100/100.0f-0.5f)*0.3f+yi;
	pp_fire[n].t = rand()%100/100+0.5f;
	pp_fire[n].dx = rand()%40 - 20;
	pp_fire[n].dy = (rand()%100);
	pp_fire[n].dz = rand()%40 - 20;
}


void updateFireParticle(int n){
	
	//update the velocity based on current pressure field	
	pp_fire[n].x+=pp_fire[n].dx/scale;
	pp_fire[n].y+=(pp_fire[n].dy)/scale - g;
	pp_fire[n].z+=pp_fire[n].dz/scale;	
	float d = (float)P/2;	
	int x = (int)(pp_fire[n].x+d);
	int y = (int)(pp_fire[n].y+d);
	int z = (int)(pp_fire[n].z+d);
	if(x<P-1 && x>0 && y<P-1 && y>0 && z<P-1 && z>0 && pp_fire[n].t>0.2f){
		float dv = rho[x][y][z];
		float dx = (rho[x-1][y][z] - rho[x+1][y][z]);
		float dy = (rho[x][y-1][z] - rho[x][y+1][z]);
		float dz = (rho[x][y][z-1] - rho[x][y][z+1]);
		pp_fire[n].dx+=dx;
		pp_fire[n].dy+=dy;
		pp_fire[n].dz+=dz;
		
		
		float dt = rand()%100/100.0f*.01f;
		//printf("dt: %f\n",dt);
		pp_fire[n].t-=dt; //reduce the temperature (this would be from conduction)
	}else{
		float xi = 0.5f*cosf(t_fire/100.0f);
		float zi = 0.5f*sinf(t_fire/90.0f);
		//float xi = xGlobal;
		//float zi = zGlobal;
		initFireParticle(n, xi, 0.0f, zi);
	}
	

}

void initFireParticles(){
	int n;
	for(n=0;n<N;n++){
		initFireParticle(n,0.0f,0.0f, 0.0f);
		//logParticle(n);
	}
}

void updateFireParticles(){
	int n;
	//printf("update particles\n");
	//for each particle
	for(n=0;n<N;n++){		
		updateFireParticle(n);
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

void drawFireParticles(){	
	glPushMatrix();	
	glClearDepth(1.0f);						// Depth Buffer Setup
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);					// Disable Depth testing
	glEnable(GL_BLEND);						// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);				// type Of Blending to Perform
	glEnable(GL_TEXTURE_2D);					// Enable texture Mapping
	glBindTexture(GL_TEXTURE_2D,texture[0]);			// Select Our texture
	glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
	glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
	glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
	glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);
	glTranslatef(xGlobal, getHeight(xGlobal,zGlobal)+7,zGlobal);	
	int n;
	for(n=0;n<N;n++){	
		glPixelTransferf(GL_RED_SCALE,0.299);
		glPixelTransferf(GL_BLUE_SCALE,0.299);
		glPixelTransferf(GL_GREEN_SCALE,0.299);
		rgb color = getColor(pp_fire[n].t);
		float size = (1.1f - pp_fire[n].t)*5000.0f/scale;
		if(size>1.0f){
			size=1.0f;
		}
		//printf("p[%d] color (%f,%f,%f)\n",n,color.r,color.b,color.g);
		//printf("p[%d] pos (%f,%f,%f), size %f\n",n,pp_fire[n].x,pp_fire[n].y,pp_fire[n].z, size);
		glColor4f(color.r,color.g,color.b, pp_fire[n].t);
		glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2d(1,1); 
			glVertex3f(pp_fire[n].x+size,pp_fire[n].y+size,pp_fire[n].z); // top Right
			glTexCoord2d(0,1);
			glVertex3f(pp_fire[n].x,pp_fire[n].y+size,pp_fire[n].z); // top Left
			glTexCoord2d(1,0); 
			glVertex3f(pp_fire[n].x+size,pp_fire[n].y,pp_fire[n].z); // Bottom Right
			glTexCoord2d(0,0); 
			glVertex3f(pp_fire[n].x,pp_fire[n].y,pp_fire[n].z); // Bottom Left
		glEnd();
	}
	fflush(stdout);
	GLfloat emitReset[] = {0.0f, 0.0f, 0.0f, 0.0f};	
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emitReset);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D); 
	glPopMatrix();
	
}

void updateFire(){
	t_fire++;
	if(t_fire>1885){
		t_fire=0;
	}
	fflush(stdout);
	updatePressure();
	fflush(stdout);
	updateFireParticles();
	drawFireParticles();
}

void initializeFire(){
	t_fire=0;
	fflush(stdout);
	if(!LoadGLtextures("Particle.bmp",0)){
		printf("UNABLE TO LOAD TEXTURE");
	}
	fflush(stdout);
	initPressure();
	fflush(stdout);
	initFireParticles();
	int i;
	for(i=0;i<N;i++){
	  updateFireParticles();
	}
}

