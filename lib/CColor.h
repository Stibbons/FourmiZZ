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
#ifndef __CCOLOR_HEADER__
#define __CCOLOR_HEADER__


// template de classe couleur
// 3 : RGB
// 4 : RGBA

template <typename T> class TColor3
{
  // il est très important que les 3 valeurs soient contigues en mémoire
  // et dans cet ordre.
  T     m_red,
        m_green,
        m_blue;

public:
  TColor3() : m_red(0), m_green(0), m_blue(0)  { }
  TColor3(T red, T green, T blue) : 
                   m_red(red), m_green(green), m_blue(blue) { }
  TColor3(T red, T green, T blue, T alpha) : 
                   m_red(red), m_green(green), m_blue(blue) { }

  inline TColor3& operator=(TColor3& c);

  inline void SetColor(const TColor3 &col );
  inline void SetColor(T red, T green, T blue );

  inline const T Red()   const {  return m_red;   }
  inline const T Green() const {  return m_green; }
  inline const T Blue()  const {  return m_blue;  }

  inline const float* RGBVector()  const { return &m_red; }


  inline void SetRed  ( T col ) {  m_red   = col; }
  inline void SetGreen( T col ) {  m_green = col; }
  inline void SetBlue ( T col ) {  m_blue  = col; }
};



template <typename T> class TColor4
{
  // il est très important que les 4 valeurs soient contigues en mémoire
  // et dans cet ordre.
  T     m_red,
        m_green,
        m_blue,
        m_alpha;

public:
  TColor4() : m_red(0), m_green(0), m_blue(0), m_alpha(0)  { }
  TColor4(T red, T green, T blue) : 
                   m_red(red), m_green(green), m_blue(blue), m_alpha(0)     { }
  TColor4(T red, T green, T blue, T alpha) : 
                   m_red(red), m_green(green), m_blue(blue), m_alpha(alpha) { }

  inline TColor4& operator=(TColor4& c);

  inline void SetColor(const TColor4 &col );
  inline void SetColor(T red, T green, T blue );
  inline void SetColor(T red, T green, T blue, T alpha);

  inline const T Red()   const {  return m_red;   }
  inline const T Green() const {  return m_green; }
  inline const T Blue()  const {  return m_blue;  }
  inline const T Alpha() const {  return m_alpha; }

  inline const float* RGBVector()  const { return &m_red; }
  inline const float* RGBAVector() const { return &m_red; }


  inline void SetRed  ( T col ) {  m_red   = col; }
  inline void SetGreen( T col ) {  m_green = col; }
  inline void SetBlue ( T col ) {  m_blue  = col; }
  inline void SetAlpha( T col ) {  m_alpha = col; }
};





typedef TColor4<float> CColor4f;
typedef TColor4<unsigned int> CColor4u;

typedef TColor3<float> CColor3f;
typedef TColor3<unsigned int> CColor3u;


// implémentation des fonctions inline //


// Couleur RGB
template <typename T> inline TColor3<T>& TColor3<T>::operator=(TColor3& c)
{
  m_red   = c.m_red;
  m_green = c.m_green;
  m_blue  = c.m_blue;
  return *this;
}

template <typename T> inline void TColor3<T>::SetColor(const TColor3 &col )
{
  m_red   = col.m_red;
  m_green = col.m_green;
  m_blue  = col.m_blue;
}

template <typename T> inline void TColor3<T>::SetColor(T red, T green, T blue )
{
  m_red   = red;
  m_green = green;
  m_blue  = blue;
}

// Couleur RGBA
template <typename T> inline TColor4<T>& TColor4<T>::operator=(TColor4& c)
{
  m_red   = c.m_red;
  m_green = c.m_green;
  m_blue  = c.m_blue;
  m_alpha = c.m_alpha;
  return *this;
}

template <typename T> inline void TColor4<T>::SetColor(const TColor4 &col )
{
  m_red   = col.m_red;
  m_green = col.m_green;
  m_blue  = col.m_blue;
  m_alpha = col.m_alpha;
}

template <typename T> inline void TColor4<T>::SetColor(T red, T green, T blue )
{
  m_red   = red;
  m_green = green;
  m_blue  = blue;
}
template <typename T> inline void TColor4<T>::SetColor(T red, T green, T blue, T alpha)
{
  m_red   = red;
  m_green = green;
  m_blue  = blue;
  m_alpha = alpha;
}



#endif /*__CCOLOR_HEADER__*/
