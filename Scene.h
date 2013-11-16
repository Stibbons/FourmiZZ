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
#ifndef __FOURMI_SCENE_H__HEADER__
#define __FOURMI_SCENE_H__HEADER__

#include "common.h"
#include "lib/font.h"

extern int g_fullscreen;


void scene_init(int width, int height );
void scene_draw(  );
int scene_draw_thread(void * nothing);
bool scene_swapfullscreen();


extern int g_draw_thread_active;
extern int g_swapfullscreen;


#endif //__FOURMI_SCENE_H__HEADER__
