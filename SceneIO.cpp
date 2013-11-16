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
#include "common.h"
#include "Scene.h"
#include "SceneIO.h"
#include "Objects.h"
#include "main.h"
#include "AI/AI_renders.h"

// TODO
#define SPEED_BEE 16.0f
#define SPEED_SCROLL 2.0f;

int p;

GLdouble g_objx=0, g_objy=0, g_objz=0;

// doit retourner false pour quitter le programme
bool io_processevents( void )
{
  
  SDL_Event event;
  
  /* Poll for events. SDL_PollEvent() returns 0 when there are no  */
  /* more events on the event queue, our while loop will exit when */
  /* that occurs.                                                  */
  while( SDL_PollEvent( &event ) )
  {
    /* We are only worried about SDL_KEYDOWN and SDL_KEYUP events */
    switch( event.type )
    {
      case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE)
      {
        Main_Mode = MENU_MAP_MODE;
        SDL_ShowCursor(true);
      }
      else if (event.key.keysym.sym == SDLK_RETURN && event.key.keysym.mod & KMOD_LALT)
        scene_swapfullscreen();
      else
        io_clavier_synchr(event.key.keysym.sym, SDL_KEYDOWN);
      
      break;
      
    case SDL_KEYUP:
      io_clavier_synchr(event.key.keysym.sym, SDL_KEYUP);
      break;
      
    case SDL_VIDEORESIZE:
      io_reshape(event.resize.w, event.resize.h);
      break;
      
    case SDL_MOUSEMOTION:
      
      io_motion(event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
      break;
      
    case SDL_QUIT:
      return true;
      
    case SDL_MOUSEBUTTONUP:
      p=0;
      
      
      if (cCam.ViewMode()!= BEE_VIEW) 
      {
        if (!world->map_->menu_down->Menuio(event.button.x, event.button.y,event.button.button))
       {
         io_mouse(event.button.button,event.button.x,event.button.y);
       }
      }
      else io_mouse(event.button.button,event.button.x,event.button.y);
      break;
    case SDL_MOUSEBUTTONDOWN :
      p=1;
      break;
      
      
    default:
      break;
    }
  }
  // traiter les évenements clavier asynchrones (sans gestion des
  // évenements)
  io_clavier_asynchr();
  
  return true;
}


/* redimensionnement de la fenetre */
void io_reshape(int w, int h)
{
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0,(float) w/h,0.2,7000.0);
}


void io_active(int x, int y)
{
  printf("Evenement activation de la fenetre reçu\n");
}

void io_motion (int x, int y, int xrel, int yrel)
{
  float 	ns = SPEED_SCROLL;
  
  switch (cCam.ViewMode())
  {
  case BEE_VIEW :
    cCam.SetMouseView(xrel,yrel);
    break;
    
  case SCROLL_VIEW :
    if ((p))
    {
      int a = x*1024/TAILLEX;
      int b = y*768/TAILLEY;
      
      
      if (world->map_->mmap->Clic(a,b))
        world->map_->CenterOn(CVector3f((a-world->map_->mmap->ulx)*world->map_->mmap->ratio,0,(world->map_->mmap->lry-b)*world->map_->mmap->ratio));
      
    }
    
    break;
    
  }
  
}


