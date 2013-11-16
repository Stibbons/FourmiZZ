/********************************************************************************
    Foumi-ZZ - Simulation de Fourmili�re ISIMA Premi�re Ann�e
    Copyright (C) 2003 DANVIN Yohan, FARGE Julien, GOUBE Florent, SEMET Ga�tan
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
#ifndef __SDLCHRONO_H__HEADER__
#define __SDLCHRONO_H__HEADER__

#include <stdio.h>
#include <SDL/SDL.h>

/* classe Chronom�tre (temps r��l) */
/* utilise la librairie SDL pour r�cup�rer le temps processeur */
class CSDLChrono
{
  bool m_enabled;
  unsigned int m_starttime;
	unsigned int m_t; // temps global
	unsigned int m_dt; // diff�rence de temps avec la pr�c�nte mise � jour
public:
	CSDLChrono() :
    m_enabled (false),
	  m_t(0),
    m_dt(0)
	{	}
  ~CSDLChrono(){};

  /* retourne si le chronom�tre est actif ou non */
  inline bool const         IsEnabled() const { return m_enabled; };
  /* retourne le temps �coul� depuis le lancement du chrono */
  inline unsigned int const GetTime()   const { return m_t;       };
  /* retourne le temps �coul� depuis le dernier update */
  inline unsigned int const Getdt()     const { return m_dt;      };

  inline void Reset()
  { 
    m_enabled   = false;
    m_t         = 0;
    m_dt        = 0;
  }
  /* lancement du chronom�tre */
  inline void Start()
  { 
    m_starttime = SDL_GetTicks();
    m_enabled   = true;
    m_t         = 0;
    m_dt        = 0;
  }
  /* arret du chronometre */
  inline void Stop() {   m_enabled = false; }
  /* mise � jour du chronom�tre */
  inline void Update()
  {
    if (m_enabled)
    {
      unsigned int new_t = SDL_GetTicks();
      m_dt = (new_t - m_starttime) - m_t;
      m_t = new_t - m_starttime;
    }
  }

};







#endif /* __SDLCHRONO_H__HEADER__ */
