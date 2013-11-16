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
#ifndef __AI_ENVIRONMENT_H__
#define __AI_ENVIRONMENT_H__

#include "AI_objects.h"

class aiRock : public aiObject
{
  friend class aiWorld ;

  aiRock(const aiRock&) ; // copie interdite !
  aiRock& operator=(const aiRock&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const ;

  // rayon du disque de centre (coords2d()) et qui contient le caillou
  const float radius_ ;

public:
/******************\
* ROCK, public ... *
\******************/

  // construit le caillou
  // (monde, coordonnées, rayon)
  aiRock(aiWorld&, const vect2f&, const float) ;

  // rayon du disque de centre (coords2d()) et qui contient le caillou
  const float& get_radius() const ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // détruit le caillou
  virtual ~aiRock() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

/******************\
* ... public, ROCK *
\******************/
} ;

// végétal
class aiVegetable : public aiLivingBeing
{
public:

  // construit l'herbe
  // (monde, coordonnées, hp, taille)
  aiVegetable(aiWorld&, const vect2f&, const uint, const float) ;

  // hauteur du brin d'herbe
  const float& get_height() const ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

  // détruit le caillou
  virtual ~aiVegetable() ;

private:

  friend class aiWorld ;

  aiVegetable(const aiVegetable&) ; // copie interdite !
  aiVegetable& operator=(const aiVegetable&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const = 0 ;

  // rayon du disque de centre (coords2d()) et qui contient le caillou
  const float height_ ;
} ;

// fleur rouge
class aiRedFlower : public aiVegetable
{
public:

  // construit la fleur rouge
  // (monde, coordonnées, taille)
  aiRedFlower(aiWorld&, const vect2f&, const float) ;

private:

  // identificateur
  virtual aiID_t get_id() const ;
} ;

// champignon
class aiMushroom : public aiVegetable
{
public:

  // construit le champignon
  // (monde, coordonnées, hp, taille)
  aiMushroom(aiWorld&, const vect2f&, const float) ;
 
private:

  // identificateur
  virtual aiID_t get_id() const ;
} ;


// brin d'herbe
class aiWeed : public aiVegetable
{
public:

  // construit le champignon
  // (monde, coordonnées, hp, taille)
  aiWeed(aiWorld&, const vect2f&, const float) ;
 
private:

  // identificateur
  virtual aiID_t get_id() const ;
} ;


// nourriture
class aiFood : public aiObject
{
  friend class aiWorld ;

  aiFood(const aiFood&) ; // copie interdite !
  aiFood& operator=(const aiFood&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const = 0 ;

  // valeur énergétique
  const uint value_ ;

  // poids
  const uint weight_ ;

public:
/******************\
* FOOD, public ... *
\******************/

  // construit la nourriture
  // (monde, coordonnées, valeur énergétique, masse)
  aiFood(aiWorld&, const vect2f&, const uint, const uint) ;

  // retourne la valeur énergétique
  uint get_value() const ;

  // retourne la masse
  uint get_weight() const ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // détruit le caillou
  virtual ~aiFood() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

/******************\
* ... public, FOOD *
\******************/
} ;


class aiSeed : public aiFood
{
  aiSeed(const aiSeed&) ; // copie interdite !
  aiSeed& operator=(const aiSeed&) ; // affectation interdite !

  // identificateur
  virtual aiID_t get_id() const ;

public:

/******************\
* SEED, public ... *
\******************/

  // construit une graine
  // (monde, coordonnées)
  aiSeed(aiWorld&, const vect2f&) ;

  // retourne la couleur dans la minimap
  virtual colorRGBA color_on_minimap() const ;

  // effectue la mise à jour graphique
  virtual void graphic_update() ;

  // détruit le caillou
  virtual ~aiSeed() ;

  // effectue le rendu dans le menu quand l'objet est highlighté
  virtual void render_in_menu();

/******************\
* ... public, SEED *
\******************/
} ;


//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_environment_impl.h"


#endif /* __AI_ENVIRONMENT_H__ */
