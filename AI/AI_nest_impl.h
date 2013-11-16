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
#ifndef __AI_NEST_IMPL_H__
#define __AI_NEST_IMPL_H__

#include "AI_environment.h"

/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_nest.h avec cette implémentation.
*/


// aiNestBaby

inline aiNestBaby::aiNestBaby(const aiID_t t)
: morphology_(t), age_(0)
{}

// aiNestEntrance

inline const vect2f& aiNestEntrance::get_coords2d() const
{
  return coords2d_ ;
}

// aiEntrance

inline const vect2f& aiNest::get_coords2d() const
{
  return coords2d_ ;
}

inline float aiNest::get_width() const
{
  return width_ ;
}

inline uint aiNest::get_food_stock() const
{
  return food_stock_ ;
}

inline uint aiNest::get_number_of_entrances() const
{
  return entrances_.get_size() ;
}

inline aiNestEntrance& aiNest::current_entrance() const
{
  return *entrances_.current() ;
}

inline void aiNest::add_entrance(aiNestEntrance& e)
{
  entrances_.add(e) ;
}

inline void aiNest::next_entrance()
{
  entrances_.next() ;
}

/////////////

inline uint aiNest::get_number_of_babies() const
{
  return babies_.get_size() ;
}

inline aiNestBaby& aiNest::current_baby()
{
  return *babies_.current() ;
}

inline void aiNest::next_baby()
{
  babies_.next() ;
}

inline void aiNest::delete_current_baby()
{
  babies_.delete_current() ;
}

/////////////

inline void aiNest::add_to_food_stock(aiFood* f)
{
  food_stock_ += f->get_value() ;
  f->vanish() ;
}


inline aiNestEntrance& aiNest::random_entrance()
{
  uint n = UINT_RAND(get_number_of_entrances()) ;
  for (uint i(0) ; i<n ; ++i, next_entrance()) ;
  return current_entrance() ;
}

inline aiNest::~aiNest()
{
  while (babies_.get_size()) babies_.delete_current() ;
  while (entrances_.get_size()) entrances_.delete_current() ;
}


#endif /* __AI_NEST_IMPL_H__ */
