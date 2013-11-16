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
#include "Menu.h"

#include <fstream>
#include <iostream>
#include <string>
#include "AI/AI_renders.h"


using namespace std;



void menu::Render()
{

  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glLoadIdentity();
    glColor4f(1,1,1,1);
  // On affiche le curseur
  //  SDL_ShowCursor(true);

  // Saisie des coordonnées de la souris
    int mouseposx, mouseposy;
    SDL_GetMouseState(&mouseposx, &mouseposy);

  // Redimensionnement en fonction de la résolution et de la taille du menu
    mouseposx = mouseposx * size.x / TAILLEX;
    mouseposy = mouseposy * size.z / TAILLEY;

  // Passage en orthoview
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();
      glOrtho(0,size.x,size.z,0,0,1);
      glMatrixMode(GL_MODELVIEW);
      glPushMatrix();
        glLoadIdentity();
        glColor4f(1.0f,1.0f,1.0f,1.0f);

  
        int i;

      // Selection de la page courante
        page_t p=pages[current];
        menu_obj_t o;

      // Parcours des objets de la page

        for (i =0;i<p.nb;i++)
        {
          o = p.p[i];
	        switch (o.type)
          {
	          case TITLE_T :
	            glEnable(GL_BLEND);  
              glEnable(GL_TEXTURE_2D);  
              glBlendFunc(GL_DST_COLOR,GL_ZERO); 
              glDisable(GL_DEPTH_TEST);                              // Disable Depth Testing  
              glBindTexture(GL_TEXTURE_2D, o.obj.t.mtex); 
		          glBegin(GL_QUADS);
			          glTexCoord2f(0,1);
			          glVertex2f(o.obj.t.ul.x, o.obj.t.ul.z);
			          glTexCoord2f(0,0);
			          glVertex2f(o.obj.t.ll.x, o.obj.t.ll.z);
			          glTexCoord2f(1,0);
			          glVertex2f(o.obj.t.lr.x, o.obj.t.lr.z);
			          glTexCoord2f(1,1);
			          glVertex2f(o.obj.t.ur.x, o.obj.t.ur.z);
      		    glEnd(); 
    
              glBlendFunc(GL_ONE, GL_ONE);  

		      
		          glBindTexture(GL_TEXTURE_2D,o.obj.t.tex); 
		          glBegin(GL_QUADS);
			          glTexCoord2f(0,1);
			          glVertex2f(o.obj.t.ul.x, o.obj.t.ul.z);
			          glTexCoord2f(0,0);
			          glVertex2f(o.obj.t.ll.x, o.obj.t.ll.z);
			          glTexCoord2f(1,0);
			          glVertex2f(o.obj.t.lr.x, o.obj.t.lr.z);
			          glTexCoord2f(1,1);
			          glVertex2f(o.obj.t.ur.x, o.obj.t.ur.z);

		          glEnd();
		          glEnable(GL_DEPTH_TEST); 
              glDisable(GL_BLEND); 
		          break;
	          case BLEND_T :
		          glEnable(GL_BLEND);
		          glDisable(GL_TEXTURE_2D);
		          glDepthMask(GL_FALSE);

		          glBlendFunc(GL_SRC_ALPHA,GL_SRC_ALPHA);
		          glBegin(GL_QUADS);
			          glColor4f(o.obj.bl.ul_color.red,o.obj.bl.ul_color.green,o.obj.bl.ul_color.blue,o.obj.bl.ul_color.alpha);
			          glVertex2i(o.obj.bl.ul.x,o.obj.bl.ul.z);
			          glColor4f(o.obj.bl.ll_color.red,o.obj.bl.ll_color.green,o.obj.bl.ll_color.blue,o.obj.bl.ll_color.alpha);
			          glVertex2i(o.obj.bl.ll.x,o.obj.bl.ll.z);
			          glColor4f(o.obj.bl.lr_color.red,o.obj.bl.lr_color.green,o.obj.bl.lr_color.blue,o.obj.bl.lr_color.alpha);
			          glVertex2i(o.obj.bl.lr.x,o.obj.bl.lr.z);
			          glColor4f(o.obj.bl.ur_color.red,o.obj.bl.ur_color.green,o.obj.bl.ur_color.blue,o.obj.bl.ur_color.alpha);
			          glVertex2i(o.obj.bl.ur.x,o.obj.bl.ur.z);
		          glEnd();
		            
		          glDisable(GL_BLEND);
		          glEnable(GL_TEXTURE_2D);
		          glDepthMask(GL_TRUE);
		          glColor4f(1.0f,1.0f,1.0f,1.0f);
		          break;

	          case BUTTON_T :
		          glEnable(GL_BLEND);  
              glEnable(GL_TEXTURE_2D);  
              glBlendFunc(GL_DST_COLOR,GL_ZERO); 
              glDisable(GL_DEPTH_TEST);                              // Disable Depth Testing  
		          if ((mouseposx>o.obj.b.sl.x)&&(mouseposx<o.obj.b.sr.x)
			          &&(mouseposy>o.obj.b.sl.z)&&(mouseposy<o.obj.b.sr.z))
		          {
			          glBindTexture(GL_TEXTURE_2D,o.obj.b.mtex[1]);
			          GetSoundManager().PlaySound(MENU_BMOTION_SOUND_ID, true);
		          }
		          else 
		          glBindTexture(GL_TEXTURE_2D,o.obj.b.mtex[0]); 
		          glBegin(GL_QUADS);
			          glTexCoord2f(0,1);
			          glVertex2f(o.obj.b.ul.x, o.obj.b.ul.z);
			          glTexCoord2f(0,0);
			          glVertex2f(o.obj.b.ll.x, o.obj.b.ll.z);
			          glTexCoord2f(1,0);
			          glVertex2f(o.obj.b.lr.x, o.obj.b.lr.z);
			          glTexCoord2f(1,1);
			          glVertex2f(o.obj.b.ur.x, o.obj.b.ur.z);

		          glEnd(); 
        
              glBlendFunc(GL_ONE, GL_ONE); 
		      

		          if ((mouseposx>o.obj.b.sl.x)&&(mouseposx<o.obj.b.sr.x)
			          &&(mouseposy>o.obj.b.sl.z)&&(mouseposy<o.obj.b.sr.z))

			          glBindTexture(GL_TEXTURE_2D,o.obj.b.tex[1]);
		          else 
		          glBindTexture(GL_TEXTURE_2D,o.obj.b.tex[0]);


		          glBegin(GL_QUADS);
			          glTexCoord2f(0,1);
			          glVertex2f(o.obj.b.ul.x, o.obj.b.ul.z);
			          glTexCoord2f(0,0);
			          glVertex2f(o.obj.b.ll.x, o.obj.b.ll.z);
			          glTexCoord2f(1,0);
			          glVertex2f(o.obj.b.lr.x, o.obj.b.lr.z);
			          glTexCoord2f(1,1);
			          glVertex2f(o.obj.b.ur.x, o.obj.b.ur.z);
		          glEnd();
	            glEnable(GL_DEPTH_TEST); 
              glDisable(GL_BLEND); 
		          break;

	          case OPTION_T :
		          option_t opt = o.obj.o;
		          if ((mouseposx>opt.text_c.x)&&(mouseposx<opt.val_c.x)
			          &&(mouseposy<opt.text_c.z)&&(mouseposy>opt.text_c.z-16))
		          {
			          glColor3f(opt.c2.red,opt.c2.green,opt.c2.blue);
		          //		GetSoundManager().PlaySound(MENU_OMOTION_ID, true);
		          }
		          else glColor3f(opt.c1.red,opt.c1.green,opt.c1.blue);

		          glPrint_chsize(opt.text_c.x,opt.text_c.z,opt.text,0,size.x,size.z);
		          char buffer[255];
		          sprintf(buffer,"%d",(int)*(opt.pval));
		          glPrint_chsize(opt.val_c.x,opt.val_c.z,buffer,0,size.x,size.z);
                 glColor4f(1.0f,1.0f,1.0f,1.0f);
		          break;
	        }
        }
	      


    // Retour a la vue perspective 
        glPopMatrix();
      glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  
  glPopMatrix();
  
  glEnable(GL_DEPTH_TEST);

}

