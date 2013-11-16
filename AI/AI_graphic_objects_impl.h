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
#ifndef __AI_GRAPHIC_OBJECTS_IMPL_H__
#define __AI_GRAPHIC_OBJECTS_IMPL_H__

/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_graphic_objects.h avec cette implémentation.
*/

inline vect2f& aiGraphicObject::coords2d()
{
  return coords3d_ ;
}

inline const vect2f& aiGraphicObject::get_coords2d() const
{
  return coords3d_ ;
}


inline aiGraphicObject::~aiGraphicObject()
{}

#endif /* __AI_GRAPHIC_OBJECTS_IMPL_H__ */

