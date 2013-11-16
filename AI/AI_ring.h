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
#ifndef __AI_RING_H__
#define __AI_RING_H__

#include "AI_shared.h"

// anneau unilat�ral avec taille, noeud courant et noeud pr�c�dent
// cet anneau construit des liens entre des instances de classe T
template<class T> class aiRing
{
  struct Link {
    Link* suc_ ;
    T* val_ ;
    Link(Link* s,T& v) : suc_(s), val_(&v) {}
  } ;

  uint size_ ; // nb d'�l�ments de l'anneau
  Link* current_ ; // pointeur sur le noeud courant
  Link* previous_ ; // pointeur sur le noeud pr�c�dent
public:
/******************\
* RING, public ... *
\******************/

/* contruction/destruction */

  // construit un anneau vide
  aiRing() ;
    
  // construit un anneau avec un �l�ment
  aiRing(T&) ;
    
  // ins�re, avant l'�l�ment courant, l'�l�ment pass� en param�tre
  void add(T&) ;
  
  // supprime le NOEUD courant et progresse dans l'anneau
  // Attention : ne d�truit pas l'OBJET courant
  // Attention : si l'anneau est vide : PLANTE
  void remove_current() ;

  // supprime l'OBJET courant
  // supprime le NOEUD courant et progresse dans l'anneau
  // Attention : si l'anneau est vide : PLANTE
  void delete_current() ;

  // d�truit l'anneau
  // Attention : ne supprime pas les OBJETS de la liste
  ~aiRing() ;

/* consultation */

  // retourne l'�l�ment courant
  // Attention : si l'anneau est vide : PLANTE
  T* current() const ;

  // retourne le nombre d'�l�ments de l'anneau
  uint get_size() const ;

  // progresse dans l'anneau
  // Attention : si l'anneau est vide : PLANTE
  void next() ;


/******************\
* ... public, RING *
\******************/
} ; // aiRing

//petite astuce pour avoir les d�finitions inline sans pour autant surcharger ce fichier en-t�te
#include "AI_ring_impl.h"

#endif /* __AI_RING_H__ */

