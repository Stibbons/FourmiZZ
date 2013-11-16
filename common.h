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
#ifndef __FOURMIS_COMMON_H__HEADER__
#define __FOURMIS_COMMON_H__HEADER__


#include <stdio.h>
#include "SDL/SDL.h"

#ifndef WIN32
typedef unsigned char BOOL ;
#else
#include <windows.h>
#endif

#ifndef WIN32
typedef unsigned int UINT ;
#endif

#ifdef GCCWIN
typedef unsigned int UINT;
#endif

#ifndef M_PI
    #define M_PI 3.1415926535897932384626433832795f
    #define M_PIDIV2 1.5707963267948966192313216916398f
#endif

#define DEGTORAD 0.017453292519943295769236907684886f
 // == M_PI / 180.0f

#define RADTODEG 57.295779513082320876798154814105f
 // == 180 / PI;

#ifdef WIN32
  #define PATHDELIM_C '\\' 
  #define PATHDELIM_S "\\"
#else
  #define PATHDELIM_C '/' 
  #define PATHDELIM_S "/"
#endif


#endif //__FOURMIS_COMMON_H__HEADER__