void io_mouse(int button,int x,int y)
{
  int a = TAILLEX/2 - 320;
  int b = TAILLEY/2 - 240;
  switch(button)
  {
    case SDL_BUTTON_LEFT :
    {
      
      if (cCam.ViewMode() != BEE_VIEW)
      {

        GLdouble modelMatrix[16];
        GLdouble projMatrix[16];
        GLint    viewport[4];
        GLdouble winx, winy, winz;
        GLdouble objx=0, objy=0, objz=0;
        CVector3f pos, dir;

        glGetDoublev(GL_MODELVIEW_MATRIX, modelMatrix);
        glGetDoublev(GL_PROJECTION_MATRIX, projMatrix);
        glGetIntegerv(GL_VIEWPORT, viewport);

        winx = x;
        winy = viewport[3] - y;
        winz = 0;
        // lecture de la profondeur... dans le Z buffer
        glReadPixels(winx,winy,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&winz);
        // ou est situé le point (en 3d) situé en winx winy (souris) et à 
        // la profondeur winz...
        gluUnProject(winx, winy, winz, modelMatrix, projMatrix, viewport,
          &objx, &objy, &objz);
//        if (cCam.ViewMode() == SCROLL_VIEW)
//        {
          CVector3f dec(world->map_->ScrollLimitf(),0);
          // récupération des coordonnées de la caméra (ce ne sont pas celle stoqué dans
          // la cCam, se sont des vrai fausses coordonnées...)
          pos = CVector3f(cCam.Position().x, 
                          cCam.Position().y + cCam.GetScrollHeight(), 
                          cCam.Position().z);
          // vecteur direction de la caméra
          dir = CVector3f(objx - pos.x, objy - pos.y, objz - pos.z);
          // temp pas bien ...
          pos += dec;
          // laid...
	        vect2f ftrueview = world->map_->ScrollLimitf() + world->map_->FrameSize();
          float mindist = 50000.0f, fdist;
          for(uint i=0 ; i< world->get_number_of_objects() ; ++i, world->next_object())
          {
            dec.x = 0 ;
            dec.y = 0 ;
            dec.z = 0 ;

            if (( ftrueview.x > world->map_->Get_Map_Size()) && (world->current_object().g_obj_->coords2d().x < ftrueview.x - world->map_->Get_Map_Size())) 
              dec.x = -(float)world->map_->Get_Map_Size() ;
            if (( ftrueview.z > world->map_->Get_Map_Size()) && (world->current_object().g_obj_->coords2d().z < ftrueview.z - world->map_->Get_Map_Size())) 
              dec.z = -(float)world->map_->Get_Map_Size() ;
            // calcul de la distance à la caméra
            if (world->current_object().g_obj_->select_from_camera(pos+dec, dir))
            {
              fdist = (pos - world->current_object().g_obj_->coords3d_).Norm();
              if (fdist > 0 && fdist < mindist)
              {
                mindist = fdist;
                world->g_world_->highlighted_object_ = &world->current_object();
              }
            }
          }
          if (mindist == 50000.0f) // même si c un float il n'y a pas de raison qu'une comparaison
          {                        // exacte ne fonctionne pas
            // bref, dans ce cas on n'a rien sélectionné
              if (world->g_world_->highlighted_object_)
                world->g_world_->highlighted_object_ = NULL;

              if (cCam.gfollow())
                cCam.chfollow(0);
          }
/*        }
        else
        {
          // récupération des coordonnées de la caméra (ce ne sont pas celle stoqué dans
          // la cCam, se sont des vrai fausses coordonnées...)
          pos = CVector3f(cCam.Position().x, 
                          cCam.Position().y, 
                          cCam.Position().z);
          // vecteur direction de la caméra
          dir = CVector3f(objx - pos.x, objy - pos.y, objz - pos.z);
          // temp pas bien ...
          for(uint i=0 ; i< world->get_number_of_objects() ; ++i, world->next_object())
          {
            world->current_object().g_obj_->select_from_camera(pos, dir);
          }
        }*/

        int a = x*1024/TAILLEX;
        int b = y*768/TAILLEY;
        
        
        if ((world->map_->mmap->Clic(a,b))&&(cCam.ViewMode()==SCROLL_VIEW))
          world->map_->CenterOn(CVector3f((a-world->map_->mmap->ulx)*world->map_->mmap->ratio,0,(world->map_->mmap->lry-b)*world->map_->mmap->ratio));
        break;
      }
      // ... temps pas bien
      
      break;
    }
	  case SDL_BUTTON_WHEELUP	:
     
      cCam.ScrollUp(-10);
      break;
    case SDL_BUTTON_WHEELDOWN	:
      cCam.ScrollUp(10);
      break;
  }
}




#define FOURMI_SPEED 3.f

/* gestion du clavier synchrone: cette fonction est appellé à chaque fois
qu'une touche est pressée ou relachée (gestion des évènements par SDL
*/
void io_clavier_synchr(SDLKey key, Uint8 event)
{
  if (event == SDL_KEYDOWN)
  {
    if (world)
      if (key == SDLK_t) 
        world->g_world_->change_rendered_token();
      switch (cCam.ViewMode())
      {
        
      case BEE_VIEW :
        if (key == SDLK_m)
        {
          cCam.ChangeMode(SCROLL_VIEW,world->map_->FrameSizeX(),world->map_->FrameSizeZ());
          SDL_ShowCursor(true);
        }
        break;
        
      case SCROLL_VIEW:
        if (key == SDLK_m)
        {
          cCam.ChangeMode(BEE_VIEW,0,0);
          SDL_ShowCursor(false);
          
        }
        break;
      }
  }
  else // if event == SDL_KEYDOWN
  {
  }
}

