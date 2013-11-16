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
#include <stdio.h>
#include "common.h"
#include "lib/CCamera.h"
#include "lib/Particles.h"
#include "AI/AI_world.h"
#include "Objects.h"
#include "lib/C3DObject.h"
#include "lib/tga.h"
#include "CMap.h"
#include "main.h"
#include "Menu.h"
#include "lib/CBubbleText.h"

t3DTextures  g_textures_list;
t3DObjects   g_objects_list;

char g_sztextureparticule[255];

/* et la correspondance entre nom d'animation (écriture rapide des animations)
 * et identificateurs (sélection rapide à l'exécution)*/
ANIMATIONS_NAMEUID AnimNameUID[] =
{
  { ANIMATION_DEFAULT_STRING,         ANIMATION_DEFAULT_UID           },
  { ANIMATION_STAY_STRING,            ANIMATION_STAY_UID              },
  { ANIMATION_SHOOT_STRING,           ANIMATION_SHOOT_UID             },
  { ANIMATION_WALK_STRING,            ANIMATION_WALK_UID              },
  { ANIMATION_HEADLEFTTURN_STRING,    ANIMATION_HEADLEFTTURN_UID      },
  { ANIMATION_HEADLEFTRETURN_STRING,  ANIMATION_HEADLEFTRETURN_UID    },
  { ANIMATION_WALKWITHSEED_STRING,  ANIMATION_WALKWITHSEED_UID    },
  { ANIMATION_WALKTOWALKWITHSEED_STRING,  ANIMATION_WALKTOWALKWITHSEED_UID},
  { ANIMATION_WALKWITHSEEDTOWALK_STRING,  ANIMATION_WALKWITHSEEDTOWALK_UID},
  { NULL,                             NULL                            }
};



/* maintenant on regarde ce qui concerne les objets que l'on peut prendre (par
 * une graine par une fourmi) */
OBJECTS_NAMEUID ObjNameUID[] =
{
  { OBJECT_SEED_STRING,          OBJECT_SEED_UID             },
  { OBJECT_SHOOTPARTICLE_STRING, OBJECT_SHOOTPARTICLE_UID    },
  { NULL,                        NULL                        }
};


/*
C3DObjElement *g_ObjFourmi1=NULL;
C3DObjElement *g_ObjFourmi2=NULL;
C3DObjElement *g_ObjFourmi3=NULL;
C3DObjElement *g_ObjFourmi4=NULL;
C3DObjElement *g_ObjFourmi5=NULL;
C3DObjElement *g_ObjFourmi6=NULL;
C3DObjElement *g_ObjFourmi7=NULL;
C3DObjElement *g_ObjFourmi8=NULL;
C3DObjElement *g_ObjFourmi9=NULL;
C3DObjElement *g_ObjFourmi10=NULL;
C3DObjElement *g_ObjFourmi11=NULL;
C3DObjElement *g_ObjFourmi12=NULL;
C3DObjElement *g_ObjFourmi13=NULL;
C3DObjElement *g_ObjFourmi14=NULL;
C3DObjElement *g_ObjFourmi15=NULL;
*/


C3DObjElements *g_ObjsSeed     =NULL;
C3DObjElements *g_ObjsHarvester=NULL;
C3DObjElements *g_ObjsWeed1=NULL;
C3DObjElements *g_ObjsWeed2=NULL;
C3DObjElements *g_ObjsMushroom=NULL;
C3DObjElements *g_ObjsRedFlower=NULL;
C3DObjElements *g_ObjsFleurRouge=NULL;
C3DObjElements *g_ObjsSpider=NULL;
C3DObjElements *g_ObjsMajor=NULL;


CConstantParticleSystem g_ParticleSystem;


CCamera cCam(350,150,350,
             260,40,280,
             0,1,0,
             0,0,0);

aiWorld *world = 0 ;
menu * menu_main=0;
menu * menu_map =0;
Map_Settings TempSettings (MenuLoadOptions());

Mode_GL Main_Mode = MENU_MODE;

int Init = 0;

//void init();
//unsigned int Menu_Tex[7];

