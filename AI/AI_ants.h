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
#ifndef __AI_ANTS_H__
#define __AI_ANTS_H__

#include "AI_shared.h"
#include "AI_objects.h"
#include "AI_myrmecology.h"
#include "AI_technical.h"
#include "AI_nest.h"
#include "AI_environment.h"

// fourmi
class aiAnt : public aiLivingBeing
{
  friend class aiWorld ;

  aiAnt(const aiAnt&) ; // copie interdite !
  aiAnt& operator=(const aiAnt&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const = 0 ;

protected:

  // construit la fourmi
  // (monde, coordonnées, hp)
  aiAnt(aiWorld&, const vect2f&, const uint) ;

  uint hunger_ ; // faim

  // retourne la fourmilière de la fourmi
  aiNest& nest() const ;

  // vivre
  virtual void live() ;

  // détruit la fourmi
  virtual ~aiAnt() ;

public:
/*******************\
* aiANT, public ... *
\*******************/

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

/*******************\
* ... public, aiANT *
\*******************/
} ;

// minor
class aiMinor : public aiAnt
{
  aiMinor(const aiMinor&) ; // copie interdite !
  aiMinor& operator=(const aiMinor&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const = 0 ;

protected:

  // identifie un objet (avec les antennes)
  aiID_t scan_object(const aiObject& o) const ;

  aiObject* carried_ ; // pointeur sur l'objet porté

  // coordonnées de la destination de l'objet
  vect2f destination_ ;


  // entrer dans la fourmilière
  void enter_the_nest() ;

  // ramasser l'objet spécifié
  void carry(aiObject&) ;

  // laisser tomber l'objet spécifié
  void drop_carried() ;

  // sécréter le token du type donné en quantité donnée
  void secret_token(const token_t, const uint) const ;

  // retourne l'élément de nourriture de valeur nutritive la plus grande que
  // peut porter la fourmi qui se trouve sur le secteur de la fourmi
  // retourne 0 si le secteur ne contient pas de nourriture
  aiFood* most_valuable_carriable_food_on_the_sector() const ;

  // vivre
  virtual void live() ;

  // flâner...
  void do_idle() ;

  // mourir.
  virtual void die() ;

public:

  // construit une minor
  // (monde, coordonnées, hp)
  aiMinor(aiWorld&, const vect2f&, uint) ;

  // coordonnées de la destination de la minor
  const vect2f& get_destination() const ;

  // retourne la vitesse de la fourmi
  float get_speed() const ;

  // retourne son facteur de taille
  const float& get_size() const ;

  // le poids que peut porter la minor
  uint get_carriable_weight() const ;

  // retourne la direction de l'entrée de la fourmilière la plus proche
  vect2f nearest_nest_entrance_direction() const ;

  // a-t-elle faim ?
  bool is_hungry() const ;

  // retourne true si l'objet spécifié est de la nourriture pour la minor
  // ATTENTION : IL EST INTERDIT DE DONNER A MANGER AUX ANIMAUX
  bool is_food(const aiObject& ob) const ;

  // retourne l'idée que se fait la fourmi de l'emplacement de la fourmilière
  vect2f hypothethic_nest_direction() const ;

  // y a-t-il de la nourriture sur le secteur spécifié
  bool there_is_food_on_sector(aiSector&) const ;

  // y a-t-il de la nourriture dans les alentours
  bool there_is_food_within_range() ;

  // retourne la tendance d'un token donnée à portée d'antennes
  vect2f token_tendency(const token_t t) ;

  // retourne la tendance d'un token de type donné aux coordonnées de la fourmi
  // qui forment un angle aigu avec la direction donnée
  vect2f token_tendency_in_that_way(const token_t, const vect2f&) ;

  // retourne le barycentre des valeurs nutritives de la nourriture à portée d'antennes
  vect2f food_tendency_within_range_in_the_way() const ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

  // détruire la minor
  ~aiMinor() ;
} ;

// récolteuse
class aiHarvester : public aiMinor
{
  aiHarvester(const aiHarvester&) ; // copie interdite !
  aiHarvester& operator=(const aiHarvester&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const ;

protected:

  // vivre
  void live() ;

  // retourne un pointeur sur la nourriture la plus nutritive du secteur transportable par la fourmi
  //aiFood* most_valuable_carriable_found_food_on_sector(aiSector*) const ;

  // retourne un pointeur sur la nourriture la plus nutritive à portée transportable par la fourmi
  //aiFood* most_valuable_carriable_found_food_within_range() const ;

  // quand rien a faire de particulier
  void do_idle() ;

  // errer autour de la fourmilière pour trouver des phéromones "found_food"
  void do_wander_around_nest_waiting_for_found_food_pheromone() ;

  // suivre les phéromones "found_food" pour trouver de la nourriture
  void do_follow_found_food_pheromone_to_find_food() ;

  // revenir à la fourmilière en prenant la nourriture rencontrée
  void do_go_back_to_the_nest_looking_for_food() ;

  // ramener la nourriture à la fourmilière
  void do_bring_found_food_to_the_nest() ;

  // chercher à entrer dans la fourmilière afin de déposer la nourriture portée
  void do_enter_the_nest_to_store_food() ;

  // mettre la nourriture dans le grenier
  void do_put_food_in_the_granary() ;

  // sortir de la fourmilière
  void do_get_out_of_the_nest() ;

public:

  // construit une récolteuse
  // (monde, coordonnées, hp)
  aiHarvester(aiWorld&, const vect2f&, uint) ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

  // détruit la récolteuse
  ~aiHarvester() ;
} ;

// éclaireuse
class aiScout : public aiMinor
{
  aiScout(const aiScout&) ; // copie interdite !
  aiScout& operator=(const aiScout&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const ;

protected:

  // vivre
  void live() ;

  // quand rien a faire de particulier
  void do_idle() ;

  // rechercher une nouvelle source de nourriture
  void do_look_for_a_new_source_of_food() ;

  // retourner à la fourmilière avec l'info sur la source de nourriture
  void do_return_to_nest_with_info_about_food() ;

public:

  // construit une éclaireuse
  // (monde, coordonnées, hp)
  aiScout(aiWorld&, const vect2f&, uint) ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

  // détruit l'éclaireuse
  ~aiScout() ;
} ;


//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_ants_impl.h"

#endif /* __AI_ANTS_H__ */
