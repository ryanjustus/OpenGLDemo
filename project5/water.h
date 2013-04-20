#define NW 2000
#define M 97

GLfloat lightSpecular1[] = {1.0, 1.0, 1.0, 1.0}; // Default
typedef struct{
	int active;
	float life;
	rgb color;
	
	//mass
	float m;
	
	//Position
	float x;
	float y;
	float z;
	
	//Momentum
	float px;
	float py;
	float pz;
}Particle;

Particle pp[NW]; 
float field[M][M][M];

int t=0;
float dt = 0.1f;

//global variables
float rate = 1.0f; //flow rate of particles
//float dLife = rate*dt/((float)N)/100.0f; //lifetime decrement from particles
float dLife=0.002f;

float gWater = 5.0f; //gravity
float drag = 0.4f;
float groundDamp = 0.6f;
float nozelHeight;
float nozelWidth;

//function prototypes
void resetParticle(int i);
void initParticle(int i);
void drawParticle(int i);
void updateParticles();
void updateParticle(int i);
void updateField();

//Background field
//potential energy at point x,y,z
void V(float x, float y, float z, float v[]); 

void clearField(){
	memset(field, 0, sizeof(field)); //clear out field
}

float getField(float x, float y, float z){
	float dx = x>0 ? 0.5f : -0.5f;
	int xf = (int)floor(x+dx) + M/2;
	float dy = y>0 ? 0.5f : -0.5f;
	int yf = (int)floor(y+dy);
	float dz = z>0 ? 0.5f : -0.5f;
	int zf = (int)floor(z+dz) + M/2;

	if(xf<M && yf<M && zf<M && xf>=0 && yf>=0 && zf>=0){			
		return field[xf][yf][zf]; //for now just sum the particles to contribute to field		
	}else{
		return 0.0f;
	}
}


//Add return potential field for particle i
void dV(int i, float v[]){		
	
	float x = pp[i].x;
	float y = pp[i].y;
	float z = pp[i].z;
	
	//initialize vector for output
	v[0]=0.0f; v[1]=0.0f; v[2]=0.0f; 	
	
	//Add in terrain features, nozel
	
	if(y<=nozelHeight && pow(nozelWidth,2) > pow(x,2)+pow(z,2) && pow(nozelWidth+1,2) < pow(x,2)+pow(z,2)){
		v[0]-= x/fabs(x);
		v[1]+= 0.0f;
		v[2]-= z/fabs(z);

	}	
	
	float yG = getHeight(x,z)+1.5f;
	
	//printf("height %f\n",yG);
	if(y<yG){
		//printf("below ground\n");
		//v[1]+=(fabs(y)-pow(y,5)); // add in potential energy from ground
		float yGx = getHeight(x+1.0f,z);
		float yGz = getHeight(x,z+1.0f);
		float yNorm  = (yG-1.0f)/((yGx+yGz)/2.0f);
		v[0]+=(yG-1.0f-yGx);
		v[1]+=yNorm*fabs(y-yG)-(pow(y-yG,3)); // add in potential energy from ground
		v[2]+=(yG-1.0f-yGz);

	}else{
		v[1]-=gWater; //add in potential energy from gravity
	}
	
	//v[0]+=0.3f;
	//v[2]+=0.3f;
	
	//add in potential energy gradient from particle pressure differential
	float Vxyz = getField(x,y,z);
	float dVdx,dVdy,dVdz;
	if(x<0){
		//dVdx = getField(x-1.0f, y, z)- Vxyz;
		dVdx = (Vxyz - getField(x-1.0f, y, z))/3.0;
	}else{
		dVdx = (Vxyz - getField(x+1.0f, y, z))/3.0;
		//dVdx = getField(x+1.0f, y, z)- Vxyz;
	}
	
	if(z<0){
		//dVdz = getField(x, y, z-1.0f)- Vxyz;
		dVdz = (Vxyz - getField(x, y, z-1.0f))/3.0;
	}else{
		dVdz = (Vxyz - getField(x, y, z+1.0f))/3.0;
		//dVdz = getField(x, y, z+1.0f)- Vxyz;
	}
	dVdy = getField(x,y+1.0f, z) - Vxyz;
	//dVdz = getField(x,y, z+1.0f) - Vxyz;
	
	v[0]+=dVdx;
	v[1]+=dVdy;
	v[2]+=dVdz;
	
	//add in potential energy gradient from drag (proportional to velocity)
	float dragPot = 0.0f;
	if(y>yG+1.0f){
		dragPot = drag;
	}else{
		dragPot = groundDamp;
	}
	
	float dVdpx = pp[i].px/pp[i].m*drag/2.0f;
	float dVdpy = pp[i].py/pp[i].m*dragPot;
	float dVdpz = pp[i].pz/pp[i].m*drag/2.0f;
	
	v[0]-=dVdpx;
	v[1]-=dVdpy;
	v[2]-=dVdpz;	
}

