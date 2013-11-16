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
#ifndef __FOURMIS_OBJECTS_H__HEADER__
#define __FOURMIS_OBJECTS_H__HEADER__

#include "lib/CCamera.h"
#include "CMap.h"
#include "lib/font.h"
#include "Menu.h"
#include "lib/CSDLChrono.h"
#include "lib/CSoundManager.h"

/* définition de toutes les animations: 
 * d'abord les identificateurs uniques */
#define ANIMATION_DEFAULT_UID               200
#define ANIMATION_STAY_UID                  200
#define ANIMATION_WALK_UID                  201
#define ANIMATION_HEADLEFTTURN_UID          202
#define ANIMATION_HEADLEFT_UID              203
#define ANIMATION_HEADLEFTRETURN_UID        204
#define ANIMATION_WALKWITHSEED_UID          205
#define ANIMATION_WALKTOWALKWITHSEED_UID    206
#define ANIMATION_WALKWITHSEEDTOWALK_UID    207
#define ANIMATION_WALKWITHSEEDTOSTAY_UID    208
#define ANIMATION_STAYWITHSEED_UID          209
#define ANIMATION_ATTACK_UID                220

#define ANIMATION_SHOOT_UID                  200

/* puis les noms (utilisé dans les fichiers .fm1 pour identifier rapidement
 * quelle animation on écrit)*/
#define ANIMATION_DEFAULT_STRING                "default"
#define ANIMATION_STAY_STRING                   "stay"
#define ANIMATION_WALK_STRING                   "walk"
#define ANIMATION_HEADLEFTTURN_STRING           "HeadLeftTurn"
#define ANIMATION_HEADLEFTRETURN_STRING         "HeadLeftReTurn"
#define ANIMATION_WALKWITHSEED_STRING           "WalkWithSEED"
#define ANIMATION_WALKTOWALKWITHSEED_STRING     "WalkToWalkWithSEED"
#define ANIMATION_WALKWITHSEEDTOWALK_STRING     "WalkWithSEEDToWalk"
#define ANIMATION_WALKWITHSEEDTOSTAY_STRING     "WalkWithSEEDToWalk"
#define ANIMATION_STAYWITHSEED_STRING           "StayWithSEED"
#define ANIMATION_ATTACK_STRING                 "Attack"

#define ANIMATION_SHOOT_STRING                  "stay"

#define OBJECT_SEED_STRING                      "SEED"
#define OBJECT_SEED_UID                         0
#define OBJECT_SHOOTPARTICLE_STRING             "Attack"
#define OBJECT_SHOOTPARTICLE_UID                10


#include "lib/Particles.h"
extern CConstantParticleSystem g_ParticleSystem;
extern CSDLChrono g_ParticleChrono;

bool objects_init(Map_Settings mSettings);
bool objects_recreate_displaylists();
void object_delete_displaylists();

#include "lib/C3DObject.h"

extern C3DObjElements *g_ObjsHarvester;
extern C3DObjElements *g_ObjsSeed;
extern C3DObjElements *g_ObjsWeed1;
extern C3DObjElements *g_ObjsWeed2;
extern C3DObjElements *g_ObjsMushroom;
extern C3DObjElements *g_ObjsRedFlower;
extern C3DObjElements *g_ObjsFleurRouge;
extern C3DObjElements *g_ObjsSpider;
extern C3DObjElements *g_ObjsMajor;


//#include "AI/AI_world.h"
extern CCamera cCam;
extern class aiWorld *world;
extern menu * menu_main;
extern menu * menu_map ;
extern Map_Settings TempSettings ;

#include "AI/AI_graphic_objects.h"

#endif //__FOURMIS_OBJECTS_H__HEADER__
