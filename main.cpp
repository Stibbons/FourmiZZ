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
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_thread.h>
#include <SDL/SDL_mixer.h>

#include "main.h"
#include "common.h"
#include "Scene.h"
#include "SceneIO.h"
#include "Objects.h"
#include "lib/CSDLChrono.h"
#include "lib/CustomAssert.h"
#include "AI/AI_ants.h"
#include "sounds.h"

float g_drtm=0; // draw time, temps mis pour dessiner la dernière frame
  // en seconde

char g_exepath[256];

/* Remarque concernant la fonction MAIN et la SDL
 * Cette fonction est altérée par sdl.h qui la redéfinie en sdlMain.
 * En effet, la SDL doit faire qque traitement avant de lancer le programme
 * Ainsi, il faut ajouter sdlmain.lib pour ajouter un main à notre application,
 * en plus de sdl.lib.
 *
 * Remarque complémentaire:
 * Amélioration: plus de console.
 * Une application Win32 GUI (c'est à dire sans console) n'est pas appelée par
 * un main, mais par la fonction WinMain qui prend d'autres arguments.
 * Hors dans notre projet, il n'y a pas de WinMain...
 * Explication : la SDL redéfini main en sdl_main. Et c'est le main de 
 * sdlmain.lib qui est appelé et qui appel ensuite sdl_main (celle là).
 * Hors, dans sdlmain.lib, il est aussi défini WinMain qui charge quelques
 * DLL (directplay, directdraw,...), et qui appelle ensuite le main de la SDL
 * (avec la ligne de commande bien traitée (main et winmain gérent différement
 * la ligne de commande)), et qui appelle ensuite le main de notre application
 * La seule chose changée est l'option dans le linker:
 * /subsystem:console => /subsystem:windows....
 * Bref, le main suivant n'est pas un vrai main... ce qui ne pose pas de 
 * probleme pour la compilation sous Linux et autre plateforme ou la SDL
 * est suportée.
 */
/* main multithread
int main(int argc, char **argv)
{

  Uint32 tm = SDL_GetTicks(), tm2;
  float fr; // frame rate

  g_draw_thread_active = 1;
  SDL_Thread  * pdraw_thread = SDL_CreateThread(&scene_draw_thread, NULL);

  /* Process incoming events. */
/*  while( io_processevents( ) && g_draw_thread_active )
  {

  }

  g_draw_thread_active = 0;
  SDL_WaitThread(pdraw_thread, NULL);

  printf("Destruction de la SDL...\n");    
  SDL_Quit();

  return 0;
}
*/

