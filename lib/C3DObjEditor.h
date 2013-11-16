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
#ifndef __C3DOBJEDITOR_H__HEADER__
#define __C3DOBJEDITOR_H__HEADER__

#include "C3DObject.h"

class C3DObjEditor
{
  C3DObjElement   *m_3dobject;
  bool             m_animated;
  int              m_current_element_id;
  C3DObjElement   *m_current_element;
  int              iterator;
//  int              m_current_animation;
//  int              m_current_frame;
  int              m_animation_uid;

  C3DObjElement * SearchObject_priv(C3DObjElement *node, int id);
  int GetNextID_priv();

public:
  C3DObjEditor() : 
      m_3dobject(NULL),
      m_animated(false), 
      m_current_element_id(1),
      m_current_element(NULL),
//      m_current_frame (0),
      //m_current_animation (0),
      m_animation_uid(0)
  { }
  void Assign3dObject(C3DObjElement *object) ;
  void Release3dObject( ) { m_3dobject = NULL; }
  C3DObjElement *SearchObject( int id);
  int GetNextID(int curid);
  void SwitchAnimation(unsigned int anim1, unsigned int anim2);
  void Render();

  C3DObjElement* Get3dObject() { return m_3dobject; }
  const unsigned int GetCurrentElementID() const { return m_current_element_id; }
  const C3DObjElement* GetCurrentElement() const { return m_current_element; }
  CBoundingBox *GetCurrentBoundingBox() { return &m_current_element->m_boundingbox; }
  CPosition* GetCurrentFrame() 
  { 
    return m_current_element->m_animations[m_current_element->m_anim_current_ind]->m_frames[m_current_element->m_animations[m_current_element->m_anim_current_ind]->m_frame_current];
  }
  void UpdateChanges()
  {
    m_current_element->m_position = *(m_current_element->m_animations[m_current_element->m_anim_current_ind]->m_frames[m_current_element->m_animations[m_current_element->m_anim_current_ind]->m_frame_current]);
  }

  void SelectNextObject();
  int GoToNextFrame();
  int GetCurrentAnimationID() { return m_current_element->m_anim_current_ind; }
  int GetCurrentFrameID() { return m_current_element->m_animations[m_current_element->m_anim_current_ind]->m_frame_current; }

  void SetCurrentAnimation(int animation_uid);
  void FixCurrentAnimationAnimation();
  bool IsAnimated() { return m_animated; }
};


inline C3DObjElement* C3DObjEditor::SearchObject(int id)
{
  iterator = 0;
  return SearchObject_priv(m_3dobject, id);
}

inline int C3DObjEditor::GetNextID(int curid)
{
  iterator = 0;
  return GetNextID_priv();
}

inline int C3DObjEditor::GoToNextFrame()
{
  m_3dobject->Update(true);
  return m_current_element->m_animations[m_current_element->m_anim_current_ind]->m_frame_current;
}

inline void C3DObjEditor::SetCurrentAnimation(int animation_uid)
{
  m_animation_uid = animation_uid;
  FixCurrentAnimationAnimation();
}

inline void C3DObjEditor::FixCurrentAnimationAnimation()
{
  for (int i=0; i<m_current_element->m_anim_number; ++i)
    if ( m_current_element->m_animations[i]->m_uid == m_animation_uid)
    {
      m_current_element->m_anim_current_ind = i;
      return;
    }
}

#endif /*__C3DOBJEDITOR_H__HEADER__ */
