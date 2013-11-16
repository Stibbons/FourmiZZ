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
#ifndef __AI_ENEMIES_H__
#define __AI_ENEMIES_H__

#include "AI_objects.h"

// araign�e (en cours d'impl�mentation)
class aiSpider : public aiLivingBeing
{
public:

  // effectue la mise � jour graphique
  virtual void graphic_update() ;

  // construit l'araign�e
  // (monde, coordonn�es, hp)
  aiSpider(aiWorld&, const vect2f&, const uint) ;

protected:

  // identifie un objet
  aiID_t scan_object(const aiObject& o) const ;

  // retourne true si l'objet sp�cifi� est de la nourriture pour l'araign�e
  // ATTENTION : IL EST INTERDIT DE DONNER A MANGER AUX ANIMAUX
  bool is_food(const aiObject& ob) const ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const ;

  // coordonn�es de la destination de l'araign�e
  const vect2f& get_destination() const ;

  // retourne la vitesse l'araign�e
  float get_speed() const ;

  // vivre
  virtual void live() ;
  
  // effectue le rendu dans le menu quand l'objet est highlight�
  virtual void render_in_menu();

  // d�truit l'araign�e
  virtual ~aiSpider() ;

private:

  // destination de l'araign�e
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


//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_enemies_impl.h"

#endif /* __AI_ENEMIES_H__ */
