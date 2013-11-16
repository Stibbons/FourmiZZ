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

#include "AI_ants.h"
#include "../Objects.h"

/****************\
* classe aiMINOR *
\****************/

inline aiMinor::aiMinor(aiWorld& w, const vect2f& c,
                        uint nrg)
: aiAnt(w,c,nrg),carried_(0), destination_(0,0)
{}

inline aiID_t aiMinor::scan_object(const aiObject& o) const
{
  return o.get_id() ;
}

void aiMinor::carry(aiObject& o)
{
  g_obj_->take_object(*o.g_obj_) ;
  carried_ = &o ;
  sector_->lose(*carried_) ;
  carried_->active_ = false ;
}

void aiMinor::drop_carried()
{
  g_obj_->drop_object(*carried_->g_obj_) ;
  carried_->active_ = true ;
  sector_->get(*carried_) ;
  carried_->coords2d() = get_coords2d() ;
  carried_->sector_ = &world_.sector_at(carried_->get_coords2d()) ;
  carried_ = 0 ;
}

bool aiMinor::there_is_food_on_sector(aiSector& s) const
{
  for(uint i=0 ; i<s.get_number_of_stuff() ; ++i, s.next_stuff()) {
    if (s.current_stuff().is_active() && is_food(s.current_stuff())) return true ;
  }
  return false ;
}

bool aiMinor::there_is_food_within_range()
{
  vect2u a(world_.indexes_of_sector_at(get_coords2d())) ;
  int i,j ;
  for(i = -FEELERS_RANGE ; i<=FEELERS_RANGE ; ++i) {
    for (j = -FEELERS_RANGE ; j<= FEELERS_RANGE ; ++j) {
      if (there_is_food_on_sector(world_.sector_at(a.x+i,a.z+j))) return true ;
    }
  }
  return false ;
}

vect2f aiMinor::token_tendency(const token_t t)
{
  aiToken b, *c ;
  vect2u a(world_.indexes_of_sector_at(get_coords2d())) ;
  int i,j ;
  for(i = -FEELERS_RANGE ; i<=FEELERS_RANGE ; ++i) {
    for (j = -FEELERS_RANGE ; j<= FEELERS_RANGE ; ++j) {
      c = &world_.update_sector(world_.sector_at(a.x+i,a.z+j)).tokens_[t] ;
      if (c->amount_)
      {
        vect2f v(c->where_-get_coords2d());
        b.add(c->amount_,world_.transform_vector(v)) ;
      }
    }
  }
  //if (b.where_()) b.where_.Normalize() ;
  return b.where_ ;
}

vect2f aiMinor::token_tendency_in_that_way(const token_t t, const vect2f& way)
{
  aiToken b, *ph ;
  vect2u a(world_.indexes_of_sector_at(get_coords2d())) ;
  vect2f v ;
  int i,j ;
  for(i = -FEELERS_RANGE ; i<=FEELERS_RANGE ; ++i) {
    for (j = -FEELERS_RANGE ; j<= FEELERS_RANGE ; ++j) {
      ph = &(world_.update_sector(world_.sector_at(a.x+i,a.z+j)).tokens_[t]) ;
      vect2f v(ph->where_-get_coords2d());
      if (ph->amount_ && dot_product(way,v=world_.transform_vector(v))>=0) 
        b.add(ph->amount_,v) ;
    }
  }
  //if (b.where_()) b.where_.Normalize() ;
  return b.where_ ;
}

void aiMinor::live()
{
  aiAnt::live() ;
  ++hunger_ ;
  if (is_hungry()) 
    lose_hp(STARVING_DAMAGE) ;
  if (destination_()) {
    world_.gain_token(*sector_,path,PATH_DIGGING_QUANTITY,get_coords2d()) ;
  }
}

aiFood* aiMinor::most_valuable_carriable_food_on_the_sector() const
{
  aiFood* max_food = 0 ;
  aiFood* cur ;
  for(uint i=0 ; i<sector_->get_number_of_stuff() ; ++i, sector_->next_stuff()) {
    if (sector_->current_stuff().is_active() && is_food(sector_->current_stuff())) {
      cur = static_cast<aiFood*>(&sector_->current_stuff()) ;
      if (!max_food || ((cur->get_weight()<=get_carriable_weight()) &&
          (cur->get_value()>max_food->get_value()))) {
        max_food = cur ;
      }
    }
  }
  return max_food ;
}

