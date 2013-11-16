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
#ifndef __MOTOR_IMPL_H__
#define __MOTOR_IMPL_H__

inline CMotor::CMotor()
{}

inline void CMotor::Translate(const vect3f& v)
{
  //last_Translate_v += v ;
  glTranslatef(v.x,v.y,v.z) ;
}

inline void CMotor::UnTranslate()
{
  glTranslatef(-last_Translate_v.x,-last_Translate_v.y,-last_Translate_v.z) ;
  last_Translate_v = vect3f(0,0,0) ;
}

inline void CMotor::Rotate(const float a, const vect3f& v)
{
  last_Rotate_v += v ;
  glRotatef(a,v.x,v.y,v.z) ;
}

inline void CMotor::xRotate(const float a)
{
  last_Rotate_v.x += a ;
  glRotatef(a,1.0,0.0,0.0) ;
}

inline void CMotor::yRotate(const float a)
{
  last_Rotate_v.x += a ;
  glRotatef(a,0.0,1.0,0.0) ;
}

inline void CMotor::zRotate(const float a)
{
  last_Rotate_v.x += a ;
  glRotatef(a,0.0,0.0,1.0) ;
}

inline void CMotor::UnRotate()
{
  glRotatef(-last_Rotate_v.x,1.0,0.0,0.0) ;
  glRotatef(-last_Rotate_v.y,0.0,1.0,0.0) ;
  glRotatef(-last_Rotate_v.z,0.0,0.0,1.0) ;
}

inline CMotor::~CMotor()
{}

#endif /* __MOTOR_IMPL_H__ */

