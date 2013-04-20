#ifndef FILE_MATERIALS_SEEN
#define FILE_MATERIALS_SEEN
typedef struct {
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat shininess;
} material;
material goldMat = {0.24725, 0.1995, 0.0745, 1.0,
		 0.75164, 0.60648, 0.22648, 1.0, 
		 0.628281, 0.555802, 0.366065, 1.0,
		 100.2};
		 
material blackRubber = {0.02, 0.02, 0.02,  1.0, 
			0.01, 0.01, 0.01, 1.0,
			0.40, 0.40, 0.40, 1.0,
			10.0};
#endif



			
			