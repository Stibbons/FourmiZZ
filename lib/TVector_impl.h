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
#ifndef __TVECTOR_IMPL_H__
#define __TVECTOR_IMPL_H__

// 180 / PI
#define RAD_TO_DEG            (57.295779513082320876798154814105f)

// TVector2

template<typename T> inline TVector2<T>::TVector2()
: x(0), z(0)
{ }

template<typename T> inline TVector2<T>::TVector2(const T fX, const T fZ)
: x(fX), z(fZ)
{ }

template<typename T> inline TVector2<T>::TVector2(const T v[])
: x(v[0]), z(v[1])
{}  

template<typename T> inline TVector2<T> TVector2<T>::operator+(const TVector2<T> &v) const
{
  return TVector2(v.x +x,v.z+z) ;
}

template<typename T> inline TVector2<T> TVector2<T>::operator-() const
{
  return TVector2(-x,-z);
}

template<typename T> inline TVector2<T> TVector2<T>::operator-(const TVector2<T> &v) const
{
  return TVector2(x-v.x,z-v.z);
}

template<typename T> inline TVector2<T> TVector2<T>::operator*(const T l) const
{
  return TVector2(l*x,l*z);
}

template<typename T> inline TVector2<T> TVector2<T>::operator/(const T l) const
{
  return TVector2(x/l,z/l);
}

template<typename T> inline TVector2<T>& TVector2<T>::operator+=(const TVector2<T>& v)
{
  x += v.x;
  z += v.z;
  return *this ;
}

template<typename T> inline TVector2<T>& TVector2<T>::operator*=(const T lambda)
{
  x *= lambda ;
  z *= lambda ;
  return *this ;
}

template<typename T> inline TVector2<T>& TVector2<T>::operator/=(const T lambda)
{
  x /= lambda ;
  z /= lambda ;
  return *this ;
}

template<typename T> inline TVector2<T>& TVector2<T>::operator-=(const TVector2<T>& v)
{
  x -= v.x ;
  z -= v.z ;
  return *this ; 
}

template<typename T> inline TVector2<T>& TVector2<T>::operator=(const TVector2<T>& v)
{
  x = v.x ;
  z = v.z ;
  return *this ;
}

template<typename T> inline bool TVector2<T>::operator==(const TVector2<T>& v) const
{
  return ((x==v.x)&&(z==v.z));
}

template<typename T> inline bool TVector2<T>::operator!=(const TVector2<T>& v) const
{
  return ((x!=v.x)||(z!=v.z));
}

template<typename T> inline T TVector2<T>::Sqr() const
{ 
 return x*x + z*z ;
}

template<typename T> inline float TVector2<T>::Norm() const
{ 
 return static_cast<float>(sqrt(x*x + z*z)) ;
}

template<typename T> inline TVector2<T>& TVector2<T>::Normalize()
{
  float n = Norm() ;
  if (n) {
    x /= n ;
    z /= n ;
  }
  return *this ;
}

template<typename T> inline unsigned int TVector2<T>::operator ()() const
{
  return (x || z) ;
}

template<typename T> inline T TVector2<T>::DotProd(const TVector2& v) const
{
  return x*v.x+z*v.z;
}


template<typename T> inline TVector2<T>::~TVector2()
{}

// TVector3

template<typename T> inline TVector3<T>::TVector3(const TVector2<T>& v, const T fY)
: TVector2<T>(v), y(fY)
{}

template<typename T> inline TVector3<T>::TVector3()
: y(0)
{ }

template<typename T> inline TVector3<T>::TVector3(const T fX, const T fY, const T fZ)
: TVector2<T>(fX,fZ), y(fY)
{ }

template<typename T> inline TVector3<T>::TVector3(const T v[])
: TVector2<T>(v[0],v[2]), y(v[1])
{}

template<typename T> inline TVector3<T> TVector3<T>::operator+(const TVector3<T> &v) const
{
  return TVector3(v.x + TVector2<T>::x, v.y + y, v.z + TVector2<T>::z) ;
}

template<typename T> inline TVector3<T> TVector3<T>::operator-() const
{
  return TVector3(-TVector2<T>::x,-y,-TVector2<T>::z);
}

