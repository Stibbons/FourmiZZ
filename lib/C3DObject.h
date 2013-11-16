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
#ifndef __FOURMIS_3DOBJECTS_H__
#define __FOURMIS_3DOBJECTS_H__

#include <fstream>
#include <stdio.h>
#include "3dsloader.h"
#include "CBoundingBox.h"
#include "CSDLChrono.h"
#include "Particles.h"
#include "material.h"
#include "../common.h"
/*
#ifndef WIN32
	typedef unsigned int  UINT;
	typedef unsigned char BOOL;
	#define TRUE 1
	#define FALSE 0
#else
	#include <windows.h>
#endif
*/


#define ANIMATION_INVALID_VALUE      ((unsigned int) -1)






/*****************************************************************************\
 * Classes                                                                   *
\*****************************************************************************/

/* class CPosition ***********************************************************\
 * Contient les informations de position et angle de l'objet RELATIVEMENT A  *
 * SON PARENT.                                                               *
 * On connait ainsi parfaitement la position de l'objet avec cette classe    *
\*****************************************************************************/
struct CPosition
{
  float x, y, z; /* position x, y, z relativement au parent !!!              */
  float anglex, angley, anglez; /* angles selon les axes (tjs relativement   */
                                /* aux parents)                              */
  
  /* Contructeur standard */
  CPosition()
  {
    x=y=z=anglex=anglex=angley=anglez=0; 
  };
  /* Constructeur */
  CPosition(float newx, float newy, float newz,
            float newanglex, float newangley, float newanglez)
  {
    x      = newx;
    y      = newy;
    z      = newz;
    anglex = newanglex;
    angley = newangley;
    anglez = newanglez;
  }
  /* surcharge de l'operateur = */
  CPosition& operator=(float vect[2][3])
  {
    x = vect[0][0];
    y = vect[0][1];
    z = vect[0][2];
    anglex = vect[1][0];
    angley = vect[1][1];
    anglez = vect[1][2];
    return *this;
  }
  CPosition& operator=(CPosition src)
  {
    x      = src.x;
    y      = src.y;
    z      = src.z;
    anglex = src.anglex;
    angley = src.angley;
    anglez = src.anglez;
    return *this;
  }
  CPosition operator*(float f)
  {
    return CPosition(x * f,
      y      * f,
      z      * f,
      anglex * f,
      angley * f,
      anglez * f);
  }

  CPosition operator+(CPosition add)
  {
    return CPosition(
      x      + add.x,
      y      + add.y,
      z      + add.z,
      anglex + add.anglex,
      angley + add.angley,
      anglez + add.anglez);
  }
};

/* class C3DObjAnim **********************************************************\
 * Gestion des différentes "animations" de l'objet. Chaque C3DObjAnim        *
 * contient toutes les données nécessaires pour UNE animation                *
\*****************************************************************************/
class C3DObjAnim
{
public:
  UINT m_uid;             /* identifier unique de l'animation                */
  char *m_name;           /* nom de l'animation                              */
  UINT m_frame_number;    /* nombre de "frame dans l'animation               */
  UINT m_frame_current;   /* frame courante dans l'animation                 */
  int  m_framerate_ratio; /* indicateur de la vitesse de l'animation         */
                          /*  >  1 : moins vite => interpolation des         */
                          /*        nouvelles frames                         */
                          /*  < -1 : + vite => drop de frame                 */
                          /*  = 0  : même vitesse de rendu                   */
                          /* (2 cas pour éviter d'utiliser un float)         */
                          /* Note : -1 et 1 sont des valeurs interdites      */
                          /* (correspondent en fait à = 0)                   */
  CPosition **m_frames;   /* tableau des frames (positions)                  */
  bool m_loop;            /* l'animation doit elle boucler ?                 */
  int m_anim_next_ind;    /* si anim_next == -1 cette animation bouclera     */
                          /* Sinon, on sélectionnera automatiquement la      */
                          /* prochaine animation dans la liste des animations*/
                          /* présente. S'il y a un probleme, on prend        */
                          /* l'animation par défaut de l'objet               */
                          /* ATTENTION: correspond à l'INDEXE de l'animation */
                          /* suivante, et non pas le UID                     */
  char *m_anim_next_str;    /* nom de l'animation suivante, on ne la connait */
                          /* pas forcément (cas d'animations pas directement */
                          /* appelés par l'IA)                               */
  C3DObjAnim();
  ~C3DObjAnim();
  C3DObjAnim& operator =(const C3DObjAnim &s);
  bool Reset();
};

