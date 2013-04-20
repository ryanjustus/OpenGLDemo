#define N 3000
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
}Particle;

Particle particles[N]; //Array to hole particles

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

void drawBall(int t){
	glPushMatrix();
		glRotatef(180-360*t/200.0, 0.0, 1.0, 0.0);
		glTranslatef(0.0,10.0,40.0);
		glRotatef(-360*t/100.0*2, 0.0, 1.0, 0.0);
		drawStaticBucky(0.5f, 1.0f, gold, blue);
	glPopMatrix();

}


void drawGlass(){
	int i=0;
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0,0.0,0.0,0.5f);				

	glBegin(GL_TRIANGLE_STRIP);
		 glVertex3f(0,10, 20); // Top Right
		 glVertex3f(-10,10, 20); // Top Left
		 glVertex3f(0,0, 20); // Bottom Right
		 glVertex3f(-10, 0, 20); // Bottom Left
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

void drawBrokenGlass(int t){
	int i=0;
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0,0.0,0.0,0.5f);
	
    glPushMatrix();
	glRotatef(-360*t/100.0, 1.0, 1.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP);  //Top
		glVertex3f(-5.0,5.0,20.0);	//Middle
		glVertex3f(0.0,10.0, 20.0); // Top Right
		glVertex3f(-10.0,10.0, 20.0); // Top Left
	glEnd();
	glPopMatrix();
	
	glPushMatrix();	
	glRotatef(360*t/100.0, 1.0, 0.0, 1.0);
	glBegin(GL_TRIANGLE_STRIP); //Left
		glVertex3f(-5.0,5.0,20.0);	//Middle
		glVertex3f(-10.0,10.0, 20.0); // Top Left
		glVertex3f(-10.0, 0.0, 20.0); // Bottom Left
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(360*t/100.0, 0.0, 1.0, 1.0);
	glBegin(GL_TRIANGLE_STRIP); //Bottom
		glVertex3f(-5.0,5.0,20.0);	//Middle
		glVertex3f(-10.0, 0.0, 20.0); // Bottom Left
		glVertex3f(0.0,0.0, 20.0); // Bottom Right
	glEnd();
	glPopMatrix();
	
	glPushMatrix();
	glRotatef(-360*t/100.0, 1.0, 0.0, 0.0);
	glBegin(GL_TRIANGLE_STRIP); //Right
		glVertex3f(-5.0,5.0,20);	//Middle
		glVertex3f(0.0,0.0, 20.0); // Bottom Right
		glVertex3f(0.0,10.0, 20.0); // Top Right
	glEnd();
	glPopMatrix();
	
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
}

void drawGround(){
	int i=0;
	glPushMatrix();
	glBlendFunc (GL_SRC_ALPHA, GL_ONE);
	glColor4f(0.1f,0.1f,0.4f,0.5f);				
	
	glBegin(GL_TRIANGLE_STRIP); 
	for(int i=-125;i<125;i++){		
		
		for(int j=-125;j<125;j++){
			float yN = ((i+j) % 2 == 0) ? 1.1f : 0.9f;
			float theta = PI*(i+j)/200.0f + phaseShift2;
			glNormal3f(-1/2*pow(cos(theta),2),0.7+0.5*yN*1/2*sin(theta), 1/2*pow(cos(theta),2));
			//glNormal3f(0.0, 1.0f, 0.0f);
			 float y = pow(sin(theta)*1.4,3);
			 float d = 1.2f;
			 glVertex3f((j+1.0f)/d,y, (i+1.0f)/d); // Top Right
			 glVertex3f((j+0.0f)/d,y, (i+1.0f)/d); // Top Left
			 glVertex3f((j+1.0f)/d,y, (i+0.0f)/d); // Bottom Right
			 glVertex3f((j+0.0f)/d, y, (i+0.0f)/d); // Bottom Left
			
		}
	}
	glEnd();

	glColor3f(0.1f,0.1f,0.8f);
	for(float y=0.0f;y>-1.0f;y-=0.1f){
		glBegin(GL_TRIANGLE_STRIP);{
			glNormal3f(0.0, 1.0f, 0.0f);
			glVertex3f(125,y, 125); // Top Right
			glVertex3f(-125,y, 125); // Top Left
			glVertex3f(125,y, -125); // Bottom Right
			glVertex3f(-125, y, -125); // Bottom Left
		
		}glEnd();
	}
	
	glPopMatrix();
}

