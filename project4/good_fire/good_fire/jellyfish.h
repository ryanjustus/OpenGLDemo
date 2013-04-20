void drawJellyfish(){

	float radius = 5.0f;
	int cuts = 20;
	int stacks = 20;
	rgb color1 = neonBlue;
	rgb color2 = neonGreen;
	float alpha = 1.0f;

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
		float xyFactor = sinf(dPhi); //same as sin(PI/stacks5
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