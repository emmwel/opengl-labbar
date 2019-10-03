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
GLfloat cohesion_weight = 0.001;
GLfloat avoidance_weight = 0.3;
GLfloat alignment_weight = 0.01;

float getRandom(float max_, float min_) {
	return (((float)rand()/(float)(RAND_MAX)) *  (max_ - min_)) + min_;
}

void SpriteBehavior() // Din kod!
{
	// L�gg till din labbkod h�r. Det g�r bra att �ndra var som helst i
	// koden i �vrigt, men mycket kan samlas h�r. Du kan utg� fr�n den
	// globala listroten, gSpriteRoot, f�r att kontrollera alla sprites
	// hastigheter och positioner, eller arbeta fr�n egna globaler.
	SpritePtr spi = gSpriteRoot;
	SpritePtr spj = spi->next;
	int counter = 0;
	GLfloat maxdist = 150.0;
	GLfloat mindist = 60.0;

	while(spi != NULL)
	{
		spi->avg_position.h = 0;
		spi->avg_position.v = 0;
		spi->avoidance.h = 0;
		spi->avoidance.v = 0;
		spi->speed_diff.h = 0;
		spi->speed_diff.v = 0;
		spi->food_attraction.h = 0;
		spi->food_attraction.v = 0;
		counter = 0;
		spj = gSpriteRoot;

		while(spj != NULL){
			if(spi != spj){
				// Calculate distance to all boids j from the boid i
				GLfloat dist = sqrt(
					(spi->position.h-spj->position.h) * (spi->position.h-spj->position.h) +
					(spi->position.v-spj->position.v) * (spi->position.v-spj->position.v));

				if (spj->type == 3 && dist < 50.0) {
					DeleteSpriteRoot();
					break;
				}
				if (spj->type == 3 && dist < 400) {
					spi->food_attraction.h += (spj->position.h - spi->position.h); // (dist + 1);
					spi->food_attraction.v += (spj->position.v - spi->position.v);// (dist + 1);
				}

				if(dist < maxdist && spi->type != 3 && spj->type != 3){

						// Sum the position of all boids within radius
						spi->avg_position.h += spj->position.h;
						spi->avg_position.v += spj->position.v;
						spi->speed_diff.h += spj->speed.h - spi->speed.h;
						spi->speed_diff.v += spj->speed.v - spi->speed.v;
						++counter;
						if(dist < mindist){
							// scale with distance
							spi->avoidance.h += -(spj->position.h - spi->position.h) /(dist + 1);
							spi->avoidance.v += -(spj->position.v - spi->position.v) / (dist + 1);
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
			spi->avoidance.h /= counter;
			spi->avoidance.v /= counter;
			spi->speed_diff.h /= counter;
			spi->speed_diff.v /= counter;
		}
		spi = spi->next;
	}

	// Reset sp and rotate boids
	spi = gSpriteRoot;
	while(spi != NULL){
		if(spi->avg_position.h > 0 || spi->avg_position.v > 0){
			// add speed components
			spi->speed.h += (spi->avg_position.h - spi->position.h) * cohesion_weight;
			spi->speed.v += (spi->avg_position.v - spi->position.v) * cohesion_weight;
			spi->speed.h += spi->avoidance.h * avoidance_weight;
			spi->speed.v += spi->avoidance.v * avoidance_weight;
			spi->speed.h += spi->speed_diff.h * alignment_weight;
			spi->speed.v += spi->speed_diff.v * alignment_weight;
			spi->speed.h += spi->food_attraction.h * 0.0005;
			spi->speed.v += spi->food_attraction.v * 0.0005;

			// add speed to position
			spi->position.h += spi->speed.h;
			spi->position.v += spi->speed.v;
		}
		if (spi->type == 0) {
			float r_h = getRandom(0.3, -0.3);
			float r_v = getRandom(0.3, -0.3);
			spi->speed.h += r_h;
			spi->speed.v += r_v;

			//printf("%f\n", random());
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

void TimerFood(int value)
{
	glutTimerFunc(6000, TimerFood, 0);
	printf("he\n");
	int h = getRandom(500.0, 0);
	int v = getRandom(500.0, 0);
	NewSprite(GetFace("bilder/mat.tga"), h, v, 0, 0, 3);
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
	// Type 0, 1, 2, 3
	TextureData *sheepFace, *blackFace, *dogFace, *foodFace;

	LoadTGATextureSimple("bilder/leaves.tga", &backgroundTexID); // Bakgrund

	sheepFace = GetFace("bilder/sheep.tga"); // Ett f�r
	blackFace = GetFace("bilder/blackie.tga"); // Ett svart f�r
	dogFace = GetFace("bilder/dog.tga"); // En hund
	foodFace = GetFace("bilder/mat.tga"); // Mat

	NewSprite(sheepFace, 100, 200, 1, 1, 0);
	NewSprite(sheepFace, 200, 100, 1.5, -1, 0);
	NewSprite(sheepFace, 250, 200, -1, 1.5, 0);
	NewSprite(sheepFace, 150, 200, 1, 1.5, 0);
	NewSprite(blackFace, 150, 150, -1, -1.5, 1);
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
	glutTimerFunc(20, TimerFood, 0); // Should match the screen synch
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Key);

	InitSpriteLight();
	Init();

	glutMainLoop();
	return 0;
}