tLightMaterial g_matDefault (0.8f, 0.8f, 0.7f,  1.0f,
                           0.5f, 0.5f, 0.5f,  1.0f,
                           1.0f, 1.0f, 1.0f,  1.0f);
tLightMaterial g_matSeed (0.8f, 0.0f, 0.0f,  1.0f,
                           0.9f, 0.0f, 0.0f,  1.0f,
                           1.0f, 1.0f, 1.0f,  1.0f);

void object_delete_displaylists()
{
  g_textures_list.clear();
         
  delete menu_main;
  delete menu_map;

  if (world)
  {
    world->map_->DeleteTextures();
    world->map_->mmap->DeleteTextures();
    delete world->map_->menu_down;
    glDeleteTextures(1,&(world->g_world_->token_texture_id_));
    g_ParticleSystem.DeleteTextures();
  }
  fontKill();
}

bool objects_recreate_displaylists()
{
  menu_main= new menu(TempSettings,"Menu.ini");
  menu_map = new menu(TempSettings,"Mapmenu.ini");
  g_textures_list.m_textures_number=0;
  // TODO : mettre tous les types d'objets ici pour recharger leur displaylist


  // ResetTextures reset les textures des particules.
  GLuint texuid;
  glGenTextures(1, &texuid);	// Create The Texture
  tgaLoad(g_sztextureparticule,texuid);
  g_ParticleSystem.ResetTextures(texuid);

  // et maintenant c'est le tour de la police.
  uint tex,mask;
	glGenTextures(1, &tex);
  tgaLoad("textures"PATHDELIM_S"font.tga",tex);
	glGenTextures(1, &mask);
  tgaLoad("textures"PATHDELIM_S"fontmask.tga",mask);
  fontBuild(tex, mask);
  fontResize(TAILLEX, TAILLEY);

  // TODO: reset des textures des phéromones

  if (world)
  {
    glGenTextures(1,&(world->g_world_->token_texture_id_));
    tgaLoad("textures"PATHDELIM_S"token.tga",world->g_world_->token_texture_id_);
    world->map_->Switch_to_fullscreen();

    if (g_ObjsHarvester)
    {
      g_ObjsHarvester->m_hiRes->RebuilDisplayLists();
  
    // c'est magique, il suffit pour les fourmis texturées de ne le faire qu'une
    // fois (la texture va être en fait simplement recréé avec le même ID 
    // qu'avant, donc les autres n'y voient que du feu
      g_ObjsHarvester->m_hiRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, &g_matDefault);
    // l'objet étant déjà en mémoire (3DS), il ne sera pas recharger sur le 
    // disque dur, donc pas la peine de donner le dossier où sont stocker les
    // fichiers
    }

    if (g_ObjsHarvester->m_lowRes)
    {
      g_ObjsHarvester->m_lowRes->RebuilDisplayLists();
      g_ObjsHarvester->m_lowRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, &g_matDefault);
    }
    if (g_ObjsSeed)
    {
      g_ObjsSeed->m_hiRes->RebuilDisplayLists();
      g_ObjsSeed->m_hiRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, NULL);
    }
    if (g_ObjsWeed1)
    {
      g_ObjsWeed1->m_hiRes->RebuilDisplayLists();
      g_ObjsWeed1->m_hiRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, NULL);
    }
    if (g_ObjsWeed2)
    {
      g_ObjsWeed2->m_hiRes->RebuilDisplayLists();
      g_ObjsWeed2->m_hiRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, NULL);
    }
    if (g_ObjsMushroom)
    {
      g_ObjsMushroom->m_hiRes->RebuilDisplayLists();
      g_ObjsMushroom->m_hiRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, NULL);
    }
    if (g_ObjsRedFlower)
    {
      g_ObjsRedFlower->m_hiRes->RebuilDisplayLists();
      g_ObjsRedFlower->m_hiRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, NULL);
    }
    if (g_ObjsSpider)
    {
      g_ObjsSpider->m_hiRes->RebuilDisplayLists();
      g_ObjsSpider->m_hiRes->Load(NULL, NULL, true,&g_textures_list, &g_objects_list, NULL);
    }

  }

  return true;
}


