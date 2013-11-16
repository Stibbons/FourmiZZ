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
#ifndef __CCAMERA_H__
#define __CCAMERA_H__

#ifdef WIN32
	#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include "TVector.h"

#define TAILLEX 800
#define TAILLEY 600
#define SCROLL_HEIGHT 100


typedef enum {
  BEE_VIEW,
  SCROLL_VIEW
} CModeCamera;

// La classe camera
class CCamera
{
public :

  // Constructeur et destructeur de base
  CCamera() {}
  ~CCamera() {}

  // Un constructeur qui permet d'initialiser directement la camera
  // (position, Vue, normale)
  CCamera(float pX, float pY, float pZ,
          float vX, float vY, float vZ,
          float nX, float nY, float nZ,
          float sX, float sY, float sZ)
  {
    vNormal.x = nX;
    vNormal.y = nY;
    vNormal.z = nZ;
    vView.x = vX;
    vView.y = vY;
    vView.z = vZ;
    vPosition.x = pX;
    vPosition.y = pY;
    vPosition.z = pZ;
    vStrafe.x = sX;
    vStrafe.y = sY;
    vStrafe.z = sZ;
    ScrollHeight = SCROLL_HEIGHT;
    follow = 0;
  }

  // Un autre constructeur avec des vecteurs
  CCamera(CVector3f pos, CVector3f v, CVector3f norm, CVector3f straf)
  {
    vNormal = norm;
    vPosition = pos;
    vView = v;
    vStrafe = straf;
    ScrollHeight = SCROLL_HEIGHT;
    follow = 0;
  }
 
  CVector3f Position() { return vPosition; }
  CVector3f View() { return vView; }
  CVector3f Normal() { return vNormal; }
  CVector3f Strafe() { return vStrafe; }
  CModeCamera ViewMode() { return mViewMode; }
  int gfollow(){ return follow; }
  void chfollow(int i) { follow = i; }

// Modification de la camera : deux methodes, une avec des reels l'autre avec des vecteurs
  void PositionCamera3fv(const CVector3f& p, const CVector3f& v,
                         const CVector3f& n, const CVector3f& s);
  void PositionCameraf(float pX, float pY, float pZ,
          float vX, float vY, float vZ,
          float nX, float nY, float nZ,
          float sX, float sY, float sZ);
  void vPositionCamera(const CVector3f&) ;

// Mise a jour de la camera
  void Update();

// Modifie le ScrollHeight
  void NewScrollHeight(float a) { ScrollHeight = a;}

// Mise a jour de la vue 
  void Look();

// Monter/Descendre la caméra
  void ScrollUp(float speed);

// Deplacement avant et arriere de la camera
  void MoveCamera(float speed);

// Deplacement lateral de la camera
  void StrafeCamera(float speed);

// Deplacement de la camera en mode scrolling 
  void ScrollCamera(float speed,CVector3f Way);
// Changement du mode de la camera
  void ChangeMode(CModeCamera mode,float x, float y);
  
// Faire tourner la vue autour d'un point, selon l'angle passe en parametre
  void RotateView(float angle, float fX, float fY, float fZ);

  void  RotateAround(CVector3f vCenter,float fangle, float fX, float fY, float fZ);
/////////////////////////////////////
// DEPEND DE LA LIBRAIRIE UTILISEE //
// PEUT ETRE A MODIFIER            //
/////////////////////////////////////
//  Ajuster la vue a la souris
  void SetMouseView(int PosX, int PosY);

  float GetScrollHeight() const {return ScrollHeight;}

private :
  // Une Camera est definie par quatre vecteurs : 1 vecteur de Position
  // ( ou point ou est placee la camera ), 1 de Vue ( vers ou regarde la camera )
  // et un qui defini la normale. ( Inclinaison de la camera ). Le dernier
  // Vecteur 'strafe' permet de déterminer la direction de deplacement lateral de la camera
  // c'est egalement autour de ce vecteur que l'on tourne.
  CVector3f vNormal;
  CVector3f vView;
  CVector3f vPosition;
  CVector3f vStrafe;
  CModeCamera mViewMode;
  float ScrollHeight;

  // Cas de la vue qui suit un objet, uniquement en mode scrolling
  int follow; 

};

// définitions inline

inline void CCamera::vPositionCamera(const CVector3f& v)
{
	vPosition = v ;
}

// Meme chose que pour MoveCamera

inline void CCamera::StrafeCamera(float speed)
{
  vPosition += vStrafe * speed;
  vView += vStrafe * speed;
}

inline void CCamera::ScrollCamera(float speed,CVector3f Way)
{
  // On applique le mouvement aux vecteurs position et vue
  vPosition += Way * speed; 
  vView += Way * speed;
}

inline void CCamera::ScrollUp(float speed)
{
	vPosition.y += speed;
}


#endif