void initParticle(int i){		
	pp[i].x= xGlobal + (rand()%10 - 5)/10.0f+0.0f;	
	pp[i].z= zGlobal + (rand()%10 - 5)/10.0f+0.0f;
	pp[i].y=getHeight(pp[i].x,pp[i].z);
	
	pp[i].px = (rand()%10 - 5)/2.0f;
	pp[i].py = 20.0f; 
	pp[i].pz = (rand()%10 - 5)/2.0f;
	
	pp[i].m = 1.0f;
	
	pp[i].life=1.0f + rand()%10/5.0f;
	pp[i].active=0;
}

void initWater(){
	int i;
	for(i=0;i<NW;i++){				
		initParticle(i);
	}
	for(int i=0;i<NW/4;i++){
		pp[i].active=1;
		updateField();
		updateParticles();
	}
}

void updateField(){
	
	clearField();
	for(int i=0;i<NW;i++){		
		int xf = (int)floor(pp[i].x+0.5f) + M/2;
		int yf = (int)floor(pp[i].y+0.5f);
		int zf = (int)floor(pp[i].z+0.5f) + M/2;
		if(xf<M-1 && yf<M-1 && zf<M-1 && xf>=1 && yf>=1 && zf>=1){
			for(int i=-1;i<=1;i++){
				for(int j=-1;i<=1;i++){
					for(int k=-1;i<=1;i++){
						if(0==i==j==k){
							field[i+xf][j+yf][k+zf]+=3.0f;
						}else{
							field[i+xf][j+yf][k+zf]+=1.0f;
						}
					}
				}
			}
		}
	}
}

void updateParticles(){		
	for(int i=0;i<NW;i++){	
		if(pp[i].life<0.0f){
			initParticle(i);
			pp[i].active=1;
		}
			
		pp[i].life-=dLife;	
		
	
		//H = K + V (K is kinetic energy, V is potential energy)
		//update particle position dq/dt = dK/dp
		pp[i].x+=pp[i].px*dt;
		pp[i].y+=pp[i].py*dt;
		pp[i].z+=pp[i].pz*dt;	

		//update particle moment dp/dt = -dV/dx
		float v[3];
		dV(i,v);
		pp[i].px+=v[0]*dt;
		pp[i].py+=v[1]*dt;
		pp[i].pz+=v[2]*dt;	
		
	}
}

void drawParticle(int i){
	glPushMatrix();
	glTranslatef(pp[i].x, pp[i].y, pp[i].z);
	drawSphere(0.9f,5,5, red, yellow);
	glPopMatrix();
}

void updateWater(){
	updateField();
	updateParticles();
	glPolygonMode( GL_FRONT , GL_FILL);
	glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
	glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
	glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
	glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPushMatrix();
		glTranslatef(xGlobal,getHeight(xGlobal,zGlobal)-2.0f,zGlobal);
		drawCone(1.0f, 3.0f, 8.0f, olive, red);
	glPopMatrix();
	for(int i=0;i<NW;i++){			
		if(!pp[i].active)
			continue;		
		drawParticle(i);		
	}	
	GLfloat emitReset[] = {0.0f, 0.0f, 0.0f, 0.0f};	
	glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION, emitReset);	
	
}
