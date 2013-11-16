/********************************************************************************
    Foumi-ZZ - Simulation de Fourmili�re ISIMA Premi�re Ann�e
    Copyright (C) 2003 DANVIN Yohan, FARGE Julien, GOUBE Florent, SEMET Ga�tan
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

// entier al�atoire de 0..X-1
#define UINT_RAND(X)          (uint(X * (float(rand()) / (RAND_MAX+1))))

// r�el al�atoire de [0,1[
#define FLOAT_RAND            (float(rand()) / (RAND_MAX+1))

// r�el al�atoire de ]-1,1]
#define SYM_RAND              (1 - 2 * float(rand()) / (RAND_MAX+1))

// effectue un test al�atoire
#define ROLL(X)               (FLOAT_RAND <= X)

// donne la valeur absolue
#define ABS(X)                ((X)>0?(X):-(X))

// donne le carr�
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

// permet d'afficher un message clair quand la m�moire est �puis�e.
void out_of_memory() ;

// affiche un warning
void throw_warning(const std::string s) ;

// affiche une erreur et termine le programme avec l'exit code 1.
void throw_error(const std::string s) ;

//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_shared_impl.h"

#endif /* __AI_SHARED_H__ */
