#define N_BOID 18
#define BOX_SIZE 50

typedef struct{
	int active;	
	//Position
	float x;
	float y;
	float z;
	float dx;
	float dz;
	float dy;
	//Velocity
	float theta;
}Boid;
int tb = 0;
Boid bb[N_BOID]; //Array to hold boids
float speedLimit = 0.09f;
float boidSize = 6.5f;
float rForce = 1.25f;
float aForce = 1.09f;

//waypoint
float wx=-30.05;
float wy=10.0f;
float wz= 10.0f;


void initBoids(){
	//randomize boids initially
	float xmax = BOX_SIZE*2.0f-4;
	float ymax = BOX_SIZE*1.0f-2;
	float zmax = BOX_SIZE*1.0f-2;
	int i=0;
	for(i=0;i<N_BOID;i++){
		
		float x = rand()%100/100.0f*xmax-xmax/2.0f;
		float y = rand()%100/100.0f*ymax;
		float z = rand()%100/100.0f*zmax-zmax/2.0f;
		bb[i].x=x;
		bb[i].y=y;
		bb[i].z=z;
		bb[i].theta=0.0f;
		bb[i].active=1;
		initTornado(16.0, 10.0, 4.0,0.13, 50*PI, 
		bb[i].x, bb[i].y, bb[i].z, 
		-bb[i].dx, -bb[i].dy, -bb[i].dz,
		i*N/N_BOID, N/N_BOID);
	}
	fflush(stdout);
}

void updateBoids(){
	tb++;
	//if(tb%100==0)
	//	printf("time: %d",tb);
	float a=aForce;
	if(tb>1000){
		tb=0;
	}
	if(tb>700){
		a*=-1;
	}
	int i,j;
	for(i=0;i<N_BOID;i++){
		
		//update position
		bb[i].x+=bb[i].dx;
		bb[i].z+=bb[i].dz;
		bb[i].y+=bb[i].dy;
		
		float sumDx = 0.0f;
		float sumDz = 0.0f;
		float sumDy = 0.0f;
		//boid boid interaction
		for(j=0;j<N_BOID;j++){
			//attract
			if(i==j)
				continue;
			
			float xd = bb[j].x -bb[i].x;
			float zd = bb[i].z-bb[j].z;
			float yd = bb[i].y-bb[j].y;
			//float zd = 0.0f;
			float d = sqrt(pow(xd,2) + pow(zd,2)+ pow(yd,2));
			if(d<0.0f){
				d=1.0f;
			}
			float rdist = d-boidSize;
			if(rdist<0.4){
				rdist=0.4f;
			}
			//printf("\tboid 1 %d x,dx: (%f,%f)\n",i,bb[i].x,bb[i].dx);
			//printf("\tboid 2 %d x,dx: (%f,%f)\n",j,bb[j].x,bb[j].dx);
			//printf("\tdx  %f\n",xd/pow(d,2));
			//printf("\tdz  %f\n",zd/pow(d,2));
			sumDx+=(xd/pow(d,1)*a - xd/pow(rdist,2))/((float)N_BOID);
			sumDz+=(zd/pow(d,1)*a - xd/pow(rdist,2))/((float)N_BOID);
			sumDy+=(yd/pow(d,1)*a - yd/pow(rdist,2))/((float)N_BOID);				
		}
		
		//boid sun interaction
		sumDx+=bb[i].x/(pow(bb[i].x,2) + pow(bb[i].y,2)+ pow(bb[i].z,2));
		sumDy+=bb[i].y/(pow(bb[i].x,2) + pow(bb[i].y,2)+ pow(bb[i].z,2));
		sumDz+=bb[i].z/(pow(bb[i].x,2) + pow(bb[i].y,2)+ pow(bb[i].z,2));
		
		/*
		//boid waypoint interaction
		float xwd = wx-bb[i].x;
		if(fabs(xwd)<0.5f){
			xwd=0.5f*fabs(xwd)/xwd;
		}
		float ywd = wy-bb[i].y;
		if(fabs(ywd)<0.5f){
			ywd=0.5f*fabs(ywd)/ywd;
		}
		float zwd = wz-bb[i].z;
		if(fabs(zwd)<0.5f){
			zwd=0.5f*fabs(zwd)/zwd;
		}
		sumDx+=2/xwd;
		sumDy+=2/ywd;
		sumDz+=2/zwd;
		*/
		
		//boid wall interaction
		if(bb[i].x < -BOX_SIZE+10.0f){
			sumDx+=0.5f;
		}
		if(bb[i].x > BOX_SIZE-10.0f){
			sumDx-=0.5f;
		}
		
		//boid wall interaction
		if(bb[i].z < -BOX_SIZE/2.0f + 10.0f){
			sumDz-=0.5f;
		}
		if(bb[i].z > BOX_SIZE/2.0f-0.0f){
			sumDz+=0.5f;
		}
		
		//boid wall interaction
		if(bb[i].y < 5.0f){
			sumDy=0.5f;
		}
		if(bb[i].y > BOX_SIZE-5.0f){
			sumDy=-0.5f;
		}
		
		//printf("boid %d x,dx: (%f,%f\n",i,bb[i].x,bb[i].dx);
		float dragx =0.0f;
		if(fabs(bb[i].dx)>0.5){
			dragx = -bb[i].dx;	
		}
		
		float dragy =0.0f;
		if(fabs(bb[i].dy)>0.5){
			dragy = 0.0f;//-bb[i].dy;	
		}
		
		float dragz =0.0f;
		if(fabs(bb[i].dz)>0.5){
			dragx = -bb[i].dz;	
		}
		
		bb[i].dx+=(sumDx + dragx)/500.0f;
		bb[i].dz-=(sumDz + dragz)/500.0f;
		bb[i].dy+=(sumDy + dragy)/500.0f;
		
		//speed limit
		if(fabs(bb[i].dx)>speedLimit){
			bb[i].dx=speedLimit*fabs(bb[i].dx)/bb[i].dx;
		}
		if(fabs(bb[i].dy)>speedLimit){
			bb[i].dy=speedLimit*fabs(bb[i].dy)/bb[i].dy;
		}
		if(fabs(bb[i].dz)>speedLimit){
			bb[i].dz=speedLimit*fabs(bb[i].dz)/bb[i].dz;
		}
		
		bb[i].theta = atan(-bb[i].dz/bb[i].dx)*180/PI;
		if(bb[i].dx<0){
			bb[i].theta+=180;
		}else if(bb[i].dx>0 && bb[i].dz>0){
			//bb[i].theta+=360;
		}
		//printf("\ttheta  %f\n",bb[i].theta);
		
	}
	
}

