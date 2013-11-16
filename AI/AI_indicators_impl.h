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
#ifndef __AI_INDICATORS_IMPL_H__
#define __AI_INDICATORS_IMPL_H__

#include "AI_shared.h"

// METHODES DE aiTime

inline aiTime::aiTime()
: value_(0)
{}

inline const ulong& aiTime::operator()() const
{
  return value_ ;
}

inline void aiTime::operator ++()
{
  ++value_ ;
}

// METHODES DE aiCounter

// le code n'est pas celui d'un compteur mais d'un décompteur
// de plus, le décompte se fait de fait de n-1 à 0 plutôt que de n à 1
// le résultat est le même, mais le code est un chouilla + simple

inline aiCounter::aiCounter(const uint nr, const bool r)
: reset_(nr), value_(r?UINT_RAND(nr)+1:reset_)
{}

inline bool aiCounter::operator()()
{
  if (--value_)
    return false ;
  else {
    value_ = reset_ ;
    return true ;
  }
}

// METHODES DE aiToken

inline aiToken::aiToken()
: amount_(0), where_(0,0)
{}

inline void aiToken::add(const uint x, const vect2f& c)
{
  //uint sum ;
  if (uint sum = amount_ + x) {
    where_ *= (static_cast<float>(amount_) / sum) ;
    where_ += c * (static_cast<float>(x) / sum) ;
    amount_ = sum ;
  }
}

inline void aiToken::operator +=(const aiToken& t)
{
  add(t.amount_, t.where_) ;
}


#endif /* __AI_INDICATORS_IMPL_H__ */
