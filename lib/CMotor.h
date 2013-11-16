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
#ifndef __MOTOR_H__
#define __MOTOR_H__

#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "TVector.h"

extern class CMotor MyMotor ;

class CMotor
{
public:
  CMotor() ;
  void Translate(const vect3f&) ;
  void UnTranslate() ;
  void Rotate(const float, const vect3f&) ;
  void xRotate(const float) ;
  void yRotate(const float) ;
  void zRotate(const float) ;
  void UnRotate() ;
  ~CMotor() ;

private:
  vect3f last_Translate_v ;
  vect3f last_Rotate_v ;
} ;

//petite astuce pour avoir les définitions inline sans pour autant surcharger ce fichier en-tête
#include "CMotor_impl.h"

#endif /* __MOTOR_H__ */
