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
#ifndef __PARTICLE_SYSTEM_H__HEADER__
#define __PARTICLE_SYSTEM_H__HEADER__

typedef unsigned long (*PARTICLE_GETTICKCOUNT_PROC) ( void );

// remarque : tous les temps sont exprimés en SECONDES.

#include "TVector.h"
#include "CColor.h"
#include "stdlib.h"

class CParticleSystem;
class CConstantParticleSystem;
class CCastParticleSystem;

class CParticle;
// fonction callback à la mise à jour d'une particule
// 
typedef void (*UPDATEPARTICLECALLBACK) (CParticle *, float);
typedef void (*CREATEPARTICLECALLBACK) (CParticle *, float, unsigned int);




class CParticle
{
protected:
  bool      m_enabled; // particule active ou non
  float     m_life;    // temps de vie restante ( < 0 = mort)
  float     m_fade;    // vitesse de "disparition"

  float     m_xsize;   // taille 
  float     m_ysize;

  CVector3f m_pos;     // position sur la scene
  CVector3f m_speed;   // vecteur vitesse
  CVector3f m_grav;    // force à laquelle la particule est soumise

  CColor4f    m_color;
  bool            m_textured;
  unsigned int    m_gltexuid;
  UPDATEPARTICLECALLBACK m_pfUpdateCallBack;
  CREATEPARTICLECALLBACK m_pfCreateCallBack;

  friend class CParticleSystem;
  friend class CConstantParticleSystem;
  friend class CCastParticleSystem;

public:
  CParticle() : 
      m_enabled(0), 
      m_life(0),
      m_fade(0), 
      m_pos(0,0,0), 
      m_speed(0,0,0),
      m_grav(0,0,0),
      m_color (0,0,0,0), 
      m_textured(false),
      m_gltexuid(0),
      m_pfUpdateCallBack(NULL), 
      m_pfCreateCallBack(NULL)
  { }
/* le constructeur par copie généré automatiquement + rapide que moi

  CParticle(CParticle &part)
  {
    m_enabled  = part.m_enabled;
    m_life     = part.m_life;
    m_fade     = part.m_fade;
    m_xsize    = part.m_xsize;
    m_ysize    = part.m_ysize;
    m_pos      = part.m_pos;
    m_speed    = part.m_speed;
    m_grav     = part.m_grav;
    m_color    = part.m_color;
    m_textured = part.m_textured;
    m_gltexuid = part.m_gltexuid;
    m_pfUpdateCallBack = part.m_pfUpdateCallBack;
    m_pfCreateCallBack = part.m_pfCreateCallBack;
  }
*/
  void init(bool enabled, float life, float fade, float red, float green,
            float blue, float alpha, bool textured, unsigned int gltextureuid,
            float xsize, float ysize,
            CVector3f &pos , CVector3f &speed, CVector3f &grav,
            CREATEPARTICLECALLBACK m_pfCreateCallBack,
            UPDATEPARTICLECALLBACK pfUpdateCallBack);

  inline const bool  IsEnabled()  const  {  return m_enabled;  }
  inline const bool  IsTextured() const  {  return m_textured; }
  inline const float GetLife()    const  {  return m_life;     }
  inline const float GetFade()    const  {  return m_fade;     }
  
  inline CColor4f GetColor() const { return m_color; }

  inline void  SetSize (float xsize, float ysize)
  {
    m_xsize = xsize;
    m_ysize = ysize;
  }

  inline const CVector3f& GetPosition( )
  {
    return m_pos;
  }
  inline const CVector3f& GetSpeed( )
  {
    return m_speed;
  }
  inline const CVector3f& GetGravity( )
  {
    return m_grav;
  }

  inline void  SetGravity (CVector3f &v)  {  m_grav  = v; }
  inline void  SetSpeed   (CVector3f &v)  {  m_speed = v; }
  inline void  SetPosition(CVector3f &v)  {  m_pos   = v; }

  inline void  SetAlpha   (float alpha)   {  m_color.SetAlpha(alpha); }

  inline void  SetColor   (float red, float green, float blue)
  {
    m_color.SetColor(red,green,blue);
  }
  inline void  SetColor   (float red, float green, float blue, float alpha)
  {
    m_color.SetColor(red,green,blue, alpha);
  }
  inline void  SetColor   (CColor4f &col )
  {
    m_color = col;
  }
  inline void SetGlTextureUID(unsigned int gltextureuid)
  {
    m_gltexuid = gltextureuid;
    m_textured = true;
  }
  inline void SetTextured(bool textured)
  {
    m_textured = textured;
  }
  inline void Enable()  { m_enabled = true;  }
  inline void Disable() { m_enabled = false; }
  inline void Die()     { m_life = -1;       }

  void Update(float dt);   // temps en secondes depuis le précédent
                           // appel à Update (0 = premier appel)
};


//TODO: 2 systeme de particules :un constant (donc les particules revivent)
// un non constant (donc gestion d'un flux)

