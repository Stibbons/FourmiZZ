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
#ifndef __FOURMI_SCENE_IO_H__HEADER__
#define __FOURMI_SCENE_IO_H__HEADER__

#include "common.h"
#include "Objects.h"

extern float fourmipos;

bool io_processevents( void );
void io_reshape(int w, int h);
void io_active(int x, int y);
void io_motion (int x, int y, int xrel, int yrel);
void io_clavier_asynchr();
void io_clavier_synchr(SDLKey key, Uint8 event);
void io_mouse(int button, int x, int y);
#endif // __FOURMI_SCENE_IO_H__HEADER__