void update_particle_callback(CParticle *part, float time)
{
  CVector3f grav = part->GetGravity();
/*  if (pos.y < monde.GetMapHeight(pos.x, pos.z))
  {
  
    pos.y = monde.GetMapHeight(pos.x, pos.z);

    pos = part->GetSpeed();
    pos.y = -pos.y;
    part->SetSpeed(pos);
//    part->SetPosition(pos);
//    CVector3d speed (0,0,0);
//    part->SetSpeed(speed);
//    part->SetGravity(speed);
  }
  else
  {*/
    grav.x += (rand() % 100 - 50) / 30;
    grav.y += (rand() % 100 - 50) / 30;
    grav.z += (rand() % 100 - 50) / 30;
    //part->SetAlpha(part->GetFade());
    part->SetGravity(grav);
  //}
}

void create_particle_callback(CParticle *part, float time, unsigned int num)
{
}



bool objects_init(Map_Settings mSettings)
{

//  Load_Menu_Textures();
	SDL_ShowCursor(false);

  char szfm1path[256];
  char szobjpath[256];
  char sztexpath[256];
  
  /* chemins vers les objets 3D */
  strcpy(szobjpath, g_exepath);
  strcat(szobjpath, "objects"PATHDELIM_S);
  /* chemins vers les textures */
  strcpy(sztexpath, g_exepath);
  strcat(sztexpath, "textures"PATHDELIM_S);



  /* chargement d'un objet Graine */
  g_ObjsSeed = new C3DObjElements ;

  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"seed.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsSeed->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsSeed->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, &g_matDefault);
  g_ObjsSeed->m_lowRes = 0 ;



  /* chargement d'un objet Fourmi Récolteuse générique */
  g_ObjsHarvester = new C3DObjElements;
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"minor.fm1");
  if (!C3DObjLoader::Load3d(szfm1patPATHDELIMh,&g_ObjsHarvester->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsHarvester->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, &g_matSeed);

    /* chargement d'un objet Fourmi Récolteuse générique */
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"minor-lowres.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsHarvester->m_lowRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsHarvester->m_lowRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, NULL);

  
    /* chargement d'un objet Herbe Type 1 */
  g_ObjsWeed1 = new C3DObjElements;
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"weed1.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsWeed1->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsWeed1->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, NULL);
  g_ObjsWeed1->m_lowRes = NULL;

      /* chargement d'un objet Herbe Type 2 */
  g_ObjsWeed2 = new C3DObjElements;
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"weed2.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsWeed2->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsWeed2->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, NULL);
  g_ObjsWeed2->m_lowRes = NULL;

      /* chargement d'un objet Champignon*/
  g_ObjsMushroom = new C3DObjElements;
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"mushroom.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsMushroom->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsMushroom->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, NULL);
  g_ObjsMushroom->m_lowRes = NULL;

      /* chargement d'un objet Fleur Rouge*/
  g_ObjsRedFlower = new C3DObjElements;
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"redflower.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsRedFlower->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsRedFlower->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, NULL);
  g_ObjsRedFlower->m_lowRes = NULL;

      /* chargement d'un objet Araignée*/
  g_ObjsSpider = new C3DObjElements;
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"spider.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsSpider->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsSpider->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, NULL);
  g_ObjsSpider->m_lowRes = NULL;

      /* chargement d'un objet Major*/
  g_ObjsMajor = new C3DObjElements;
  strcpy(szfm1path, g_exepath);
  strcat(szfm1path, "objects"PATHDELIM_S"major.fm1");
  if (!C3DObjLoader::Load3d(szfm1path,&g_ObjsMajor->m_hiRes, AnimNameUID, ObjNameUID))
    return false;
  g_ObjsMajor->m_hiRes->Load(szobjpath, sztexpath, true, &g_textures_list, &g_objects_list, NULL);
  g_ObjsMajor->m_lowRes = NULL;
  //g_ObjsMajor->m_hiRes->SelectDefaultAnimation(ANIMATION_WALK_UID);
  //g_ObjsMajor->m_hiRes->ChangeFrameRateRatio(4);
/*  CVector3f grav (0,-0.8f,0);
  CVector3f randplan  (0.0f, 0.0f, 0.0f);
  CVector3f randspeed (0.3f, 0.8f, 0.4f);*/
/*g_ObjsMajor->m_hiRes->InitParticleSystem(OBJECT_SHOOTPARTICLE_UID, 
                               20.0f, 20, 5.0f, 1.0f,
                               0.3f, 1.0f, 0.3f, 0.8f,
                               false,0, 0.5f,0.5f, grav, 
                               randplan, randspeed, NULL,NULL);
  g_ObjsMajor->m_hiRes->Reset();*/
//g_ObjsMajor->m_hiRes->StartParticleSystem();



  init_world(world,"AI_test.wld", mSettings);

//  minor.load3d();
 
  /* initialisation des objets par copie */
  
/*
  g_ObjGraine = new C3DObjElement;
  *(g_ObjGraine) = *(ObjGraine);
  g_ObjGraine->ChangeFrameRateRatio(10.0);
  


  g_ObjFourmi1 = new C3DObjElement;
  *(g_ObjFourmi1) = *(ObjOuvriere);
  g_ObjFourmi1->ChangeFrameRateRatio(40.0);
  g_ObjFourmi1->SelectDefaultAnimation(ANIMATION_STAY_UID);
  g_ObjFourmi1->RandomAnimationStart(ANIMATION_STAY_UID);

  
  g_ObjFourmi2 = new C3DObjElement;
  *(g_ObjFourmi2) = *(ObjOuvriere);
  g_ObjFourmi2->ChangeFrameRateRatio(10.0);
  g_ObjFourmi2->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi2->RandomAnimationStart(ANIMATION_WALK_UID);

  
  g_ObjFourmi3 = new C3DObjElement;
  *(g_ObjFourmi3) = *(ObjOuvriere);
  g_ObjFourmi3->ChangeFrameRateRatio(10.0);
  g_ObjFourmi3->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi3->RandomAnimationStart(ANIMATION_WALK_UID);

  
  g_ObjFourmi4 = new C3DObjElement;
  *(g_ObjFourmi4) = *(ObjOuvriere);
  g_ObjFourmi4->ChangeFrameRateRatio(10.0);
  g_ObjFourmi4->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi4->RandomAnimationStart(ANIMATION_WALK_UID);


  g_ObjFourmi5 = new C3DObjElement;
  *(g_ObjFourmi5) = *(ObjOuvriere);
  g_ObjFourmi5->ChangeFrameRateRatio(10.0);
  g_ObjFourmi5->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi5->RandomAnimationStart(ANIMATION_WALK_UID);


  g_ObjFourmi6 = new C3DObjElement;
  *(g_ObjFourmi6) = *(ObjOuvriere);
  g_ObjFourmi6->ChangeFrameRateRatio(10.0);
  g_ObjFourmi6->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi6->RandomAnimationStart(ANIMATION_WALK_UID);


  g_ObjFourmi7 = new C3DObjElement;
  *(g_ObjFourmi7) = *(ObjOuvriere);
  g_ObjFourmi7->ChangeFrameRateRatio(10.0);
  g_ObjFourmi7->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi7->RandomAnimationStart(ANIMATION_WALK_UID);

  
  g_ObjFourmi8 = new C3DObjElement;
  *(g_ObjFourmi8) = *(ObjOuvriere);
  g_ObjFourmi8->ChangeFrameRateRatio(10.0);
  g_ObjFourmi8->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi8->RandomAnimationStart(ANIMATION_WALK_UID);

  g_ObjFourmi9 = new C3DObjElement;
  *(g_ObjFourmi9) = *(ObjOuvriere);
  g_ObjFourmi9->ChangeFrameRateRatio(10.0);
  g_ObjFourmi9->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi9->RandomAnimationStart(ANIMATION_WALK_UID);


  g_ObjFourmi10 = new C3DObjElement;
  *(g_ObjFourmi10) = *(ObjOuvriere);
  g_ObjFourmi10->ChangeFrameRateRatio(10.0);
  g_ObjFourmi10->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi10->RandomAnimationStart(ANIMATION_WALK_UID);


  g_ObjFourmi11 = new C3DObjElement;
  *(g_ObjFourmi11) = *(ObjOuvriere);
  g_ObjFourmi11->ChangeFrameRateRatio(10.0);
  g_ObjFourmi11->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi11->RandomAnimationStart(ANIMATION_WALK_UID);

  g_ObjFourmi12 = new C3DObjElement;
  *(g_ObjFourmi12) = *(ObjOuvriere);
  g_ObjFourmi12->ChangeFrameRateRatio(10.0);
  g_ObjFourmi12->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi12->RandomAnimationStart(ANIMATION_WALK_UID);


  g_ObjFourmi13 = new C3DObjElement;
  *(g_ObjFourmi13) = *(ObjOuvriere);
  g_ObjFourmi13->ChangeFrameRateRatio(10.0);
  g_ObjFourmi13->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi13->RandomAnimationStart(ANIMATION_WALK_UID);


  g_ObjFourmi14 = new C3DObjElement;
  *(g_ObjFourmi14) = *(ObjOuvriere);
  g_ObjFourmi14->ChangeFrameRateRatio(10.0);
  g_ObjFourmi14->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi14->RandomAnimationStart(ANIMATION_WALK_UID);

  
  g_ObjFourmi15 = new C3DObjElement;
  *(g_ObjFourmi15) = *(ObjOuvriere);
  g_ObjFourmi15->ChangeFrameRateRatio(10.0);
  g_ObjFourmi15->SelectDefaultAnimation(ANIMATION_WALK_UID);
  g_ObjFourmi15->RandomAnimationStart(ANIMATION_WALK_UID);

  */

  CVector3f PositionInitiale( 600,400,600 );
  CVector3f VitesseInitiale( 5.,-180.,0 );
  CVector3f Force( 0.f,-9.81f*80.f,-500.f );

  CVector3f RandomPlan( 1200,0.0,1200 );
  CVector3f RandomSpeed( 0, -40, 0 );
  
  strcpy(g_sztextureparticule, g_exepath);
  strcat(g_sztextureparticule, "textures"PATHDELIM_S"particle.tga");
  GLuint texuid;
  glGenTextures(1, &texuid);	// Create The Texture
  tgaLoad(g_sztextureparticule,texuid);
  g_ParticleSystem.Init(1000, 5, 10, 
                        0.8f, 1.0f, 1.0f, 0.25f,
                        true, texuid, 10.,13.,
                        PositionInitiale, VitesseInitiale, Force,
                        RandomPlan, RandomSpeed,&create_particle_callback,
                        NULL);
  g_ParticleChrono.Reset();

  return true;
}


