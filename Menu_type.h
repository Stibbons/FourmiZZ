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
#ifndef __MENU_TYPE_H__
#define __MENU_TYPE_H__



typedef enum {
  MENU_MODE,
  MAP_MODE,
  MENU_MAP_MODE
} Mode_GL;

typedef enum {
	STEPSIZE,
	MAPSIZE,
	MATSIZE,
	NBHILLS,
	FARMAX,
	MAXRADIUS,
	MINRADIUS,
	MAPHEIGHT,
	FLOORSIZE,
  NBREDFLOWERS,
  NBSOURCEFOOD,
  NBFOODBYSOURCE,
  NBGRASS,
  NBMUSHROOMS,
  NBSPIDER,
  COORD_NEST_X,
  COORD_NEST_Z,
  NEST_W



} options_list;


typedef enum {
	END,
	START,
	CHANGEPAGE,
	SAVEOPTIONS,
	RESUME,
	BACKTOMAINMENU,
	MENU_MAP,
	PHEROMONE,
  FOLLOWME,
  CENTERONNEST,
  BACK_TO_BEE_VIEW 
} actions_list;

typedef enum {
	BUTTON_T,
	OPTION_T,
	TITLE_T,
	BLEND_T
} menu_types_t;

struct color_rgb {
  float red;
  float green;
  float blue;
};

struct color_rgba : public color_rgb{
  float alpha;
};


struct point{
  unsigned int x,z;
};



struct button_t{
  
	point ul,ll,ur,lr,sl,sr;
	unsigned int tex[2];
	unsigned int mtex[2];
	actions_list act;
	int np;
} ;

struct title_t{
  
	point ul,ur,ll,lr;
	unsigned int tex,mtex;
};

struct blend_t {
  
	point ul,ur,ll,lr;
	color_rgba ul_color,ur_color,ll_color,lr_color;
} ;

struct option_t {
	
  point val_c,text_c;
	char * text;
	color_rgb c1,c2;
	options_list val;
  int * pval;
	
} ;


union type_t{
  
  button_t b;
	option_t o;
	title_t t;
	blend_t bl;
} ;


struct menu_obj_t{
  union type_t obj;
	menu_types_t type;
} ;

typedef struct{
  menu_obj_t * p;
	int nb;
}page_t;



class menu {
public :
  
// Constructeur 
  menu(Map_Settings &set,char *filename);

// Affichage du menu
  void Render();

// Changer une option
  void Change_Option(options_list,int);

// Effectuer une action ( clic sur un bouton )
  void Solve_action(menu_obj_t );

// Gérer les entrées/sorties
  bool processevents();

// Entrée / sortie en fonction de la souris
  bool Menuio( int x, int y ,int button);
  
// Destructeur     
   ~menu();

private :

// Un menu est une liste de pages
  page_t * pages;

// numéro de la page courante
  unsigned int current;

// Taille du menu ( pour l'orthoview )
  vect2u size;
  
// nombre de pages
  unsigned int nbpages;

// Pointeur vers la map_settings qui correspond
  Map_Settings  &set;
};


















#endif
