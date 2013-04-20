# Project 1
# Generate Project 1
project5: project5.c shapes.h colors.h materials.h water.h rterrain.h particle_fire.h
	g++ project5.c -o project5 glut32.lib -lopengl32 -lglu32 -lcygwin

rterrain: rterrain.c
	g++ rterrain.c -o rterrain glut32.lib -lopengl32 -lglu32 -lcygwin	

kinematics: kinematics.c 
	g++ kinematics.c -o kinematics glut32.lib -lopengl32 -lglu32 -lcygwin	
	
clean: 
	rm *.exe