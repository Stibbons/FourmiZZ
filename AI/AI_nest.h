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
#ifndef __AI_NEST_H__
#define __AI_NEST_H__

#include "AI_shared.h"
#include "AI_ring.h"
#include "AI_technical.h"

class aiNestEntrance
{

public:

  // construit une entr�e de fourmili�re
  aiNestEntrance(const vect2f&) ;

  // retourne les coordonn�es de l'entr�e (non relatives � la fourmili�re)
  const vect2f& get_coords2d() const ;

  // construit une entr�e de fourmili�re
  ~aiNestEntrance() ;

private:

  // coordonn�es de l'entr�e (non relatives � la fourmili�re)
  vect2f coords2d_ ;
} ;

struct aiNestBaby
{
  // la morphologie quand l'enfant-fourmi sera adulte
  aiID_t morphology_ ;

  // le "stade de d�veloppement"
  uint age_ ;

  // construit un enfant-fourmi
  aiNestBaby(const aiID_t) ;
} ;

class aiNest
{

public:

  // construit une fourmili�re
  // (monde, coordonn�es, 
  aiNest(class aiWorld&, const vect2f&, const float) ;

  // retourne les coordonn�es 2D du sommet de la fourmili�re
  const vect2f& get_coords2d() const ;

  // retourne la largeur de la fourmili�re
  float get_width() const ;

  // retourne la quantit� de nourriture stock�e dans la fourmili�re
  uint get_food_stock() const ;

  // ajoute de la nourriture au grenier
  void add_to_food_stock(class aiFood*) ;

  // a-t-elle besoin de plus d'�claireuses ?
  bool needs_a_scout() const ;

  // a-t-elle besoin de plus de r�colteuses ?
  bool needs_a_harvester() const ;

  // met � jour la fourmili�re au niveau de l'intelligence
  void ai_update() ;

  // effectue le rendu de la fourmili�re en mode Abeille
  void render_in_bee_view() ;

  // effectue le rendu de la fourmili�re en mode Scrolling
  void render_in_scroll_view() ;

/////////////////////////////////////////////////////////////

  // retourne le nombre de fourmis en d�veloppement
  uint get_number_of_babies() const ;

  // retourne la fourmi courante
  aiNestBaby& current_baby() ;

  // enfant-fourmi suivant
  void next_baby() ;

  // supprimer l'enfant-fourmi courant
  void delete_current_baby() ;

/////////////////////////////////////////////////////////////

  // retourne le nombre d'entr�es � la fourmili�re
  uint get_number_of_entrances() const ;

  // retourne l'entr�e courante
  aiNestEntrance& current_entrance() const ;

  // ajoute une entr�e � la fourmili�re
  void add_entrance(aiNestEntrance&) ;

  // passe � l'entr�e suivante
  void next_entrance() ;

  // retourne une entr�e al�atoirement parmi toutes les entr�es
  aiNestEntrance& random_entrance() ;

/////////////////////////////////////////////////////////////

  // d�truit une fourmili�re
  ~aiNest() ;

private:

  friend void reset_world(class aiWorld*&, const std::string, struct Map_Settings&) ;
  friend void init_world(class aiWorld*&, const std::string, struct Map_Settings&) ;

  // monde auquel elle appartient
  class aiWorld& world_ ;

  // coordonn�es 2D du sommet de la fourmili�re
  vect2f coords2d_ ;

  // largeur de la fourmili�re
  float width_ ;

  // stock de nourriture
  uint food_stock_ ;

  // les entr�es de la fourmili�re
  aiRing<aiNestEntrance> entrances_ ;

  aiRing<aiNestBaby> babies_ ;

} ;

//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_nest_impl.h"

#endif /* __AI_NEST_H__ */
