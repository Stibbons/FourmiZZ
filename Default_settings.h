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
#ifndef __DEFAULT_SETTINGS_H__
#define __DEFAULT_SETTINGS_H__

// Taille de chaque triangles
#define DEFAULT_STEP_SIZE 32

// Taille de la matrice
#define DEFAULT_MAT_SIZE 64

//Taille de la map
#define DEFAULT_MAP_SIZE (DEFAULT_STEP_SIZE * DEFAULT_MAT_SIZE)	


// Nombre de bosses
#define DEFAULT_NB_HILLS 50

// Distance maximum a laquelle un centre peut se trouver en dehors de la map 
#define DEFAULT_FAR_MAX 50

//  Le rayon sera tiré entre ces deux constantes
#define DEFAULT_MAX_RADIUS 60
#define DEFAULT_MIN_RADIUS 120

// La carte sera échelonnée entre 0 et MAP_MAX_HEIGHT
#define DEFAULT_MAP_MAX_HEIGHT 50

// Nombre de texture nécessaire pour la map
#define NBTEX_MAP 14

// Taille du cadre d'affichage en SCROLLING MODE
#define FRAME_SIZE_X 480
#define FRAME_SIZE_Y 480



// Les numéros des textures
#define BACK_ID 1
#define FRONT_ID 2
#define TOP_ID 3
#define BOTTOM_ID 4
#define LEFT_ID 5
#define RIGHT_ID 6
#define EARTH_ID 0
#define EARTH1_ID 7
#define EARTH2_ID 8
#define EARTH3_ID 9
#define EARTH4_ID 10
#define EARTH5_ID 11
#define EARTH6_ID 12
#define EARTH7_ID 13

#define EARTH_NB 7

// Les coordonnées pour le SkyBox
#define SKYCENTER_X (DEFAULT_MAP_SIZE / 2)
#define SKYCENTER_Z (DEFAULT_MAP_SIZE / 2)
#define SKYCENTER_Y (DEFAULT_MAP_MAX_HEIGHT*10)
#define SKY_WIDTH ( DEFAULT_MAP_SIZE*3)
#define SKY_HEIGHT (DEFAULT_MAP_MAX_HEIGHT*60)
#define SKY_LARGE (DEFAULT_MAP_SIZE*3)

// La taille du sol
#define DEFAULT_FLOOR_SIZE (-50)

//Le soleil
#define DEFAULT_SUNDIRX 10
#define DEFAULT_SUNDIRY -1
#define DEFAULT_SUNDIRZ 0

#endif