void drawRocket(int i){
	glPushMatrix();
	glTranslatef(bb[i].x,bb[i].y,bb[i].z);	
	
	glRotatef(bb[i].theta-180, 0.0, 1.0, 0.0); //rotate y	
	float phi = bb[i].dy/speedLimit*30;
	glRotatef(90-phi, 0.0, 0.0, 1.0); //rotate x
	
	drawCylinder(1.0f, 3.0f, red, white);
	
	glNormal3f(1.0f,0.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(red.r,red.g,red.b);		
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 4.0f);
		glVertex3f(0.0f, 4.0f, 0.0f);
	}glEnd();
	
	glNormal3f(1.0f,0.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(red.r,red.g,red.b);		
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, -4.0f);
		glVertex3f(0.0f, 4.0f, 0.0f);
	}glEnd();
	
	glNormal3f(1.0f,0.0f, 0.0f);
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(red.r,red.g,red.b);		
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(3.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 2.0f, 0.0f);
	}glEnd();
	
	
	glTranslatef(0.0f, 3.0f, 0.0f);
	drawSphere(1.2f,4,4, white, red);	
	glRotatef(tb*10%360,0.0f,1.0f,0.0f);
	glNormal3f(0.0f,0.0f, 1.0f);
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(white.r,white.g,white.b);		
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(3.0f, 0.0f, 1.0f);
		glVertex3f(3.0f, 0.0f, -1.0f);
	}glEnd();
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(red.r,red.g,red.b);
		glNormal3f(0.0f,0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-3.0f, 0.0f, 1.0f);
		glVertex3f(-3.0f, 0.0f, -1.0f);
	}glEnd();
	
	glNormal3f(0.0f,0.0f, -1.0f);
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(white.r,white.g,white.b);
		glNormal3f(0.0f,0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(3.0f, 0.0f, 1.0f);
		glVertex3f(3.0f, 0.0f, -1.0f);
	}glEnd();
	glBegin(GL_TRIANGLE_FAN);{
		glColor3f(red.r,red.g,red.b);
		glNormal3f(0.0f,0.0f, 1.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(-3.0f, 0.0f, 1.0f);
		glVertex3f(-3.0f, 0.0f, -1.0f);
	}glEnd();
	glPopMatrix();
}

