# Project 1
# Generate Project 1
project3: project3.c shapes.h colors.h tornado.h materials.h
	g++ project3.c -o project3 glut32.lib -lopengl32 -lglu32 -lcygwin	

lox: loxodrome.c
	g++ loxodrome.c -o lox glut32.lib -lopengl32 -lglu32 -lcygwin	

water: water.c 	noise.c noise.h
	g++ water.c -o water glut32.lib -lopengl32 -lglu32 -lcygwin
	
clean: 
	rm *.exe