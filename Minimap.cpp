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
#include "Minimap.h"
#include "CMap.h"



void minimap::Render()
{
  // Clear The Screen And The Depth Buffer
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
  
    glPushMatrix();
      glLoadIdentity();
  
      glOrtho(0,1024,768,0,0,1);
  
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();

  
     
      glColor4f(0.82f,0.52f,0.376f,1.0f);
      glBindTexture(GL_TEXTURE_2D, tex); 
		  glBegin(GL_QUADS);
        glTexCoord2f(0,1);
        glVertex2i(ulx, uly);
        glTexCoord2f(0,0);
        glVertex2i(ulx, lry);
        glTexCoord2f(1,0);
        glVertex2i(lrx, lry);
        glTexCoord2f(1,1);
        glVertex2i(lrx, uly);
    
      glEnd(); 
      glDisable(GL_TEXTURE_2D);
		  glColor4f(1.0f,1.0f,0.4f,1.0f);
      int a  = (int)((float)ulx + (float)map_.ScrollLimitZf()/ratio);
      int b = (int)((float)lry -  (float)map_.ScrollLimitXf()/ratio);
      glBegin(GL_LINES);
        /*
        glVertex2f(a,b);
        if (a +40 > lrx)
        {
          glVertex2f(static_cast<float>(lrx),b);
          glVertex2f(static_cast<float>(ulx),b);
          glVertex2f(static_cast<float>(ulx)-a +40,b);
        }
        else
          glVertex2f(a + 40,b);
    
        glVertex2f(a,b);
    
        if (b-40 < uly)
        {
          glVertex2f(a,static_cast<float>(uly));
          glVertex2f(a,static_cast<float>(lry));
          glVertex2f(a,b-static_cast<float>(lry) -40);
        }
        else
          glVertex2f(a,b - 40);
    
        if (a +40 > lrx)
        {
          glVertex2f(a-static_cast<float>(ulx) +40,b);
        }
        else
          glVertex2f(a + 40,b);
    
        if ((a +40 > lrx) && (b-40 < uly))
        {
          glVertex2f(a-static_cast<float>(ulx)+40,static_cast<float>(uly));
          glVertex2f(a-static_cast<float>(ulx)+40,static_cast<float>(lry));
          glVertex2f(a-static_cast<float>(ulx)+40,b-static_cast<float>(lry) -40);
        }
        else if (b-40 < uly)
        {
          glVertex2f(a+40,static_cast<float>(uly));
          glVertex2f(a+40,static_cast<float>(lry));
          glVertex2f(a+40,b-static_cast<float>(lry) -40);
        }
        else if ( a+40 >lrx)
          glVertex2f(a-static_cast<float>(ulx) +40,b-40);
        else
          glVertex2f(a + 40,b - 40);
    
        //	glVertex2f(ulx + (float)y/ratio,lry -  (float)x/ratio -40);
        //	glVertex2f(ulx + (float)y/ratio + 40,lry -  (float)x/ratio -40);
        */
#define SIZE_BOX 70
        glVertex2f(a,b);  
        if (a +SIZE_BOX > lrx)  
        {  
          glVertex2i((lrx),b);  
          glVertex2i((ulx),b);  
          glVertex2i((ulx)+(a+SIZE_BOX-lrx),b);  
        }  
        else  
          glVertex2i(a + SIZE_BOX,b);  
      



        //
        glVertex2f(a,b);  
      
        if (b-SIZE_BOX< uly)  
        {  
          glVertex2i(a,(uly));  
          glVertex2i(a,(lry));  
          glVertex2i(a,(lry)-(uly-b+SIZE_BOX));  
        } 
        else  
          glVertex2i(a,b - SIZE_BOX);  
     

  //
      if (a +SIZE_BOX> lrx)  
      {  
           glVertex2i((ulx)+(a+SIZE_BOX-lrx),b);  
      }  
      else  
        glVertex2i(a + SIZE_BOX,b);  
        
      if ((a +SIZE_BOX> lrx) && (b-SIZE_BOX< uly))  
      {  
        glVertex2i((ulx)+(a+SIZE_BOX-lrx),(uly));  
        glVertex2i((ulx)+(a+SIZE_BOX-lrx),(lry));  
        glVertex2i((ulx)+(a+SIZE_BOX-lrx),(lry)-(uly-b+SIZE_BOX));  
      }  
      else if (b-SIZE_BOX< uly)  
      {  
        glVertex2i(a+SIZE_BOX,(uly));  
        glVertex2i(a+SIZE_BOX,(lry));  
        glVertex2i(a+SIZE_BOX,(lry)-(uly-b+SIZE_BOX));  
      }  
      else if ( a+SIZE_BOX>lrx)  
        glVertex2i((ulx)+(a+SIZE_BOX-lrx),b-SIZE_BOX);  
      else  
        glVertex2i(a + SIZE_BOX,b - SIZE_BOX);  
  
      if (b -SIZE_BOX< uly)  
      {  
           glVertex2i(a,(lry)-(uly-b+SIZE_BOX));  
      }  
      else  
        glVertex2i(a,b-SIZE_BOX);  
        
      if ((a +SIZE_BOX> lrx) && (b-SIZE_BOX< uly))  
      {  
        glVertex2i((lrx),(lry)-(uly-b+SIZE_BOX));  
        glVertex2i((ulx),(lry)-(uly-b+SIZE_BOX));  
        glVertex2i((ulx)+(a+SIZE_BOX-lrx),(lry)-(uly-b+SIZE_BOX));  
      }  
      else if (b-SIZE_BOX< uly)  
      {  
        glVertex2i(a+SIZE_BOX,(lry)-(uly-b+SIZE_BOX));  
      }  
      else if ( a+SIZE_BOX>lrx)  
      {  
         glVertex2i((lrx),b-SIZE_BOX);  
        glVertex2i((ulx),b-SIZE_BOX);  
        glVertex2i((ulx)+(a+SIZE_BOX-lrx),b-SIZE_BOX);  
      }  
      else  
        glVertex2i(a + SIZE_BOX,b - SIZE_BOX);  
        



    //


      glEnd();
      glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  glEnable(GL_DEPTH_TEST);
}

minimap::~minimap()
{
  glDeleteTextures(1,&tex);
}


minimap::minimap(CMap* ma, float rat,int ulx,int uly, int lrx, int lry)
: map_(*ma), ratio(rat), ulx(ulx), uly(uly), lrx(lrx), lry(lry)
{
 // glGenTextures(1, &(tex));
 // tgaLoad("textures"PATHDELIM_S"earth1.tga",tex);
  //ma->CreateMinimap();
}

