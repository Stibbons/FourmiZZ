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
#ifndef __MATERIAL__HEADER__
#define __MATERIAL__HEADER__


struct tLightMaterial
{
  float diffuse[4];
  float ambiant[4];
  float specular[4];
  tLightMaterial(float dR, float dG, float dB, float bT, 
            float aR, float aG, float aB, float aT,
            float sR, float sG, float sB, float sT)
  {
    #define R 0
    #define G 1
    #define B 2
    #define T 3

    diffuse[R] = dR;
    diffuse[G] = dG;
    diffuse[B] = dB;
    diffuse[T] = bT;
    ambiant[R] = aR;
    ambiant[G] = aG;
    ambiant[B] = aB;
    ambiant[T] = aT;
    specular[R] = sR;
    specular[G] = sG;
    specular[B] = sB;
    specular[T] = sT;

    #undef R
    #undef G
    #undef B
    #undef T
  }
  tLightMaterial()  { };

};


#endif /* __MATERIAL__HEADER__ */
