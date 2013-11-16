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
#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "lib/TVector.h"
#include "Default_settings.h"

/* Structure des paramètres de la carte */

typedef enum{
	MAP_SIZE,
	MAT_SIZE,
	TRIANGLE_SIZE,
	HILLS_NB,
	FAR_MAX,
	RADIUS_MIN,
	RADIUS_MAX,
	MAP_HEIGHT,
	FLOOR_SIZE
} Settings;
	
struct w_object {
  int nbspiders;
  int nbredflowers;
  int nbgrass;
  int nbmushrooms;
  int nbfoodsources;
  int nb_food_by_source;
  vect2i coord_nest;
  int nest_width;
};


struct Map_Settings {

/* Taille de la carte */
  int Map_Size;
  
/* Taille de la matrice de stockage */
  int Mat_Size;

/* Taille d'un coté de triangle ( puissance de 2 ) */
  int Step_Size;

/* Nombre de collines */
  int Nb_Hills;

/* Eloignement maximum pour le centre d'une bosse */
  int Far_Max;

/* Minimum et maximum du rayon d'une bosse */
  int Max_Radius;
  int Min_Radius;

/* Hauteur maximale de la carte */
  int Map_Max_Height;

/* Taille de la fenetre en mode scrolling */
  vect2f Frame_Size ;

/* Taille du sol en mode BEE */
  int Floor_Size;


/* La carte est elle plate ou non ? */
  unsigned char Flat;

/* Paramètre pour la génération des objets */
  w_object o_cpt;

} ;






extern Map_Settings TempSettings;


#endif
