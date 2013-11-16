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
#ifndef __AI_GRAPHIC_OBJECTS_H__
#define __AI_GRAPHIC_OBJECTS_H__

#include "AI_shared.h"
#include "../lib/C3DObject.h"
#include "AI_technical.h"

// objet graphique de lien (OGL) entre
// l'Intelligence et le Rendu Graphique
struct aiGraphicObject
{
  // éléments 3D de l'OGL
  C3DObjElements objs3d_ ;

  // la map auquel il appartient
  class CMap& map_ ;

  // coordonnées de l'OGL utilisées conjointement par l'Intelligence et
  // le Rendu Graphique
  vect3f coords3d_ ;

  // partie 2D des coordonnées 3D
  vect2f& coords2d() ;
  const vect2f& get_coords2d() const ;

  // orientation de l'OGL
  vect3f angles3d_  ;

  // action précédente
  uint oldaction_ ;

  //construit l'OGL (Objet Graphique de Lien)
  // (modèle d'objets 3D, map, coordonnées 2D, angle y)
  aiGraphicObject(const C3DObjElements& e, CMap*,
                  const vect2f& c, const float an) ;

  // retourne l'élément 3D de l'OGL
  const C3DObjElement& get_3D_object_element() const ;

  // retourne les coordonnées 3D de l'OGL
  const vect3f& get_coords3d() const ;

  // change d'animation le cas échéant au passe d'une action à une autre
  void update_animation(const action_type, const action_type) ;

  // effectue le rendu de l'OGL
  void render() ;

  // retourne la distance de l'OGL à la caméra en mode abeille
  float get_distance_from_bee_camera() const ;

  // prendre un objet
  void take_object(aiGraphicObject&);

  // lacher un objet
  void drop_object(aiGraphicObject&);

  // détruit l'OGL
  ~aiGraphicObject() ;

  bool select_from_camera(const CVector3f& pos, const CVector3f& dir);
} ;

//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_graphic_objects_impl.h"

#endif /* __AI_GRAPHIC_OBJECTS_H__ */
