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
#ifndef __AI_ENEMIES_IMPL_H__
#define __AI_ENEMIES_IMPL_H__

/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_enemies.h avec cette implémentation.
*/

inline aiID_t aiSpider::scan_object(const aiObject& o) const
{
  return o.get_id() ;
}

inline colorRGBA aiSpider::color_on_minimap() const
{
  return colorRGBA(MINIMAP_COLOR_SPIDER) ;
}

inline aiID_t aiSpider::get_id() const
{
  return SPIDER ;
}

inline float aiSpider::get_speed() const
{
  return SPIDER_BASE_SPEED ;
}

inline bool aiSpider::is_food(const aiObject& ob) const
{
  uint t(ob.get_id()) ;
  return ((t==HARVESTER)||(t==SCOUT)||(t==MAJOR)) ;
}

inline const vect2f& aiSpider::get_destination() const
{
  return destination_ ;
}

#endif /* __AI_ENEMIES_IMPL_H__ */
