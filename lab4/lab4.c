// Demo of heavily simplified sprite engine
// by Ingemar Ragnemalm 2009
// used as base for lab 4 in TSBK03.
// OpenGL 3 conversion 2013.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// uses framework Cocoa
#else
	#include <GL/gl.h>
	#include "MicroGlut.h"
#endif

#include <stdlib.h>
#include "LoadTGA.h"
#include "SpriteLight.h"
#include "GL_utilities.h"
#include <math.h>

// L�gg till egna globaler h�r efter behov.
GLfloat avg_h;
GLfloat avg_v;
GLfloat cohesion_weight = 0.001;
GLfloat avoidance_weight = 0.005;

void SpriteBehavior() // Din kod!
{
	// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
	// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
	// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
	// hastigheter och positioner, eller arbeta fr�n egna globaler.
	SpritePtr spi = gSpriteRoot;
	SpritePtr spj = spi->next;
	avg_h = 0;
	avg_v = 0;
	int counter = 0;
	GLfloat maxdist = 120;
	GLfloat mindist = 40;
	while(spi != NULL)
	{
		spi->avg_position.h = 0;
		spi->avg_position.v = 0;
		spi->avoidance.h = 40;
		spi->avoidance.v = 40;
		counter = 0;
		spj = gSpriteRoot;
		while(spj != NULL){
			if(spi != spj){
				// Calculate distance to all boids j from the boid i
				GLfloat dist = sqrt(
					(spi->position.h-spj->position.h) * (spi->position.h-spj->position.h) +
					(spi->position.v-spj->position.v) * (spi->position.v-spj->position.v));
				if(dist < maxdist){
					// fprintf(stderr, " BOP ");
					// Sum the position of all boids within radius
					spi->avg_position.h += spj->position.h;
					spi->avg_position.v += spj->position.v;
					++counter;
					if(dist < mindist){
						spi->avoidance.h += spi->position.h - spj->position.h;
						spi->avoidance.v += spi->position.v - spj->position.v;
					}
				}
			}
			spj = spj->next;
		}
		// avg_h += sp->position.h;
		// avg_v += sp->position.v;
		// counter++;

		// Divide by number of boids within radius to get average
		if(counter>0){
			spi->avg_position.h /= counter;
			spi->avg_position.v /= counter;
			// spi->avoidance.h /= counter;
			// spi->avoidance.v /= counter;
		}
		spi = spi->next;
	}
	// avg_h /= counter;
	// avg_v /= counter;

	// Print avg position
	// char * buf_h[3];
	// char * buf_v[3];
	// gcvt(avg_h, 3, buf_h);
	// gcvt(avg_v, 3, buf_v);
	// fprintf(stderr, "Average position = (");
	// fprintf(stderr, buf_h);
	// fprintf(stderr, ",");
	// fprintf(stderr, buf_v);
	// fprintf(stderr, ")\n");

	// Reset sp and rotate boids
	spi = gSpriteRoot;
	while(spi != NULL){
		if(spi->avg_position.h > 0 || spi->avg_position.v > 0){
			spi->speed.h += (spi->avg_position.h - spi->position.h) * cohesion_weight;
			spi->speed.v += (spi->avg_position.v - spi->position.v) * cohesion_weight;
			spi->speed.h += fmax(0, (mindist-spi->avoidance.h)) * avoidance_weight;
			spi->speed.v += fmax(0, (mindist-spi->avoidance.v)) * avoidance_weight;
			spi->position.h += spi->speed.h;
			spi->position.v += spi->speed.v;
		}
		spi = spi->next;
	}
}

// Drawing routine
void Display()
{
	SpritePtr sp;

	glClearColor(0, 0, 0.2, 1);
	glClear(GL_COLOR_BUFFER_BIT+GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	DrawBackground();

	SpriteBehavior(); // Din kod!

// Loop though all sprites. (Several loops in real engine.)
	sp = gSpriteRoot;
	do
	{
		HandleSprite(sp); // Callback in a real engine
		DrawSprite(sp);
		sp = sp->next;
	} while (sp != NULL);

	glutSwapBuffers();
}

void Reshape(int h, int v)
{
	glViewport(0, 0, h, v);
	gWidth = h;
	gHeight = v;
}

void Timer(int value)
{
	glutTimerFunc(20, Timer, 0);
	glutPostRedisplay();
}

// Example of user controllable parameter
float someValue = 0.0;

void Key(unsigned char key,
         __attribute__((unused)) int x,
         __attribute__((unused)) int y)
{
  switch (key)
  {
    case '+':
    	someValue += 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case '-':
    	someValue -= 0.1;
    	printf("someValue = %f\n", someValue);
    	break;
    case 0x1b:
      exit(0);
  }
}

void Init()
{
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat

	NewSprite(sheepFace, 100, 200, 1, 1);
	NewSprite(blackFace, 200, 100, 1.5, -1);
	NewSprite(dogFace, 250, 200, -1, 1.5);
	NewSprite(foodFace, 150, 200, 1, 1.5);
	NewSprite(sheepFace, 150, 150, -1, -1.5);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(800, 600);
	glutInitContextVersion(3, 2);
	glutCreateWindow("SpriteLight demo / Flocking");

	glutDisplayFunc(Display);
	glutTimerFunc(20, Timer, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