menu::menu(Map_Settings &set, char * filename)
:set(set)
{
  
  ifstream f(filename) ;
  if (!f) 
    cerr <<" impossible d'ouvrir le fichier : " <<filename <<endl ;
  
  string s ;
  string ss;
  int taille;
  f>>size.x;
  f>>size.z;
  f >> taille ;
  nbpages= taille;
  pages=new page_t[taille];
  current =0;

  for (int i =0;i<taille;i++)
  {
	  int nbobj;
	  f>> nbobj;
	
	  pages[i].nb=nbobj;
    pages[i].p=new menu_obj_t[nbobj];
	
	  for (int j=0;j<nbobj;j++)
    {

		  f>>s;
		  if (s=="TITLE")
      {
			  f >> pages[i].p[j].obj.t.ul.x;
				f >> pages[i].p[j].obj.t.ul.z;
				f >> pages[i].p[j].obj.t.ur.x;
				f >> pages[i].p[j].obj.t.ur.z;
				f >> pages[i].p[j].obj.t.ll.x;
				f >> pages[i].p[j].obj.t.ll.z;
				f >> pages[i].p[j].obj.t.lr.x;
				f >> pages[i].p[j].obj.t.lr.z;
				f>>ss;
			
		    glGenTextures(1, &(pages[i].p[j].obj.t.tex));
				string path = "textures"PATHDELIM_S+ss;
		    if (tgaLoad((char *)path.c_str(),pages[i].p[j].obj.t.tex)!=1) cout <<"erreur";
			  f>>ss;
			  glGenTextures(1, &(pages[i].p[j].obj.t.mtex));
				path = "textures"PATHDELIM_S+ss;
		    if (tgaLoad((char *)path.c_str(),pages[i].p[j].obj.t.mtex)!=1) cout <<"mask erreur";
			  pages[i].p[j].type = TITLE_T;
  		}
	  	else if (s=="OPTION")
		  {
			  f >> pages[i].p[j].obj.o.text_c.x;
			  f >> pages[i].p[j].obj.o.text_c.z;
			  f >> pages[i].p[j].obj.o.val_c.x;
			  f >> pages[i].p[j].obj.o.val_c.z;
    		f >> pages[i].p[j].obj.o.c1.red;
        f >> pages[i].p[j].obj.o.c1.green;
			  f >> pages[i].p[j].obj.o.c1.blue;
			  f >> pages[i].p[j].obj.o.c2.red;
			  f >> pages[i].p[j].obj.o.c2.green;
			  f >> pages[i].p[j].obj.o.c2.blue;
  			char buf[255];
	    	f.getline(buf,255,'!');
   			pages[i].p[j].obj.o.text=new char[strlen(buf)+1];
	    	strcpy(pages[i].p[j].obj.o.text,buf);
				f >> ss;
			  if (ss=="Step_Size") 
			  {
				  pages[i].p[j].obj.o.val= STEPSIZE;
          pages[i].p[j].obj.o.pval= &(set.Step_Size);
				  
		  	}
			  else if (ss=="Map_Size") 
			  {
			  	pages[i].p[j].obj.o.val= MAPSIZE;
          pages[i].p[j].obj.o.pval= &(set.Map_Size);
				  
			  }
			  else if (ss=="Mat_Size")
			  {
  				pages[i].p[j].obj.o.val= MATSIZE;
          pages[i].p[j].obj.o.pval=&(set.Mat_Size);
	  			
		  	}
			  else if (ss=="Nb_Hills")
			  {
				  pages[i].p[j].obj.o.val= NBHILLS;
          pages[i].p[j].obj.o.pval= &(set.Nb_Hills);
				  
			  }
			  else if (ss=="Far_Max")
			  {
			    pages[i].p[j].obj.o.val= FARMAX;
          pages[i].p[j].obj.o.pval= &(set.Far_Max);				  
			  }
			  else if (ss=="Max_Radius")
			  {
				  pages[i].p[j].obj.o.val= MAXRADIUS;  
          pages[i].p[j].obj.o.pval=&(set.Max_Radius); 
          
			  }
			  else if (ss=="Min_Radius")
			  {
				  pages[i].p[j].obj.o.val= MINRADIUS;
          pages[i].p[j].obj.o.pval=&(set.Min_Radius); 
				  
			  }
			  else if (ss=="Map_Max_Height")
			  {
			  	pages[i].p[j].obj.o.val= MAPHEIGHT;
          pages[i].p[j].obj.o.pval= &(set.Map_Max_Height);				  
			  }
			  else if (ss=="Floor_Size") 
			  {
				  pages[i].p[j].obj.o.val= FLOORSIZE;
          pages[i].p[j].obj.o.pval= &(set.Floor_Size);				  
				  
			  }
    else if (ss=="Nbredflowers")  
    {
      pages[i].p[j].obj.o.val= NBREDFLOWERS;
      pages[i].p[j].obj.o.pval= &(set.o_cpt.nbredflowers);				  
    }
    else if (ss=="Nbsourcefood")  

    {
      pages[i].p[j].obj.o.val= NBSOURCEFOOD;
      pages[i].p[j].obj.o.pval= &(set.o_cpt.nbfoodsources);				  

 
    }
    else if (ss=="Nbfoodbysource") 
    {
      pages[i].p[j].obj.o.val= NBFOODBYSOURCE;
      pages[i].p[j].obj.o.pval= &(set.o_cpt.nb_food_by_source);				  
    }

    else if (ss=="Nbmushrooms")

    {
      pages[i].p[j].obj.o.val= NBMUSHROOMS;
                     pages[i].p[j].obj.o.pval= &(set.o_cpt.nbmushrooms);				  
    
    }
    else if (ss=="Nbspider")       
    {
      pages[i].p[j].obj.o.val= NBSPIDER; 
      pages[i].p[j].obj.o.pval= &(set.o_cpt.nbspiders);				 
    }

    else if (ss=="Coordnestx")    
    { 
      pages[i].p[j].obj.o.val= COORD_NEST_X;
      pages[i].p[j].obj.o.pval=&(set.o_cpt.coord_nest.x);
    
    }
    else if (ss=="Coordnestz")     
    {
      pages[i].p[j].obj.o.val= COORD_NEST_Z; 
      pages[i].p[j].obj.o.pval=&(set.o_cpt.coord_nest.z);
    }
		else if(ss=="nestwidth")
    {
      pages[i].p[j].obj.o.val= NEST_W; 
      pages[i].p[j].obj.o.pval=&(set.o_cpt.nest_width);

    }
    else if(ss=="nbgrass")
    {
      pages[i].p[j].obj.o.val= NBGRASS; 
      pages[i].p[j].obj.o.pval=&(set.o_cpt.nbgrass);

    }
			  pages[i].p[j].type = OPTION_T;
    
		
      }
		  else if ( s=="BLEND")
		  {
			  f >> pages[i].p[j].obj.bl.ul.x;
			  f >> pages[i].p[j].obj.bl.ul.z;
			  f >> pages[i].p[j].obj.bl.ur.x;
			  f >> pages[i].p[j].obj.bl.ur.z;
			  f >> pages[i].p[j].obj.bl.ll.x;
			  f >> pages[i].p[j].obj.bl.ll.z;
			  f >> pages[i].p[j].obj.bl.lr.x;
			  f >> pages[i].p[j].obj.bl.lr.z;
			  f >> pages[i].p[j].obj.bl.ul_color.red;
        f >> pages[i].p[j].obj.bl.ul_color.green;
        f >> pages[i].p[j].obj.bl.ul_color.blue;
        f >> pages[i].p[j].obj.bl.ul_color.alpha;
			  f >> pages[i].p[j].obj.bl.ur_color.red;
        f >> pages[i].p[j].obj.bl.ur_color.green;
        f >> pages[i].p[j].obj.bl.ur_color.blue;
        f >> pages[i].p[j].obj.bl.ur_color.alpha;
        
        
			  f >> pages[i].p[j].obj.bl.ll_color.red;
        f >> pages[i].p[j].obj.bl.ll_color.green;
        f >> pages[i].p[j].obj.bl.ll_color.blue;
        f >> pages[i].p[j].obj.bl.ll_color.alpha;
        
        
			  f >> pages[i].p[j].obj.bl.lr_color.red;
        f >> pages[i].p[j].obj.bl.lr_color.green;
        f >> pages[i].p[j].obj.bl.lr_color.blue;
        f >> pages[i].p[j].obj.bl.lr_color.alpha;
			   pages[i].p[j].type = BLEND_T;
		  }
		  else if ( s=="BUTTON")
		  {
			  f >> pages[i].p[j].obj.b.ul.x;
			  f >> pages[i].p[j].obj.b.ul.z;
			  f >> pages[i].p[j].obj.b.ur.x;
			  f >> pages[i].p[j].obj.b.ur.z;
  			f >> pages[i].p[j].obj.b.ll.x;
			  f   >> pages[i].p[j].obj.b.ll.z;
			  f >> pages[i].p[j].obj.b.lr.x;
  			f >> pages[i].p[j].obj.b.lr.z;
			  f >> pages[i].p[j].obj.b.sl.x;
	  		f >> pages[i].p[j].obj.b.sl.z;
		  	f >> pages[i].p[j].obj.b.sr.x;
			  f >> pages[i].p[j].obj.b.sr.z;

			  f>>ss;
		    
			  glGenTextures(1, &(pages[i].p[j].obj.b.tex[0]));
        string path = "textures"PATHDELIM_S+ss;
		   
			  tgaLoad((char *)path.c_str(),pages[i].p[j].obj.b.tex[0]) ;
  			f>>ss;
			
	  		glGenTextures(1, &(pages[i].p[j].obj.b.tex[1]));
        path = "textures"PATHDELIM_S+ss;
		    tgaLoad((char *)path.c_str(),pages[i].p[j].obj.b.tex[1]);
			  f>>ss;
		    
			  glGenTextures(1, &(pages[i].p[j].obj.b.mtex[0]));
        path = "textures"PATHDELIM_S+ss;
		   
			  tgaLoad((char *)path.c_str(),pages[i].p[j].obj.b.mtex[0]	);	   
			
			  f>>ss;
			
			  glGenTextures(1, &(pages[i].p[j].obj.b.mtex[1]));
        path = "textures"PATHDELIM_S+ss;
		    tgaLoad((char *)path.c_str(),pages[i].p[j].obj.b.mtex[1]);

			  pages[i].p[j].type = BUTTON_T;
	
			  f>>ss;
			
			
		  	pages[i].p[j].obj.b.np = 0;
			  if (ss=="END") pages[i].p[j].obj.b.act = END;
			  else if (ss=="START") pages[i].p[j].obj.b.act = START;
			  else if (ss=="CHANGEPAGE") 
			  {
				  pages[i].p[j].obj.b.act = CHANGEPAGE;
				  f>>pages[i].p[j].obj.b.np;
  			}
	  		else if (ss=="RESUME") pages[i].p[j].obj.b.act = RESUME;
		  	else if (ss=="BACKTOMAINMENU") pages[i].p[j].obj.b.act = BACKTOMAINMENU;
			  else if (ss=="SAVEOPTIONS") pages[i].p[j].obj.b.act = SAVEOPTIONS;
		  	else if (ss=="MENU_MAP") pages[i].p[j].obj.b.act = MENU_MAP;
		  	else if (ss=="PHEROMONE") pages[i].p[j].obj.b.act = PHEROMONE;
        else if(ss=="FOLLOWME") 
          pages[i].p[j].obj.b.act = FOLLOWME;
        else if (ss=="CENTERONNEST") pages[i].p[j].obj.b.act = CENTERONNEST;
        else if (ss=="BACKTOBEEVIEW")pages[i].p[j].obj.b.act = BACK_TO_BEE_VIEW;

      }
     }
   
}
			

}

