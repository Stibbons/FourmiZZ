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
#ifndef __AI_ENEMIES_H__
#define __AI_ENEMIES_H__

#include "AI_objects.h"

// araignée (en cours d'implémentation)
class aiSpider : public aiLivingBeing
{
public:

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // construit l'araignée
  // (monde, coordonnées, hp)
  aiSpider(aiWorld&, const vect2f&, const uint) ;

protected:

  // identifie un objet
  aiID_t scan_object(const aiObject& o) const ;

  // retourne true si l'objet spécifié est de la nourriture pour l'araignée
  // ATTENTION : IL EST INTERDIT DE DONNER A MANGER AUX ANIMAUX
  bool is_food(const aiObject& ob) const ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const ;

  // coordonnées de la destination de l'araignée
  const vect2f& get_destination() const ;

  // retourne la vitesse l'araignée
  float get_speed() const ;

  // vivre
  virtual void live() ;
  
  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

  // détruit l'araignée
  virtual ~aiSpider() ;

private:

  // destination de l'araignée
  vect2f destination_ ;

  aiSpider(const aiSpider&) ; // copie interdite !
  aiSpider& operator=(const aiSpider&) ; // affectation interdite !

  // identificateur
  aiID_t get_id() const ;

  // quand rien a faire de particulier
  void do_idle() ;

  // chasser les fourmis
  void do_hunt_ants() ;
} ;


//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_enemies_impl.h"

#endif /* __AI_ENEMIES_H__ */
