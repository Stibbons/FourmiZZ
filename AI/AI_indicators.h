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
#ifndef __AI_INDICATORS_H__
#define __AI_INDICATORS_H__

#include "AI_shared.h"

class aiTime
{
  ulong value_ ; // "date"

public:

  // construit une "date"
  aiTime() ;

  // retourne la "date"
  const ulong& operator()() const ;

  // incr�mente la "date" de 1
  void operator ++() ;
} ;


class aiCounter
{
  const uint reset_ ;
  uint value_ ;

public:

  // construit le compteur de 1 � n
  // ("taille" du compteur > +1, bool�en : initialiser avec une valeur al�atoire?)
  aiCounter(const uint, const bool) ;

  // incr�mente le compteur, puis
  // retourne true si le compteur a atteint la valeur sp�cifi�e.
  // dans ce cas, le compteur est r�initialis�.
  bool operator()() ;
} ;

// coordonn�es pond�r�es
struct aiToken
{
  uint amount_ ;
  vect2f where_ ;

  // construit un Token "vide"
  aiToken() ;

  // ajoute des coordonn�es pond�r�es
  void add(const uint, const vect2f&) ;

  // ajoute des coordonn�es pond�r�es � travers un autre token
  void operator +=(const aiToken&) ;

} ;

//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_indicators_impl.h"

#endif /* __AI_INDICATORS_H__ */

