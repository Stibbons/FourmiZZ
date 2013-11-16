/********************************************************************************
    Foumi-ZZ - Simulation de Fourmilière ISIMA Première Année
    Copyright (C) 2003 DANVIN Yohan, FARGE Julien, GOUBE Florent, SEMET Gaëtan
                       Xeberon.net http://www.xeberon.net

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
********************************************************************************/
#include <stdio.h>
#include "CBubbleText.h"
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "TVector.h"
#include "tga.h"


unsigned int g_bubbletext_texid=0;
unsigned int g_bubbletext_dlist=0;


void CBubbleTexture::Render() const
{
  if (m_enabled)
  {
    float modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    CVector3f rotX(modelview[0],modelview[4],modelview[8]), 
              rotY(modelview[1],modelview[5],modelview[9]);
 
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

  	glDisable(GL_DEPTH_TEST);						// Disable Depth Testing
    if (m_texmasked)
      glBlendFunc(GL_DST_COLOR, GL_ZERO);


    CVector3f coord(m_x, m_y, m_z);
    glPushMatrix();
      glTranslatef(m_x, m_y, m_z);
      glColor4f(1, 1, 1, 1);
      if (m_texmasked)
      {
        glBindTexture(GL_TEXTURE_2D, m_glmasktexid);
 		    glBegin(GL_POLYGON);

        //TODO
          coord = coord - rotX * (m_sizex/2.0f) - rotY * (m_sizey/2.0f);
			    glTexCoord2d(0,1);
          glVertex3f(coord.x, coord.y, coord.z);

          coord -= rotY * m_sizey;
			    glTexCoord2d(0,0);
          glVertex3f(coord.x, coord.y, coord.z);

          coord += rotX * m_sizex;
          glTexCoord2d(1,0);
          glVertex3f(coord.x, coord.y, coord.z);

          coord += rotY * m_sizey;
			    glTexCoord2d(1,1);
          glVertex3f(coord.x, coord.y, coord.z);

		    glEnd();
      }

      coord = CVector3f(m_x, m_y, m_z);

      glBlendFunc(GL_ONE, GL_ONE);
      glBindTexture(GL_TEXTURE_2D, m_gltexid);

		  glBegin(GL_POLYGON);
        coord = coord - rotX * (m_sizex/2.0f) - rotY * (m_sizey/2.0f);
			  glTexCoord2d(0,1);
        glVertex3f(coord.x, coord.y, coord.z);

        coord -= rotY * m_sizey;
			  glTexCoord2d(0,0);
        glVertex3f(coord.x, coord.y, coord.z);

        coord += rotX * m_sizex;
        glTexCoord2d(1,0);
        glVertex3f(coord.x, coord.y, coord.z);

        coord += rotY * m_sizey;
			  glTexCoord2d(1,1);
        glVertex3f(coord.x, coord.y, coord.z);
    
		  glEnd();

    glPopMatrix();

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
  }

}