Map_Settings MenuLoadOptions()
{
  ifstream f("Map.ini") ;

  if (!f) throw_error("Map.ini n'existe pas.") ;

  Map_Settings mSettings ;

  mSettings.Step_Size = DEFAULT_STEP_SIZE;
  mSettings.Map_Size = DEFAULT_MAP_SIZE;
  mSettings.Mat_Size = DEFAULT_MAT_SIZE;
  mSettings.Nb_Hills = DEFAULT_NB_HILLS;
  mSettings.Far_Max = DEFAULT_FAR_MAX;
  mSettings.Min_Radius = DEFAULT_MIN_RADIUS;
  mSettings.Max_Radius = DEFAULT_MAX_RADIUS;
  mSettings.Map_Max_Height = DEFAULT_MAP_MAX_HEIGHT;
  mSettings.Frame_Size.x = FRAME_SIZE_X;
  mSettings.Frame_Size.z = FRAME_SIZE_Y;
  mSettings.Floor_Size = DEFAULT_FLOOR_SIZE;
  mSettings.Flat = 1;


  string Param ;
  int valeur ;

  while (!f.eof())
  {
    f >> Param ;
    f >> valeur ;
    if      (Param=="Step_Size")      mSettings.Step_Size = valeur ;
    else if (Param=="Map_Size")       mSettings.Map_Size = valeur ;
    else if (Param=="Mat_Size")       mSettings.Mat_Size = valeur ;
    else if (Param=="Nb_Hills")       mSettings.Nb_Hills = valeur ;
    else if (Param=="Far_Max")        mSettings.Far_Max = valeur ;
    else if (Param=="Max_Radius")     mSettings.Max_Radius = valeur ;
    else if (Param=="Min_Radius")     mSettings.Min_Radius = valeur ;
    else if (Param=="Map_Max_Height") mSettings.Map_Max_Height = valeur ;
    else if (Param=="Frame_Size_x")   mSettings.Frame_Size.x = (valeur );
    else if (Param=="Frame_Size_z")   mSettings.Frame_Size.z = (valeur );
    else if (Param=="Floor_Size")     mSettings.Floor_Size = valeur ;
    else if (Param=="Flat")           mSettings.Flat = static_cast<unsigned char>(valeur) ;
    else if (Param=="Nbredflowers")   mSettings.o_cpt.nbredflowers = valeur;
    else if (Param=="Nbsourcefood")   mSettings.o_cpt.nbfoodsources = valeur;
    else if (Param=="Nbfoodbysource") mSettings.o_cpt.nb_food_by_source = valeur;
    else if (Param=="Nbmushrooms")    mSettings.o_cpt.nbmushrooms = valeur;
    else if (Param=="Nbspider")       mSettings.o_cpt.nbspiders = valeur;
    else if (Param=="Coordnestx")     mSettings.o_cpt.coord_nest.x = valeur;
    else if (Param=="Coordnestz")     mSettings.o_cpt.coord_nest.z = valeur;
    else if (Param=="nestwidth")      mSettings.o_cpt.nest_width = valeur;
    else if (Param=="nbgrass")        mSettings.o_cpt.nbgrass = valeur;



  }
  f.close() ;

  mSettings.Mat_Size = mSettings.Map_Size / mSettings.Step_Size ;
  mSettings.Frame_Size/=mSettings.Step_Size;

  return mSettings;
}


