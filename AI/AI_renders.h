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
#ifndef __AI_RENDERS_H__
#define __AI_RENDERS_H__

#include "AI_world.h"
#include "AI_objects.h"
#include "AI_ants.h"
#include "AI_environment.h"
#include "AI_graphic_objects.h"
#include "AI_tokens.h"
#include "AI_enemies.h"
#include "AI_nest.h"
#include "../CMap.h"
#include "AI_shared.h"


#define DIST_LOWRES_OBJECT                250u

struct AnimsLink {
  uint current_anim_id_ ;
  uint transition_anim_id_ ;
  uint next_anim_id_ ;
  AnimsLink() ;
  AnimsLink(const uint, const uint, const uint) ;
} ;

struct AnimsCell {
  action_type next_action_ ;

  AnimsLink val_ ;

  AnimsCell* next_ ;

  AnimsCell(const action_type, const AnimsLink&, AnimsCell*) ;
};


extern AnimsCell* anims_links_[ACTION_TYPE_NB];

// ajoute un lien d'animation dans le tableau
void add_anims_link(const action_type, const action_type, const uint, const uint, const uint) ;

// ajoute un lien d'animation dans le tableau sans transition prévue
void add_anims_link(const action_type, const action_type, const uint, const uint) ;

// remplit le tableau anims_links_ de liens entre les états des objets et les animations
void create_animations_links() ;

// supprime les liens d'animation entre les états des objets et les animations contenus 
// dans le tableau anims_links_
void delete_animations_links() ;


//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_renders_impl.h"


#endif /* __AI_RENDERS_H__ */
