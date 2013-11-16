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
#ifndef __AI_WORLD_IMPL_H__
#define __AI_WORLD_IMPL_H__

#include <string.h>
#include <fstream>
#include "AI_shared.h"
#include "AI_technical.h"
#include "AI_myrmecology.h"
#include "AI_ring.h"
#include "AI_objects.h"
#include "../CMap.h"
#include "../Objects.h"

/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_world.h avec cette implémentation.
*/

/*****************\
* classe aiSECTOR *
\*****************/

inline aiObject& aiSector::current_stuff() const { return *stuff_.current() ; }

inline uint aiSector::get_number_of_stuff() const { return stuff_.get_size() ; }

inline void aiSector::next_stuff()
{
  stuff_.next() ;
}

inline void aiSector::remove_stuff() {
  stuff_.remove_current() ;
}

/****************\
* classe aiWORLD *
\****************/

inline bool aiWorld::visible_in_scrolling_view(const vect2f& v) const
{
  vect2f ll(map_->ScrollLimitf()) ;
  vect2f ur(ll+map_->FrameSize()) ;
  vect2f v2(v) ;

  if ((ll.x<=v2.x && v2.x<=ur.x) &&(ll.z<=v2.z && v2.z<=ur.z)) return true ;
  v2.x += map_->Get_Map_Size() ;
  if ((ll.x<=v2.x && v2.x<=ur.x) &&(ll.z<=v2.z && v2.z<=ur.z)) return true ;
  v2.z += map_->Get_Map_Size() ;
  if ((ll.x<=v2.x && v2.x<=ur.x) &&(ll.z<=v2.z && v2.z<=ur.z)) return true ;
  v2.x -= map_->Get_Map_Size() ;
  if ((ll.x<=v2.x && v2.x<=ur.x) &&(ll.z<=v2.z && v2.z<=ur.z)) return true ;
  return false ;
}

inline bool aiWorld::visible_in_bee_view(const vect3f& v) const
{
  return dot_product(cCam.View()-cCam.Position(),v-cCam.Position())>0 ;
}

inline vect2f& aiWorld::transform_vector(vect2f& v) const
{
  uint sx = v.x>0 ? 1 : 0 ;
  uint sz = v.z>0 ? 1 : 0 ;
  vect2f w(v.x*sx,v.z*sz) ;
  if (2 * w.x >= size_) v.x -= sx*size_;
  if (2 * w.z >= size_) v.z -= sx*size_ ;
  return v ;
}

inline float aiWorld::distance(const vect2f& v1, const vect2f& v2) const
{
  vect2f v = v2-v1;
  // pour éviter une erreur avec gcc qui veut un vrai vecteur 
  // erreur : initialization of non-const reference type `struct vect2f &
  // from rvalue of type `TVector2<float>'
  return transform_vector(v).Norm() ;
}

inline uint aiWorld::get_size() const
{
  return size_ ;
}

inline void aiWorld::change_sun_angle() const
{
  float angle((get_time() % VIRTUAL_DAY ) * PIx2 / VIRTUAL_DAY) ;
  map_->vSunDirection = CVector3f(0,-sin(angle),cos(angle)) ;
}


inline void aiWorld::fix_coords(vect2f& c) const
{
  if (c.x < 0) c.x += size_ ;
  else if (c.x >= size_) c.x -= size_ ;
  if (c.z < 0) c.z += size_ ;
  else if (c.z >= size_) c.z -= size_ ;
}

inline vect2u aiWorld::indexes_of_sector_at(const vect2f& c) const
{
  vect2f c2 = c ;
  //fix_coords(c2) ;
  return vect2u(static_cast<int>(c2.x) / SECTOR_STEP,
                static_cast<int>(c2.z) / SECTOR_STEP) ;
}

inline void aiWorld::next_token(const token_t t)
{
  tokens_[t].next();
  while (tokens_[t].current()->amount_ == 0)
    tokens_[t].remove_current();
/*  if (tokens_[t].current()->amount_==0)
    tokens_[t].delete_current() ;
  else
    tokens_[t].next() ;
    */
}

inline void aiWorld::next_object()
{
  objects_.next() ;
}

inline void aiWorld::gain_token(aiSector& s,const token_t t, const uint x, const vect2f& c)
{
  uint u = (s.tokens_[t].amount_) ;
  s.tokens_[t].add(x,c) ;
  if (!u) tokens_[t].add(s.tokens_[t]) ;
}

inline void aiWorld::ai_update_every_object()
{
  for(uint i=0 ; i< get_number_of_objects() ; ++i, next_object()) {
    if (current_object().vanished()) {
      delete_current_object() ;
    } else {
      current_object().ai_update() ;
    }
  }
}

inline uint aiWorld::get_number_of_tokens(const token_t t) const
{
  return tokens_[t].get_size() ;
}

inline uint aiWorld::get_number_of_objects() const
{
  return objects_.get_size() ;
}

inline void aiWorld::delete_current_object()
{
  objects_.delete_current() ;
}

inline const uint aiWorld::get_sectors_nb() const { return sectors_nb_ ; }

inline const ulong& aiWorld::get_time() const
{
  return time_() ;
}

inline void aiWorld::inc_time()
{
  ++time_ ;
}

inline aiSector& aiWorld::sector_at(const vect2f& c) const
{
  return sector_at(indexes_of_sector_at(c)) ;
}

inline aiSector& aiWorld::sector_at(const vect2u& c) const
{
  return sector_at(c.x,c.z) ;
}

inline aiSector& aiWorld::sector_at(const int x, const int z) const
{
  return sectors_[x % sectors_nb_][z % sectors_nb_] ;
}

inline aiToken& aiWorld::current_token(const token_t t) const
{
  return *tokens_[t].current() ;
}

inline aiObject& aiWorld::current_object() const
{
  return *objects_.current() ;
}

inline void aiWorld::add_object(aiObject& o)
{
  objects_.add(o) ;
}

#endif /* __AI_WORLD_IMPL_H__ */
