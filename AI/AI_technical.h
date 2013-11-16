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
#ifndef __AI_TECHNICAL_H__
#define __AI_TECHNICAL_H__

#include "AI_shared.h"

// types d'objets qu'une Minor ou une Spider peut identifier
enum aiID_t {
  HARVESTER, SCOUT, ROCK, SEED, MAJOR, SPIDER, WEED, REDFLOWER, MUSHROOM
} ;

// toutes les actions que peuvent accomplir les aiObjects
enum action_type
{
  IDLE,
  WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE,
  FOLLOW_FOUND_FOOD_PHEROMONE_TO_FIND_FOOD,
  GO_BACK_TO_THE_NEST_LOOKING_FOR_FOOD,
  BRING_FOOD_TO_THE_NEST,
  ENTER_THE_NEST_TO_STORE_FOOD,
  PUT_FOOD_IN_THE_GRANARY,
  LOOK_FOR_A_NEW_SOURCE_OF_FOOD,
  RETURN_TO_NEST_WITH_INFO_ABOUT_FOOD,
  GET_OUT_OF_THE_NEST,
  HUNT_ANTS
} ;

#define ACTION_TYPE_NB            (11)

// largeur d'un secteur
// devrait être une puissance de 2 (performances)
#define SECTOR_STEP               (8)
 
// nombre de secteurs selon x et z par défaut
#define DEFAULT_SECTORS_NB        (64)

#define DEFAULT_INITIAL_HOURS     (6)

#define DEFAULT_INITIAL_MINUTES   (30)

// durée d'une minute dans le simulateur en nombre de frames
#define VIRTUAL_MIN               (10)

// durée d'une heure dans le simulateur en nombre de frames
#define VIRTUAL_HOUR              (VIRTUAL_MIN * 60)

// durée du jour dans le simulateur en nombre de frames
#define VIRTUAL_DAY               (VIRTUAL_HOUR * 24)

// donne la fréquence d'utilisation de l'IA en frames (8 par défaut)
#define IA_FREQUENCY              (8)

// couleur dans la minimap                R     G     B     A
#define MINIMAP_COLOR_HARVESTER           1.0f, 1.0f, 0.0f, 1.0f
#define MINIMAP_COLOR_SCOUT               0.0f, 1.0f, 1.0f, 1.0f
#define MINIMAP_COLOR_SEED                1.0f, 1.0f, 1.0f, 0.3f
#define MINIMAP_COLOR_SPIDER              1.0f, 0.0f, 0.0f, 1.0f
#define MINIMAP_COLOR_VEGETABLE           0.0f, 1.0f, 0.0f, 0.3f

#define DEFAULT_FOUND_FOOD_TOKEN_COLOR    1.0f, 0.0f, 0.0f, 1.0f
#define DEFAULT_PATH_TOKEN_COLOR          0.0f, 0.0f, 0.1f, 1.0f

#define TOKEN_SIZE 10

#define NEST_ENTRANCE_RADIUS      (10u)

#define STORING_FOOD_TIME         (300u)

#endif /* __AI_TECHNICAL_H__ */
