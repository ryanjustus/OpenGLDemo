#define N 2000
GLfloat lightSpecular1[] = {1.0, 1.0, 1.0, 1.0}; // Default
typedef struct{
	int active;
	float life;
	float fade;
	rgb color;
	//Position
	float x;
	float y;
	float z;
	//Velocity
	float xv;
	float yv;
	float zv;
	//Acceleration
	float xa;
	float ya;
	float za;
}ParticleTornado;

ParticleTornado particles[N]; //Array to hole particles

GLuint  texture[2];         // Storage For Our Particle Texture

float slowdown=2.0f;
float phaseShift = 0;
float phaseShift2 = 0;
rgb colors[6];

int LoadGLTextures(char* file, int textureNo)							// Load Bitmap And Convert To A Texture
{
	int Status;
	GLubyte *tex = new GLubyte[32 * 32 * 3];
	FILE *tf;

	tf = fopen ( file, "rb" );
	fread ( tex, 1, 32 * 32 * 3, tf );
	fclose ( tf );

	// do stuff
	Status=1;						// Set The Status To TRUE
	glGenTextures(1, &texture[0]);				// Create One Texture

	glBindTexture(GL_TEXTURE_2D, texture[textureNo]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, 32, 32,
		0, GL_RGB, GL_UNSIGNED_BYTE, tex);

	delete [] tex;

	return Status;							// Return The Status
}


void initBackground(){
	if(!LoadGLTextures("Lightning_Bolt.bmp",1)){
		printf("UNABLE TO LOAD TEXTURE");
	}
}

void initTornado(float a,float b,float c,float k, float maxTheta, 
				float xi, float yi, float zi,
				float xvi, float yvi, float zvi, int offset, int len){
	if(!LoadGLTextures("Particle.bmp",0)){
		printf("UNABLE TO LOAD TEXTURE");
	}
	
	colors[0] = yellow;
	colors[1] = white;
	colors[2] = neonOrange;
	colors[3] = white;
	colors[4] = neonBlue;
	colors[5] = neonGreen;
	
	int i;
	float theta=0;
	float dTheta = maxTheta/N;
	for(i=offset;i<offset+len;i++){	
		
		float x = a*exp(-k*theta)*cos(theta) + xi;
		float y = -c*theta + yi;
		float z = b*exp(-k*theta)*sin(theta) + zi;
		theta+=dTheta;
		
		particles[i].active=1;
		particles[i].life=1.0f;
		particles[i].fade= rand()%100/1000.0f+0.9f;
		particles[i].color = colors[i%6];
		
		particles[i].x=x;
		particles[i].y=y;
		particles[i].z=z;
		
		particles[i].xv = xvi;
		particles[i].yv = yvi; 
		particles[i].zv = zvi;
		
		particles[i].xa = 0.5f;
		particles[i].ya = 0.5f;
		particles[i].za = 0.5f;		
	}
}

void updateTornado(float a,float b,float c,float k, float maxTheta,  
					float xi, float yi, float zi,float xvi, 
					float yvi, float zvi, int offset, int len){
	int i;
	
	float dTheta = maxTheta/N;
	
	phaseShift+=PI/10;
	if(phaseShift>=2*PI){
		phaseShift=0;
	}
	
	phaseShift2+=PI/500;
	if(phaseShift2>=2*PI){
		phaseShift2=0;
	}
	glPushMatrix();
	
	glClearDepth(1.0f);						// Depth Buffer Setup
	glDisable(GL_DEPTH_TEST);					// Disable Depth Testing
	glEnable(GL_BLEND);						// Enable Blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);				// Type Of Blending To Perform
	glEnable(GL_TEXTURE_2D);					// Enable Texture Mapping
	glBindTexture(GL_TEXTURE_2D,texture[0]);			// Select Our Texture	
	glDisable(GL_LIGHTING);
	//glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);
	float theta=0;
	for(i=offset;i<offset+len;i++){				
		
		theta+=dTheta;
		//draw the particle
		
		glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
		glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
		glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
		glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);
		
		//GLfloat emit[] = {0.2*particles[i].color.r, 0.2*particles[i].color.g, 0.2*particles[i].color.b, 1.0};
		//GLfloat emit[] = {particles[i].color.r, particles[i].color.g, particles[i].color.b, 0.0};			
		//glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emit);
		glPixelTransferf(GL_RED_SCALE,0.299);
		glPixelTransferf(GL_BLUE_SCALE,0.299);
		glPixelTransferf(GL_GREEN_SCALE,0.299);
		glColor4f(particles[i].color.r,particles[i].color.g,particles[i].color.b,particles[i].life);
		glBegin(GL_TRIANGLE_STRIP);
			float size = 0.9f-particles[i].life/1.0f;
			glTexCoord2d(0.5,0.5); glVertex3f(particles[i].x+size,particles[i].y+size,particles[i].z); // Top Right
			glTexCoord2d(-0.5,0.5f); glVertex3f(particles[i].x-size,particles[i].y+size,particles[i].z); // Top Left
			glTexCoord2d(0.5,-0.5); glVertex3f(particles[i].x+size,particles[i].y-size,particles[i].z); // Bottom Right
			glTexCoord2d(-0.5,-0.5); glVertex3f(particles[i].x-size,particles[i].y-size,particles[i].z); // Bottom Left
		glEnd();

		

		//update the particles position
		particles[i].x+=particles[i].xv/(slowdown*1000);   
		particles[i].y+=particles[i].yv/(slowdown*1000);   
		particles[i].z+=particles[i].zv/(slowdown*1000); 
		
		//update the particles velocity
		particles[i].xv+=particles[i].xa;   
		particles[i].yv+=particles[i].ya;   
		particles[i].zv+=particles[i].za; 
		
		
		particles[i].life-=particles[i].fade;
		
		//if the particles is burned out turn it off, reset it
		if(particles[i].life<0.0f){

			float x = a*exp(-k*theta)*cos(theta+phaseShift) + xi;
			float y = -c*theta + yi;
			float z = b*exp(-k*theta)*sin(theta+phaseShift) + zi;
			
			particles[i].life=1.0f;
			particles[i].fade= rand()%100/1000.0f+0.015f;
			
			//Reset the particle to the new location
			particles[i].x=x;
			particles[i].y=y;
			particles[i].z=z;
			
			particles[i].xv = xvi;
			particles[i].yv = yvi; 
			particles[i].zv = zvi;
			particles[i].color = colors[i%4];		
		}
	}
	
	GLfloat emitReset[] = {0.0f, 0.0f, 0.0f, 0.0f};	
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emitReset);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D); 
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