/* gestion du clavier asynchrone : à chaque fois qu'on appelle cette fonction,
on regarde l'état du clavier (chaque touche, pressée, pas pressée), et on
agit en conséquence). Ainsi on n'est pas dépendant de la file de message
de la fenetre
*/
void io_clavier_asynchr()
{
  float ns;
  float f;
  Uint8 *touches =  SDL_GetKeyState(NULL);
  /*
  if (g_drtm)
  {
  ns = SPEED * g_drtm;
  }
  */
  switch (cCam.ViewMode())
  {
  case BEE_VIEW :
    
    if (g_drtm)
    {
      ns = SPEED_BEE * g_drtm;
    }
    if (touches[SDLK_w])
      cCam.MoveCamera(-ns*10.0f);
    if (touches[SDLK_s]) 
      cCam.MoveCamera(ns*10.0f);
    
    if (touches[SDLK_a]) 
      cCam.StrafeCamera(-ns*10.0f);
    if (touches[SDLK_d])
      cCam.StrafeCamera(ns*10.0f);
    
    
      /*      if (touches[SDLK_UP]) 
      {
      static  char b = 0;
      if (g_drtm)
      {
      float a = FOURMI_SPEED * g_drtm;
      if (b==0)
      {
      b=1;
      // on change l'état courant : de STAY on passe à WALK
      g_ObjFourmi1->ChangeAnimation(ANIMATION_STAY_UID, ANIMATION_WALK_UID); 
      // on change aussi toutes les next animations qui devraient retombées sur
      // STAY en les faisant continuer sur WALK
      g_ObjFourmi1->ChangeNextAnimation(ANIMATION_STAY_UID, ANIMATION_WALK_UID); 
      }
      fourmipos += a;
      }
      }
      if (touches[SDLK_DOWN]) 
      {
      if (g_drtm)
      {
      float a = FOURMI_SPEED * g_drtm;
      fourmipos -= a;
      }
  }*/
    break;
    
    
  case SCROLL_VIEW :
    ns = SPEED_SCROLL;
    if (!cCam.gfollow())
    {
      if (touches[SDLK_SPACE])
        world->map_->CenterOn(vect3f(world->nest_->get_coords2d(),0));
      if (touches[SDLK_a])
      {
        world->map_->SLimitX(world->map_->ScrollLimitX()-cCam.Strafe().x *ns/10.0f);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()-cCam.Strafe().z *ns/10.0f);
      
      }
      if (touches[SDLK_d])
      {
      
        world->map_->SLimitX(world->map_->ScrollLimitX()+cCam.Strafe().x *ns/10.0f);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()+cCam.Strafe().z *ns/10.0f);
      }
    
      if (touches[SDLK_w]) 
      {
        CVector3f vWay = cross_product(cCam.Normal(),cCam.Strafe());
        vWay.Normalize();
		    world->map_->SLimitX(world->map_->ScrollLimitX()+vWay.x *ns/10.0f);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()+vWay.z *ns/10.0f);
      }
    
      if (touches[SDLK_s] )
      {
		    CVector3f vWay = cross_product(cCam.Normal(),cCam.Strafe());
        vWay.Normalize();
        world->map_->SLimitX(world->map_->ScrollLimitX()-vWay.x *ns/10.0f);
        world->map_->SLimitZ(world->map_->ScrollLimitZ()-vWay.z *ns/10.0f);
      }
    }
     if (touches[SDLK_z])
       cCam.MoveCamera(ns);
     if (touches[SDLK_x])
       cCam.MoveCamera(-ns);
     if ((touches[SDLK_o])) 
       cCam.ScrollUp(10);
     if ((touches[SDLK_p])) 
       cCam.ScrollUp(-10);
    
    
     world->map_->CheckScroll();
     // obligé de créer un nouveau vecteur sinon gcc il pleure...
     CVector2f v (world->map_->ScrollLimitXf()+cCam.Position().x,
      world->map_->ScrollLimitZf()+cCam.Position().z);
     f = world->map_->GetMapHeight(v) ;
     cCam.NewScrollHeight(f);
    
    
    break;
  }
  
  
  
  if (touches[SDLK_q])
    cCam.RotateView(-0.05f,0.0f,1.0f,0.0f);
  if (touches[SDLK_e])
    cCam.RotateView(0.05f,0.0f,1.0f,0.0f);
 // if (world)
 // {
 //   if (touches[SDLK_l])
 //     world->map_->MoveSun(0.01f);
 //   if (touches[SDLK_k])
 //     world->map_->MoveSun(-0.01f);
 // }
  
  
}
