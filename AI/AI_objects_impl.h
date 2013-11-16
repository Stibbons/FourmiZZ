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
#ifndef __AI_OBJECTS_IMPL_H__
#define __AI_OBJECTS_IMPL_H__

#include "AI_technical.h"
#include "AI_world.h"

/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_objects.h avec cette implémentation.
*/

/*****************\
* classe aiOBJECT *
\*****************/


inline aiObject::aiObject(aiWorld& wo, const vect2f& co)
: world_(wo), sector_(&wo.sector_at(co)), g_obj_(0), action_(IDLE), action_timer_(0),
vanished_(false), active_(true)
{
  sector_->get(*this) ;
}

inline vect2f& aiObject::coords2d() const
{
  return g_obj_->coords3d_ ;
}

inline const vect2f& aiObject::get_coords2d() const
{
  return g_obj_->coords2d() ;
}

inline void aiObject::now_do(const action_type act, const uint tim)
{
  g_obj_->update_animation(action_,act) ;
  action_ = act ;
  action_timer_ = tim ;
}

inline bool aiObject::finished() const
{
  return !action_timer_ ;
}

inline bool aiObject::vanished() const
{
  return vanished_ ;
}

inline void aiObject::vanish()
{
  vanished_ = true ;
  sector_->lose(*this) ;
  if (world_.g_world_->highlighted_object_ == this)
    world_.g_world_->highlighted_object_ = 0 ;
}

inline bool aiObject::is_active() const
{
  return active_ && !vanished_ ;
}

inline void aiObject::ai_update()
{
  if (action_timer_)
    --action_timer_ ;
}

inline void aiObject::graphic_update()
{}

inline void aiObject::render()
{
  g_obj_->render() ;
}

inline aiObject::~aiObject()
{
  //sector_->lose(*this) ;
  delete g_obj_ ;
}



/**********************\
* classe aiLIVINGBEING *
\**********************/

inline aiLivingBeing::aiLivingBeing(aiWorld& wo, const vect2f& co, const uint nrg)
:aiObject(wo,co), hp_(nrg), counter_(IA_FREQUENCY,true)
{}

inline uint aiLivingBeing::get_hp() const
{
  return hp_ ;
}

inline bool aiLivingBeing::is_alive() const
{
  return (hp_!=0) ;
}

inline bool aiLivingBeing::is_dead() const
{
  return (hp_==0) ;
}

inline void aiLivingBeing::die()
{
  hp_ = 0 ;
  vanish() ;
}

inline void aiLivingBeing::live()
{}

inline void aiLivingBeing::ai_update()
{
  aiObject::ai_update() ;
  if (is_alive() && counter_()) live() ;
}

#endif /* __AI_OBJECTS_IMPL_H__ */
