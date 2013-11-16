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
#ifndef __AI_WORLD_H__
#define __AI_WORLD_H__

#include "AI_shared.h"
#include "AI_ring.h"
#include "AI_tokens.h"
#include "AI_indicators.h"
#include "../CMap.h"

// construit un monde � partir d'un pointeur de monde,d'un fichier .wld et de Map_Settings
void init_world(class aiWorld*&, const std::string, Map_Settings&) ;

// d�truit un monde
void done_world(class aiWorld*&) ;

// reconstruit un monde d�j� existant � partir d'un pointeur de monde et d'un fichier .wld
void reset_world(class aiWorld*&, const std::string, Map_Settings&) ;

class aiSector
{

public:
  // liste de tous les objets sur le secteur
  aiRing<class aiObject> stuff_ ;
  // tableau contenant les barycentres de toutes les ph�romones
  // d'un m�me type d�pos�es sur le secteur
  aiToken tokens_[TOKEN_T_NB] ;

  //
  const aiToken& get_token(token_t) const ;

  // ajoute un token (pond�r�) au secteur
  void gain_token(const token_t, const uint, const vect2f&) ;

  // "date" de la derni�re mise � jour du secteur
  ulong last_time_updated_ ;

  // constructeur de secteur
  aiSector() ;

  // lie au secteur l'objet pass� en param�tre
  // Attention : l'objet ne doit pas d�j� �tre sur le secteur
  void get(class aiObject&) ;

  // retire le lien � l'objet pass� en param�tre
  // Attention : si l'objet n'est pas sur le secteur : BOUCLE INFINIE
  void lose(class aiObject&) ;

  // nombre d'objets sur le secteur
  uint get_number_of_stuff() const ;

  // retire l'objet courant de la liste
  void remove_stuff() ;

  // retourne l'objet courant
  aiObject& current_stuff() const ;

  // progresse dans la liste d'objets
  void next_stuff() ;

  // d�truit le secteur
  ~aiSector() ;
} ;

// �l�ment graphique de monde
class aiGraphicWorld
{
  aiWorld& world_ ;
public:
  // construit l'�l�ment graphique
  aiGraphicWorld(aiWorld&) ;

  // retourne l'identifieur de la texture du token
  uint get_token_texture_id() const ;

  // retourne l'indicateur pour le rendu des tokens
  // -1: aucun token, autre: le type de token
  int get_rendered_token() const ;

  // change la couleur RGB d'un type de token sp�cifi�
  void set_token_RGBA_color(const token_t, const colorRGBA&) ;

  // retourne la couleur RGB du token rendu
  const colorRGBA& get_rendered_token_RGBA_color() const ;

  // changer de token rendu
  void change_rendered_token() ;

  // recharger la texture des tokens
  void reload_token_texture() ;

  // effectue le rendu
  void render() ;

  // d�truit l'�l�ment graphique
  ~aiGraphicWorld() ;

 //private: //TEMP

  // objet en surbrillance
  aiObject* highlighted_object_ ;

  // tableau des tableaux de couleurs RGB des tokens
  colorRGBA tokens_RGBA_colors_[TOKEN_T_NB] ;

  // identifieur de la texture du token
  uint token_texture_id_ ;

  // indicateur pour le rendu des tokens
  // -1: aucun token, autre: le type de token
  int rendered_token_ ;

  // effectue le rendu du surlignement de l'objet s�lectionn�
  void render_highlighted_object() ;

  // effectue le rendu graphique des tokens en mode Scrolling
  void render_tokens_in_scroll_view() ;

  // effectue le rendu graphique des tokens en mode Abeille 
  void render_tokens_in_bee_view() ;

  // effectue le rendu de tous les objets visibles sur la minimap
  void render_every_object_on_minimap() ;
} ;


class aiWorld
{
public:
  friend class aiSector ;
  friend class aiObject ;
  friend class aiAnt ;
  friend class aiMinor ;

  friend void init_world(class aiWorld*&, const std::string, Map_Settings&) ;
  friend void done_world(class aiWorld*&) ;
  friend void reset_world(class aiWorld*&, const std::string, Map_Settings&) ;

  aiWorld(const aiWorld&) ; // copie interdite
  aiWorld& operator=(const aiWorld&) ; // affectation interdite !

  // vecteur-nombre de secteurs
  const uint sectors_nb_ ;

  // taille du monde (largeur ou longueur)
  const uint size_ ;

  // la fourmili�re
  class aiNest* nest_ ;

