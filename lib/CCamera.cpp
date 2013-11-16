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
#include "CCamera.h"

// Modification de la position de la camera
void CCamera::PositionCamera3fv(const CVector3f& p, const CVector3f& v,
                                const CVector3f& n, const CVector3f& s)
{
  vPosition = p;
  vView = v;
  vNormal = n;
  vStrafe = s;
}

void CCamera::PositionCameraf(float pX, float pY, float pZ, 
                         float vX, float vY, float vZ,
                         float nX, float nY, float nZ,
                         float sX, float sY, float sZ)
{
  vPosition.x = pX;
  vPosition.y = pY;
  vPosition.z = pZ;
  vView.x = vX;
  vView.y = vY;
  vView.z = vZ;
  vNormal.x = nX;
  vNormal.y = nY;
  vNormal.z = nZ;
  vStrafe.x = sX;
  vStrafe.y = sY;
  vStrafe.z = sZ;
}

// Mise a jour de la vue 
void CCamera::Look()
{
  switch(mViewMode)
  {
  case BEE_VIEW :
    gluLookAt(vPosition.x,vPosition.y,vPosition.z,
            vView.x, vView.y, vView.z,
            vNormal.x, vNormal.y, vNormal.z);
    break;
  case SCROLL_VIEW :
	  gluLookAt(vPosition.x,vPosition.y + ScrollHeight,vPosition.z,
            vView.x, vView.y+ScrollHeight, vView.z,
            vNormal.x, vNormal.y, vNormal.z);
    break;
  }
}

// Cette methode permet de deplacer en avant et en arriere la camera
// selon le vecteur Vue - Position 

void CCamera::MoveCamera(float speed)
{
  CVector3f Way = vPosition - vView ;
  Way.Normalize();
  // On applique le mouvement aux vecteurs position et vue
  vPosition.x += Way.x * speed; 
  // A mettre en commentaire pour empecher le deplacement vertical
  ////////////////////////////
  vPosition.y += Way.y * speed;///
  ////////////////////////////
  vPosition.z += Way.z * speed; 

  vView.x += Way.x * speed;
  // A mettre en commentaire pour empecher le deplacement vertical
  /////////////////////////
  vView.y += Way.y * speed;///
  /////////////////////////
  vView.z += Way.z * speed;

}


// tourne la vue autour de ce vecteur
void CCamera::RotateView(float angle, float X, float Y, float Z)
{
  CVector3f vNewView;

  // La direction vers laquelle on regarde
  CVector3f vWay = vView - vPosition;
  
  // Calcul des cos et sin une seule fois
  float cosT = (float) cos(angle);
  float sinT = (float) sin(angle);
    

  // On applique la matrice de rotation
 
	vNewView.x = (cosT + (1 - cosT) *X *X) * vWay.x;
	vNewView.x += ((1-cosT)*X*Y -Z*sinT) * vWay.y;
	vNewView.x += ((1 - cosT) *X *Z +Y*sinT) * vWay.z;

  
  vNewView.y = ((1 - cosT)*Y * X + Z * sinT) * vWay.x;
	vNewView.y += (cosT + (1 - cosT) * Y * Y) * vWay.y;
	vNewView.y += ((1 - cosT) * Y * Z - X * sinT) * vWay.z;

	
	
	vNewView.z = ((1 - cosT) * X * Z - Y * sinT) * vWay.x;
	vNewView.z += ((1 - cosT) * Y * Z + X * sinT) * vWay.y;
  vNewView.z += (cosT + (1 - cosT) * Z * Z) * vWay.z;
  
  
  // On tourne la vue seulement
  vView = vPosition + vNewView;
  
}

// tourne la vue autour du point
void CCamera::RotateAround(CVector3f vCenter,float angle, float fX, float fY, float fZ)
{
  CVector3f vNewView;

  // La direction vers laquelle on regarde
  CVector3f vWay = vPosition - vCenter;
  
  // Calcul des cos et sin une seule fois
  float cosT = (float) cos(angle);
  float sinT = (float) sin(angle);

  // On applique la matrice de rotation
 
	vNewView.x = (cosT + (1 - cosT) *fX *fX) * vWay.x;
	vNewView.x += ((1-cosT)*fX*fY -fZ*sinT) * vWay.y;
	vNewView.x += ((1 - cosT) *fX *fZ +fY*sinT) * vWay.z;

	vNewView.y = (cosT + (1 - cosT) *fY *fY) * vWay.y;
	vNewView.y += ((1-cosT)*fY*fX +fZ*sinT) * vWay.x;
	vNewView.y += ((1 - cosT) *fY *fZ -fX*sinT) * vWay.z;

	
	vNewView.z = (cosT + (1 - cosT) *fZ *fZ) * vWay.z;
	vNewView.z += ((1-cosT)*fX*fZ -fY*sinT) * vWay.x;
	vNewView.z += ((1 - cosT) *fY *fZ +fX*sinT) * vWay.y;

  // On tourne la vue seulement
  
  vPosition = vCenter + vNewView;
  
}


void CCamera::SetMouseView(int relX, int relY)
{
 // Calcul du milieu   
	  int milx = TAILLEX >>1;
		int mily = TAILLEY >>1;
 // On crée un carré de 10 sur 10 autour du centre, ces deux
 // variables permettent de sauvegarder la position du pointeur
 // afin de le remettre au milieu
		static float pox = (float) milx;
		static float poy = (float) mily;
 // Les angles de rotation associés aux mouvement de la souris
    float angley; 
		float anglez;
 
		

 // Calcul des angles => vitesse du mouvement de camera
		angley = (float)(-relX)/500.0f;
		anglez = (float)(relY)/500.0f;
        
 // Empeche les loopings
		if((relX== 0) && (relY == 0)) return;
		 
  // Tourner autour du vecteur strafe et de la normale 
    RotateView(-anglez,vStrafe.x,vStrafe.y,vStrafe.z);
		RotateView(angley,0,1,0);
		
}

// Recalcul du strafe

void CCamera::Update()
{
  vStrafe = cross_product(vView - vPosition, vNormal);
  vStrafe.Normalize();
}

// Change le mode de vue de la camera
void CCamera::ChangeMode(CModeCamera mode,float x, float y)
{
  mViewMode = mode;
  switch (mode) 
  {
  case BEE_VIEW :
	  vPosition.y+=ScrollHeight;
    follow=0;
	  break;
// Vue Scrolling
  case SCROLL_VIEW :
    follow=0;

// On positionne la camera 
// La normale est toujours la même
    CVector3f vNewNormal(0,1,0);
// On regarde en 150,0,70
    CVector3f vNewView(x / 4 , 0, y / 2);

// On se place au dessus du point ou l'on regarde avec un léger angle
    CVector3f vNewPosition(-20 , SCROLL_HEIGHT,x / 2);
    CVector3f vNewStrafe = cross_product(vNewPosition - vNewView, vNewNormal);
    PositionCamera3fv(vNewPosition, vNewView, vNewNormal, vNewStrafe);
    break;
  }
}

