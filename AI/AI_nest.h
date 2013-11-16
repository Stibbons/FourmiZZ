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
#ifndef __AI_NEST_H__
#define __AI_NEST_H__

#include "AI_shared.h"
#include "AI_ring.h"
#include "AI_technical.h"

class aiNestEntrance
{

public:

  // construit une entrée de fourmilière
  aiNestEntrance(const vect2f&) ;

  // retourne les coordonnées de l'entrée (non relatives à la fourmilière)
  const vect2f& get_coords2d() const ;

  // construit une entrée de fourmilière
  ~aiNestEntrance() ;

private:

  // coordonnées de l'entrée (non relatives à la fourmilière)
  vect2f coords2d_ ;
} ;

struct aiNestBaby
{
  // la morphologie quand l'enfant-fourmi sera adulte
  aiID_t morphology_ ;

  // le "stade de développement"
  uint age_ ;

  // construit un enfant-fourmi
  aiNestBaby(const aiID_t) ;
} ;

class aiNest
{

public:

  // construit une fourmilière
  // (monde, coordonnées, 
  aiNest(class aiWorld&, const vect2f&, const float) ;

  // retourne les coordonnées 2D du sommet de la fourmilière
  const vect2f& get_coords2d() const ;

  // retourne la largeur de la fourmilière
  float get_width() const ;

  // retourne la quantité de nourriture stockée dans la fourmilière
  uint get_food_stock() const ;

  // ajoute de la nourriture au grenier
  void add_to_food_stock(class aiFood*) ;

  // a-t-elle besoin de plus d'éclaireuses ?
  bool needs_a_scout() const ;

  // a-t-elle besoin de plus de récolteuses ?
  bool needs_a_harvester() const ;

  // met à jour la fourmilière au niveau de l'intelligence
  void ai_update() ;

  // effectue le rendu de la fourmilière en mode Abeille
  void render_in_bee_view() ;

  // effectue le rendu de la fourmilière en mode Scrolling
  void render_in_scroll_view() ;

/////////////////////////////////////////////////////////////

  // retourne le nombre de fourmis en développement
  uint get_number_of_babies() const ;

  // retourne la fourmi courante
  aiNestBaby& current_baby() ;

  // enfant-fourmi suivant
  void next_baby() ;

  // supprimer l'enfant-fourmi courant
  void delete_current_baby() ;

/////////////////////////////////////////////////////////////

  // retourne le nombre d'entrées à la fourmilière
  uint get_number_of_entrances() const ;

  // retourne l'entrée courante
  aiNestEntrance& current_entrance() const ;

  // ajoute une entrée à la fourmilière
  void add_entrance(aiNestEntrance&) ;

  // passe à l'entrée suivante
  void next_entrance() ;

  // retourne une entrée aléatoirement parmi toutes les entrées
  aiNestEntrance& random_entrance() ;

/////////////////////////////////////////////////////////////

  // détruit une fourmilière
  ~aiNest() ;

private:

  friend void reset_world(class aiWorld*&, const std::string, struct Map_Settings&) ;
  friend void init_world(class aiWorld*&, const std::string, struct Map_Settings&) ;

  // monde auquel elle appartient
  class aiWorld& world_ ;

  // coordonnées 2D du sommet de la fourmilière
  vect2f coords2d_ ;

  // largeur de la fourmilière
  float width_ ;

  // stock de nourriture
  uint food_stock_ ;

  // les entrées de la fourmilière
  aiRing<aiNestEntrance> entrances_ ;

  aiRing<aiNestBaby> babies_ ;

} ;

//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_nest_impl.h"

#endif /* __AI_NEST_H__ */
