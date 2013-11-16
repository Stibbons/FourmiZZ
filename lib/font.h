#ifndef __FONT_H__
#define __FONT_H__
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

GLvoid fontBuild(unsigned int newtextureid, unsigned int newmaskid);
GLvoid fontKill(GLvoid);
GLvoid fontPrint(GLint x, GLint y, char *string, int set);
#define glPrint(a,b,c,d) fontPrint(a,b,c,d)
GLvoid fontResize(GLint newsizex, GLint newsizey);
GLvoid glPrint_chsize(GLint x, GLint y, char *string, int set, int sizex, int sizey);
#endif
