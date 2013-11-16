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
#ifndef __TVECTOR_H__
#define __TVECTOR_H__

#include <math.h>

// couple de T
// intéressant si sizeof(T)<=sizeof(void*)
template <typename T> class TVector2
{
public:

  // composantes du vecteur
  T x, z ;

  // construit le vecteur nul
  TVector2() ;

  // construit le vecteur à partir de deux T
  TVector2(const T, const T);

  // construit le vecteur à partir d'un tableau de 2 T
  TVector2(const T[]) ;

  unsigned int operator ()() const ;
  TVector2 operator+(const TVector2 &v) const ;
  TVector2 operator-() const ;
  TVector2 operator-(const TVector2 &v) const ;
  TVector2 operator*(const T l) const ;
  TVector2 operator/(const T l) const ;
  TVector2& operator+=(const TVector2 &v) ;
  TVector2& operator*=(const T lambda) ;
  TVector2& operator/=(const T lambda) ;
  TVector2& operator-=(const TVector2 &v) ;
  TVector2& operator=(const TVector2 &v) ;
  TVector2& operator=(const T &t) { x = t; z = t; return *this; } ;
  bool operator==(const TVector2 &v) const ;
  bool operator!=(const TVector2 &v) const ;
  T Sqr() const ;
  float Norm() const ;
  T DotProd(const TVector2& v)  const;
  TVector2& Normalize() ;
  ~TVector2() ;
};

// triplet de T
// intéressant si sizeof(T)<=sizeof(void*)
template <typename T> class TVector3 : public TVector2<T>
{
public:

  T y ;

  // construit le vecteur nul
  TVector3() ;
  
  // construit le vecteur à partir de trois
  TVector3(const T fX, const T fY, const T fZ) ;

  // construit le vecteur à partir de deux T
  TVector3(const TVector2<T>&, const T);

  // construit le vecteur à partir d'un tableau de 3 T
  TVector3(const T[]) ;

  unsigned int operator ()() const ;
  TVector3 operator+(const TVector3 &v) const ;
  TVector3 operator-() const ;
  TVector3 operator-(const TVector3 &v) const ;
  TVector3 operator*(const T l) const ;
  TVector3 operator/(const T l) const ;
  TVector3& operator+=(const TVector3 &v) ;
  TVector3& operator*=(const T lambda) ;
  TVector3& operator/=(const T lambda) ;
  TVector3& operator-=(const TVector3 &v) ;
  TVector3& operator=(const TVector3 &v) ;
  TVector2<T>& operator=(const T &t) { TVector2<T>::operator=(t); y = t; return *this; }
  bool operator==(const TVector3 &v) const ;
  bool operator!=(const TVector3 &v) const ;
  T Sqr() const ;
  float Norm() const ;
  TVector3& Normalize() ;
  T DotProd(const TVector3& v)  const;
  TVector3 CrossProd(const TVector3& v) const;


  ~TVector3() ;

};

 
typedef TVector2<int>            vect2i ;
typedef TVector2<unsigned int>   vect2u ;
typedef TVector2<float>          vect2f ;
typedef TVector3<float>          vect3f ;

typedef TVector3<int>          CVector3i;
typedef TVector3<float>        CVector3f;
typedef TVector3<unsigned int> CVector3u;
typedef TVector3<int>          CVector3i;

typedef TVector2<int>          CVector2i;
typedef TVector2<float>        CVector2f;
typedef TVector2<unsigned int> CVector2u;
typedef TVector2<int>          CVector2i;


// retourne l'angle xÔz en radians du vecteur spécifié
float rad_angle(const vect2f& v) ;

// retourne l'angle xÔz en degrés du vecteur spécifié
float deg_angle(const vect2f& v) ;

// produits scalaires
float dot_product(const vect2f& v1, const vect2f& v2) ;
float dot_product(const vect3f& v1, const vect3f& v2) ;

// produit vectoriel
vect3f cross_product(const vect3f& v1, const vect3f& v2) ;

#include "TVector_impl.h"

#endif /* __TVector_H__ */

