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
#include "AI_nest.h"
#include "AI_environment.h"
#include "AI_ants.h"
#include "AI_myrmecology.h"
#include "../sounds.h"

aiNest::aiNest(aiWorld& wo, const vect2f& co, const float w)
: world_(wo), coords2d_(co), width_(w), food_stock_(0)
{}

aiNestEntrance::aiNestEntrance(const vect2f& v)
: coords2d_(v)
{}

void aiNest::ai_update()
{
  for (uint i(0) ; i < babies_.get_size() ; ++i) {
    if (++babies_.current()->age_ > DEVELOPMENT_DURATION) {
      aiNestEntrance& ent = random_entrance() ;
      switch (babies_.current()->morphology_) {
        case HARVESTER:
          world_.add_object(*new aiHarvester(world_,ent.get_coords2d(),UINT_RAND(HARVESTER_INITIAL_HP))) ;
          break ;
        case SCOUT:
          world_.add_object(*new aiScout(world_,ent.get_coords2d(),UINT_RAND(SCOUT_INITIAL_HP))) ;
          break ;
        //case MAJOR:
        //  world_.add_object(*new aiMajor(*world_,ent.get_coords2d(),MAJOR_INITIAL_HP)) ;
        //  break ;
      }
      delete_current_baby() ;
    } else next_baby() ;
  }
  if (needs_a_scout()) {
    if (food_stock_ >= FOOD_STOCK_NEEDED_TO_GROW_A_SCOUT) {
      babies_.add(*new aiNestBaby(SCOUT)) ;
      food_stock_ -= FOOD_STOCK_NEEDED_TO_GROW_A_SCOUT ;
      GetSoundManager().PlaySound(NEWANT_SOUND_ID, false);
    }
  } else if (needs_a_harvester()) {
    if (food_stock_ >= FOOD_STOCK_NEEDED_TO_GROW_A_HARVESTER) {
      babies_.add(*new aiNestBaby(HARVESTER)) ;
      food_stock_ -= FOOD_STOCK_NEEDED_TO_GROW_A_HARVESTER ;
      GetSoundManager().PlaySound(NEWANT_SOUND_ID, false);
    }
  }
}

#define PROBABILITY_NEEDS_A_HARVESTER     (0.5f)
#define PROBABILITY_NEEDS_A_SCOUT         (0.05f)

bool aiNest::needs_a_harvester() const
{
  return ROLL(PROBABILITY_NEEDS_A_HARVESTER) ;
}

bool aiNest::needs_a_scout() const
{
  return ROLL(PROBABILITY_NEEDS_A_SCOUT) ;
}


aiNestEntrance::~aiNestEntrance()
{}