class CParticleSystem
{
protected:
  CVector3f       m_randomplan;
  CVector3f       m_randomspeed;
  CParticle       m_initialParticle;
  unsigned int    m_number;
  float           m_average_lifetime;
  float           m_initialised_number; // oui oui c'est un float
  CParticle     **m_particles;
  friend class    CParticle;
  bool            m_enabled;

public:
  CParticleSystem(): 
    m_randomplan(0,0,0),
    m_randomspeed(0,0,0),
    m_number(0),
    m_initialised_number(0),
    m_particles(NULL),
    m_enabled(false)
  {
  }
  virtual ~CParticleSystem();
  inline unsigned int GetParticleNumber() const {  return m_number;  }
  inline CParticle& GetParticle(unsigned int i) const
  {
    return *(m_particles[i]);
  }
  inline virtual void Init(unsigned int number,bool enabled, float life, float fade, float red, 
                     float green, float blue, float alpha,  bool textured, 
                     unsigned int gltextureuid, float xsize, float ysize,
                     CVector3f &pos , CVector3f &speed, CVector3f &grav,
                     CVector3f &randomplan, CVector3f randomspeed,
                     CREATEPARTICLECALLBACK pfCreateCallBack,
                     UPDATEPARTICLECALLBACK pfUpdateCallBack);
  void RandomizeStartParticle(CParticle &part);
  void Render(); // dessine toutes les particules
  virtual void Update(float dt) = 0;
  void ResetTextures(unsigned int newtexid);
  void Enable()  { m_enabled = true;  }
  void Disable() { m_enabled = false;  }
  void Reset();
  bool const IsEnabled() const { return m_enabled;  }
  void DeleteTextures();
};

inline void CParticleSystem::Reset()
{ 
  m_initialised_number=0;
  for (unsigned int i=0; i< m_number; ++i)
    m_particles[i]->Disable();
}



class CConstantParticleSystem : public CParticleSystem
{
public:
  CConstantParticleSystem()
  {
    //CParticleSystem::CParticleSystem();
    m_number=0;
  }

  virtual ~CConstantParticleSystem();

  void Init(unsigned int number,  float life, float fade,
            float red, float green, float blue, float alpha, 
            bool textured, unsigned int gltextureuid,
            float xsize, float ysize,
            CVector3f &pos, CVector3f &speed, CVector3f &grav,
            CVector3f &randomplan, CVector3f randomspeed,
            CREATEPARTICLECALLBACK pfCreateCallBack,
            UPDATEPARTICLECALLBACK pfUpdateCallBack);

  void Update(float dt);

};

inline CConstantParticleSystem::~CConstantParticleSystem()
{
}


class CCastParticleSystem : public CParticleSystem
{
  float           m_debit;

public:
  CCastParticleSystem() : m_debit(0)
  {
    m_number=0;
//    CParticleSystem();
  }
  virtual ~CCastParticleSystem();

  void Init(unsigned int number, float  debit, float life, float fade,
            float red, float green, float blue, float alpha, 
            bool textured, unsigned int gltextureuid,
            float xsize, float ysize,
            CVector3f &pos, CVector3f &speed, CVector3f &grav,
            CVector3f &randomplan, CVector3f randomspeed,
            CREATEPARTICLECALLBACK pfCreateCallBack,
            UPDATEPARTICLECALLBACK pfUpdateCallBack);

  void Update(float dt);
};


inline CCastParticleSystem::~CCastParticleSystem()
{
}




inline void CParticleSystem::Init(unsigned int number, bool enabled, float life, float fade, float red, 
                     float green, float blue, float alpha,  bool textured, 
                     unsigned int gltextureuid, float xsize, float ysize,
                     CVector3f &pos , CVector3f &speed, CVector3f &grav,
                     CVector3f &randomplan, CVector3f randomspeed,
                     CREATEPARTICLECALLBACK pfCreateCallBack,
                     UPDATEPARTICLECALLBACK pfUpdateCallBack)
{
  m_average_lifetime = life;
  m_randomplan  = randomplan;
  m_randomspeed = randomspeed;
  m_number = number;
  m_initialParticle.init(false, life, fade, red, green, blue, alpha, textured,
                          gltextureuid, xsize, ysize, pos, speed, grav,
                          pfCreateCallBack, pfUpdateCallBack);
  m_particles = new CParticle*[m_number];
  m_initialised_number = 0;
  for (unsigned int i=0; i<m_number; ++i)
  {
    m_particles[i] = new CParticle;
    *(m_particles[i]) = m_initialParticle;
    RandomizeStartParticle((*m_particles[i]));
    m_particles[i]->m_life = (float) rand() / (float) RAND_MAX  * (m_initialParticle.m_life/2.0f);

    if (pfCreateCallBack)
    {
      pfCreateCallBack(m_particles[i], 0.0f, i);
    }
  }
}


inline void CConstantParticleSystem::Init(unsigned int number, 
                           float life, float fade, 
                           float red, float green, float blue, float alpha, 
                           bool textured, unsigned int gltextureuid,
                           float xsize, float ysize,
                           CVector3f &pos , CVector3f &speed, CVector3f &grav, 
                           CVector3f &randomplan, CVector3f randomspeed,
                           CREATEPARTICLECALLBACK pfCreateCallBack,
                           UPDATEPARTICLECALLBACK pfUpdateCallBack)
{
  CParticleSystem::Init(number, false, life, fade, red, green, blue, alpha, textured, gltextureuid,
    xsize, ysize, pos, speed, grav, randomplan, randomspeed, pfCreateCallBack, pfUpdateCallBack);
}

inline void CCastParticleSystem::Init(unsigned int number, float debit, 
                           float life, float fade, 
                           float red, float green, float blue, float alpha, 
                           bool textured, unsigned int gltextureuid,
                           float xsize, float ysize,
                           CVector3f &pos , CVector3f &speed, CVector3f &grav, 
                           CVector3f &randomplan, CVector3f randomspeed,
                           CREATEPARTICLECALLBACK pfCreateCallBack,
                           UPDATEPARTICLECALLBACK pfUpdateCallBack)
{
  m_debit = debit;
  CParticleSystem::Init(number, false, life, fade, red, green, blue, alpha, textured, gltextureuid,
    xsize, ysize, pos, speed, grav, randomplan, randomspeed, pfCreateCallBack, pfUpdateCallBack);

}


#endif // __PARTICLE_SYSTEM_H__HEADER__