typedef struct 
{
  char  *m_strName;
  UINT   m_iUID;
} ANIMATIONS_NAMEUID, OBJECTS_NAMEUID, UNKNOWN_NAMEUID;

#define TRIGGER_NONE          0
#define TRIGGER_TAKE_OBJECT   1
#define TRIGGER_DROP_OBJECT   2

/* class C3DObjElement
 * Element de l'objet 3D. Chaque élément peut avoir des enfants (dont les 
 * positions vont dépendre), et avoir plusieurs animations différentes.
 * On a ainsi une organisation hierarchique permettant de faire faire
 * à l'objet complet plusieurs type d'animation et en même temps!
 */
class C3DObjElement
{
  char *m_str3dfile;                 /* Fichiers 3ds à charger               */
  bool  m_loaded;                    /* fichier chargé ou juste initialisé?  */
  CLoad3DS *m_obj3d;                 /* objet 3ds chargé (mais pas les       */
                                     /* textures, qui le devront être +      */
                                     /* tard)                                */
                                     /* ATTENTION: ne pas deleter ce         */
                                     /* pointer !! il pointe vers une liste  */
                                     /* d'objet unique qui sera elle         */
                                     /* détruite proprement                  */
  CPosition m_position;              /* position/angle courant               */
  float        m_boundingsphere;     /* bounding sphere (sert à tester si    */
                                     /* l'objet est dans vue de la caméra,   */
                                     /* pour l'occlusion (caché par d'autres */
                                     /* objets), ...)                        */
  CBoundingBox m_boundingbox;        /* bounding box (pour la gestion des    *
                                      * collisions)                          */
  UINT m_anim_number;                /* nombre d'animations                  */
  C3DObjAnim **m_animations;         /* tableau (dynamique) de toutes les    */
                                     /* animations disponibles               */
  UINT m_anim_default_ind;           /* animation par défaut (indexe!!)      */
  char * m_anim_default_str;         /* nom de l'animation par défaut, on ne */
                                     /* la connait pas directement au        */
                                     /* chargement                           */
  UINT m_anim_current_ind;           /* animation courante   (indexe!!)      */

  UINT m_children_number;            /* Nombre d'enfant de l'objet           */
  C3DObjElement **m_children;        /* Tableau des pointeurs vers les       */
                                     /* enfants                              */

  int m_ObjectUID;                     /* identificateur unique de l'objet   */
                                     /* (utilisé uniquement pour les objets  */
                                     /* qui doivent être pris par d'autre)   */
                                     /* (UID réservé : -1 == non défini)     */
  C3DObjElement *m_attached_object;  /* Objet attaché                        */
  bool          m_attached_enabled;  /* booleen d'activation de l'attachement*/
  int           m_attached_object_uid;/* identificateur unique de l'objet qui*/
                                     /* peut être lié                        */
  CPosition     m_attached_position; /* position de l'objet attaché          */
  UINT          m_attached_trigger;  /* "gachette" à utiliser:               */
                                     /*  Choisir l'animation d'attachement ? */
                                     /*  Choisir l'animation de Drop d'objet */

  int           m_anim_takeobj_ind;  /* animation pour prendre un objet      */
  int           m_anim_dropobj_ind;  /* et pour dropper (poser) un objet     */
    /* attention, ce sont des indexes, et pas des UID d'animation !          */
  char         *m_anim_takeobjstr;   /* buffer de nom (avant de pouvoir      */
  char         *m_anim_dropobjstr;   /* trouver le bon index, que l'on ne    */
                                     /* connait pas a priori)                */
  CCastParticleSystem *m_particles;
  CPosition    m_particle_position;
  UINT         m_particle_uid;
  CSDLChrono   m_particle_timer;

 // TODO gerer la prise de plusieur type d'objet par élément (liste)
  bool m_enabled;
  bool m_displayed;                    /* afficher l'objet ?                 */

  /* traitement interne à faire quand on prend un objet... */
  UINT TakeObject_priv(C3DObjElement *pObject, UINT uObjUID);
  UINT DropObject_priv(UINT uObjUID);

  friend class C3DObjLoader;
  friend class C3DObjEditor;

public:
  /* constructeur/destructeur */
  C3DObjElement();
  ~C3DObjElement();

  C3DObjElement& operator=(const C3DObjElement &d);

