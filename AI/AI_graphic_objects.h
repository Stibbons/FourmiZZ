/********************************************************************************
    Foumi-ZZ - Simulation de Fourmili�re ISIMA Premi�re Ann�e
    Copyright (C) 2003 DANVIN Yohan, FARGE Julien, GOUBE Florent, SEMET Ga�tan
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
#ifndef __AI_GRAPHIC_OBJECTS_H__
#define __AI_GRAPHIC_OBJECTS_H__

#include "AI_shared.h"
#include "../lib/C3DObject.h"
#include "AI_technical.h"

// objet graphique de lien (OGL) entre
// l'Intelligence et le Rendu Graphique
struct aiGraphicObject
{
  // �l�ments 3D de l'OGL
  C3DObjElements objs3d_ ;

  // la map auquel il appartient
  class CMap& map_ ;

  // coordonn�es de l'OGL utilis�es conjointement par l'Intelligence et
  // le Rendu Graphique
  vect3f coords3d_ ;

  // partie 2D des coordonn�es 3D
  vect2f& coords2d() ;
  const vect2f& get_coords2d() const ;

  // orientation de l'OGL
  vect3f angles3d_  ;

  // action pr�c�dente
  uint oldaction_ ;

  //construit l'OGL (Objet Graphique de Lien)
  // (mod�le d'objets 3D, map, coordonn�es 2D, angle y)
  aiGraphicObject(const C3DObjElements& e, CMap*,
                  const vect2f& c, const float an) ;

  // retourne l'�l�ment 3D de l'OGL
  const C3DObjElement& get_3D_object_element() const ;

  // retourne les coordonn�es 3D de l'OGL
  const vect3f& get_coords3d() const ;

  // change d'animation le cas �ch�ant au passe d'une action � une autre
  void update_animation(const action_type, const action_type) ;

  // effectue le rendu de l'OGL
  void render() ;

  // retourne la distance de l'OGL � la cam�ra en mode abeille
  float get_distance_from_bee_camera() const ;

  // prendre un objet
  void take_object(aiGraphicObject&);

  // lacher un objet
  void drop_object(aiGraphicObject&);

  // d�truit l'OGL
  ~aiGraphicObject() ;

  bool select_from_camera(const CVector3f& pos, const CVector3f& dir);
} ;

//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_graphic_objects_impl.h"

#endif /* __AI_GRAPHIC_OBJECTS_H__ */
