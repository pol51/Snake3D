#include <GL/glut.h>

#include <volumes.h>

void addCube(GLfloat x)
{
	glBegin(GL_QUADS);
	
	glNormal3f( 0, 0, 1);
	glTexCoord2f( 0, 0);	glVertex3f( x, 0, 0);
	glTexCoord2f( 1, 0);	glVertex3f( x, x, 0);
	glTexCoord2f( 1, 1);	glVertex3f( 0, x, 0);
	glTexCoord2f( 0, 1);	glVertex3f( 0, 0, 0);
	
	glNormal3f( 0, 1, 1);
	glTexCoord2f( 0, 0);	glVertex3f( x, x, 0);
	glTexCoord2f( 1, 0);	glVertex3f( x, x,-x);
	glTexCoord2f( 1, 1);	glVertex3f( 0, x,-x);
	glTexCoord2f( 0, 1);	glVertex3f( 0, x, 0);
	
	glNormal3f( 1, 0, 0);
	glTexCoord2f( 0, 1);	glVertex3f( x, 0, 0);
	glTexCoord2f( 0, 0);	glVertex3f( x, 0,-x);
	glTexCoord2f( 1, 0);	glVertex3f( x, x,-x);
	glTexCoord2f( 1, 1);	glVertex3f( x, x, 0);
	
	glNormal3f( 0,-1, 0);
	glTexCoord2f( 0, 0);	glVertex3f( 0, 0, 0);
	glTexCoord2f( 1, 0);	glVertex3f( 0, 0,-x);
	glTexCoord2f( 1, 1);	glVertex3f( x, 0,-x);
	glTexCoord2f( 0, 1);	glVertex3f( x, 0, 0);
	
	glNormal3f(-1, 0, 0);
	glTexCoord2f( 0, 1);	glVertex3f( 0, x, 0);
	glTexCoord2f( 0, 0);	glVertex3f( 0, x,-x);
	glTexCoord2f( 1, 0);	glVertex3f( 0, 0,-x);
	glTexCoord2f( 1, 1);	glVertex3f( 0, 0, 0);
	
	glEnd();
}

void addPlateau(GLfloat x)
{
	glBegin(GL_QUADS);
	
	GLfloat z = x / 16;
	
	glNormal3f( 0, 0, 1);
	glTexCoord2f( 0, 0);	glVertex3f( x, 0, 0);
	glTexCoord2f( x, 0);	glVertex3f( x, x, 0);
	glTexCoord2f( x, x);	glVertex3f( 0, x, 0);
	glTexCoord2f( 0, x);	glVertex3f( 0, 0, 0);
	
	glNormal3f( 0, 0, 1);
	glTexCoord2f( 0, 0);	glVertex3f( x, x, 0);
	glTexCoord2f( z, 0);	glVertex3f( x, x,-z);
	glTexCoord2f( z, x);	glVertex3f( 0, x,-z);
	glTexCoord2f( 0, x);	glVertex3f( 0, x, 0);
	
	glNormal3f( 1, 0, 0);
	glTexCoord2f( 0,-z);	glVertex3f( x, 0, 0);
	glTexCoord2f( 0, 0);	glVertex3f( x, 0,-z);
	glTexCoord2f( x, 0);	glVertex3f( x, x,-z);
	glTexCoord2f( x,-z);	glVertex3f( x, x, 0);
	
	glNormal3f( 0,-1, 0);
	glTexCoord2f( 0, 0);	glVertex3f( 0, 0, 0);
	glTexCoord2f( z, 0);	glVertex3f( 0, 0,-z);
	glTexCoord2f( z, x);	glVertex3f( x, 0,-z);
	glTexCoord2f( 0, x);	glVertex3f( x, 0, 0);
	
	glNormal3f(-1, 0, 0);
	glTexCoord2f( 0,-z);	glVertex3f( 0, x, 0);
	glTexCoord2f( 0, 0);	glVertex3f( 0, x,-z);
	glTexCoord2f( x, 0);	glVertex3f( 0, 0,-z);
	glTexCoord2f( x,-z);	glVertex3f( 0, 0, 0);
	
	glEnd();
}
