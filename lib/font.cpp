#include <stdio.h>
#include "font.h"
#include <string.h>

unsigned int g_glfont_texturesid=0;
unsigned int g_glfont_texturesmaskid=0;

unsigned int g_glfont_texturelist=0;
unsigned int g_glfont_texturelistmask=0;

unsigned int g_glfont_screensizex=1024;
unsigned int g_glfont_screensizey=768;

GLvoid fontBuild(unsigned int newtextureid, unsigned int newmaskid)								// Build Our Font Display List
{
	float	cx;											// Holds Our X Character Coord
	float	cy;											// Holds Our Y Character Coord
  int loop;

  g_glfont_texturesid = newtextureid;
  g_glfont_texturesmaskid = newmaskid;

  g_glfont_texturelist=glGenLists(256);								// Creating 256*2 Display Lists
  g_glfont_texturelistmask=glGenLists(256);								// Creating 256*2 Display Lists

	glBindTexture(GL_TEXTURE_2D, g_glfont_texturesid);			// Select Our Font Texture
	for (loop=0; loop<256; ++loop)						// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(g_glfont_texturelist+loop,GL_COMPILE);				// Start Building A List
			glTranslated(13,0,0);						// Move To The Right Of The Character
			glBegin(GL_QUADS);
			// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
				glVertex2i(16,16);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2i(0,16);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built

	glBindTexture(GL_TEXTURE_2D, g_glfont_texturesmaskid);			// Select Our Font Texture
	for (loop=0; loop<256; ++loop)						// Loop Through All 256 Lists
	{
		cx=float(loop%16)/16.0f;						// X Position Of Current Character
		cy=float(loop/16)/16.0f;						// Y Position Of Current Character

		glNewList(g_glfont_texturelistmask+loop,GL_COMPILE);				// Start Building A List
			glBegin(GL_QUADS);
			// Use A Quad For Each Character
				glTexCoord2f(cx,1-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2i(0,0);						// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,0);						// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1-cy);			// Texture Coord (Top Right)
				glVertex2i(16,16);						// Vertex Coord (Top Right)
				glTexCoord2f(cx,1-cy);					// Texture Coord (Top Left)
				glVertex2i(0,16);						// Vertex Coord (Top Left)
			glEnd();									// Done Building Our Quad (Character)
			glTranslated(13,0,0);						// Move To The Right Of The Character
		glEndList();									// Done Building The Display List
	}													// Loop Until All 256 Are Built
}

GLvoid fontKill(GLvoid)									// Delete The Font From Memory
{
	glDeleteLists(g_glfont_texturelist,512);
  glDeleteTextures(1, &g_glfont_texturesid);
  glDeleteTextures(1, &g_glfont_texturesmaskid);
}

GLvoid fontResize(GLint newsizex, GLint newsizey)
{
  g_glfont_screensizex = newsizex;
  g_glfont_screensizey = newsizey;
}

GLvoid fontPrint(GLint x, GLint y, char *string, int set)	// Where The Printing Happens
{
	if (set>1)
		set=1;
  glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);							// Disables Depth Testing
  glEnable(GL_BLEND);

  while (*string=='\n')
    ++string;

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glPushMatrix();										// Store The Projection Matrix
	  glLoadIdentity();									// Reset The Projection Matrix
	  glOrtho(0,g_glfont_screensizex,0, g_glfont_screensizey,-1,1);							// Set Up An Ortho Screen
	  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	  glPushMatrix();										// Store The Modelview Matrix
    //  glColor4f(1, 1, 1, 1);

      glBlendFunc(GL_DST_COLOR,GL_ZERO);
      glBindTexture(GL_TEXTURE_2D, g_glfont_texturesmaskid);			// Select Our Font Texture
	    glTranslated(x,g_glfont_screensizey-y,0);								// Position The Text (0,0 - Bottom Left)
	    glListBase(g_glfont_texturelistmask-32+(128*set));						// Choose The Font Set (0 or 1)
	    glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen

      glLoadIdentity();									// Reset The Modelview Matrix

      glBlendFunc(GL_ONE, GL_ONE);
      glBindTexture(GL_TEXTURE_2D, g_glfont_texturesid);			// Select Our Font Texture
	    glTranslated(x-13,g_glfont_screensizey-y,0);								// Position The Text (0,0 - Bottom Left)
	    glListBase(g_glfont_texturelist-32+(128*set));						// Choose The Font Set (0 or 1)
	    glCallLists(strlen(string),GL_BYTE,string);			// Write The Text To The Screen

	    glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	  glPopMatrix();										// Restore The Old Projection Matrix
	  glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glPopMatrix();										// Restore The Old Projection Matrix
  glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
}

GLvoid glPrint_chsize(GLint x, GLint y, char *string, int set, int sizex, int sizey)
{
	int a = g_glfont_screensizex;
	int b = g_glfont_screensizey;

	g_glfont_screensizex=sizex;
	g_glfont_screensizey=sizey;	

	glPrint(x,y,string,set);

	g_glfont_screensizex=a;
	g_glfont_screensizey=b;
}
