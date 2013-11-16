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
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "SDL/SDL.h"

#include "Scene.h"
#include "SceneIO.h"
#include "common.h"
#include "Objects.h"

#include "lib/CBubbleText.h"
#include "lib/CSDLChrono.h"

#include "AI/AI_world.h"

/* Couleurs des matières */
float	MatSpec[4]	= {1.0f, 1.0f, 1.0f,  1.0f};
float	MatDif[4]		= {0.5f, 0.5f, 0.5f,  1.0f};
float	MatAmb[4]		= {0.1f, 0.2f, 0.2f,  1.0f};
/* Couleurs des lumières */
float	Light1Pos[4]	= {0.0f, -1.0f,0.0f, 0.0f}; /* position */
float	Light1Dif[4]	= {2.0f, 1.8f, 1.0f,  1.0f};
float	Light1Amb[4]	= {0.1f, 0.1f, 0.1f,  1.0f};
float	Light1Spec[4]	= {0.5f, 0.5f, 0.5f,  1.0f};

float	Light2Pos[4]	= {0.0f, -1.0f,0.0f, 0.0f}; /* position */
float	Light2Dif[4]	= {0.1f, 0.1f, 0.3f,  1.0f};
float	Light2Amb[4]	= {0.1f, 0.1f, 0.4f,  1.0f};
float	Light2Spec[4]	= {0.0f, 0.0f, 0.0f,  1.0f};


int g_fullscreen;
int g_swapfullscreen;
int g_draw_thread_active;

#define FOURMI_SPEED 3.f

float fourmipos = 0;

CSDLChrono g_ParticleChrono;

void scene_init(int width, int height )
{
  
  //  float ratio = (float) width / (float) height;
  
  // Model d'ombres 
  glShadeModel( GL_SMOOTH );
  
  // GL_culling
  glCullFace( GL_BACK );
  glFrontFace( GL_CCW );
  glEnable( GL_CULL_FACE );
  
  // Clear color
  glClearColor( 0, 0, 0, 0 );
  
  // Point de vue
  glViewport( 0, 0, width, height );
  
  // Activation du buffer de profondeur 
  glEnable(GL_DEPTH_TEST);

  // Activation des textures
  glEnable(GL_TEXTURE_2D);

  // Activation des matériaux
  glEnable(GL_COLOR_MATERIAL);  

  // Type de la fonction pour tester la profondeur
  glDepthFunc(GL_LEQUAL);			


  glClearDepth(1.0f);				
 
   // définition des paramètres des matériaux
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,MatDif);
  glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,30);
  
  // définition des paramètres de la lampe 0 
  glLightfv(GL_LIGHT0,	GL_DIFFUSE,			Light1Dif);
  glLightfv(GL_LIGHT0,	GL_SPECULAR,		Light1Spec);
  glLightfv(GL_LIGHT0,	GL_AMBIENT,			Light1Amb);
  
  // Activation de la lampe
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT1,	GL_DIFFUSE,			Light2Dif);
  glLightfv(GL_LIGHT1,	GL_SPECULAR,		Light2Spec);
  glLightfv(GL_LIGHT1,	GL_AMBIENT,			Light2Amb);
  
  // Activation de la lampe


  glEnable(GL_LIGHT1);
  // Redimensionnement
  io_reshape(width, height);
  
  // On enlève le curseur
  SDL_ShowCursor(true);
  SDL_WM_GrabInput(SDL_GRAB_ON);

  // Sélection de la couleur
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  
  g_swapfullscreen=0;
  
}


// scene_draw() : Affichage de la scene 

