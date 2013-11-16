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
/************************************************************************\
 * Particle System                                                     *
/************************************************************************/

#include <stdio.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "Particles.h"


/************************************************************************\
 * Class abstraite CParticleSystem                                      * 
 * Contient les membres et fonctions communes aux systemes de           *
 * particules                                                           *
\************************************************************************/

/* destruction de toutes les particules sans se poser de questions      */
CParticleSystem::~CParticleSystem()
{
  for (unsigned int i=0; i<m_number;++i)
    delete m_particles[i];
  delete[] m_particles;
}


void CParticleSystem::RandomizeStartParticle(CParticle &part)
{
  part.m_pos.x   += (float) rand() / (float) RAND_MAX * m_randomplan.x - m_randomplan.x / 2.0f;
  part.m_pos.y   += (float) rand() / (float) RAND_MAX * m_randomplan.y - m_randomplan.y / 2.0f;
  part.m_pos.z   += (float) rand() / (float) RAND_MAX * m_randomplan.z - m_randomplan.z / 2.0f;
  part.m_speed.x += (float) rand() / (float) RAND_MAX * m_randomspeed.x - m_randomspeed.x / 2.0f;
  part.m_speed.y += (float) rand() / (float) RAND_MAX * m_randomspeed.y - m_randomspeed.y / 2.0f;
  part.m_speed.z += (float) rand() / (float) RAND_MAX * m_randomspeed.z - m_randomspeed.z / 2.0f;
  part.m_fade     = (float) rand() / (float) RAND_MAX  * m_initialParticle.m_fade;
}


void CParticleSystem::DeleteTextures()
{
  glDeleteTextures(1, &(m_initialParticle.m_gltexuid));
  for (unsigned int i = 0 ; i < m_number ; ++i)
  {
    m_particles[i]->m_gltexuid = 0;
  }
}


void CParticleSystem::ResetTextures(unsigned int newtexid)
{
  m_initialParticle.m_gltexuid = newtexid;
  for (unsigned int i = 0 ; i < m_number ; ++i)
  {
    m_particles[i]->m_gltexuid = newtexid;
  }
}
 

void CParticleSystem::Render()
{  
  if (m_enabled)
  {
    float modelview[16];

    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    CVector3f rotX(modelview[0],modelview[4],modelview[8]), 
              rotY   (modelview[1],modelview[5],modelview[9]);
 
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glDepthMask(GL_FALSE);
    if (m_initialParticle.m_textured)
      glEnable(GL_TEXTURE_2D);
    else
      glDisable(GL_TEXTURE_2D);

    for (unsigned int i=0; i<(unsigned int) m_initialised_number; ++i)
    {
      CParticle *part = m_particles[i];
      if (part->IsEnabled())
      {
        CVector3f coord;
        glPushMatrix();
          glTranslatef(part->m_pos.x, part->m_pos.y, part->m_pos.z);
          glColor4fv(part->m_color.RGBAVector());
          //Red(), part->m_color.Green(), part->m_color.Blue(), part->m_color.Alpha()
          if (m_initialParticle.m_textured)
                glBindTexture(GL_TEXTURE_2D, part->m_gltexuid);
			    glBegin(GL_POLYGON);
            coord =  part->m_pos - rotX * (part->m_xsize/2.0f) - rotY * (part->m_ysize/2.0f);
				    glTexCoord2d(0,1);
            glVertex3f(coord.x, coord.y, coord.z);

            coord -= rotY * part->m_ysize;
				    glTexCoord2d(0,0);
            glVertex3f(coord.x, coord.y, coord.z);

            coord += rotX * part->m_xsize;
            glTexCoord2d(1,0);
            glVertex3f(coord.x, coord.y, coord.z);

            coord += rotY * part->m_ysize;
				    glTexCoord2d(1,1);
            glVertex3f(coord.x, coord.y, coord.z);

          
			    glEnd();

        glPopMatrix();
      }
    }
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);
    if (m_initialParticle.m_textured)
      glDisable(GL_TEXTURE_2D);
  }
}






/************************************************************************\
 * Classe systeme particule "flot" (les particules revivent quand elles * 
 * meurent)                                                             *
\************************************************************************/



void CConstantParticleSystem::Update(float time)
{
  if (m_enabled)
  {
    if (time < 0.00003)
      return;
    unsigned int s, e;
    // toutes les particules n'ont pas été encore toutes libérés (rampe
    // de lancement)
    if (m_initialised_number != m_number)
    {
      s = (unsigned int) m_initialised_number;
      m_initialised_number += (float) floor((float) m_number / m_average_lifetime * time);
      if (m_initialised_number > m_number )
        m_initialised_number = (float) m_number;
      e = (unsigned int) m_initialised_number;
      for (unsigned int i=s; i<e; ++i)
      {
        m_particles[i]->m_enabled = true;
      }
    }
    else
    {
      s = 0;
      e = m_number;
    }
    for (unsigned int i=0; i<e; ++i)
    {
      m_particles[i]->Update(time);
      if (!m_particles[i]->m_enabled)
      {
        *(m_particles[i]) = m_initialParticle;
        m_particles[i]->Enable();
        RandomizeStartParticle((*m_particles[i]));
        if (m_particles[i]->m_pfCreateCallBack)
        {
          m_particles[i]->m_pfCreateCallBack(m_particles[i], time, i);
        }
      }
    }
  }
}




/************************************************************************\
 * Classe systeme particule "jet" (les particules meurent et ne revivent*
 * pas)                                                                 *
\************************************************************************/





void CCastParticleSystem::Update(float time)
{
  if (m_enabled)
  {
    if (time < 0.00003)
      return;
    float s, e;
    // toutes les particules n'ont pas été encore toutes libérés (rampe
    // de lancement)
    if (m_initialised_number > m_number + 0.002f ||
        m_initialised_number < m_number)
    {
      s = (float) m_initialised_number;
      m_initialised_number += m_debit * time;
      if (m_initialised_number > (float) m_number )
        m_initialised_number = (float) m_number;
      e = m_initialised_number;
      for (unsigned int i=(unsigned int) s; (float) i <e; ++i)
      {
        m_particles[i]->m_enabled = true;
      }
    }
    else
    {
      s = 0;
      e = (float) m_number;
    }
    for (unsigned int i=0; i<e; ++i)
    {
      m_particles[i]->Update(time);
    }
  }
}







void CParticle::init(bool enabled, float life, float fade, float red, 
                     float green, float blue, float alpha,  bool textured, 
                     unsigned int gltextureuid, float xsize, float ysize,
                     CVector3f &pos , CVector3f &speed, CVector3f &grav,
                     CREATEPARTICLECALLBACK pfCreateCallBack,
                     UPDATEPARTICLECALLBACK pfUpdateCallBack)
{
  m_enabled          = enabled;
  m_life             = life;
  m_fade             = fade;
  m_pos              = pos;
  m_speed            = speed;
  m_grav             = grav;
  m_color.SetColor(red, green, blue, alpha);
  m_textured         = textured;
  m_gltexuid         = gltextureuid;
  m_xsize            = xsize;
  m_ysize            = ysize;
  m_pfCreateCallBack = pfCreateCallBack;
  m_pfUpdateCallBack = pfUpdateCallBack;
}


void CParticle::Update(float dt)
{
  if (m_enabled)
  {
    m_speed += m_grav  * dt;
    m_pos   += m_speed * dt;
  
    m_life  -= dt*(m_fade);
    if (m_life < 0)
      m_enabled = false;
  }
  if (m_pfUpdateCallBack)
    m_pfUpdateCallBack(this, dt);
}