vect2f aiMinor::food_tendency_within_range_in_the_way() const
{
  aiSector* s ;
  vect2u a(world_.indexes_of_sector_at(get_coords2d())) ;
  aiToken ft ;
  aiFood* cur ;
  vect2f v ;
  int i,j ;
  for(i = -FEELERS_RANGE ; i<=FEELERS_RANGE ; ++i) {
    for (j = -FEELERS_RANGE ; j<= FEELERS_RANGE ; ++j) {
      s = &world_.sector_at(a.x+i,a.z+j) ;
      for(uint i=0 ; i<s->get_number_of_stuff() ; ++i, s->next_stuff()) {
        if (s->current_stuff().is_active() && is_food(s->current_stuff())) {
          cur = static_cast<aiFood*>(&s->current_stuff()) ;
          vect2f v (cur->get_coords2d()-get_coords2d());
          if (dot_product(destination_,v=world_.transform_vector(v))>=0) {
            ft.add(cur->get_value(),v) ;
          }
        }
      }
    }
  }
  return ft.where_  ;
}

vect2f aiMinor::nearest_nest_entrance_direction() const
{
/*
  float dist2, dist = world_.distance(nest().current_entrance().get_coords2d(),get_coords2d()) ;
  const aiNestEntrance* entr = &nest().current_entrance() ;

  for (uint i(0) ; i < nest().get_number_of_entrances() - 1 ; ++i, nest().next_entrance()) {
    if ((dist2=world_.distance(nest().current_entrance().get_coords2d(),get_coords2d()))<dist) {
      dist = dist2 ;
      entr = &nest().current_entrance() ;
    }
  }
  return *entr ;
*/
  vect2f av (nest().current_entrance().get_coords2d()-get_coords2d());
  vect2f v(world_.transform_vector(av)) ;
  float sqr_dist(v.Sqr()) ;
  vect2f v2 ;
  float sqr_dist2 ;
  for (uint i(0) ; i < nest().get_number_of_entrances() - 1 ; ++i) {
    nest().next_entrance() ;
    vect2f tv(nest().current_entrance().get_coords2d()-get_coords2d());
    v2 = world_.transform_vector(tv) ;
    if ((sqr_dist2=v2.Sqr()) < sqr_dist) { v = v2 ; sqr_dist = sqr_dist2 ; }
  }
  return v ;
}


void aiMinor::die()
{
  if(carried_)
    drop_carried() ;
  destination_ = 0 ;
  aiAnt::die() ;
}

aiMinor::~aiMinor()
{}

/********************\
* classe aiHARVESTER *
\********************/

aiHarvester::aiHarvester(aiWorld& w, const vect2f& c, uint nrg)
: aiMinor(w,c,nrg)
{
  g_obj_ = new aiGraphicObject(*g_ObjsHarvester, w.map_, c, UINT_RAND(360)) ;
  destination_ = vect2f(cos(g_obj_->angles3d_.y),sin(g_obj_->angles3d_.y)) ;
}

void aiHarvester::live()
{
  aiMinor::live() ;
  switch (action_) {
    case IDLE :
      do_idle() ;
      break ;
    case WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE :
      do_wander_around_nest_waiting_for_found_food_pheromone() ;
      break ;
    case FOLLOW_FOUND_FOOD_PHEROMONE_TO_FIND_FOOD :
      do_follow_found_food_pheromone_to_find_food() ;
      break ;
    case BRING_FOOD_TO_THE_NEST :
      do_bring_found_food_to_the_nest() ;
      break ;
    case GO_BACK_TO_THE_NEST_LOOKING_FOR_FOOD :
      do_go_back_to_the_nest_looking_for_food() ;
      break ;
    case ENTER_THE_NEST_TO_STORE_FOOD:
      do_enter_the_nest_to_store_food() ;
      break ;
    case PUT_FOOD_IN_THE_GRANARY :
      do_put_food_in_the_granary() ;
      break ;
    case GET_OUT_OF_THE_NEST:
      do_get_out_of_the_nest() ;
      break ;
  }
}

void aiHarvester::do_idle()
{
  now_do(WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE) ;
}

