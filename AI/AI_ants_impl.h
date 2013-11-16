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
#ifndef __AI_ANTS_IMPL_H__
#define __AI_ANTS_IMPL_H__

#include "../Objects.h"


/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_ants.h avec cette implémentation.
*/

/**************\
* classe aiANT *
\**************/

inline aiAnt::aiAnt(aiWorld& w, const vect2f& c,
                    uint nrg)
: aiLivingBeing(w,c,nrg), hunger_(0)
{}

inline aiNest& aiAnt::nest() const
{
  return *world_.nest_ ;
}

inline void aiAnt::live()
{
  aiLivingBeing::live();
}

inline aiAnt::~aiAnt()
{}

/****************\
* classe aiMINOR *
\****************/

inline uint aiMinor::get_carriable_weight() const
{
   return MINOR_BASE_CARRIABLE_WEIGHT ;
}

inline const vect2f& aiMinor::get_destination() const
{
  return destination_ ;
}

inline vect2f aiMinor::hypothethic_nest_direction() const
{
  float x = nest().get_coords2d().z-get_coords2d().x ;
  if (2*x>world_.get_size()) x += world_.get_size() ;

  float z = nest().get_coords2d().z-get_coords2d().z ;
  if (2*z>world_.get_size()) z += world_.get_size() ;

  // parfaite intuition
  vect2f v(x,z) ;
  v.Normalize() ;
  return v ;
}

inline float aiMinor::get_speed() const
{
  return MINOR_BASE_SPEED ;
}

inline void aiMinor::secret_token(const token_t t, const uint a) const
{
  world_.gain_token(*sector_,t,a,get_coords2d()) ;
}

inline bool aiMinor::is_hungry() const
{
  return (hunger_ > MINOR_HUNGRY) ;
}


inline bool aiMinor::is_food(const aiObject& ob) const
{
  uint t(ob.get_id()) ;
  return (t==SEED) ;
}

/********************\
* classe aiHARVESTER *
\********************/

inline aiID_t aiHarvester::get_id() const
{
  return HARVESTER ;
}

inline colorRGBA aiHarvester::color_on_minimap() const
{
  return colorRGBA(MINIMAP_COLOR_HARVESTER) ;
}

inline aiHarvester::~aiHarvester()
{}


/****************\
* classe aiSCOUT *
\****************/

inline aiID_t aiScout::get_id() const
{
  return SCOUT ;
}

inline colorRGBA aiScout::color_on_minimap() const
{
  return colorRGBA(MINIMAP_COLOR_SCOUT) ;
}

inline aiScout::~aiScout()
{}


#endif /* __AI_ANTS_IMPL_H__ */
