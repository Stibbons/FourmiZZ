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
#include "AI_objects.h"


/*****************\
* classe aiOBJECT *
\*****************/

void aiObject::teleport_to(const vect2f& c)
{
  vect2f c2(c) ;
  world_.fix_coords(c2) ;
  aiSector* old_sector=sector_ ;
  coords2d() = c2 ;
  sector_ = &world_.sector_at(c2) ;
  if (old_sector!=sector_) {
    old_sector->lose(*this) ;
    sector_->get(*this) ;
  }
}

/**********************\
* classe aiLIVINGBEING *
\**********************/

void aiLivingBeing::lose_hp(uint e)
{
  if (hp_ > e)
    hp_ -= e ;
  else
    die() ;
}

