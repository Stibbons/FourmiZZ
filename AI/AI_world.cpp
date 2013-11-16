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
#include "AI_world.h"
#include "AI_ants.h"
#include "AI_renders.h"
#include "AI_shared.h"

void init_world(aiWorld*& wo, const std::string filename, Map_Settings& ms)
{
  ifstream f(filename.c_str()) ;
  if (!f) throw_error(" impossible d'ouvrir le fichier : "+filename) ;

  uint sectors_nb(DEFAULT_SECTORS_NB) ;
  uint initial_hours(DEFAULT_INITIAL_HOURS) ;
  uint initial_minutes(DEFAULT_INITIAL_MINUTES) ;

  string s ;

  // recherche des paramètres constants
  while (!f.eof()) {
    f >> s ;
    if (s=="SECTORS_NB") f >> sectors_nb ;
  }
  f.close() ;

  // création du monde
  wo = new aiWorld(ms.Map_Size / SECTOR_STEP) ;

  // recherche des paramètres non constants
  f.clear() ;
  f.open(filename.c_str()) ;

  float f1, f2 ;
  while (!f.eof()) {

    f >> s ;

    if (s=="HARVESTER") {
      f >> f1 >> f2 ;
      wo->add_object(*new aiHarvester(*wo,vect2f(f1,f2),UINT_RAND(HARVESTER_INITIAL_HP))) ;
    }
    if (s=="SCOUT") {
      f >> f1 >> f2 ;
      wo->add_object(*new aiScout(*wo,vect2f(f1,f2),UINT_RAND(SCOUT_INITIAL_HP))) ;
    }
    if (s=="SEED") {
      f >> f1 >> f2 ;
      wo->add_object(*new aiSeed(*wo,vect2f(f1,f2))) ;
    }
    if (s=="SPIDER") {
      f >> f1 >> f2 ;
      wo->add_object(*new aiSpider(*wo,vect2f(f1,f2),SPIDER_INITIAL_HP)) ;
    }
    if (s=="REDFLOWER") {
      f >> f1 >> f2 ;
      wo->add_object(*new aiRedFlower(*wo,vect2f(f1,f2),1 + FLOAT_RAND * 0.7)) ;
    }
    if (s=="WEED") {
      f >> f1 >> f2 ;
      wo->add_object(*new aiWeed(*wo,vect2f(f1,f2),1 + FLOAT_RAND * 0.7)) ;
    }
    if (s=="MUSHROOM") {
      f >> f1 >> f2 ;
      wo->add_object(*new aiMushroom(*wo,vect2f(f1,f2),1 + FLOAT_RAND * 0.7)) ;
    }
  }
  f.close() ;

  uint i,j ;

  for (i = 0 ; i < ms.o_cpt.nbspiders ; ++i) {
    wo->add_object(*new aiSpider(*wo, vect2f(UINT_RAND(ms.Map_Size),UINT_RAND(ms.Map_Size)),
                                         SPIDER_INITIAL_HP)) ;
  }

  for (i = 0 ; i < ms.o_cpt.nbfoodsources ; ++i) {
    vect2f v(UINT_RAND(ms.Map_Size),UINT_RAND(ms.Map_Size)) ;
    for (j = 0 ; j < ms.o_cpt.nb_food_by_source ; ++j) {
      wo->add_object(*new aiSeed(*wo,v + vect2f(SYM_RAND,SYM_RAND)*30)) ;
    }
  }


  for (i = 0 ; i < ms.o_cpt.nbgrass ; ++i) {
    wo->add_object(*new aiWeed(*wo, vect2f(UINT_RAND(ms.Map_Size),UINT_RAND(ms.Map_Size)),
                                       FLOAT_RAND*WEED_MAX_HEIGHT)) ;
  }

  for (i = 0 ; i < ms.o_cpt.nbredflowers ; ++i) {
    wo->add_object(*new aiRedFlower(*wo, vect2f(UINT_RAND(ms.Map_Size),UINT_RAND(ms.Map_Size)),
                                         FLOAT_RAND*RED_FLOWER_MAX_HEIGHT)) ;
  }

  for (i = 0 ; i < ms.o_cpt.nbmushrooms ; ++i) {
    wo->add_object(*new aiMushroom(*wo, vect2f(UINT_RAND(ms.Map_Size),UINT_RAND(ms.Map_Size)),
                                         FLOAT_RAND*MUSHROOM_MAX_HEIGHT)) ;
  }

  wo->nest_ = new aiNest(*wo,
                         vect2f(static_cast<float>(ms.o_cpt.coord_nest.x),
                                static_cast<float>(ms.o_cpt.coord_nest.z)),
                         static_cast<float>(ms.o_cpt.nest_width)) ;

  float theta = FLOAT_RAND * 10 ;
  wo->nest_->add_entrance(*new aiNestEntrance(wo->nest_->coords2d_+
                                                vect2f(cos(theta)*wo->nest_->get_width(),
                                                       sin(theta)*wo->nest_->get_width()))) ;


  //TEMP...

  for(i=0 ; i < 30; ++i)
  {
    wo->add_object(*new aiHarvester(*wo,wo->nest_->coords2d_ + vect2f(SYM_RAND*150,SYM_RAND*150),
                                    HARVESTER_INITIAL_HP)) ;
  }
  for(i=0 ; i < 7 ; ++i)
  {
    wo->add_object(*new aiScout(*wo,wo->nest_->coords2d_ + vect2f(SYM_RAND*150,SYM_RAND*150),
                                    SCOUT_INITIAL_HP)) ;
  }
  //...TEMP

  wo->map_->Init(ms) ;

  cCam.vPositionCamera(vect3f(wo->nest_->coords2d_,wo->map_->GetMapHeight(wo->nest_->coords2d_)+50)) ;

}

