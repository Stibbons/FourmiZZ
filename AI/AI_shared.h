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
#ifndef __AI_SHARED_H__
#define __AI_SHARED_H__

#include <string>
#include "../lib/TVector.h"

// 2*PI
#define PIx2                  (6.283185307179586476925286766559)

// entier aléatoire de 0..X-1
#define UINT_RAND(X)          (uint(X * (float(rand()) / (RAND_MAX+1))))

// réel aléatoire de [0,1[
#define FLOAT_RAND            (float(rand()) / (RAND_MAX+1))

// réel aléatoire de ]-1,1]
#define SYM_RAND              (1 - 2 * float(rand()) / (RAND_MAX+1))

// effectue un test aléatoire
#define ROLL(X)               (FLOAT_RAND <= X)

// donne la valeur absolue
#define ABS(X)                ((X)>0?(X):-(X))

// donne le carré
#define SQR(X)                ((X)*(X))

typedef unsigned int uint ;

typedef unsigned long ulong ;

struct colorRGBA
{
  float red, green, blue, alpha ;

  // construit une couleur
  colorRGBA(const float, const float, const float, const float) ;

  // construit la couleur noire
  colorRGBA() ;
} ;

// permet d'afficher un message clair quand la mémoire est épuisée.
void out_of_memory() ;

// affiche un warning
void throw_warning(const std::string s) ;

// affiche une erreur et termine le programme avec l'exit code 1.
void throw_error(const std::string s) ;

//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_shared_impl.h"

#endif /* __AI_SHARED_H__ */