void drawBoids(){
	

	int i=0;
	for(i=0;i<N_BOID;i++){
		if(bb[i].dz>=0){
			updateTornado(1.0, 1.0, -1.0,0.83, 3*PI,
					bb[i].x, bb[i].y,bb[i].z,
					-bb[i].dx, -bb[i].dy, -bb[i].dz,
					i*N/N_BOID, N/N_BOID);
		}
					
		glPolygonMode( GL_FRONT_AND_BACK , GL_FILL);
		glMaterialfv(GL_FRONT, GL_AMBIENT, goldMat.ambient);         
		glMaterialfv(GL_FRONT, GL_DIFFUSE, goldMat.diffuse);         
		glMaterialfv(GL_FRONT, GL_SPECULAR, goldMat.specular);         
		glMaterialf(GL_FRONT, GL_SHININESS, goldMat.shininess);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
		glColor3f(red.r,red.b, red.g);
		glEnable(GL_LIGHTING);
		//printf("boid: %d (%f, %f, %f),(%f, %f, %f)\n",i,bb[i].x,bb[i].y,bb[i].z,bb[i].dx,bb[i].dy,bb[i].dz);
		drawRocket(i);
		
		if(bb[i].dz<0){
			updateTornado(1.0, 1.0, -1.0,1.03, 4*PI,
					bb[i].x, bb[i].y,bb[i].z,
					-bb[i].dx, -bb[i].dy, -bb[i].dz,
					i*N/N_BOID, N/N_BOID);
		}
		
		
	}
	
}



void drawBox(){
	glDisable(GL_LIGHTING);
	int i=0;
	//draw bottom
	glColor3f(paleGreen.r, paleGreen.g, paleGreen.b);
	glBegin(GL_LINES);
	for(i=0;i<=BOX_SIZE;i+=2){		
		glVertex3f(i, 0, -BOX_SIZE/2);
		glVertex3f(i, 0, BOX_SIZE/2);
		
		glVertex3f(i-BOX_SIZE, 0, -BOX_SIZE/2);
		glVertex3f(i-BOX_SIZE, 0, BOX_SIZE/2);
		
		glVertex3f(-BOX_SIZE, 0, i-BOX_SIZE/2);
		glVertex3f(BOX_SIZE, 0, i-BOX_SIZE/2);			
	}
	
	
	glColor3f(blue.r, blue.g, blue.b);
	for(i=0;i<=BOX_SIZE;i+=5){		
			glVertex3f(-BOX_SIZE, i, BOX_SIZE/2);
			glVertex3f(-BOX_SIZE, i, -BOX_SIZE/2);			
			glVertex3f(-BOX_SIZE, 0, i-BOX_SIZE/2);
			glVertex3f(-BOX_SIZE, BOX_SIZE, i-BOX_SIZE/2);			
			glVertex3f(BOX_SIZE, i, BOX_SIZE/2);
			glVertex3f(BOX_SIZE, i, -BOX_SIZE/2);			
			glVertex3f(BOX_SIZE, 0, i-BOX_SIZE/2);
			glVertex3f(BOX_SIZE, BOX_SIZE, i-BOX_SIZE/2);			
		
		//draw back
			glVertex3f(i, 0, -BOX_SIZE/2);
			glVertex3f(i, BOX_SIZE, -BOX_SIZE/2);			
			glVertex3f(i-BOX_SIZE, 0, -BOX_SIZE/2);
			glVertex3f(i-BOX_SIZE, BOX_SIZE, -BOX_SIZE/2);			
			glVertex3f(-BOX_SIZE, i, -BOX_SIZE/2);
			glVertex3f(BOX_SIZE, i, -BOX_SIZE/2);			
	}
	glEnd();
	
	glEnable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(0.0f,BOX_SIZE/2.0f,0.0f);
	drawSphere(4.0f,10,10, yellow, white);
	glPopMatrix();
}