void aiHarvester::do_put_food_in_the_granary()
{
  if (!action_timer_ && carried_) {
    nest().add_to_food_stock(static_cast<aiFood*>(carried_)) ;
    carried_ = 0 ;
    g_obj_->objs3d_.m_hiRes->Reset() ;
    if (g_obj_->objs3d_.m_lowRes) g_obj_->objs3d_.m_lowRes->Reset() ;
    now_do(GET_OUT_OF_THE_NEST,TIME_TO_GET_OUT) ;
  }
}

void aiHarvester::do_get_out_of_the_nest()
{
  if (!action_timer_) {
    // sort par une entrée de la fourmilière au hasard
    teleport_to(nest().random_entrance().get_coords2d()) ;
    active_ = true ;
    now_do(IDLE) ;
  }
}

void aiHarvester::do_wander_around_nest_waiting_for_found_food_pheromone()
{
  aiFood* to_carry ;
  if (to_carry=most_valuable_carriable_food_on_the_sector()) {
    carry(*to_carry) ;
    secret_token(found_food,HARVESTER_FOUND_FOOD_SECRETED_QUANTITY) ;
    destination_ = hypothethic_nest_direction() ;
    now_do(BRING_FOOD_TO_THE_NEST) ;
  } else {
    vect2f v(food_tendency_within_range_in_the_way()) ;
    if (v()) {
      destination_ = v ;
    } else {
      v = token_tendency_in_that_way(found_food,-hypothethic_nest_direction()) ;
      if (v()) {
        now_do(FOLLOW_FOUND_FOOD_PHEROMONE_TO_FIND_FOOD) ;
      } else if (ROLL(HARVESTER_GO_BACK_PROBABILITY*(get_coords2d() - nest().get_coords2d()).Norm())) {
        v = token_tendency_in_that_way(path,hypothethic_nest_direction()) ;
        destination_ *= HARVESTER_OLD_WAY_INFLUENCE ;
        destination_ += v.Normalize() * (1 - HARVESTER_OLD_WAY_INFLUENCE) ;
      } else if (ROLL(HARVESTER_TURNING_PROBABILITY)) {
        destination_ *= HARVESTER_OLD_WAY_INFLUENCE ;
        destination_ += vect2f(SYM_RAND,SYM_RAND).Normalize() * (1- HARVESTER_OLD_WAY_INFLUENCE) ;
      }
    }
  }
}

void aiHarvester::do_follow_found_food_pheromone_to_find_food()
{
  aiFood* to_carry ;
  if (to_carry=most_valuable_carriable_food_on_the_sector()) {
    carry(*to_carry) ;
    secret_token(found_food,HARVESTER_FOUND_FOOD_SECRETED_QUANTITY) ;
    destination_ = hypothethic_nest_direction() ;
    now_do(BRING_FOOD_TO_THE_NEST) ;
  } else {
    vect2f v(food_tendency_within_range_in_the_way()) ;
    if (v()) {
      destination_ *= HARVESTER_OLD_WAY_INFLUENCE ;
      destination_ += v.Normalize() * (1 - HARVESTER_OLD_WAY_INFLUENCE) ;
    } else {
      vect2f v(token_tendency_in_that_way(found_food,-hypothethic_nest_direction())) ;
      if (v()) {
        destination_ *= HARVESTER_OLD_WAY_INFLUENCE ;
        destination_ += v.Normalize() * (1 - HARVESTER_OLD_WAY_INFLUENCE) ;
      } else {
        now_do(GO_BACK_TO_THE_NEST_LOOKING_FOR_FOOD) ;
      }
    }
  }
}

void aiHarvester::do_go_back_to_the_nest_looking_for_food()
{
  aiFood* to_carry ;
  if ((nest().get_coords2d()-coords2d()).Norm()<nest().get_width()) {
    now_do(IDLE) ;
  } if (to_carry=most_valuable_carriable_food_on_the_sector()) {
    carry(*to_carry) ;
    secret_token(found_food,HARVESTER_FOUND_FOOD_SECRETED_QUANTITY) ;
    destination_ = hypothethic_nest_direction() ;
    now_do(BRING_FOOD_TO_THE_NEST) ;
  }
  else {
    vect2f v(food_tendency_within_range_in_the_way()) ;
    if (v()) {
      destination_ = v ;
    } else {
      vect2f v(token_tendency_in_that_way(path,hypothethic_nest_direction())) ;
      if (v()) {
        destination_ *= HARVESTER_OLD_WAY_INFLUENCE ;
        destination_ += v.Normalize() * (1 - HARVESTER_OLD_WAY_INFLUENCE) ;
      } else {
        v = token_tendency_in_that_way(found_food,hypothethic_nest_direction()) ;
        destination_ *= HARVESTER_OLD_WAY_INFLUENCE ;
        destination_ += v.Normalize() * (1- HARVESTER_OLD_WAY_INFLUENCE) ;
      }
    }
  }
}