void scene_draw()
{
  
  glMatrixMode(GL_MODELVIEW);
  
  glLoadIdentity();
  int x, y;
  float ns = 0.02f;
  if (world)
  {
    
// mise a jour de la caméra    
    cCam.Update();
    cCam.Look(); 

// mise a jour de la position de la lumiere ( le soleil est déplacé par l'IA )
    Light1Pos[0] = -world->map_->vSunDirection.x;
    Light1Pos[1] = -world->map_->vSunDirection.y;
    Light1Pos[2] = -world->map_->vSunDirection.z;
    
    Light2Pos[0] = world->map_->vSunDirection.x;
    Light2Pos[1] = world->map_->vSunDirection.y;
    Light2Pos[2] = world->map_->vSunDirection.z;
    glLightfv(GL_LIGHT0,	GL_POSITION, Light1Pos);
    glLightfv(GL_LIGHT1,	GL_POSITION, Light2Pos);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    
    switch (cCam.ViewMode())
    {

// Affichage en vue abeille
    case BEE_VIEW :
      glEnable(GL_TEXTURE_2D);
      if (world)
        world->map_->Render();
      glDisable(GL_TEXTURE_2D);
      break;
      
// Affichage en mode scrolling
    case SCROLL_VIEW :
      
// Si l'on se trouve sur le bord, la carte défile
      SDL_GetMouseState(&x, &y); 

// Cas ou l'on suit une fourmi
      if (cCam.gfollow())
      {
        world->map_->CenterOn(vect3f (world->g_world_->highlighted_object_->g_obj_->coords3d_.z,0,world->g_world_->highlighted_object_->g_obj_->coords3d_.x));
      }    
      if (x>TAILLEX-2) 
      {
        world->map_->SLimitX(world->map_->ScrollLimitX()+cCam.Strafe().x *ns);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()+cCam.Strafe().z *ns);
      }
      else if (x<1) 
      {
        world->map_->SLimitX(world->map_->ScrollLimitX()-cCam.Strafe().x *ns);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()-cCam.Strafe().z *ns);
      }
      if (y<1)
      {
        CVector3f vWay = cross_product(cCam.Normal(),cCam.Strafe());
        vWay.Normalize();
        world->map_->SLimitX(world->map_->ScrollLimitX()+vWay.x *ns);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()+vWay.z *ns);
      }
      else if (y>TAILLEY-2 )
      {
        CVector3f vWay = cross_product(cCam.Normal(),cCam.Strafe());
        vWay.Normalize();
        world->map_->SLimitX(world->map_->ScrollLimitX()-vWay.x *ns);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()-vWay.z *ns);
      }
      
      glEnable(GL_TEXTURE_2D);
      if (world)
        world->map_->RenderScrolling();
      glDisable(GL_TEXTURE_2D);
      break;
    }  


    static bool firstpass = true;
    if (firstpass)
    {
      g_ParticleSystem.Enable();
      g_ParticleChrono.Start();
      firstpass = false;
    }
//    g_ParticleSystem.Update(g_ParticleChrono.Getdt()/1000.0f);
//    g_ParticleSystem.Render();
//    g_ParticleChrono.Update();



    world->render();
    
  }
  
  
}


bool scene_swapfullscreen()
{
  const SDL_VideoInfo* info = NULL;
  
  /* Dimensions of our window. */
  int width = 0;
  int height = 0;
  /* Color depth in bits of our window. */
  int bpp = 0;
  /* Flags we will pass into SDL_SetVideoMode. */
  unsigned int flags = 0;

  info = SDL_GetVideoInfo( );
  
  width = TAILLEX;
  height = TAILLEY;
  bpp = info->vfmt->BitsPerPixel;
  
  //flags = (flags & SDL_FULLSCREEN) ? flags & ~SDL_FULLSCREEN : flags | SDL_FULLSCREEN;
  g_fullscreen = !g_fullscreen;
  flags = SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE | (g_fullscreen?SDL_FULLSCREEN:0);
  
  printf("Changement du mode vidéo : %dx%d en %d bits, %s\n", width, height, bpp,
    (g_fullscreen?"plein écran":"fenetré"));

  // on supprime touts les objets (display list et textures)
  object_delete_displaylists();

  if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 )
  {
    fprintf( stderr, "Impossible de sélectionne le mode video\nErreur: %s\n",
      SDL_GetError() );
    SDL_Quit();
    return false;
  }
  
  /* Il faut tout recréer après avoir redimensionner. En effet, le viewport a 
  changé, l'instance opengl a été détruite pour en recréer une.
  Note: sous Linux, le viewport n'est pas détruit, mais tout simplement
  redimensionner, parce que l'on ne peut pas changer de profondeur de 
  couleur, ce qui est possible sous Windows, ce qui oblige à détruire 
  le viewport OpenGL.
  Mais dans tous les cas, on détruit tous les objets graphiques et on 
  les reconstruits.
  Celà concerne:
  - les displaylist
  - les textures
  */

  // on recréé tout
  scene_init(width, height);
  objects_recreate_displaylists();

  fontResize(width, height);
  

  if (!world)
    SDL_ShowCursor(true);
  else
    SDL_ShowCursor(cCam.ViewMode() == SCROLL_VIEW);
  return true;
}

// attention: tjs mettre g_draw_thread_active à 0 pour
// terminer cette fonction

/*int scene_draw_thread(void * nothing)
{
Uint32 tm = SDL_GetTicks(), tm2;
float fr; // frame rate

  g_draw_thread_active = 1;
  
    
      SDL_WM_SetCaption("Simulation fourmilière Isima Première Année", NULL);
      
        scene_init( width, height );
        
          objects_init();
          
            while( io_processevents() )
            {
            //SDL_PollEvent( NULL );
            if (SDL_QuitRequested())
            break;
            tm2 = SDL_GetTicks();
            fr = 1000.0 / ((float) (tm2 - tm));
            printf("\r%.2f images / secondes", fr);
            tm = tm2;
            
              // Draw the screen.
              scene_draw( );
              }
              
                g_draw_thread_active = 0;
                return 0;
                }
*/