void menu::Solve_action(menu_obj_t o)
{

	switch(o.obj.b.act)
	{
	case END :
		SDL_Event nev;  
		nev.type = SDL_QUIT; 
		SDL_PushEvent(&nev);  
		break;
	case START :
		GetSoundManager().PlaySound(MENU_BCLIC_SOUND_ID, false);
		Main_Mode = MAP_MODE;
		objects_init(set);
		break;
	case CHANGEPAGE :
    
		current=o.obj.b.np;
		break;
	case BACKTOMAINMENU :
		Main_Mode = MENU_MODE;
		done_world(world);
		break;
	case RESUME :
		Main_Mode = MAP_MODE;
		break;
	case MENU_MAP :
		Main_Mode = MENU_MAP_MODE;
		break;
	case PHEROMONE :
		world->g_world_->change_rendered_token();
		break;

  case FOLLOWME :
    if (cCam.ViewMode()==SCROLL_VIEW) cCam.chfollow(1-cCam.gfollow());
    if (!world->g_world_->highlighted_object_)
       cCam.chfollow(0);
    
    break;

  case CENTERONNEST :
    world->map_->CenterOn(vect3f(set.o_cpt.coord_nest.x,0,set.o_cpt.coord_nest.z));
    break;
  case BACK_TO_BEE_VIEW :
    cCam.ChangeMode(BEE_VIEW,0,0);
    SDL_ShowCursor(false);
    break;

	

	case SAVEOPTIONS :
    ofstream f("Map.ini") ;
		f << "Step_Size "      << set.Step_Size << endl ;
		f << "Map_Size "       << set.Map_Size << endl ;
		f << "Mat_Size "       << set.Map_Size / set.Step_Size << endl ;
		f << "Nb_Hills "       << set.Nb_Hills << endl ;
		f << "Far_Max "        << set.Far_Max << endl ;
		f << "Max_Radius "     << set.Max_Radius << endl ;
		f << "Min_Radius "     << set.Min_Radius << endl ;
		f << "Map_Max_Height " << set.Map_Max_Height << endl ;
		f << "Frame_Size_x "   << set.Frame_Size.x*set.Step_Size << endl ;
		f << "Frame_Size_z "   << set.Frame_Size.z*set.Step_Size << endl ;
		f << "Floor_Size "     << set.Floor_Size << endl ;
		f << "Flat "           << static_cast<int>(set.Flat) << endl ;
    f << "Floor_Size "     << set.Floor_Size << endl ;
    f << "Nbredflowers "    << set.o_cpt.nbredflowers << endl ;
    f << "Nbsourcefood "    << set.o_cpt.nbfoodsources << endl ;
    f << "Nbfoodbysource "  << set.o_cpt.nb_food_by_source <<endl;
    f <<"Nbmushrooms "   << set.o_cpt.nbmushrooms  << endl;
    f <<"Nbspider "      << set.o_cpt.nbspiders  << endl;
    f <<"Coordnestx "    << set.o_cpt.coord_nest.x  << endl;
    f <<"Coordnestz "    << set.o_cpt.coord_nest.z  << endl;
    f <<"nestwidth "     <<set.o_cpt.nest_width << endl;
    f <<"nbgrass "       <<set.o_cpt.nbgrass <<endl;



		f.close();
		break;




	
	}

}