void aiHarvester::do_bring_found_food_to_the_nest()
{
  secret_token(found_food,HARVESTER_FOUND_FOOD_SECRETED_QUANTITY) ;
  if (world_.distance(nest().get_coords2d(),coords2d())<2*nest().get_width()) {
    now_do(ENTER_THE_NEST_TO_STORE_FOOD) ;
  } else {
    vect2f v(token_tendency_in_that_way(path,hypothethic_nest_direction())) ;
    destination_ *= HARVESTER_OLD_WAY_INFLUENCE ;
    destination_ += v.Normalize() * (1- HARVESTER_OLD_WAY_INFLUENCE) ;
  }
}

void aiHarvester::do_enter_the_nest_to_store_food()
{
  secret_token(found_food,HARVESTER_FOUND_FOOD_SECRETED_QUANTITY) ;
  //vect2f v(world_.transform_vector(nearest_nest_entrance().get_coords2d()-get_coords2d())) ;
  //vect2f v(nearest_nest_entrance().get_coords2d()-get_coords2d()) ;
  vect2f v(nearest_nest_entrance_direction()) ;
  if (v.Sqr() < SQR(NEST_ENTRANCE_RADIUS)) {
    active_ = false ;
    now_do(PUT_FOOD_IN_THE_GRANARY,STORING_FOOD_TIME) ;
  } else {
    destination_ = v ;
  }
}

/****************\
* classe aiSCOUT *
\****************/

aiScout::aiScout(aiWorld& w, const vect2f& c,
                 uint nrg)
: aiMinor(w,c,nrg)
{
  g_obj_ = new aiGraphicObject(*g_ObjsHarvester, w.map_, c, UINT_RAND(360)) ;
  destination_ = vect2f(cos(g_obj_->angles3d_.y),sin(g_obj_->angles3d_.y)) ;
}

void aiScout::live()
{
  aiMinor::live() ;
  switch (action_) {
    case IDLE :
      do_idle() ;
      break ;
    case LOOK_FOR_A_NEW_SOURCE_OF_FOOD :
      do_look_for_a_new_source_of_food() ;
      break ;
    case RETURN_TO_NEST_WITH_INFO_ABOUT_FOOD :
      do_return_to_nest_with_info_about_food();
      break ;
  }
}

void aiScout::do_idle()
{
  now_do(LOOK_FOR_A_NEW_SOURCE_OF_FOOD) ;
}

void aiScout::do_look_for_a_new_source_of_food()
{
  vect2f tt(token_tendency(found_food)) ;
  if (!tt() && there_is_food_on_sector(*sector_)) { 
    secret_token(found_food,HARVESTER_FOUND_FOOD_SECRETED_QUANTITY) ;
    destination_ = hypothethic_nest_direction() ;
    now_do(RETURN_TO_NEST_WITH_INFO_ABOUT_FOOD) ;
  } else {
    vect2f ft=food_tendency_within_range_in_the_way() ;
    if (!tt() && ft())
      destination_ = ft ;
    else if (ROLL(SCOUT_TURNING_PROBABILITY))  {
      destination_ *= SCOUT_OLD_WAY_INFLUENCE ;
      destination_ += vect2f(SYM_RAND,SYM_RAND).Normalize() * (1- SCOUT_OLD_WAY_INFLUENCE) ;
    }
  }
}

void aiScout::do_return_to_nest_with_info_about_food()
{
  secret_token(found_food,SCOUT_FOUND_FOOD_SECRETED_QUANTITY) ;
  if ((nest().get_coords2d()-coords2d()).Norm()<nest().get_width()) {
    now_do(LOOK_FOR_A_NEW_SOURCE_OF_FOOD) ;
  } else {
    vect2f v(token_tendency_in_that_way(path,hypothethic_nest_direction())) ;
    destination_ *= SCOUT_OLD_WAY_INFLUENCE ;
    destination_ += v.Normalize() * (1- SCOUT_OLD_WAY_INFLUENCE) ;
  }
}

