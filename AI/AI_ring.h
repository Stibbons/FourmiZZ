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
#ifndef __AI_RING_H__
#define __AI_RING_H__

#include "AI_shared.h"

// anneau unilatéral avec taille, noeud courant et noeud précédent
// cet anneau construit des liens entre des instances de classe T
template<class T> class aiRing
{
  struct Link {
    Link* suc_ ;
    T* val_ ;
    Link(Link* s,T& v) : suc_(s), val_(&v) {}
  } ;

  uint size_ ; // nb d'éléments de l'anneau
  Link* current_ ; // pointeur sur le noeud courant
  Link* previous_ ; // pointeur sur le noeud précédent
public:
/******************\
* RING, public ... *
\******************/

/* contruction/destruction */

  // construit un anneau vide
  aiRing() ;
    
  // construit un anneau avec un élément
  aiRing(T&) ;
    
  // insère, avant l'élément courant, l'élément passé en paramètre
  void add(T&) ;
  
  // supprime le NOEUD courant et progresse dans l'anneau
  // Attention : ne détruit pas l'OBJET courant
  // Attention : si l'anneau est vide : PLANTE
  void remove_current() ;

  // supprime l'OBJET courant
  // supprime le NOEUD courant et progresse dans l'anneau
  // Attention : si l'anneau est vide : PLANTE
  void delete_current() ;

  // détruit l'anneau
  // Attention : ne supprime pas les OBJETS de la liste
  ~aiRing() ;

/* consultation */

  // retourne l'élément courant
  // Attention : si l'anneau est vide : PLANTE
  T* current() const ;

  // retourne le nombre d'éléments de l'anneau
  uint get_size() const ;

  // progresse dans l'anneau
  // Attention : si l'anneau est vide : PLANTE
  void next() ;


/******************\
* ... public, RING *
\******************/
} ; // aiRing

//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "AI_ring_impl.h"

#endif /* __AI_RING_H__ */

