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
#ifndef __BOUNDINGBOX_H__HEADER__
#define __BOUNDINGBOX_H__HEADER__

#define BOUNDINGBOX_POINTS 8

/* class CBoundingBox ********************************************************\
 * Gestion des bounding box (permet de gerer les collisions)                 *
\*****************************************************************************/
class CBoundingBox
{
public:
  CBoundingBox()
    : m_active(false)
  {
  }
  CBoundingBox& operator=(const CBoundingBox &d)
  {
    m_active = d.m_active;
    memcpy(m_point, d.m_point, sizeof(d.m_point));
    return *this;
  }

  bool  m_active;
  float m_point[BOUNDINGBOX_POINTS][3];
};

#endif
