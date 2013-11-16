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
#include "AI_enemies.h"

aiSpider::aiSpider(aiWorld& w, const vect2f& c, uint nrg)
: aiLivingBeing(w,c,nrg), destination_(0,0)
{
  g_obj_ = new aiGraphicObject(*g_ObjsSpider, w.map_, c, UINT_RAND(360)) ;
  destination_ = vect2f(cos(g_obj_->angles3d_.y),sin(g_obj_->angles3d_.y)) ;
}

void aiSpider::live()
{
  aiLivingBeing::live() ;
  switch (action_) {
    case IDLE:
      do_idle() ;
      break ;
    case HUNT_ANTS:
      do_hunt_ants() ;
      break ;
  }
}

void aiSpider::do_idle()
{
  now_do(HUNT_ANTS) ;
}

void aiSpider::do_hunt_ants()
{
  destination_ *= SPIDER_OLD_WAY_INFLUENCE ;
  destination_ += vect2f(SYM_RAND,SYM_RAND).Normalize() * (1- SPIDER_OLD_WAY_INFLUENCE) ;
}

aiSpider::~aiSpider()
{}