  // la liste des tokens du monde
  aiRing<aiToken> tokens_[TOKEN_T_NB] ;

  // le temps �coul� dans le monde
  aiTime time_ ;

  // vecteur [0..(sectors_nb_-1)] de vecteurs [0..(sectors_nb_-1)].
  aiSector** sectors_ ;

  aiRing<class aiObject> objects_ ; // liste des objets du monde.

  //////////////////////////////////////////////////////////////////////////////////////////////

  // retourne les index du secteur situ� aux coordonn�es sp�cifi�es
  vect2u indexes_of_sector_at(const vect2f&) const ;

  // retourne le secteur qui est situ� aux coordonn�es sp�cifi�es
  aiSector& sector_at(const vect2f&) const ;

  // retourne le secteur d'index sp�cifi�s
  aiSector& sector_at(const vect2u&) const ;

  // retourne le secteur dont les index sont les param�tres.
  // corrige les index au cas o� ils ne seraient pas dans [0..sectors_nb_-1]x[0..sectors_nb_-1]
  aiSector& sector_at(const int, const int) const ;

  //////////////////////////////////////////////////////////////////////////////////////////////

  // ajoute un objet � la liste
  void add_object(aiObject& ) ;

  // retourne l'objet courant
  aiObject& current_object() const ;

  // progresse dans la liste d'objets
  void next_object() ;

  // supprime l'objet courant
  void delete_current_object() ;

  // met � jour tous les objets du monde
  void ai_update_every_object() ;

  // effectue la mise � jour graphique de tous les objets
  void graphic_update_objects() ;

  // effectue le rendu graphique de tous les objets visibles en mode Abeille
  void render_visible_objects_in_bee_view() ;

  // effectue le rendu graphique de tous les objets visibles en mode Scrolling
  void render_visible_objects_in_scroll_view() ;

  // effectue le rendu graphique de tous les objets
  void render_every_object() ;

  //////////////////////////////////////////////////////////////////////////////////////////////

  // retourne le token courant du type donn�
  aiToken& current_token(const token_t) const ;

  // progresse dans la liste de tokens du type donn�
  void next_token(const token_t) ;

  // ajoute un token dans le secteur sp�cifi�
  void gain_token(aiSector&,const token_t, const uint, const vect2f&) ;

  //////////////////////////////////////////////////////////////////////////////////////////////

  // incr�mente la "date" du monde
  void inc_time() ;

/*******************\
* WORLD, public ... *
\*******************/
public:

  // construit un monde
  // (nombre de secteurs)
  aiWorld(const uint) ;

  CMap* map_ ;

  // �l�ment graphique du monde
  aiGraphicWorld* g_world_ ;

  // retourne le vecteur-nombre de secteurs du monde
  const uint get_sectors_nb() const ;

  // retourne la taille du monde (longueur ou largeur)
  uint get_size() const ;

  // retourne la "date" du monde
  const ulong& get_time() const ;

  // met � jour le secteur sp�cifi�.
  aiSector& update_sector(aiSector&) const ;

  // modifie les coordonn�es-param�tres afin qu'elles appartiennent �
  // [0,sectors_nb_*SECTOR_SIZE[x[0,_sectors_nb_*SECTOR_SIZE[
  void fix_coords(vect2f&) const ;

  // transforme le vecteur sp�cifi� pour qu'il devienne coh�rent (le monde est sph�rique)
  vect2f& transform_vector(vect2f&) const ;

  // donne la distance entre deux points (en consid�rant qu'il appartient � un monde sph�rique)
  float distance(const vect2f&, const vect2f&) const ;

  // change l'angle que fait le soleil avec la direction "est" (lever du soleil)
  void change_sun_angle() const ;

  // retourne le nombre d'objets du type donn� dans le monde
  uint get_number_of_tokens(const token_t) const ;

  // retourne le nombre d'objets dans le monde
  uint get_number_of_objects() const ;

  // les coordonn�es sont-elles visibles en mode Scrolling
  bool visible_in_scrolling_view(const vect2f& v) const ;

  // les coordonn�es sont-elles visibles en mode Abeille
  bool visible_in_bee_view(const vect3f& v) const ;

  // met � jour le monde
  void ai_update() ;

  // effectue le rendu graphique du monde
  void render() ;

  // d�truit le monde
  ~aiWorld() ;

/*******************\
* ... public, WORLD *
\*******************/
} ;

//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_world_impl.h"

#endif /* __AI_WORLD_H__ */

