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

#ifndef __BUTTONS_H__
#define __BUTTONS_H__




#define MENUX (TAILLEX/2 - 320)
#define MENUY (TAILLEY/2 - 240)

// Le TITRE

#define MENU_TITLE_L MENUX
#define MENU_TITLE_T (MENUY + 30)
#define MENU_TITLE_B (MENUY + 130)
#define MENU_TITLE_R (MENUX + 640)

// Boutton START

#define MENU_START_L MENUX
#define MENU_START_T (MENUY + 130)
#define MENU_START_B (MENUY + 230)
#define MENU_START_R (MENUX + 640)

// Boutton OPTIONS

#define MENU_OPTIONS_L MENUX
#define MENU_OPTIONS_T (MENUY + 230)
#define MENU_OPTIONS_B (MENUY + 330)
#define MENU_OPTIONS_R (MENUX + 640)

// Boutton QUIT

#define MENU_QUIT_L MENUX
#define MENU_QUIT_T (MENUY + 330)
#define MENU_QUIT_B (MENUY + 430)
#define MENU_QUIT_R (MENUX + 640)

// Les Options 
#define TEXT_X (MENUX + 180)
#define VALUE_X (MENUX + 466)
#define TEXT_OPTION_Y (MENUY + 210)
#define TEXT_MAPSIZE_Y (MENUY + 240)
#define TEXT_NBHILLS_Y (MENUY + 270)
#define TEXT_TRIANGLESIZE_Y (MENUY + 300)
#define TEXT_FLOORSIZE_Y (MENUY + 330)
#define TEXT_MAPHEIGHT_Y (MENUY + 360)
#define TEXTSIZE 10

typedef enum{
	START_B,
	OPTIONS_B,
	QUIT_B,
	MAPSIZE_B,
	TRIANGLESIZE_B,
	NBHILLS_B,
	FLOORSIZE_B,
	MAPHEIGHT_B
} button;
	
/*

int IsMouseHere(int x, int y, button b)
{
  switch (b)
	{
	case START_B : 
		return ( (x<MENU_START_R) && (x>MENU_START_L) && (y<MENU_START_B) && (y>MENU_START_T) );
		break; 
	case OPTIONS_B : 
		return ((x<MENU_OPTIONS_R)&&(x>MENU_OPTIONS_L)&&(y<MENU_OPTIONS_B)&&(y>MENU_OPTIONS_T));
		break;
    case QUIT_B : 
		return ((x<MENU_QUIT_R)&&(x>MENU_QUIT_L)&&(y<MENU_QUIT_B)&&(y>MENU_QUIT_T));
		break;
	case MAPSIZE_B : 
		return ( (y>TEXT_MAPSIZE_Y) && (y< TEXT_MAPSIZE_Y +TEXTSIZE) );
		break;
	case TRIANGLESIZE_B : 
		return ((y>TEXT_TRIANGLESIZE_Y)&&(y<TEXT_TRIANGLESIZE_Y+TEXTSIZE));
		break;
	case NBHILLS_B : 
		return ((y>TEXT_NBHILLS_Y)&&(y<TEXT_NBHILLS_Y+TEXTSIZE));
		break;
	case FLOORSIZE_B : 
		return ((y>TEXT_FLOORSIZE_Y)&&(y<TEXT_FLOORSIZE_Y+TEXTSIZE));
		break;
	case MAPHEIGHT_B : 
		return ((y>TEXT_MAPHEIGHT_Y)&&(y<TEXT_MAPHEIGHT_Y+TEXTSIZE));
		break;
	}
  return 0;
}
*/
#endif
