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
 #ifndef __AI_MYRMECOLOGY_H__
#define __AI_MYRMECOLOGY_H__

// dommages caus�s par la faim � l'organisme par instant de vie
// (en points de vie : hp)
#define STARVING_DAMAGE                                        (1u)

// port�e des antennes des fourmis en ce qui concerne la d�tection des
// ph�romones autour d'elles (en secteurs)
#define FEELERS_RANGE                                          (3)

// quantit� de token chemin cr�� par une fourmi en mouvement
#define PATH_DIGGING_QUANTITY                                  (400u)

// seuil de faim � partir duquel une Minor commence � perdre des points de vie
#define MINOR_HUNGRY		                                       (5000u)

// poids de base que peut porter une minor
#define MINOR_BASE_CARRIABLE_WEIGHT                            (500u)

// vitesse de base d'une minor
#define MINOR_BASE_SPEED                                       (0.2f)

// nombre initial de points de vie de la r�colteuse
#define HARVESTER_INITIAL_HP                                   (4000)

// influence de l'ancienne direction quand une r�colteuse se d�place
#define HARVESTER_OLD_WAY_INFLUENCE                            (0.7f)

// probabilit� qu'une r�colteuse tourne lorsqu'elle n'a plus de ph�romones devant elle
// en mode recherche
#define HARVESTER_TURNING_PROBABILITY                          (0.3f)

// facteur s'appliquant quand une r�colteuse tourne parce qu'elle n'a plus de ph�romones
// devant elle en mode recherche
#define HARVESTER_TURNING_FACTOR                               (0.2f)

// quantit� de token "found_food" s�cr�t�e par une r�colteuse
#define HARVESTER_FOUND_FOOD_SECRETED_QUANTITY                 (250u)

// probabilit� qu'une r�colteuse cherchant des ph�romones "found_food"
// (par unit� de distance d'�loignement de la fourmili�re)
#define HARVESTER_GO_BACK_PROBABILITY                          (0.01)

// nombre initial de points de vie de l'�claireuse
#define SCOUT_INITIAL_HP                                       (5000)

// influence de l'ancienne direction quand une �claireuse se d�place
#define SCOUT_OLD_WAY_INFLUENCE                                (0.8f)

// probabilit� qu'une �claireuse tourne lorsqu'elle n'a plus de ph�romones devant elle
// en mode recherche
#define SCOUT_TURNING_PROBABILITY                              (0.4f)

// facteur s'appliquant quand une �claireuse tourne parce qu'elle n'a plus de ph�romones
// devant elle en mode recherche
#define SCOUT_TURNING_FACTOR                                   (0.4f)

// quantit� de token "found_food" s�cr�t�e par l'�claireuse
#define SCOUT_FOUND_FOOD_SECRETED_QUANTITY                     (2000u)

// valeur nutritive de base d'une graine
#define SEED_FOOD_VALUE                                        (400u)

// masse de base d'une graine
#define SEED_WEIGHT                                            (450u)

// points de vie initiaux d'une araign�e
#define SPIDER_INITIAL_HP                                      (5000u)

// vitesse de base d'une araign�e
#define SPIDER_BASE_SPEED                                      (0.2f)

// influence de l'ancienne direction quand une araign�e se d�place
#define SPIDER_OLD_WAY_INFLUENCE                               (0.8f)

// temps en frame
#define TIME_TO_GET_OUT                                        (300u)

// stock de nourriture n�cessaire dans le grenier pour cr�er une �claireuse
#define FOOD_STOCK_NEEDED_TO_GROW_A_SCOUT                      (1000)

// stock de nourriture n�cessaire dans le grenier pour cr�er une r�colteuse
#define FOOD_STOCK_NEEDED_TO_GROW_A_HARVESTER                  (700)

// dur�e du d�veloppement des enfants-fourmis
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
