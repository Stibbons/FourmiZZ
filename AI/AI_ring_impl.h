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
#ifndef __AI_RING_IMPL_H__
#define __AI_RING_IMPL_H__

/*
J'ai placé ici les définitions des fonctions-membres template afin
de ne pas surcharger l'en-tête AI_ring.h avec cette implémentation.
*/

template<class T> inline aiRing<T>::aiRing()
: size_(0), current_(0), previous_(0)
{}

template<class T> inline aiRing<T>::aiRing(T& v)
: size_(1), current_(new Link(0,v)), previous_(current_)
{
  current_->suc_ = current ;
}

template<class T> void aiRing<T>::add(T& v)
{
  Link* l = new Link(current_,v) ;
  if (previous_) previous_->suc_ = l ; else {
    current_ = l ;
    l->suc_ = l ;
  }
  previous_ = l ;
  ++size_ ;
}

template<class T> void aiRing<T>::remove_current()
{
  if (size_>1) {
    previous_->suc_ = current_->suc_ ;
    delete current_ ;
    current_ = previous_->suc_ ;
  } else {
    delete current_ ;
    current_ = 0 ;
    previous_ = 0 ;
  }
  --size_ ;

}

template<class T> inline void aiRing<T>::delete_current()
{
  delete current_->val_ ;
  remove_current() ;
}

template<class T> inline T* aiRing<T>::current() const
{
  return current_->val_ ;
}

template<class T> inline uint aiRing<T>::get_size() const
{
  return size_ ;
}

template<class T> inline void aiRing<T>::next()
{
    previous_ = current_ ;
    current_ = current_->suc_ ;
}

template<class T> inline aiRing<T>::~aiRing()
{
  while (size_>0) remove_current() ; // pas optimisé, mais ça n'est pas crucial
}


#endif /* __AI_RING_IMPL_H__ */
