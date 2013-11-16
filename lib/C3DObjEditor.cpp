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
#include "C3DObjEditor.h"
#include "CustomAssert.h"

void C3DObjEditor::Assign3dObject(C3DObjElement *object) 
{
  m_3dobject = object; 
  m_current_element = object;
  m_current_element_id = 1;
}


C3DObjElement * C3DObjEditor::SearchObject_priv(C3DObjElement *node, int id)
{
  C3DObjElement * found;
  ++(iterator);
  if (iterator==id)
    return node;
  else
  {
    for (int i=0; i < node->GetChildrenNumber(); ++i)
      if (found = SearchObject_priv(node->m_children[i], id))
        return found;
    return NULL;
  }
}

int C3DObjEditor::GetNextID_priv()
{
  C3DObjElement *tmp;
  tmp = SearchObject_priv(m_3dobject, m_current_element_id+1);
  if(tmp)
    return iterator;
  return 1;
}

void C3DObjEditor::SwitchAnimation(unsigned int anim1, unsigned int anim2)
{

  if (!m_animated)
  {
    m_3dobject->ChangeFrameRateRatio(20.0f);
  }
  else
    m_3dobject->ChangeFrameRateRatio(1.0f);
  m_3dobject->Update(true);
  m_animated = !m_animated;
}



void C3DObjEditor::Render()
{
  Assert(m_3dobject, "Un objet doit être assigné à l'éditeur");
  m_3dobject->Render(!m_animated);
  glPushMatrix();
    m_current_element->RenderBoundingBox(false);
  glPopMatrix();
}

void C3DObjEditor::SelectNextObject()
{
  iterator=0;
  m_current_element_id = GetNextID_priv();
  iterator=0;
  m_current_element = SearchObject_priv(m_3dobject, m_current_element_id);
  FixCurrentAnimationAnimation();
}

