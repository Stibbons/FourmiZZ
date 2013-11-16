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
#include "AI_graphic_objects.h"

#include "../Objects.h"
#include "AI_world.h"

aiGraphicObject::aiGraphicObject(const C3DObjElements& e, CMap* ma,
                                 const vect2f& co, const float an)
: map_(*ma), coords3d_(co,0), angles3d_(0,an,0)
{
  objs3d_ = e;
  objs3d_.m_hiRes->ChangeFrameRateRatio(3.0f) ;

  if (objs3d_.m_lowRes)
  {
    objs3d_.m_lowRes->ChangeFrameRateRatio(3.0f) ;
  }
}

void aiGraphicObject::take_object(aiGraphicObject& obj)
{
  objs3d_.m_hiRes->TakeObject(obj.objs3d_.m_hiRes, OBJECT_SEED_UID);
  // maintenant on dit "tout les animations qui devaient retomber sur
  // WALK retomberont maintenant sur WALKWITHSEED
  objs3d_.m_hiRes->ChangeNextAnimation(ANIMATION_WALK_UID, ANIMATION_WALKWITHSEED_UID);
  // là on dit "toutes les animations qui tournent sur WALK iront sur
  // WALTOWALKWITHSEED, transition vers WALKWITHSEED
  objs3d_.m_hiRes->ChangeAnimation(ANIMATION_WALK_UID, ANIMATION_WALKTOWALKWITHSEED_UID);
}

void aiGraphicObject::drop_object(aiGraphicObject& obj)
{
  objs3d_.m_hiRes->DropObject(OBJECT_SEED_UID);
  objs3d_.m_hiRes->ChangeNextAnimation(ANIMATION_WALKWITHSEED_UID, ANIMATION_WALK_UID);
  objs3d_.m_hiRes->ChangeAnimation(ANIMATION_WALKWITHSEED_UID, ANIMATION_WALKWITHSEEDTOWALK_UID);
}


bool aiGraphicObject::select_from_camera(const CVector3f& pos, const CVector3f& dir)
{
  // calcul de la distance entre un point (coords3d_) et la droite (pos, dir);
  CVector3f ori(coords3d_.x, world->map_->GetMapHeight(coords3d_)/*-objs3d_.m_hiRes->GetBoundingSphere()*/, coords3d_.z);
  float dist = (ori - pos).CrossProd(dir).Norm();  
  dist /= dir.Norm();
  //cout << "\nDistance calculée = " << dist << endl;
  return dist < objs3d_.m_hiRes->GetBoundingSphere() ;
}
