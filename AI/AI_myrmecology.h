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
 #ifndef __AI_MYRMECOLOGY_H__
#define __AI_MYRMECOLOGY_H__

// dommages causés par la faim à l'organisme par instant de vie
// (en points de vie : hp)
#define STARVING_DAMAGE                                        (1u)

// portée des antennes des fourmis en ce qui concerne la détection des
// phéromones autour d'elles (en secteurs)
#define FEELERS_RANGE                                          (3)

// quantité de token chemin créé par une fourmi en mouvement
#define PATH_DIGGING_QUANTITY                                  (400u)

// seuil de faim à partir duquel une Minor commence à perdre des points de vie
#define MINOR_HUNGRY		                                       (5000u)

// poids de base que peut porter une minor
#define MINOR_BASE_CARRIABLE_WEIGHT                            (500u)

// vitesse de base d'une minor
#define MINOR_BASE_SPEED                                       (0.2f)

// nombre initial de points de vie de la récolteuse
#define HARVESTER_INITIAL_HP                                   (4000)

// influence de l'ancienne direction quand une récolteuse se déplace
#define HARVESTER_OLD_WAY_INFLUENCE                            (0.7f)

// probabilité qu'une récolteuse tourne lorsqu'elle n'a plus de phéromones devant elle
// en mode recherche
#define HARVESTER_TURNING_PROBABILITY                          (0.3f)

// facteur s'appliquant quand une récolteuse tourne parce qu'elle n'a plus de phéromones
// devant elle en mode recherche
#define HARVESTER_TURNING_FACTOR                               (0.2f)

// quantité de token "found_food" sécrétée par une récolteuse
#define HARVESTER_FOUND_FOOD_SECRETED_QUANTITY                 (250u)

// probabilité qu'une récolteuse cherchant des phéromones "found_food"
// (par unité de distance d'éloignement de la fourmilière)
#define HARVESTER_GO_BACK_PROBABILITY                          (0.01)

// nombre initial de points de vie de l'éclaireuse
#define SCOUT_INITIAL_HP                                       (5000)

// influence de l'ancienne direction quand une éclaireuse se déplace
#define SCOUT_OLD_WAY_INFLUENCE                                (0.8f)

// probabilité qu'une éclaireuse tourne lorsqu'elle n'a plus de phéromones devant elle
// en mode recherche
#define SCOUT_TURNING_PROBABILITY                              (0.4f)

// facteur s'appliquant quand une éclaireuse tourne parce qu'elle n'a plus de phéromones
// devant elle en mode recherche
#define SCOUT_TURNING_FACTOR                                   (0.4f)

// quantité de token "found_food" sécrétée par l'éclaireuse
#define SCOUT_FOUND_FOOD_SECRETED_QUANTITY                     (2000u)

// valeur nutritive de base d'une graine
#define SEED_FOOD_VALUE                                        (400u)

// masse de base d'une graine
#define SEED_WEIGHT                                            (450u)

// points de vie initiaux d'une araignée
#define SPIDER_INITIAL_HP                                      (5000u)

// vitesse de base d'une araignée
#define SPIDER_BASE_SPEED                                      (0.2f)

// influence de l'ancienne direction quand une araignée se déplace
#define SPIDER_OLD_WAY_INFLUENCE                               (0.8f)

// temps en frame
#define TIME_TO_GET_OUT                                        (300u)

// stock de nourriture nécessaire dans le grenier pour créer une éclaireuse
#define FOOD_STOCK_NEEDED_TO_GROW_A_SCOUT                      (1000)

// stock de nourriture nécessaire dans le grenier pour créer une récolteuse
#define FOOD_STOCK_NEEDED_TO_GROW_A_HARVESTER                  (700)

// durée du développement des enfants-fourmis
#define DEVELOPMENT_DURATION                                   (1000u)

// nombre initial de points de vie de la fleur rouge
#define REDFLOWER_INITIAL_HP                                   (5000u)

// nombre initial de points de vie du champignon
#define MUSHROOM_INITIAL_HP                                    (5000u)

#define WEED_INITIAL_HP                                        (5000u)

#define RED_FLOWER_MAX_HEIGHT                                  (300.0f)
#define WEED_MAX_HEIGHT                                        (150.0f)
#define MUSHROOM_MAX_HEIGHT                                    (60.0f)

#endif /* __AI_MYRMECOLOGY_H__ */