int main(int argc, char **argv)
{
/* si on ne veut pas de la console sous Windows:
  #ifdef WIN32
  FreeConsole();
  #endif
*/



  try
  {
    

    float fr; // frame rate

    srand(SDL_GetTicks());

    strcpy(g_exepath, argv[0]);
    *(strrchr(g_exepath, PATHDELIM_C))=0;
    strcat(g_exepath, PATHDELIM_S);

    g_draw_thread_active = 1;



    /* Initialisation de la fenetre (ce qui est nécessaire pour SDL + OpenGL) */

    cout << "Initialisation de la SDL (video).\n" << endl;
  
    if((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)==-1)) 
    { 
      cerr << "Erreur à l'initialisation de la SDL.\nErreur: " << 
        SDL_GetError() << endl;
      g_draw_thread_active=0;
      return 1;
    }

    cout << "SDL initialisé.\n" << endl;

    const SDL_VideoInfo* info = NULL;

    /* valeurs que l'on va passer à la SDL */
    int width = 0;  // largeur de la fenetre
    int height = 0; // hauteur
    int bpp = 0;    // nbr de couleur (en bit)
    int flags = 0;  // et les flags...

    /* Récupération des informations videos courants */
    info = SDL_GetVideoInfo();

    if( !info ) 
    {
        /* Erreur: pas les droits? */
        cerr << "Erreur lors de la récupération des informations vidéo par la SDL: " <<
             SDL_GetError() << endl;
        SDL_Quit();
        g_draw_thread_active=0;
        return 2;
    }
    /*
     * on va faire du 640 x 480 (on ne change pas la profondeur des couleurs, par exemple
     * sous X c'est impossible)
     */
    width = TAILLEX;
    height = TAILLEY;
    bpp = info->vfmt->BitsPerPixel;

    /*
     * Mise en place des attributs Opengl de la SDL
     */
    cout << "Mise en place des attributs OpenGL de la SDL..." << endl;

    // 8 bits de rouge
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE    , 8  );
    // 8 bits de vert
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE  , 8  );
    // 8 bits de bleu
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE   , 8  );
    // 8 bits d'alpha
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE  , 8  );
    // 16 bits pour le buffer de profondeur
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE  , 16 );
    // on utilisera le double-buffering
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1  );

    /*
     * Nos flags: On veut OpenDL, l'accélération Hardware si possible,
     * ainsi qu'une fenetre redimensionnable, s'il vous plait
     */
    flags = SDL_OPENGL | SDL_HWSURFACE | SDL_RESIZABLE;

    g_fullscreen = 0;

    // TODO : regarder la ligne de commande (passage en mode plein écran directement,
    // résolution,...

    /*
     * Changement du mode vidéo
     */
    cout << "Changement du mode vidéo : " << width << "x" << height << 
      " en " << bpp << " bits, " << (g_fullscreen?"plein écran":"fenetré") << endl;
    if( SDL_SetVideoMode( width, height, bpp, flags ) == 0 ) 
    {
        cerr << "Impossible de sélectionne le mode video\nErreur: " <<
             SDL_GetError() << endl;
        SDL_Quit();
        g_draw_thread_active=0;
        return 3;
    }

    SDL_WM_SetCaption("Simulation fourmilière Isima Première Année", NULL);

	  CSoundManager *soundmanager = new CSoundManager;
    // décommenter la ligne suivante pour faire "planter" (assert)
	  //new CSoundManager;

    /* initialisation de SDL_Mixer */
    GetSoundManager().InitAudio();
    GetSoundManager().AddMusic(MAIN_MUSIC_1_PATH, MAIN_MUSIC_1_ID);
    GetSoundManager().AddMusic(MAIN_MUSIC_2_PATH, MAIN_MUSIC_2_ID);
    GetSoundManager().AddMusic(MAIN_MUSIC_3_PATH, MAIN_MUSIC_3_ID);
    GetSoundManager().AddMusic(SIMUL_MUSIC_1_PATH, SIMUL_MUSIC_1_ID);
    GetSoundManager().AddMusic(SIMUL_MUSIC_2_PATH, SIMUL_MUSIC_2_ID);
    //GetSoundManager().AddMusic(SIMUL_MUSIC_3_PATH, SIMUL_MUSIC_3_ID);
    //GetSoundManager().AddSound(ANT_WALK_PATH, ANT_WALK_ID);
    GetSoundManager().AddSound(ANT_TAKEOBJECT_SOUND_PATH, ANT_TAKEOBJECT_SOUND_ID);
    GetSoundManager().AddSound(NIGHT_SOUND_PATH, NIGHT_SOUND_ID);
     GetSoundManager().AddSound(MENU_START_SOUND_PATH, MENU_START_SOUND_ID);
    GetSoundManager().AddSound(RAIN_SOUND_PATH, RAIN_SOUND_ID);
    //GetSoundManager().AddSound(MENU_BMOTION_SOUND_PATH, MENU_BMOTION_SOUND_ID);
    GetSoundManager().AddSound(NEWANT_SOUND_PATH, NEWANT_SOUND_ID);
    GetSoundManager().AddSound(MENU_BCLIC_SOUND_PATH, MENU_BCLIC_SOUND_ID);
    GetSoundManager().AddSound(MENU_OCLIC_SOUND_PATH, MENU_OCLIC_SOUND_ID);
    //GetSoundManager().Disable();
    GetSoundManager().PlayMusic(MAIN_MUSIC_1_ID, true);

    scene_init( width, height );
    cout <<"Chargement du menu";
    menu_main= new menu(TempSettings,"Menu.ini");;
    menu_map = new menu(TempSettings,"Mapmenu.ini");;
    
  
    unsigned int tex, mask;

    printf("\nConstruction de la font!\n");
	  glGenTextures(1, &(tex));
    printf("texture generee\n");
    tgaLoad("textures"PATHDELIM_S"font.tga",tex);
	  glGenTextures(1, &mask);
    tgaLoad("textures"PATHDELIM_S"fontmask.tga",mask);
    printf("texture chargee\n");
    fontBuild(tex, mask);
    printf("font construite");
    fontResize(width, height);


    cout <<"font crée \n";

    CSDLChrono chrono;
    chrono.Start();

    // objects_init();
    glClear(((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));
    int non_fin = 1;

    while( non_fin && !SDL_QuitRequested())
    {
	    glClear(((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));
      switch (Main_Mode)
      {
        case MAP_MODE :
          Assert(world, "Le monde ne peut pas être null");
		      chrono.Update();
		      g_drtm = (float) chrono.Getdt() / 1000.0f;
		      fr = 1.0 / g_drtm;
		      cout << "\r" << fr << "images / secondes";
          world->ai_update();
          /* Draw the screen. */
          scene_draw();
          non_fin=io_processevents();

		    break;
	      case MENU_MODE :

		      non_fin = menu_main->processevents();
		      menu_main->Render();

		      break;
	      case MENU_MAP_MODE :
          Assert(world, "Le monde ne peut pas être null");

		  	  chrono.Update();
		      g_drtm = (float) chrono.Getdt() / 1000.0f;
		      fr = 1.0 / g_drtm;
		      cout << "\r" << fr << "images / secondes";
          world->ai_update();
          /* Draw the screen. */
          scene_draw();
          menu_map->Render();
          non_fin = menu_map->processevents();
		      break;
	    }
	    glFlush();
      SDL_GL_SwapBuffers();
    }

    fontKill();
    delete soundmanager;
  }
  catch(...)
  {
    cerr << "Je vous préviens, je large le parachute !!" << endl;
  }

  printf("Destruction de la SDL...\n");    
  Mix_CloseAudio();
  SDL_Quit();
  
  return 0;
}

