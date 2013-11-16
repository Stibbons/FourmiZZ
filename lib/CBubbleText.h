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
#ifndef __BUBBLETEXT__
#define __BUBBLETEXT__

#include <string>

/* affiche une bulle façon bande dessiner à l'écran */
/* utilise une texture donnée en parametre */
class CBubbleTexture
{
  float m_x,m_y,m_z;
  float m_sizex, m_sizey;
  //std::string m_sztext; //TODO gerrer texte
  bool m_enabled;
  unsigned int m_gltexid;
  unsigned int m_glmasktexid;
  bool m_texmasked;
public:
  /* constructeur "on ne met rien dedans" */
  CBubbleTexture() :
      m_x(0), 
      m_y(0),
      m_z(0), 
      m_sizex(10),
      m_sizey(10), 
      m_gltexid(0),
      m_glmasktexid(0),
      m_texmasked(false),
      m_enabled(false) 
      { }
  /* constructeur "on met des valeurs dedans" */
  CBubbleTexture(float x, float y, float z, unsigned int texid, int masktexid, float sizex, float sizey, bool enable) : 
    m_x(x), 
    m_y(y), 
    m_z(z), 
    m_sizex(sizex),
    m_sizey(sizey),
    m_gltexid(texid),
    m_texmasked((masktexid==-1? false:true)),
    m_glmasktexid(masktexid),
    m_enabled(enable)
    {  }
  /* fixe la texture à utilisé (id rendu par glGenTextures) */
  void SetTexture(unsigned int texid);
  /* pareil mais pour une texture maskée */
  void SetTexture(unsigned int texid, unsigned int masktexid);
  /* pareil mais on peut fixer en + la taille !! */
  void SetTexture(unsigned int texid, unsigned int masktexid, float sizex, float sizey);
  /* place la position (dans le monde 3d) de la bulle */
  void SetPosition(float x, float y, float z)
  {
    m_x = x;
    m_y = y;
    m_z = z;
  }
  /* fixe la taille */
  void SetSize(float sizex, float sizey)
  {
    m_sizex = sizex;
    m_sizey = sizey;
  }
  /* la bulle sera affichée */
  void Enable()  { m_enabled = true; }
  /* ou pas */
  void Disable() { m_enabled = false; }
  /* dessin de la bulle */
  void Render() const;
};

inline void CBubbleTexture::SetTexture(unsigned int texid)
{
  m_gltexid = texid;
  m_texmasked = false;
}

inline void CBubbleTexture::SetTexture(unsigned int texid, unsigned int masktexid)
{
  m_gltexid = texid;
  m_texmasked = true;
  m_glmasktexid = masktexid;
}

inline void CBubbleTexture::SetTexture(unsigned int texid, unsigned int masktexid, float sizex, float sizey)
{
  m_gltexid = texid;
  m_sizex = sizex;
  m_sizey = sizey;
  m_texmasked = true;
  m_glmasktexid = masktexid;
}



#endif // __BUBBLETEXT__