void done_world(class aiWorld*& wo)
{
  if (wo) delete wo ;
  wo = 0 ;
}

void reset_world(class aiWorld*& wo,const std::string filename, Map_Settings& ms)
{
  if (wo) delete wo ;
  init_world(wo,filename,ms) ;
}

/***********************\
* fonctions de aiSECTOR *
\***********************/

aiSector::aiSector()
: tokens_(), stuff_(), last_time_updated_(0)
{}

void aiSector::get(aiObject& t)
{
  stuff_.add(t) ;
}

void aiSector::lose(aiObject& t)
{
  for (uint i(0) ; i < get_number_of_stuff() ; ++i) {
    if (&current_stuff()==&t)
      remove_stuff() ;
    else
      next_stuff() ;
  }
/*
  while (&current_stuff()!=&t) next_stuff() ;
    // attention : boucle infinie si t n'est pas sur le secteur
  remove_stuff() ;
*/
}

aiSector::~aiSector()
{
  while (get_number_of_stuff()) stuff_.delete_current() ;
}

/**********************\
* fonctions de aiWORLD *
\**********************/

aiWorld::aiWorld(const uint sn)
: sectors_nb_(sn), size_(sn*SECTOR_STEP), nest_(0), map_(0), tokens_()
{
  g_world_ = new aiGraphicWorld(*this) ;
  sectors_ = new aiSector*[sectors_nb_] ;
  for(uint i=0;i<sectors_nb_;++i)
    sectors_[i] = new aiSector[sectors_nb_] ;

  map_ = new CMap ;
}

aiSector& aiWorld::update_sector(aiSector& s) const
{
  if (time_()>s.last_time_updated_) {
    if (get_time() - s.last_time_updated_ < s.tokens_[found_food].amount_)
      s.tokens_[found_food].amount_ -= get_time() - s.last_time_updated_ ;
    else
      s.tokens_[found_food].amount_ = 0 ;
  }
  s.last_time_updated_ = get_time() ;
  return s ;
}

void aiWorld::ai_update()
{
  inc_time() ;
  change_sun_angle() ;
  ai_update_every_object() ;
  nest_->ai_update() ;
}

void aiWorld::graphic_update_objects()
{
  for(uint i(0) ; i< get_number_of_objects() ; ++i, next_object()) {
    if (current_object().vanished()) {
      delete_current_object() ;
    } else {
      if (current_object().is_active()) current_object().graphic_update() ;
    }
  }
}


void aiWorld::render_visible_objects_in_bee_view()
{
  for(uint i(0) ; i< get_number_of_objects() ; ++i, next_object()) {
    if (current_object().is_active()) current_object().render() ;
  }
}

void aiWorld::render_visible_objects_in_scroll_view()
{
/*
  vect2f ll_coords(map_->ScrollLimitf()) ;

  vect2f frame_size(map_->FrameSize()) ;

  vect2u ll_index(indexes_of_sector_at(ll_coords)) ;
  vect2u ur_index(indexes_of_sector_at(ll_coords+frame_size)) ;
  aiSector* s ;
  for (uint i(ll_index.x) ; i < ur_index.x ; ++i) {
    for (uint j(ll_index.z) ; j < ur_index.z ; ++j) {
      s = &sector_at(i,j) ;
      for (uint k(0) ; k < s->get_number_of_stuff() ; ++k, s->next_stuff()) {
        if (s->get_number_of_stuff()!=5)
        s->current_stuff().render() ;
      }
    }
  }*/
  for(uint i(0) ; i< get_number_of_objects() ; ++i, next_object()) {
    if (current_object().is_active() && visible_in_scrolling_view(current_object().get_coords2d()))
      current_object().render() ;
  }
}

void aiWorld::render()
{
  graphic_update_objects() ;
  g_world_->render() ;


//TEMP...
/*  uint nb(0) ;
  for (uint i(0) ; i < sectors_nb_ ; ++i) for (uint j(0) ; j < sectors_nb_ ; ++j) {
    nb += sectors_[i][j].get_number_of_stuff() ;
  }
  //cout << nb << endl ;
*/
//...TEMP


}

aiWorld::~aiWorld()
{
  for(uint i(0) ; i<sectors_nb_;++i) delete[] sectors_[i] ;
  delete[] sectors_ ;
  delete g_world_ ;
  delete map_ ;
  delete nest_ ;
}