void menu::Change_Option(options_list opt,int add)
{
	switch (opt)
	{
	case STEPSIZE :
    if (add)
		set.Step_Size *= 2;
    else set.Step_Size /= 2;
		if (set.Step_Size>128) set.Step_Size = 4;
    else if (set.Step_Size<4) set.Step_Size = 128;
		break;
	case MAPSIZE :
    if (add)
		set.Map_Size *= 2;
    else set.Map_Size /= 2;
		if (set.Map_Size>16384) set.Map_Size = 128;
    else if ( set.Map_Size<128) set.Map_Size = 16384;
		set.Mat_Size = set.Map_Size / set.Step_Size;
		break;
	case NBHILLS :
    if (add)
		set.Nb_Hills+=10;
    else set.Nb_Hills-=10;
		if (set.Nb_Hills > 500 ) set.Nb_Hills = 0;
    else if (set.Nb_Hills<0) set.Nb_Hills = 500;
		break;
	case FLOORSIZE :
    if (add)
		set.Floor_Size+=10;
    else
      set.Floor_Size-=10;

		if (set.Floor_Size > 500 ) set.Floor_Size = 0;
    else if (set.Floor_Size < 0 ) set.Floor_Size = 500;
		break;
	case MAPHEIGHT :
		if (add)
      set.Map_Max_Height+=1;
    else set.Map_Max_Height-=1;
		if (set.Map_Max_Height > 200 ) set.Map_Max_Height = 1;
    else if (set.Map_Max_Height < 1 ) set.Map_Max_Height = 200;
		break;
	case MINRADIUS :
		if (add)
    {
		if (set.Min_Radius +1 < set.Max_Radius)
			set.Min_Radius+=1;
 		 
    }
    else 
    {
      if (set.Min_Radius)
      set.Min_Radius--;
    }
      
		break;
	case MAXRADIUS :
    if (add)
    {
		  set.Max_Radius+=1;
		  if (set.Max_Radius > 200 ) 
		  {
			  set.Max_Radius = 200;
			}
    }
    else
    {
      if (set.Min_Radius  < set.Max_Radius-1)
			set.Max_Radius--;
    }
		break;
  case  NBREDFLOWERS :
    if (add)
      set.o_cpt.nbredflowers++;
    else if (set.o_cpt.nbredflowers)
      set.o_cpt.nbredflowers--;
    break;
  case  NBSOURCEFOOD :
    if (add)
      set.o_cpt.nbfoodsources++;
    else if (set.o_cpt.nbfoodsources)
      set.o_cpt.nbfoodsources--;
    break;
  case NBFOODBYSOURCE :
    if (add)
      set.o_cpt.nb_food_by_source++;
    else if (set.o_cpt.nb_food_by_source)
      set.o_cpt.nb_food_by_source--;
    break;
  case NBGRASS :
    if (add)
      set.o_cpt.nbgrass++;
    else if (set.o_cpt.nbgrass)
      set.o_cpt.nbgrass--;
    break;
  case NBMUSHROOMS :
    if (add)
      set.o_cpt.nbmushrooms++;
    else if (set.o_cpt.nbmushrooms)
      set.o_cpt.nbmushrooms--;
    break;
  case NBSPIDER :
    if (add)
      set.o_cpt.nbspiders++;
    else if (set.o_cpt.nbspiders)
      set.o_cpt.nbspiders--;
    break;
  case COORD_NEST_X :
    if ((add)&&(set.o_cpt.coord_nest.x+1<(set.Map_Size-set.Map_Size/4)))
      set.o_cpt.coord_nest.x++;
    else if (set.o_cpt.coord_nest.x-1>(set.Map_Size/4))
      set.o_cpt.coord_nest.x--;
    break;
  case COORD_NEST_Z :
    if ((add)&&(set.o_cpt.coord_nest.z+1<(set.Map_Size-set.Map_Size/4)))
      set.o_cpt.coord_nest.z++;
    else if (set.o_cpt.coord_nest.z-1>(set.Map_Size/4))
      set.o_cpt.coord_nest.z--;
    break;
  case NEST_W :
    if ((add)&&(set.o_cpt.nest_width+1<50))
      set.o_cpt.nest_width++;
    else if (set.o_cpt.nest_width-1>30)
      set.o_cpt.nest_width--;
    break;

	}
}


