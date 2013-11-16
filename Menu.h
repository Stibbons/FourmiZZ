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
#ifndef __MENU_H__
#define __MENU_H__

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "lib/CCamera.h"
#include "lib/tga.h"
#include "common.h"
#include "Settings.h"
#include "lib/font.h"
#include "Buttons.h"
#include <stdio.h>
#include "AI/AI_shared_impl.h"
#include "Objects.h"
#include "Scene.h"
#include "SceneIO.h"
#include "sounds.h"
#include "Menu_type.h"

	



extern Mode_GL Main_Mode;
extern unsigned int Menu_Tex[7];
extern int Init;

/*void Draw_Menu(menu_t);
menu_t Load_Menu(Map_Settings *set,char * filename);

void Solve_Menu_Action(menu_t*,actions_list);
void Change_Option(options_list);


//void Draw_Options_menu(Map_Settings);
*/Map_Settings MenuLoadOptions();/*
bool Menuprocessevents(menu_t *menu,Map_Settings *set);

void Menuio(int x, int y, menu_t *menu,Map_Settings *m);*/
#endif
