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
#ifndef __CMAP_H__
#define __CMAP_H__

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#ifdef WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include "AI/AI_shared.h"
#include "common.h"
#include "lib/CBoundingBox.h"
#include "Settings.h"
#include "Default_settings.h"
#include "lib/font.h"
#include "Menu_type.h"
#include "Minimap.h"

// Un vertex est composé d'un point et de sa normale

struct vertex {
  vect3f pt;
  vect3f n;
};


// Une face est composée de trois indices dans le tableau de vertex associé

struct face {
  int v1,v2,v3;
  unsigned int tex;
};



// La classe CMap
// Au départ la CMap était définie par une matrice des hauteurs accompagnée
// d'une matrice de texture et d'une matrice des normales pour les ombres. 
// Les fonctions qui utilisaient cette structure sont toujours présentes
// mais en commentaires.
// La CMap contient maintenant une liste de face et une liste de vertex 
// ainsi qu'un tableau qui permet de trier les faces par textures.

class CMap
{
public :

  // construit la map
  CMap() ;

  // détruit la map
  ~CMap() ;

  // Rendu de la map sur l'ecran en vue abeille
  void Render(void);

  // Couleur en fonction de la position du soleil
  // void Get_time_color(float color[]);

  // Rendu en scrolling
  void RenderScrolling(void);

  // Vérifie les coordonnées du scrolling
  void CheckScroll();

  // Bouge le soleil
  // void MoveSun(float);

  // Rendu du skybox
  void SkyBox(float, float, float, float, float, float);

  // Initialisation de la Map
  void Init(Map_Settings mSettings);

  // Saisie des Paramètres de la carte
  void Load_Settings();

  // Renvoi un des paramètres de la carte
  //  int Get_Settings(Settings);

  // Met l'ombre sur un point
  // float SetShadow(int i, int j,float *time_color);

  // Dessine un carré
  // void RenderQuad(int i, int j,float *time_color);

  // Calcul des normales
  void NormalizeMap(void);

  // Retourne les ScrollLimit
  const vect2f& GetScrollLimit() const ;
  float ScrollLimitX() const ;
  float ScrollLimitZ() const ;
  vect2f ScrollLimitf() const ;
  float ScrollLimitXf() const ;
  float ScrollLimitZf() const ;

  // Modifie les ScrollLimit
  void SLimitX(float x) ;
  void SLimitZ(float y) ;

  // Retourne les Framesize
  const vect2f FrameSize() const ;
  float FrameSizeX() const ;
  float FrameSizeZ() const ;

  // Affichage d'un vertex
  void Rendervertex(int );

  // Affichage d'une face
  void Renderface(int,int);

  // Afficher toutes les faces
  void Render_every_faces();

  // Afficher les faces qu'il faut en mode scrolling
  void Render_faces_in_scroll_view();

  // Centrer la vue sur un élément en mode scrolling
  void CenterOn(const CVector3f& v) ;

  // Renvoi la hauteur
  float Height(int x, int y);
  
  // Charge toutes les textures
  void LoadTextures();

  // Detruit toutes les textures associées a la map
  void DeleteTextures();

  // Passage en mode plein ecran
  void Switch_to_fullscreen();

  // Chargement des textures
  CVector3f vSunDirection;
  
  //uint tex(int a) { return Textures[a];}

  // Retourne la taille de la carte
  uint Get_Map_Size();

  // Retourne le step_size
  uint Get_Step_Size();

  // Recupere la hauteur de la carte en un point 
  float GetMapHeight ( CVector2f& pos );

  // Pose un objet sur la carte ( calcul de l'angle et de la position )
  void PutOnTheMap(float *p1, float *p2, float *p3, float *p4, float theta, CVector2f pos);

  // retourne la correspondance des coordonnées spécifiées en mode Scrolling
  vect2f ScrollingOffset(const vect2f& pos) ;

  // Menu associé a la carte en mode scrolling
  menu *menu_down ;

  // Minimap associée a la map en mode scrolling
  minimap* mmap ;

  // Génère la texture de la minimap en fonction des hauteurs
  void CreateMinimap();

private :

  // Les paramètres de la carte
  Map_Settings mSettings;

  // La matrice des hauteurs
  float **HeightMatrix;

  // La matrice des textures
  // unsigned char **TexMatrix;

  // Le nombre de faces que contient la carte
  unsigned int numoffaces;

  // Le nombre de vertex que contient la carte
  unsigned int numofvertices;

  // Liste de tous les vertex
  vertex *vertextab;

  // Liste de toutes les faces
  face *facetab;

  // Indice des faces triées par textures
  int **sortbytex;

  // Nombre de faces associées a chaque textures
  int nbtex[NBTEX_MAP];

  // matrice des normales
  // CVector3f **Normal;

  // Point en bas a gauche en mode scrolling
  vect2f ScrollLimit ;

