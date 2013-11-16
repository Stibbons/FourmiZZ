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
#ifndef __AI_ENVIRONMENT_IMPL_H__
#define __AI_ENVIRONMENT_IMPL_H__

#include "AI_world.h"
#include "AI_objects.h"
#include "../Objects.h"
#include "AI_myrmecology.h"

/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_environment.h avec cette implémentation.
*/

/***************\
* classe aiROCK *
\***************/

inline aiRock::aiRock(aiWorld& w, const vect2f& c, const float r)
: aiObject(w,c), radius_(r)
{
  // g_obj_ = new aiGraphicObject(*gRock, w.map_, c,?) ;
}

inline aiID_t aiRock::get_id() const
{
  return ROCK ;
}

inline const float& aiRock::get_radius() const
{
  return radius_ ;
}

inline aiRock::~aiRock()
{}


/********************\
* classe aiVEGETABLE *
\********************/

inline aiVegetable::aiVegetable(aiWorld& w, const vect2f& c, const uint nrg, const float h)
: aiLivingBeing(w,c,nrg), height_(h)
{
}

inline const float& aiVegetable::get_height() const
{
  return height_ ;
}

inline colorRGBA aiVegetable::color_on_minimap() const
{
  return colorRGBA(MINIMAP_COLOR_SCOUT) ;
}

inline aiVegetable::~aiVegetable()
{}

/***************\
* classe aiWEED *
\***************/

inline aiWeed::aiWeed(aiWorld& w, const vect2f& c, const float h)
: aiVegetable(w,c,WEED_INITIAL_HP,h)
{
  g_obj_ = new aiGraphicObject(UINT_RAND(2)==0?*g_ObjsWeed1:*g_ObjsWeed2, w.map_, c,UINT_RAND(360)) ;
}

inline aiID_t aiWeed::get_id() const
{
  return WEED ;
}

/*******************\
* classe aiMUSHROOM *
\*******************/

inline aiMushroom::aiMushroom(aiWorld& w, const vect2f& c, const float h)
: aiVegetable(w,c,MUSHROOM_INITIAL_HP,h)
{
  g_obj_ = new aiGraphicObject(*g_ObjsMushroom, w.map_, c, UINT_RAND(360)) ;
}

inline aiID_t aiMushroom::get_id() const
{
  return MUSHROOM ;
}

/********************\
* classe aiREDFLOWER *
\********************/

inline aiRedFlower::aiRedFlower(aiWorld& w, const vect2f& c, const float h)
: aiVegetable(w,c,REDFLOWER_INITIAL_HP,h)
{
  g_obj_ = new aiGraphicObject(*g_ObjsRedFlower, w.map_, c,UINT_RAND(360)) ;
}

inline aiID_t aiRedFlower::get_id() const
{
  return REDFLOWER ;
}



/***************\
* classe aiFOOD *
\***************/

inline aiFood::aiFood(aiWorld& w, const vect2f& c,
                      const uint va, const uint we)
: aiObject(w,c), value_(va), weight_(we)
{}

inline uint aiFood::get_value() const
{
  return value_ ;
}

inline uint aiFood::get_weight() const
{
  return weight_ ;
}

inline aiFood::~aiFood()
{}

/***************\
* classe aiSEED *
\***************/

inline aiSeed::aiSeed(aiWorld& w, const vect2f& c)
: aiFood(w,c,SEED_FOOD_VALUE,SEED_WEIGHT)
{
  g_obj_ = new aiGraphicObject(*g_ObjsSeed, w.map_, c, UINT_RAND(360)) ;
}

inline aiID_t aiSeed::get_id() const
{
  return SEED ;
}

inline colorRGBA aiSeed::color_on_minimap() const
{
  return colorRGBA(MINIMAP_COLOR_SEED) ;
}


inline aiSeed::~aiSeed()
{}

#endif /* __AI_ENVIRONMENT_IMPL_H__ */
