#include <points.h>
#include <snake.h>
#include <tempo.h>
#include <volumes.h>
#include <texture_manager/texture.h>

#include <GL/glut.h>

#include <list>
#include <cmath>
#include <cstdlib>

#define TEX_DIR		"../data/"
#define TEX_GRASS	TEX_DIR "grass.bmp"
#define TEX_SNAKE	TEX_DIR "snake.bmp"
#define TEX_FOOD	TEX_DIR	"food.bmp"

const GLfloat R_LH  = 1.6;//1.333333;

const int INC	    = 6;	//increment
const int ST_SIZE	= 6;	//taille de depart
const int TAILLE	= 48;	//nombre de case par cote
const int PLATEAU	= 2;	//taille du jeu

const int FACT          = TAILLE / PLATEAU;
const int MAX           = FACT * PLATEAU - 1;
const float CCUBE       = 1. / FACT;
const int MAX_SIZE      = TAILLE * TAILLE - TAILLE;

Tempo loop_timer;
Tempo rotate_timer;
Tempo draw_timer;

char label[64];

void quit();
void drawString(const char *s);
void drawStringBig(const char *s);
void sw_pause_game();
void affichage();
void subaffichage();
void clavier(unsigned char touche, int x, int y);
void special(int touche, int x, int y);
void idle();
void calcTableCosSin();

//textures
GLuint tex[3];

//serpent
Snake snake(INC, ST_SIZE, TAILLE);

//vision
int angle = 180;
int newangle = 180;
int angleok = 0;
GLfloat hauteur = 2;

//jeu
bool fullscreen = false;
bool pause_game = false;
bool init = false;
int win_main;
int win_sub;
	
//table des sinus et cosinus
GLfloat Sin[360], Cos[360];

//lumieres
GLfloat L0pos[] = {-3.0, 1.0, 3.0};
GLfloat L0dif[] = { 0.7, 0.9, 0.7};
GLfloat L1pos[] = { 3.0, 1.0, 3.0};
GLfloat L1dif[] = { 0.7, 0.7, 0.9};
GLfloat Mspec[] = { 1.0, 1.0, 0.5};
GLfloat Mshiny = 500;

int main(int argc, char **argv)
{
	//initialisation des windows
	win_main = 0;
	win_sub = 1;
	
	//initialisation des nombres alleatoires
	srand(time(NULL));
	
  //precalcul de la table des sinus et cosinus
	calcTableCosSin();
	
	//initialisation de glut et creation de la fenetre principale
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	/*glutInitWindowPosition(0, 0);
	glutInitWindowSize(1680, 1050);
  win_main = glutCreateWindow("Snake 3D");*/
  win_main = glutEnterGameMode();
  glutSetCursor(GLUT_CURSOR_NONE);
  
	//initialisation d'OpenGL
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);

	//mise en place de la perspective
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, R_LH, 0.5, 16);
	glMatrixMode(GL_MODELVIEW);

  //parametrage des lumieres
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, L0dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, L0dif);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, L1dif);
	glLightfv(GL_LIGHT1, GL_SPECULAR, L1dif); 
	
  //parametrage du materiau
	glMaterialfv(GL_FRONT, GL_SPECULAR, Mspec);
	glMaterialf(GL_FRONT, GL_SHININESS, Mshiny);
	
	//textures
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glEnable(GL_TEXTURE_2D);
	
  //chargement des textures
	CTextureManager *pTexMgr = CTextureManager::GetInstance();
	pTexMgr->Initialize();
	tex[0] = pTexMgr->LoadTexture(TEX_GRASS);
	tex[1] = pTexMgr->LoadTexture(TEX_SNAKE);
	tex[2] = pTexMgr->LoadTexture(TEX_FOOD);
	
	//activation du culling
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
  glEnable(GL_CULL_FACE);
	
  //mise en place du tampon de profondeur
  glEnable(GL_DEPTH_TEST);

	//mise en place des fonctions de rappel
	glutDisplayFunc(affichage);
	glutKeyboardFunc(clavier);
	glutSpecialFunc(special);
	glutIdleFunc(idle);
	
	//initialisation du snake
	snake.reinit();
	sw_pause_game();

	//entree dans la boucle principale
	glutMainLoop();
	
	return 0;
}

void quit()
{
	//nettoyage
	CTextureManager::Kill();
	
	exit(EXIT_SUCCESS);
}

void drawString(const char *s)
{
	do glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *s);
	while(*s++);
}

void drawStringBig(const char *s)
{
	do glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *s);
	while(*s++);
}

void sw_pause_game()
{
	//basculement de la variable
	pause_game = !pause_game;
	//lights
	glEnable(GL_LIGHT0);
	if (pause_game)	glDisable(GL_LIGHT0);
	//mise a jour des affichages
	glutSetWindow(win_sub);
	glutPostRedisplay();
	glutSetWindow(win_main);
	glutPostRedisplay();
}

