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

  // retourne les coordonn�es (x,z) de l'objet
  const vect2f& get_coords2d() const ;

  // a-t-il termin� son action ?
  bool finished() const ;

  // est-il amen� � dispara�tre d�finitivement ?
  bool vanished() const ;

  // est-il actif ?
  bool is_active() const ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const = 0 ;

  // met � jour l'objet au niveau de l'IA
  virtual void ai_update() ;

  // effectue la mise � jour graphique
  virtual void graphic_update() ;

  // effectue le rendu graphique
  void render() ;

  // d�truit l'objet
  virtual ~aiObject() ;

  // effectue le rendu dans le menu quand l'objet est highlight�
  virtual void render_in_menu();

protected:

  // construit un objet
  // (monde, coordonn�es)
  aiObject(class aiWorld&, const vect2f&) ;

  // monde auquel il appartient
  class aiWorld& world_ ;

  // secteur sur lequel l'objet est actuellement
  class aiSector* sector_ ;

  // demande de faire dispara�tre l'objet
  void vanish() ;

  bool vanished_ ; // si 'true', il est amen� � dispara�tre d�finitivement

  bool active_ ; // actif ? (si oui, il sera rendu entre autres)

  // num�ro de l'action en cours
  action_type action_ ;

  // nombre d'instants de vie restants avant la fin de l'action en cours
  uint action_timer_ ;

  // maintenant faire l'action n� tant pendant tant d'instants de vie
  // 0 signifie "� dur�e ind�termin�e"
  void now_do(const action_type, const uint = 0) ;

  // retourne les coordonn�es de l'objet
  vect2f& coords2d() const ;

  // t�l�porte l'objet aux coordonn�es sp�cifi�es
  // cela para�t un peu �sot�rique, mais c'est la fonction � utiliser pour le d�placer
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

  // perdre la quantit� d'hp sp�cifi�e
  void lose_hp(uint) ;


public:

  // construit un �tre vivant
  // (monde, coordonn�es, hp)
  aiLivingBeing(aiWorld&, const vect2f& c, const uint) ;

  //vivre
  virtual void live();

  // mourir
  virtual void die() ;

  // en vie ?
  bool is_alive() const ;

  // mort ?
  bool is_dead() const ;

  // retourne les points de vie de l'�tre vivant
  uint get_hp() const ;

  // met a jour l'�tre vivant
  void ai_update() ;

  // effectue la mise � jour graphique
  virtual void graphic_update() ;

  // effectue le rendu dans le menu quand l'objet est highlight�
  virtual void render_in_menu();

} ;

//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_objects_impl.h"

#endif /* __AI_OBJECTS_H__ */
