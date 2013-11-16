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
#ifndef __MINIMAP_H__
#define __MINIMAP_H__

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "lib/tga.h"
#include "common.h"


struct minimap
{
  const class CMap& map_ ;
	const float ratio;
	const int ulx,uly,lrx,lry;

	void Render() ;
	unsigned int tex;
	minimap(CMap*, float,int,int,int,int);
	~minimap();
	bool Clic(int x, int y);
  void DeleteTextures();
};

// définitions inline

inline bool minimap::Clic(int x, int y)
{
	return( (x> ulx) && (x< lrx) && (y< lry) && (y> uly) );
}

inline void minimap::DeleteTextures()
{
  glDeleteTextures(1, &tex);
}





#endif