/*

float objects_GetMapHeight ( float posx, float posz )
{
  float height;

  int curi = (int) floor(posx/STEP_SIZE) % MAT_SIZE_X;
  int curj = (int) floor(posz/STEP_SIZE) % MAT_SIZE_Y;

  // détermination de la hauteur
  // petite explication de ce qui va être calculé (c très simple)
  // notre carré :
  //
  // B (i+1,j)               D (i+1, j+1)
  //  + ------------------- +
  //  |                  /  |
  //  |      A         /    |
  //  |     x        /      |
  //  |          O /        |
  //  |         x/          |
  //  |        /            |
  //  |      /              |
  //  |T1  /                |
  //  |  /                  | 
  //  |/ T2                 |                i                       posx
  //  + ------------------- + (i, j+1)     ^                       ^
  //  C                      E             |      * STEP_SIZE =>   |
  //  (i,j)                                o-->  j                 o--> posz
  //                                                                 
  // Remarque: i et j ne sont pas dans le sens "habituel": i est vers le
  // haut, j vers la droite.

  // produit scalaire entre le vecteur OA et (-1, 1), i.e. la normale à la 
  // diagonale se dirigeant vers E
  // (curi+1) * STEP_SIZE => position réelle sur les x du point i+1 de la 
  //                         matrice 
  // (curi+1) * STEP_SIZE - posx) => position sur les x           -> ->
  //                                 du point A dans le repere (B,BC,BD)
  // (curi+0.5) * STEP_SIZE) : position sur les x du point O dans ce repere.
  //
  // donc si ce produit scalaire est négatif, on est dans le triangle T1 (du haut)
  float a = (posx - ((curi + 0.5) * STEP_SIZE)) * -1;
  float b = (posz - (curj + 0.5) * STEP_SIZE) * 1;
  if ( a + b < 0)
  { // triangle T1          -> ->              B
    // Soit le repere : (B, BC,BD)              x ---->
    //                                          |
    //                                   ->     V
    // principe: on décompose le vecteur BA selon les vecteurs de la base
    // (on travaille dans le plan uniquement)
    // on divise par la taille du coté du coté du carré => on a ainsi alpha 
    // pour les x et beta pour les y:
    //          /            \      / 
    // alpha = |   x   - x    |   /    STEP_SIZE
    //          \   i+1   A  /  /
    //          /            \      / 
    // beta  = |   z  - z     |   /    STEP_SIZE
    //          \   A    j+1 /  /
    // on a en fait le % des vecteurs de la base "utilisé".
    // en gros on fait         ->          ->   ->
    //                 alpha * BC + beta * BD = BA
    //
    // reste plus qu'à utiliser ces vecteurs dans l'espace 3D pour déterminer
    // la hauteur :                   /         \            /        \
    //        hauteur = y  + alpha * |  y  - y   | + beta * | y  - y   |
    //                   B            \  C    B /            \ D    B /
    // Rappel: le sol est défini par xOz et la HAUTEUR est selon l'axe Oz
    //
    // simplifié
    height = monde.Height(curi+1,curj) +
              ( (curi+1) - posx / STEP_SIZE) * 
                (monde.Height(curi,curj) - monde.Height(curi+1,curj) )
             +
              ( posz / STEP_SIZE - curj ) * 
              ( monde.Height(curi+1,curj+1) - monde.Height(curi+1,curj) );
  }
  else
  { //                                                       ->  ->
    // même principe mais avec le triangle T2 et la base (E, EC, ED)
    height = monde.Height(curi,curj+1) + 
               ( posx / STEP_SIZE - (curi)) * 
                 (monde.Height(curi+1,curj+1) - monde.Height(curi,curj+1) )
              +
               ( posz / STEP_SIZE - (curj+1) ) *
               ( monde.Height(curi,curj+1) - monde.Height(curi,curj) );
  }

  return height;
}


void objects_PutOnTheMap(CBoundingBox *boundingbox, float theta, float posx, float posz)
{
//
//              sizex
//   <-------------------------->
//  D ------------------------- C
//  |                           |    ^
//  |                           |    |
//  |                           |    |
//  |             x---->        |    | sizez
//  |                           |    |            ^ Z
//  |                           |    |            |
//  |                           |    V            x---> X
//  A ------------------------- B                Y
//

  float sizex, sizez, sizexd2, sizezd2;
  float bbposx, bbposz;

  sizex = boundingbox->point[1][X] - boundingbox->point[0][X];
  sizez  = boundingbox->point[2][Z] - boundingbox->point[1][Z];
  if ( sizex < 0 )
    sizex  = -sizex;
  if ( sizez < 0 )
    sizez = - sizez;

  sizexd2 = sizex / 2;
  sizezd2 = sizez / 2;

  bbposx = boundingbox->point[0][X] + sizexd2;
  bbposz = boundingbox->point[0][Z] + sizezd2;

  CVector3 pta, ptb, ptc, ptd;
  CVector3 *ptmax1,*ptmax2,*ptmax3;

  pta.x = posx + bbposx + cos(-theta * DEGTORAD) * (-sizexd2) - sin(-theta * DEGTORAD) * (sizezd2);
  pta.z = posz + bbposz + sin(-theta * DEGTORAD) * (-sizexd2) + cos(-theta * DEGTORAD) * (sizezd2);
  pta.y = objects_GetMapHeight(pta.x, pta.z)/255*MAP_MAX_HEIGHT;

  ptb.x = posx + bbposx + cos(-theta * DEGTORAD) * (-sizexd2) - sin(-theta * DEGTORAD) * (-sizezd2);
  ptb.z = posz + bbposz + sin(-theta * DEGTORAD) * (-sizexd2) + cos(-theta * DEGTORAD) * (-sizezd2);
  ptb.y = objects_GetMapHeight(ptb.x, ptb.z)/255*MAP_MAX_HEIGHT;

  ptc.x = posx + bbposx + cos(-theta * DEGTORAD) * (sizexd2) - sin(-theta * DEGTORAD) * (sizezd2);
  ptc.z = posz + bbposz + sin(-theta * DEGTORAD) * (sizexd2) + cos(-theta * DEGTORAD) * (sizezd2);
  ptc.y = objects_GetMapHeight(ptc.x, ptc.z)/255*MAP_MAX_HEIGHT;

  ptd.x = posx + bbposx + cos(-theta * DEGTORAD) * (sizexd2) - sin(-theta * DEGTORAD) * (-sizezd2);
  ptd.z = posz + bbposz + sin(-theta * DEGTORAD) * (sizexd2) + cos(-theta * DEGTORAD) * (-sizezd2);
  ptd.y = objects_GetMapHeight(ptd.x, ptd.z)/255*MAP_MAX_HEIGHT;


  ptmax1 = &pta;
  ptmax2 = &ptc;
  ptb.x = (ptb.x + ptd.x)/2.0 ;
  ptb.y = (ptb.y + ptd.y)/2.0 ;
  ptb.z = (ptb.z + ptd.z)/2.0 ;
  ptmax3 = &ptb;

#if 0
  glPushMatrix();
    glColor3f(0.0,0.0,1.0);
    glLineWidth(5.0);
    //glTranslatef(0.0f,0.1f,0.0f);

    glBegin(GL_TRIANGLES);
      glVertex3f(ptmax1->x, ptmax1->y, ptmax1->z);
      glVertex3f(ptmax2->x, ptmax2->y, ptmax2->z);
      glVertex3f(ptmax3->x, ptmax3->y, ptmax3->z);
    glEnd();

    glLineWidth(1.0);
  glPopMatrix();
#endif 

  CVector3 pla;
  CVector3 v1,v2;
  // produit vectoriel entre vec(ptmax1, ptmax2) et vec(ptmax1,ptmax3)
  v1.x = ptmax2->x - ptmax1->x;
  v1.y = ptmax2->y - ptmax1->y;
  v1.z = ptmax2->z - ptmax1->z;

  v2.x = ptmax3->x - ptmax1->x;
  v2.y = ptmax3->y - ptmax1->y;
  v2.z = ptmax3->z - ptmax1->z;

  pla.x = v1.y * v2.z - v2.y * v1.z;
  pla.y = v1.z * v2.x - v2.z * v1.x;
  pla.z = v1.x * v2.y - v2.x * v1.y;
  // par ex le vecteur normal au plan

  CVector3 rot; // vector de rotation !!!
  // produit scalaire entre pla et le vecteur Oy (0,1,0)
  rot.x = -pla.z;
  rot.y = 0;
  rot.z = pla.x;
  // on a l'axe de rotation... manque plus que l'angle => produit scalaire!
  // ->  ->                            ->    ->
  // u . v = x * x + y * y + z * z = ||x||.||y||.cos(an)
  //          u   v   u   v   u   v
  float an = acos((pla.y) / (sqrt(pla.x * pla.x + pla.y * pla.y + pla.z * pla.z) * 1));

  /////////////////////////////////////////////////////////////////////////////////////////////

  float height = objects_GetMapHeight(posx, posz);

  glTranslatef(posx, height / 255 * MAP_MAX_HEIGHT, posz);

#if 0
    glColor3f(0.0,0.0,1.0);
    glBegin(GL_LINES      );
      glVertex3f(0, 0, 0);
      glVertex3f(pla.x, pla.y, pla.z);
    glEnd();
    glColor3f(0.0,1.0,1.0);
    glBegin(GL_LINES      );
      glVertex3f(0, 0, 0);
      glVertex3f(rot.x*20, rot.y*20, rot.z*20);
    glEnd();
#endif

  glRotatef(-an * RADTODEG, rot.x, rot.y, rot.z);

#if 0
    glColor3f(1.0,0.0,0.0);
    glBegin(GL_LINES      );
      glVertex3f(0,-2000, 0);
      glVertex3f(0, 2000, 0);
    glEnd();

#endif
  glRotatef(theta, 0.0, 1.0, 0.0);

}


*/