void affichage()
{
	//angle pour affichage
	int ang = angle;
	if (ang >= 360)	ang -= 360;
	if (ang < 0)	ang += 360;

	//init
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	//lights
	glLightfv(GL_LIGHT0, GL_POSITION, L0pos);
	glLightfv(GL_LIGHT1, GL_POSITION, L1pos);
	
	//point de vue
	gluLookAt(
		(4.5 - hauteur) * Sin[ang], (4.5 - hauteur) * Cos[ang], hauteur - 0.5,
		0., 0., 0.,
		0., 0., 1.);
		
	//plateau
	glBindTexture(GL_TEXTURE_2D, tex[0]);
	glTranslatef(-(PLATEAU>>1), -(PLATEAU>>1), 0.);
	addPlateau(PLATEAU);
	glPushMatrix();
	
	//serpent
	glBindTexture(GL_TEXTURE_2D, tex[1]);
	const list<Point>* snakePoints = snake.snakePoints();
	list<Point>::const_iterator it;
	for (it = snakePoints->begin(); it != snakePoints->end(); it++)
	{
		glPushMatrix();
		glTranslatef(it->xf(FACT), it->yf(FACT), CCUBE);
		addCube(CCUBE);
		glPopMatrix();
	}
	
	//nourriture
	const Point* foodPoint = snake.foodPoint();
	glBindTexture(GL_TEXTURE_2D, tex[2]);
	glTranslatef(foodPoint->xf(FACT), foodPoint->yf(FACT), CCUBE);
	addCube(CCUBE);
	glPopMatrix();
	
	//actualisation
	glFlush();
	glutSwapBuffers();
}

void subaffichage()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);

	sprintf(label, "Score = %d", snake.score());
	glRasterPos2f(-1, 0.5);
	drawStringBig(label);
	
	if (snake.mort())
	{
		glRasterPos2f(-1, 0);
		drawStringBig("Perdu! ('r' pour rejouer)");
	}
	else if (pause_game)
	{
		glRasterPos2f(-1, 0);
		drawStringBig("[-pause-]");
	}
	
	glFlush();
	glutSwapBuffers();
}

void clavier(unsigned char touche, int x, int y)
{
	switch (touche)
	{
		case 'q':
			if (newangle >= angle)	newangle = newangle - 90;
			angleok = 0;
			break;
		case 'd':
			if (newangle <= angle)	newangle = newangle + 90;
			angleok = 0;
			break;
		case 'z':
			hauteur += 0.25;
			if (hauteur > 4)		hauteur = 4;
			break;
		case 's':
			hauteur -= 0.25;
			if (hauteur < 2)		hauteur = 2;
			break;
		/*case 'f':
			if (!fullscreen)		glutFullScreen();
			else					glutReshapeWindow(800, 600);
			fullscreen = !fullscreen;
			break;*/
		case ' ':
			if (snake.vivant() && (snake.xMove() || snake.yMove()))
				sw_pause_game();
			break;
		case 'r':
			if (snake.mort())
			{
				snake.reinit();
				pause_game = false;
				sw_pause_game();
			}
			break;
		case 27:
			quit();
    }
}

void special(int touche, int x, int y)
{
	if (snake.mort()) return;

	if (pause_game)
		sw_pause_game();
	
	int ch = newangle / 90 + 3;
	switch (touche)
	{
		case GLUT_KEY_LEFT:		ch++;
		case GLUT_KEY_DOWN:		ch++;
		case GLUT_KEY_RIGHT:	ch++;
		case GLUT_KEY_UP:		break;
		default: return;
	}
	ch = ch % 4;
	switch (ch)
	{
		case 0:	//haut
			if (!snake.xMove())	snake.setMovement(-1, 0);	break;
		case 1:	//droite
			if (!snake.yMove())	snake.setMovement(0, 1);	break;
		case 2:	//bas
			if (!snake.xMove())	snake.setMovement(1, 0);	break;
		case 3:	//gauche
			if (!snake.yMove())	snake.setMovement(0, -1);	break;
	}
}

void idle()
{
	if (!init)
	{
		glutFullScreen();
		fullscreen = true;
		init = true;
		//creation de la fenetre secondaire
		win_sub = glutCreateSubWindow(win_main, 0, 0, 320, 100);
		glutDisplayFunc(subaffichage);
		gluPerspective(45.0, 2., 0.1, 16.0);
		glutSetWindow(win_main);
	}
	
	//deplacement du snake
	if (loop_timer.time() > 50)
	{
		loop_timer.reset();
		if (!pause_game && snake.vivant())
		{
			int score = snake.score();
			snake.move();
			//maj du hud
			if (score != snake.score())
			{
				glutSetWindow(win_sub);
				glutPostRedisplay();
				glutSetWindow(win_main);
			}
			if (snake.mort())
			{
				glutSetWindow(win_sub);
				glutPostRedisplay();
				glutSetWindow(win_main);
				glDisable(GL_LIGHT0);
			}
		}
	}
	
	//angle du plateau
	if (rotate_timer.time() > 10)
	{
		rotate_timer.reset();
		if (angleok == 0)
		{
			if (angle < newangle)		angle += 6;
			else if (angle > newangle)	angle -= 6;
			if (angle == newangle)
			{
				if (angle >= 360)		angle -= 360;
				else if (angle < 0)		angle += 360;
				newangle = angle;
				angleok = 1;
			}
		}
	}
	
	//affichage
	if ((draw_timer.time()) > 30 && loop_timer.time() < 10)
	{
    glutPostRedisplay();
    draw_timer.reset();
  }
}

void calcTableCosSin()
{
	// calcul du tableau des sinus et cosinus
  for (int i = 360; --i >= 0; )
	{
		Cos[i] = cos(((GLfloat)i) / 180.0 * M_PI);
		Sin[i] = sin(((GLfloat)i) / 180.0 * M_PI);
	}
}