void initParticles(float a,float b,float c,float k, float maxTheta){
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
	for(i=0;i<N;i++){	
		
		float x = a*exp(-k*theta)*cos(theta);
		float y = -c*theta;
		float z = b*exp(-k*theta)*sin(theta);
		theta+=dTheta;
		
		particles[i].active=1;
		particles[i].life=1.0f;
		particles[i].fade= rand()%100/1000.0f+0.9f;
		particles[i].color = colors[i%6];
		
		particles[i].x=x;
		particles[i].y=y;
		particles[i].z=z;
		
		particles[i].xv = (rand()%50*1.0f-26.0f)*8.0f;
		particles[i].yv = ((rand()%50)*1.0f-26.0f)*21.0f; 
		particles[i].zv = ((rand()%50)*1.0f-26.0f)*8.0f;
		
		particles[i].xa = 0.5f;
		particles[i].ya = 11.9f;
		particles[i].za = 0.5f;		
	}
}

void updateParticles(float a,float b,float c,float k, float maxTheta){
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
	
	//glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular1);
	float theta=0;
	for(i=0;i<N;i++){				
		
		theta+=dTheta;
		//draw the particle
		
		glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
		glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
		glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
		glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);
		
		//GLfloat emit[] = {0.2*particles[i].color.r, 0.2*particles[i].color.g, 0.2*particles[i].color.b, 1.0};
		GLfloat emit[] = {particles[i].color.r, particles[i].color.g, particles[i].color.b, 0.0};			
		glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emit);
		glPixelTransferf(GL_RED_SCALE,0.299);
		glPixelTransferf(GL_BLUE_SCALE,0.299);
		glPixelTransferf(GL_GREEN_SCALE,0.299);
		glColor4f(particles[i].color.r,particles[i].color.g,particles[i].color.b,particles[i].life);
		glBegin(GL_TRIANGLE_STRIP);
			float size = 1.1f-particles[i].life/1.2f;
			glTexCoord2d(1,1); glVertex3f(particles[i].x+size,particles[i].y+size,particles[i].z); // Top Right
			glTexCoord2d(0,1); glVertex3f(particles[i].x-size,particles[i].y+size,particles[i].z); // Top Left
			glTexCoord2d(1,0); glVertex3f(particles[i].x+size,particles[i].y-size,particles[i].z); // Bottom Right
			glTexCoord2d(0,0); glVertex3f(particles[i].x-size,particles[i].y-size,particles[i].z); // Bottom Left
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
			float xOffset = cos(phaseShift2)*20;
			float zOffset = sin(phaseShift2)*10;
			float x = a*exp(-k*theta)*cos(theta+phaseShift) + xOffset;
			float y = -c*theta;
			float z = b*exp(-k*theta)*sin(theta+phaseShift) + zOffset;
			
			particles[i].life=1.0f;
			particles[i].fade= rand()%100/1000.0f+0.015f;
			
			//Reset the particle to the new location
			particles[i].x=x;
			particles[i].y=y;
			particles[i].z=z;
			
			particles[i].xv = ((rand()%50)*1.0f-26.0f)*8.0f;
			particles[i].yv = ((rand()%50)*1.0f-26.0f)*21.0f; 
			particles[i].zv = ((rand()%50)*1.0f-26.0f)*8.0f;
			particles[i].color = colors[i%4];		
		}
	}
	GLfloat emitReset[] = {0.0f, 0.0f, 0.0f, 0.0f};	
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emitReset);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_TEXTURE_2D); 
	glPopMatrix();
}