bool menu::Menuio(int x, int y, int button)
{
  int i;
  page_t p=pages[current];
  menu_obj_t o;
  bool b = 0;
  x = x*size.x / TAILLEX;
  y = y*size.z / TAILLEY;
  for (i =0;i<p.nb;i++)
  {
    o = p.p[i];
	  switch (o.type)
	  {
	  case TITLE_T :
  		break;


	  case BUTTON_T :
		  if ((x>o.obj.b.sl.x)&&(x<o.obj.b.sr.x)
			&&(y>o.obj.b.sl.z)&&(y<o.obj.b.sr.z))
		{
			GetSoundManager().PlaySound(MENU_BCLIC_SOUND_ID, false);
        Solve_action(o);
        b=1;

			
		}

		break;

	case OPTION_T :
		option_t opt = o.obj.o;
		if ((x>opt.text_c.x)&&(x<opt.val_c.x)
			&&(y<opt.text_c.z)&&(y>opt.text_c.z-16))
		{
			GetSoundManager().PlaySound(MENU_OCLIC_SOUND_ID, false);
    if ((button == SDL_BUTTON_LEFT)||(button == SDL_BUTTON_WHEELUP))
		  Change_Option(opt.val,1);
    else Change_Option(opt.val,0);
    b=1;
		//	
		}

		break;
	}
  }
  return b;
	
}