  inline CBoundingBox& GetBoundingBox()    { return m_boundingbox; }
  inline float& GetBoundingSphere()        { return m_boundingsphere; }
  inline char* Get3dfilename() const       { return m_str3dfile; }
  inline CPosition& GetPosition()          { return m_position;  }
  inline int& GetObjectUID()               { return m_ObjectUID;  }
  inline unsigned int& GetChildrenNumber()          { return m_children_number;  }
  inline C3DObjElement* GetChildren(int i) { return m_children[i];  }
  inline void Disable() { m_enabled = false; }
  inline void Enable()  { m_enabled = true;  }
  /* Sélection de l'animation */
  bool ChangeAnimation(UINT curUID, UINT newUID);
  /* Changement des next anim' */
  bool ChangeNextAnimation(UINT oldUID, UINT newUID);
  /* Sélection de l'animation par défaut */
  bool SelectDefaultAnimation(UINT forcedUID = ANIMATION_INVALID_VALUE);
  /* Chargement de l'objet */
  bool Load( char * szobjpath, char * sztexpath, bool loadchildren, 
             t3DTextures *textures_list, t3DObjects *objects_list,
             tLightMaterial *light_material);
  /* Rendu de l'objet */
  bool Render(bool force=false);
  /* Mise à jour de l'objet */
  bool Update(bool recurse=false);
  /* Changement du frame rate */
  bool ChangeFrameRateRatio(float newrate);
  /* Point de départ de l'animation au hazard */
  bool RandomAnimationStart(UINT AnimUID, bool samepos = false, int gem = -1);
  /* Dessin des bounding box de l'objet */
  bool RenderBoundingBox(bool recurse=true);
  /* Reconstruire les display list */
  bool RebuilDisplayLists();
  /* évenement prendre l'objet */
  /* Remarque: l'ia doit lancer en même temps l'animation "prendre objet" */
  bool TakeObject(C3DObjElement *pObject, UINT uObjUID);
  /* évenement dropper l'objet */
  /* Remarque: l'ia doit lancer en même temps l'animation "prendre objet" */
  bool DropObject(UINT uObjUID);
  /* remise à zero */
  bool Reset();
  /* Initialisation du systeme de particule*/
  bool InitParticleSystem(unsigned int partuid, unsigned int number, float  debit, float life, float fade,
                          float red, float green, float blue, float alpha, 
                          bool textured, unsigned int gltextureuid,
                          float xsize, float ysize,
                          CVector3f &grav,
                          CVector3f &randomplan, CVector3f randomspeed,
                          CREATEPARTICLECALLBACK pfCreateCallBack,
                          UPDATEPARTICLECALLBACK pfUpdateCallBack);
  void StartParticleSystem();
  void StopParticleSystem();
  void DeleteParticleSystem();
};

/*****************************************************************************\
 * Fonctions génériques                                                      *
\*****************************************************************************/

class C3DObjLoader
{
public:
  /* Chargement d'un objet complet (à partir d'un fichier de définition) */
  static bool Load3d(string filename, C3DObjElement **p3DObject,
                                            ANIMATIONS_NAMEUID *AnimNameUIDList,
                                            OBJECTS_NAMEUID    *ObjNameUIDList);
  static UINT getUIDFromName(char *strName, UNKNOWN_NAMEUID *NameUIDList);
  static bool getValueString(char *strbuf, char *strdest);
  static bool getValueIntList(char *strbuf, ifstream *f, int *iline, 
                       int point[], int maxCount);
  static bool getValueFloatVectorsList(char *strbuf, ifstream *f, int *iline,
                                float point[][3], int maxVectCount);
  static bool getValueInt( char *strbuf, int *iValue);
  static bool strtrim(char *str);
  static void strrmcomments(char *strbuf);

};

// couple de C3DObjElement
class C3DObjElements
{
public:
  C3DObjElement* m_lowRes, * m_hiRes ;

  // construit un couple de C3DObjElement
  C3DObjElements() : m_lowRes(0), m_hiRes(0) {}

  C3DObjElements(const C3DObjElements& c) {
    operator=(c);
  }

  C3DObjElements& operator=(const C3DObjElements& c)
  {
    if (c.m_lowRes)
    {
      m_lowRes = new C3DObjElement;
      *m_lowRes = *c.m_lowRes;
    }
    if (c.m_hiRes)
    {
      m_hiRes = new C3DObjElement;
      *m_hiRes = *c.m_hiRes;
    }
    return *this ;
  }

  // détruit le couple de C3DObjElement
  ~C3DObjElements()
  {
    if (m_lowRes) 
      delete m_lowRes ;
    if (m_hiRes)
      delete m_hiRes ;
  }
} ;


#endif /* __FOURMIS_3DOBJECTS_H__ */
