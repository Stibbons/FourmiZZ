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
#ifndef __AI_OBJECTS_H__
#define __AI_OBJECTS_H__

#include "AI_shared.h"
#include "AI_graphic_objects.h"
#include "AI_indicators.h"
#include "AI_shared.h"

class aiObject
{
public:

  // objet graphique de lien avec le rendu graphique
  aiGraphicObject* g_obj_ ;

  // retourne les coordonnées (x,z) de l'objet
  const vect2f& get_coords2d() const ;

  // a-t-il terminé son action ?
  bool finished() const ;

  // est-il amené à disparaître définitivement ?
  bool vanished() const ;

  // est-il actif ?
  bool is_active() const ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const = 0 ;

  // met à jour l'objet au niveau de l'IA
  virtual void ai_update() ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // effectue le rendu graphique
  void render() ;

  // détruit l'objet
  virtual ~aiObject() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

protected:

  // construit un objet
  // (monde, coordonnées)
  aiObject(class aiWorld&, const vect2f&) ;

  // monde auquel il appartient
  class aiWorld& world_ ;

  // secteur sur lequel l'objet est actuellement
  class aiSector* sector_ ;

  // demande de faire disparaître l'objet
  void vanish() ;

  bool vanished_ ; // si 'true', il est amené à disparaître définitivement

  bool active_ ; // actif ? (si oui, il sera rendu entre autres)

  // numéro de l'action en cours
  action_type action_ ;

  // nombre d'instants de vie restants avant la fin de l'action en cours
  uint action_timer_ ;

  // maintenant faire l'action n° tant pendant tant d'instants de vie
  // 0 signifie "à durée indéterminée"
  void now_do(const action_type, const uint = 0) ;

  // retourne les coordonnées de l'objet
  vect2f& coords2d() const ;

  // téléporte l'objet aux coordonnées spécifiées
  // cela paraît un peu ésotérique, mais c'est la fonction à utiliser pour le déplacer
  void teleport_to(const vect2f&) ;

private:

  friend class aiWorld ;
  friend class aiNest ;

  aiObject(const aiObject&) ; // copie interdite !
  aiObject& operator=(const aiObject&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const = 0 ;

  friend class aiMinor ;  // pour scan_object
  friend class aiSpider ; // pour scan_object
} ;


class aiLivingBeing : public aiObject {
  friend class aiWorld ;

  aiLivingBeing(const aiObject&) ; // copie interdite !
  aiLivingBeing& operator=(const aiObject&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const = 0 ;

  uint hp_ ; // nombre de points de vie

  aiCounter counter_ ;

protected:

  // perdre la quantité d'hp spécifiée
  void lose_hp(uint) ;


public:

  // construit un être vivant
  // (monde, coordonnées, hp)
  aiLivingBeing(aiWorld&, const vect2f& c, const uint) ;

  //vivre
  virtual void live();

  // mourir
  virtual void die() ;

  // en vie ?
  bool is_alive() const ;

  // mort ?
  bool is_dead() const ;

  // retourne les points de vie de l'être vivant
  uint get_hp() const ;

  // met a jour l'être vivant
  void ai_update() ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

} ;

//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_objects_impl.h"

#endif /* __AI_OBJECTS_H__ */
