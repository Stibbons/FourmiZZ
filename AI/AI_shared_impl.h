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
#ifndef __AI_SHARED_IMPL_H__
#define __AI_SHARED_IMPL_H__

#include <iostream>
#include <new>
#include <string>
#include <math.h>
#include "AI_shared.h"

/*
J'ai placé ici les définitions des méthodes inline afin
de ne pas surcharger l'en-tête AI_shared.h avec cette implémentation.
*/

inline colorRGBA::colorRGBA(const float r, const float g, const float b, const float a)
: red(r), green(g), blue(b), alpha(a)
{}

inline colorRGBA::colorRGBA()
: red(0.0f), green(0.0f), blue(0.0f), alpha(0.0f)
{}

#endif /* __AI_SHARED_IMPL_H__ */