  // Texture de la map
  uint Textures[NBTEX_MAP];
  //UINT MenuTextures[1];

};


// définitions inline

inline CMap::CMap()
: HeightMatrix(0)//, TexMatrix(0), Normal(0)
{}

inline CMap::~CMap()
{
  uint i ;
  for (i=0 ; i< (uint) mSettings.Mat_Size ; ++i)
  {
    delete[] HeightMatrix[i] ;
   // delete[] Normal[i] ;
   // delete[] TexMatrix[i] ;
    
  }
  delete[] HeightMatrix;
  //delete[] Normal ;
  //delete[] TexMatrix ;
  delete menu_down;
  delete mmap;
  delete[] vertextab;
  delete[] facetab;
  for (i = 0; i< NBTEX_MAP; i++)
    delete[] sortbytex[i];
  delete[] sortbytex;
  DeleteTextures();
  
  
  
}


/*-------------------------------------------------------------*/
/* Retourne les hauteurs de la matrice                         */
/*-------------------------------------------------------------*/

inline float CMap::Height(int x, int y)
{
  if ( HeightMatrix == 0)
    return 0;
  if ( x<0) x+=mSettings.Mat_Size;
  if ( y<0) y+=mSettings.Mat_Size;
  
  int i = x % mSettings.Mat_Size;
  int j = y % mSettings.Mat_Size;
  
  return HeightMatrix[i][j];
};

inline const vect2f& CMap::GetScrollLimit() const
{
  return ScrollLimit ;
}

inline float CMap::ScrollLimitX() const
{
  return ScrollLimit.x;
}

inline float CMap::ScrollLimitZ() const
{
  return ScrollLimit.z ;
}

inline vect2f CMap::ScrollLimitf() const
{
  return ScrollLimit * mSettings.Step_Size ;
}

inline float CMap::ScrollLimitXf() const
{
  return ScrollLimitf().x ;
}

inline float CMap::ScrollLimitZf() const
{
  return ScrollLimitf().z ;
}

inline void CMap::SLimitX(float x)
{
  ScrollLimit.x = x ;
}

inline void CMap::SLimitZ(float z)
{
  ScrollLimit.z = z ;
}

inline const vect2f CMap::FrameSize() const 
{
    return mSettings.Frame_Size*mSettings.Step_Size;
}

inline float CMap::FrameSizeX() const
{
  return (float)FrameSize().x ;
}

inline float CMap::FrameSizeZ() const
{
  return (float)FrameSize().z ;
}

inline uint CMap::Get_Map_Size()
{
  return mSettings.Map_Size ;
}

inline uint CMap::Get_Step_Size()
{
  return mSettings.Step_Size ;
}

inline void CMap::CheckScroll()
{
  // %= impossible
  if (ScrollLimit.x < 0) ScrollLimit.x += mSettings.Mat_Size ;
  else if (ScrollLimit.x >= mSettings.Mat_Size) ScrollLimit.x -= mSettings.Mat_Size ;

  if (ScrollLimit.z < 0) ScrollLimit.z += mSettings.Mat_Size ;
  else if (ScrollLimit.z >= mSettings.Mat_Size) ScrollLimit.z -= mSettings.Mat_Size ;
}


inline void CMap::CenterOn(const CVector3f& v)
{

	ScrollLimit.x  = (v.z - FrameSizeX() / 4) / mSettings.Step_Size;
	ScrollLimit.z  = (v.x - FrameSizeZ() / 2) / mSettings.Step_Size;

}

inline vect2f CMap::ScrollingOffset(const vect2f& pos)
{
  vect2f dec(ScrollLimitf()) ;

	vect2f ftrueview = ScrollLimitf() + FrameSize();

  if (( ftrueview.x > Get_Map_Size()) && (pos.x < ftrueview.x - Get_Map_Size())) 
    dec.x -= Get_Map_Size() ;
  if (( ftrueview.z > Get_Map_Size()) && (pos.z < ftrueview.z - Get_Map_Size())) 
    dec.z -= Get_Map_Size() ;
  return dec;
}

inline void CMap::Switch_to_fullscreen()
{
  LoadTextures();
  menu_down = new menu(mSettings,"leftmenu.ini");
  mmap = new minimap(this,mSettings.Map_Size/180.0f,12,581,192,761);
  CreateMinimap();
}



inline void CMap::Renderface(int i,int t)
{
  face f= facetab[i];
// 
  glTexCoord2f(t,t);
  Rendervertex(f.v1);

  glTexCoord2f(t,1-t);
  Rendervertex(f.v2);

  glTexCoord2f(1-t,1-t);
  Rendervertex(f.v3);

}

inline void CMap::Rendervertex(int i)
{
  vertex v = vertextab[i];
  glNormal3f(v.n.x,v.n.y,v.n.z);
  glVertex3f(v.pt.x,v.pt.y,v.pt.z);

}

#endif