bool menu::processevents()
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
			switch(Main_Mode)
			{
				case MENU_MAP_MODE :
					Main_Mode = MAP_MODE;
          SDL_ShowCursor(cCam.ViewMode() == SCROLL_VIEW);
				break;
				case MENU_MODE :
					return false;
					break;
			}
		  }
        else if (event.key.keysym.sym == SDLK_RETURN && event.key.keysym.mod & KMOD_LALT)
          scene_swapfullscreen();
    //    else
      //    io_clavier_synchr(event.key.keysym.sym, SDL_KEYDOWN);

        break;

  /*    case SDL_KEYUP:
        io_clavier_synchr(event.key.keysym.sym, SDL_KEYUP);
        break;
*/
      case SDL_VIDEORESIZE:
        io_reshape(event.resize.w, event.resize.h);
        break;
/*
      case SDL_MOUSEMOTION:
        io_motion(event.motion.xrel, event.motion.yrel);
        break;*/

      case SDL_QUIT:
        return false;

	  case SDL_MOUSEBUTTONUP:
		  Menuio(event.button.x,event.button.y,event.button.button);
		  break;

      default:
        break;
    }
  }
  // traiter les évenements clavier asynchrones (sans gestion des
  // évenements)

  return true;
}


menu::~menu()
{ 
 
  
  int taille=nbpages;
  for (int i =0;i<taille;i++)
  {
	int nbobj = pages[i].nb;
	for (int j=0;j<nbobj;j++)
	{
		if (pages[i].p[j].type==TITLE_T)
		{
			glDeleteTextures(1,&(pages[i].p[j].obj.t.tex));
		    glDeleteTextures(1, &(pages[i].p[j].obj.t.mtex));
		}
		else if (pages[i].p[j].type==OPTION_T)
		{
			delete[] pages[i].p[j].obj.o.text;
		}
		else if ( pages[i].p[j].type==BUTTON_T)
		{
			glDeleteTextures(1, &(pages[i].p[j].obj.b.tex[0]));
      glDeleteTextures(1, &(pages[i].p[j].obj.b.tex[1]));
      glDeleteTextures(1, &(pages[i].p[j].obj.b.mtex[0]));
      glDeleteTextures(1, &(pages[i].p[j].obj.b.mtex[1]));
		}
	}
	delete [] pages[i].p;
  }
  delete[] pages;
}
	
