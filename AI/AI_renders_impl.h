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
#ifndef __AI_RENDERS_IMPL_H__
#define __AI_RENDERS_IMPL_H__

/*
J'ai placé ici les définitions des fonctions inline afin de ne
pas surcharger l'en-tête AI_renders.h avec cette implémentation.
*/


// aiGraphicWorld

inline float aiGraphicObject::get_distance_from_bee_camera() const
{
  return (get_coords2d() - cCam.Position()).Norm() ;
}

inline uint aiGraphicWorld::get_token_texture_id() const
{
  return token_texture_id_ ;
}

inline const colorRGBA& aiGraphicWorld::get_rendered_token_RGBA_color() const
{
  return tokens_RGBA_colors_[rendered_token_] ;
}

inline void aiGraphicWorld::set_token_RGBA_color(const token_t t, const colorRGBA& color)
{
  tokens_RGBA_colors_[t] = color ;
}

inline void aiGraphicWorld::change_rendered_token()
{
  if (++rendered_token_ == TOKEN_T_NB) rendered_token_ = -1 ;
}

inline int aiGraphicWorld::get_rendered_token() const
{
  return rendered_token_ ;
}

// AnimsLink

inline AnimsLink::AnimsLink()
: current_anim_id_(ANIMATION_INVALID_VALUE),
  transition_anim_id_(ANIMATION_INVALID_VALUE),
  next_anim_id_(ANIMATION_INVALID_VALUE)
{}


inline AnimsLink::AnimsLink(const uint ci, const uint ti, const uint ni)
: current_anim_id_(ci),
  transition_anim_id_(ti),
  next_anim_id_(ni)
{}


// AnimsCell

inline AnimsCell::AnimsCell(const action_type na, const AnimsLink& al, AnimsCell* acp)
: next_action_(na),
  val_(al),
  next_(acp)
{}

#endif /* __AI_RENDERS_IMPL_H__ */