template<typename T> inline TVector3<T> TVector3<T>::operator-(const TVector3<T> &v) const
{
  return TVector3(TVector2<T>::x-v.x,y-v.y,TVector2<T>::z-v.z);
}

template<typename T> inline TVector3<T> TVector3<T>::operator*(const T l) const
{
  return TVector3(l*TVector2<T>::x,l*y,l*TVector2<T>::z);
}

template<typename T> inline TVector3<T> TVector3<T>::operator/(const T l) const
{
  return TVector3(TVector2<T>::x/l,y/l,TVector2<T>::z/l) ;
}

template<typename T> inline TVector3<T>& TVector3<T>::operator+=(const TVector3<T>& v)
{
  TVector2<T>::x += v.x ;
  y += v.y ;
  TVector2<T>::z += v.z ;
  return *this ;
}

template<typename T> inline TVector3<T>& TVector3<T>::operator*=(const T lambda)
{
  TVector2<T>::x *= lambda ;
  y *= lambda ;
  TVector2<T>::z *= lambda ;
  return *this ;
}

template<typename T> inline TVector3<T>& TVector3<T>::operator/=(const T lambda)
{
  TVector2<T>::x /= lambda ;
  y /= lambda ;
  TVector2<T>::z /= lambda ;
  return *this ;
}

template<typename T> inline TVector3<T>& TVector3<T>::operator-=(const TVector3<T>& v)
{
  TVector2<T>::x -= v.x ;
  y -= v.y ;
  TVector2<T>::z -= v.z ;
  return *this ; 
}

template<typename T> inline TVector3<T>& TVector3<T>::operator=(const TVector3<T>& v)
{
  TVector2<T>::x = v.x ;
  y = v.y ;
  TVector2<T>::z = v.z ;
  return *this ;
}

template<typename T> inline bool TVector3<T>::operator==(const TVector3<T>& v) const
{
  return ((TVector2<T>::x==v.x)&&(y==v.y)&&(TVector2<T>::z==v.z)) ;
}

template<typename T> inline bool TVector3<T>::operator!=(const TVector3<T>& v) const
{
  return !operator==(v) ;
}

template<typename T> inline T TVector3<T>::Sqr() const
{ 
 return TVector2<T>::x*TVector2<T>::x + y*y + TVector2<T>::z*TVector2<T>::z ;
}

template<typename T> inline float TVector3<T>::Norm() const
{ 
 return static_cast<float>(sqrt(Sqr())) ;
}

template<typename T> inline TVector3<T>& TVector3<T>::Normalize()
{
  float n = Norm() ;
  if (n) {
    TVector2<T>::x /= n ;
    y /= n ;
    TVector2<T>::z /= n ;
  }
  return *this ;
}

template<typename T> inline unsigned int TVector3<T>::operator ()() const
{
  return (TVector2<T>::x || y || TVector2<T>::z) ;
}

template<typename T> inline TVector3<T>::~TVector3()
{}

template<typename T> inline T TVector3<T>::DotProd(const TVector3& v) const
{
  return TVector2<T>::x*v.x+y*v.y+TVector2<T>::z*v.z;
}

template<typename T> inline TVector3<T> TVector3<T>::CrossProd(const TVector3& v) const
{
  return TVector3(y*v.z-TVector2<T>::z*v.y,
                  TVector2<T>::z*v.x-TVector2<T>::x*v.z,
                  TVector2<T>::x*v.y-y*v.x);
}


// autres

inline float dot_product(const vect2f& v1, const vect2f& v2)
{
  return v1.DotProd(v2) ;
}

inline float dot_product(const vect3f& v1, const vect3f& v2)
{
  return v1.DotProd(v2) ;
}

inline vect3f cross_product(const vect3f& v1, const vect3f& v2)
{
  return v1.CrossProd(v2) ;
}

inline float rad_angle(const vect2f& v)
{
  return static_cast<float>(acos(v.z)) * (v.x<0 ? -1 : +1) ;
}

inline float deg_angle(const vect2f& v)
{
  return rad_angle(v) * RAD_TO_DEG ;
}

#endif /* __TVECTOR_IMPL_H__ */
