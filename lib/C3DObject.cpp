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
#include <fstream>
#include <string>
#include <iostream>
#ifdef WIN32
#include <windows.h>
#endif
//#include <stdio.h>
//#include <stdlib.h>
#include <math.h>
//#include <stdlib.h> // rand/srand
#include <time.h>   // time()
//#include <string.h> // strncase sous linux
#include "C3DObject.h"
#include <stack> 
 // utilisé pour ne pas avoir à utiliser une fonction récursive pour
 // assigner les héritages dans la hierarchie des objets.
#include <GL/gl.h>
#include <GL/glu.h>
#include "tga.h"
#include "CustomAssert.h"
#include "material.h"

using namespace std;

#ifndef WIN32
// petite incompatibilité Windows/linux...
#define strcmpi(a,b) strncasecmp(a,b, strlen(b))
#endif

// méthode un peu violente pour faire une norme avec un vecteur de dimension
// trois....
#define Norme3fv(v) (sqrt( v[0] * v[0] + v[1] * v[1] + v[2] * v[2] ))
// utilise pour calculé le rayon de la bounding sphere en fonction de la
// bounding box.

/* objstruc ******************************************************************\
 * Récupération des données temporaire lors du chargement d'un fichier       *
\*****************************************************************************/
struct objstruct
{
  C3DObjElement *p3DObjElement;
  UINT           objID;
  objstruct     *pnext;
  UINT           children_number;
  UINT          *childrenID;
  objstruct()
  {
    children_number=0;
  }
  ~objstruct()
  {
    if (children_number)
    {
      delete[] childrenID;
    }
  }
};
// utilisé uniquement dans Load3d mais il faut mettre la définition
// en global à cause du template std::stack

/*****************************************************************************\
 * Principe de fonctionnement:                                               *
 * On veut un Objet 3d qui peut avoir plusieurs animations possibles, et     *
 * même plusieurs animations en même temps.                                  *
 * Pour celà il faut descrire tous les mouvements possibles.                 *
 * Il y a alors 2 approches possibles:                                       *
 *   Soit encoder la position de TOUS les points (vertexes) pour CHAQUE      *
 *     frame (unite lors de l'animation) => typiquement les md2 et md3 de    *
 *     Quake                                                                 *
 *   Soit utiliser une organisation hierarchique et avoir des sous-objets    *
 *     représentant des membres qui ne pourront plus être modifier. En       *
 *     d'autres terme, c'est un peut un SQUELETTE.                           *
 *     Il existe un format semblable de ID Software pour Quake III: le MD4   *
 *     (ou format de MilkShape je sais plus)                                 *
 *     Mais celui-ci est très peu documenté.                                 *
 * C'est cette dernière approche qui est utilisée, certes moins efficace     *
 * pendant le rendu (car il faut quand même calculer la nouvelle position à  *
 * chaque frame), mais c'est nettement plus simple pour la définition des    * 
 * mouvements.                                                               *
 *                                                                           *
 * Concepts:                                                                 *
 *  - Position : en fait : position et angle. Permet de caractériser l'état  *
 *              de l'objet de manière complète                               *
 *  - BoundingBox: "boite" représentant l'objet très simplifié. Utilisé      *
 *              pour une éventuelle détection de collision en utilisant      *
 *              l'algorithme OBB (Oriented Bounding Box)                     *
 *  - Animation : chaque objet élémentaire (bras, jambe, tronc,...) a        *
 *              plusieurs mouvement possible (un seul en même temps!!)       *
 *              qui est représenté par une liste d'animations                *
 *  - frame     : unité élémentaire de dessin. Pour chaque frame, on a une   *
 *              position. L'ensemble des frames forme une animation.         *
 * Chaque objet 3D est défini par un ARBRE d'objet élémentaire. Chaque objet *
 * élémentaire est caractèrisé par une position (+angle) RELATIVE à son      *
 * parent (sauf root où c'est la position absolue)                           *
 * Pour chaque objet 3d complet, il y a un objet racinne. Celui-ci ne DOIT   *
 * PAS être dessiner (ne doit pas contenir de point). Il est là simplement   *
 * pour pouvoir définir un Bounding box englobant TOUT l'objet.              *
 *                                                                           *
 * Pour définir un objet global, on a donc besoin d'objet élémentaire FIXE   *
 * Dans un premier temps, il s'agit de fichier 3DS Meshes.                   *
 * Il faut un fichier pour orchestrer les dépendances entre les objets       *
 * élémentaires. C'est le role du format de fichier f3d.                     *
 * fm1 version 1.0:                                                          *
 * Type de fichier : texte.                                                  *
 *   - Possibilité de mettre en commentaire le reste de n'importe quelle     *
 *     ligne par '//'                                                        *
 *   - Les listes de nombres sont séparés par des espaces                    *
 *   - Les informations "listes" peuvent prendre plusieurs lignes avec       *
 *     autant d'espace que l'on veut.                                        *
 *   - fin d'une donnée par ";"                                              *
 * Exemple:                                                                  *
 *                                                                           *
 * ElementID = 0;                                                            *
 * BoundingBox = 0 0 0                                                       *
 *               1 0 0                                                       *
 *               1 1 0                                                       *
 *               ....                                                        *
 *               0 0 1;                                                      *
 * ChildrenNumber = 3;                                                       *
 * Children = 1 2;                                                           *
 * AnimNumber = 3;                                                           *
 * AnimDefault = default;                                                    *
 * AnimName = HeadTurn;                                                      *
 * AnimNext = default;                                                       *
 * AnimFrameNumber = 3;                                                      *
 * AnimFramePosition = 0 0 0  1 0 0;                                         *
 * AnimFramePosition = 0 0 2  1 0 0;                                         *
 * AnimFramePosition = 0 0 4  1 0 0;                                         *
 * AnimName = default;                                                       *
 * AnimNext = NULL;                                                          *
 * AnimFrameNumber = 1                                                       *
 * AnimPosition = 0 1 ...                                                    *
 * ...                                                                       *
 *                                                                           *
 * ObjID = 1                                                                 *
 * BoundingBox =...                                                          *
 *                                                                           *
 * EndOfFile                                                                 *
 *                                                                           *
 * Remarques :                                                               *
 *   ObjID       : identifiant unique de l'objet élément. Utilisé pour voir  *
 *                 les dépendance pere/fils.                                 *
 *   BoundingBox : liste des points du bounding box (8 points)               *
 *   Children    : liste des ID des objets enfants. UNIQUEMENT si            *
 *                 ChildrenNumber est != 0                                   *
 *   AnimNumber  : nombre d'animation pour cet objet. DOIT être suivit pas   *
 *                 les définitions de ces animations                         *
 *   AnimName    : nom de l'animation, utilisé dans le programme pour        *
 *                 choisir cette animation                                   *
 *   AnimNext    : Nom de la prochaine animation. Sera sélectionné APRES que *
 *                 l'animation soit terminée. Si == NULL, alors l'animation  *
 *                 bouclera sur elle-même.                                   *
 *   AnimFramePosition : liste des coordonées du point et des angles pour    *
 *                 UNE frame (attention, ne pas changer le mot clé pour      *
 *                 toutes les frames de l'animation)                         *
 *  Au moins une frame si l'objet doit être renderer                         *
 *   EndOfFile   : fin du fichier de description, tout le reste du fichier   *
 *                 sera ignoré                                               *
 * IMPORTANT : les mots clé ObjID et AnimName change le contexte du          *
 * programme:                                                                *
 * En un mot après ObjID, on a un NOUVEAU objet, après AnimName on a une     *
 * NOUVELLE animation.                                                       *
 * Attention: le format de fichier n'est pas très "strict", dans le sens ou  *
 * l'ordre et la présence de tous les champs est obligatoire (ou non, par    *
 * exemple children ne dois pas être présent quand childrenNumber est mis    *
 * à 0)                                                                      *
 * fm2 version 2.0 : fichier binaire (donc réalisation d'un éditeur !!)      *
 *   => à venir                                                              *
\*****************************************************************************/


/*****************************************************************************\
 * Classe C3DObjAnim                                                         *
\*****************************************************************************/

/* constructeur */
C3DObjAnim::C3DObjAnim()
{
  m_name            = NULL;
  m_uid             =  0;
  m_frame_number    =  0;
  m_frame_current   =  0;
  m_frames          = NULL;
  m_anim_next_ind   = -1;
  m_framerate_ratio =  0;
  m_loop            = false;
  m_anim_next_str    = NULL;
}

/* destructeur */
C3DObjAnim::~C3DObjAnim()
{
  if (m_name)
    delete[] m_name;
  if (m_frames)
  {
    for (UINT i=0; i < m_frame_number; ++i)
    {
      if (m_frames[i])
        delete m_frames[i];
    }
    delete[] m_frames;
  }
}

bool C3DObjAnim::Reset()
{
  m_frame_current=0;
  return true;
}

C3DObjAnim& C3DObjAnim::operator =(const C3DObjAnim &s)
{
  m_uid = s.m_uid;
  m_name = new char[strlen(s.m_name) + 1];
  strcpy(m_name, s.m_name);
  m_frame_number = s.m_frame_number;
  m_frame_current = s.m_frame_current;
  m_frames = new CPosition*[m_frame_number];
  for (int i=0; i<m_frame_number; ++i)
  {
    m_frames[i] = new CPosition;
    *(m_frames[i]) = *(s.m_frames[i]);
  }
  m_framerate_ratio = s.m_framerate_ratio;
  m_loop  = s.m_loop;
  m_anim_next_ind = s.m_anim_next_ind;
  m_anim_next_str = NULL;
  return *this;
}


/*****************************************************************************\
 * Classe C3DObjElement                                                      *
\*****************************************************************************/

/* C3DObjElement() ***********************************************************\
 * Constructeur par défaut                                                   *
\*****************************************************************************/
C3DObjElement::C3DObjElement()
{
  m_position.x       = 0;
  m_position.y       = 0;
  m_position.z       = 0;
  m_position.anglex  = 0;
  m_position.angley  = 0;
  m_position.anglez  = 0;
  memset(m_boundingbox.m_point, 0, sizeof(float) * BOUNDINGBOX_POINTS);
  m_anim_number      = 0;
  m_animations       = NULL;
  m_anim_default_ind = 0;
  m_anim_default_str = NULL;
  m_anim_current_ind = 0;
  m_boundingsphere   = 0.0f;

  m_str3dfile        = NULL;
  m_children_number  = 0;
  m_children         = NULL;
  m_loaded           = false;
  m_obj3d            = NULL;
  m_ObjectUID        = -1;
  m_attached_object  = NULL;
  m_displayed        = true;

  m_anim_takeobj_ind = ANIMATION_INVALID_VALUE;
  m_anim_dropobj_ind = ANIMATION_INVALID_VALUE;
  m_anim_takeobjstr  = NULL;
  m_anim_dropobjstr  = NULL;
  m_attached_object_uid = (unsigned int) -1;
  m_enabled = true;
  m_particles = NULL;
  m_particle_uid = 0;
}

/* ~C3DObjElement() **********************************************************\
 * Destructeur                                                               *
\*****************************************************************************/
C3DObjElement::~C3DObjElement()
{
  if (m_children)
  {
    for (UINT i=0; i < m_children_number ; ++i)
    {
      if (m_children[i])
      {
        delete m_children[i];
      }
    }
    delete[] m_children;
  }

  if (m_animations)
  {
    for (UINT i=0; i < m_anim_number ; ++i)
    {
      if (m_animations[i])
      {
        delete m_animations[i];
      }
    }
    delete[] m_animations;
  }
  if (m_str3dfile)
    delete[] m_str3dfile;
  //delete m_obj3d; => ne pas supprimer l'object 3D car il est mutualisé entre
  //                 tous les objets!
  // il sera supprimer après indépendament
  if (m_particles)
  {
    delete m_particles;
    m_particles = NULL;
    m_particle_timer.Stop();
  }

}

bool C3DObjElement::Reset()
{
  int i;
  m_anim_default_str    = NULL;
  m_anim_current_ind    = m_anim_default_ind;
  m_anim_dropobjstr     = NULL;
  m_anim_takeobjstr     = NULL;
  for (i=0; i<m_anim_number; ++i)
    m_animations[i]->Reset();
  m_attached_object     = NULL;
  m_attached_enabled    = false;
  m_attached_trigger    = TRIGGER_NONE;
  for (i=0; i<m_children_number; ++i)
    m_children[i]->Reset();
  m_enabled = true;
  if (m_particles)
    delete m_particles;
  m_particles = NULL;
  return true;
}

C3DObjElement& C3DObjElement::operator=(const C3DObjElement &d)
{
  int i;

  m_anim_current_ind    = d.m_anim_current_ind;
  m_anim_default_ind    = d.m_anim_default_ind;
  m_anim_default_str    = NULL;
  m_anim_dropobj_ind    = d.m_anim_dropobj_ind;
  m_anim_dropobjstr     = NULL;
  m_anim_number         = d.m_anim_number;
  m_anim_takeobj_ind    = d.m_anim_takeobj_ind;
  m_anim_takeobjstr     = NULL;
  m_boundingsphere      = d.m_boundingsphere;
  m_animations          = new C3DObjAnim*[d.m_anim_number];
  for (i=0; i<m_anim_number; ++i)
  {
    m_animations[i]     = new C3DObjAnim;
    *(m_animations[i])  = *(d.m_animations[i]);
  }
  m_attached_object     = NULL;
  m_attached_enabled    = false;
  m_attached_object_uid = d.m_attached_object_uid;
  m_attached_position   = d.m_attached_position;
  m_attached_trigger    = TRIGGER_NONE;

  m_boundingbox         = d.m_boundingbox;
  m_children_number     = d.m_children_number;

  m_children            = new C3DObjElement*[m_children_number];
  for (i=0; i<m_children_number; ++i)
  {
    m_children[i]       = new C3DObjElement;
    *(m_children[i])    = *(d.m_children[i]);
  }

  m_displayed           = d.m_displayed;
  m_loaded              = d.m_loaded;
  m_ObjectUID           = d.m_ObjectUID;
  m_position            = d.m_position;

  if (d.m_str3dfile)
  {
    m_str3dfile           = new char[strlen(d.m_str3dfile) + 1];
    strcpy(m_str3dfile, d.m_str3dfile);
  }

  m_obj3d               = d.m_obj3d;
  m_enabled             = d.m_enabled;
  m_particles           = NULL;
  m_particle_uid        = d.m_particle_uid;
  m_particle_position   = d.m_particle_position;
  return *this;
}


/* SelectDefaultAnimation() **************************************************\
 * Sélection de l'animation par défaut (initialisation des anim_current à    *
 * anim_defaut en fait                                                       *
 * On peut spécifier une paramètre qui FORCERA TOUTES les animations à       *
 * celle-là (si elle existe bien sur)                                        *
\*****************************************************************************/
bool C3DObjElement::SelectDefaultAnimation(UINT forcedUID) 
// = ANIMATION_NO_ANIMATION)
{
  unsigned int i;
  for (i = 0; i < m_children_number; ++i)
    m_children[i]->SelectDefaultAnimation(forcedUID);

  if (forcedUID != ANIMATION_INVALID_VALUE)
    for (i = 0; i < m_anim_number ; ++i)
    {
      if (m_animations[i]->m_uid == forcedUID)
      {
        m_anim_current_ind = i;
        return true;
      }    
    }
  /* dans le cas où l'animation n'est pas trouvée, il faut quand même mettre 
   * une animation !!!
   */
  m_anim_current_ind = m_anim_default_ind;
  return false;
}

/* ChangeAnimation() *********************************************************\
 * Change l'animation olduid en l'animation newid. Pas besoin que cette      *
 * animation soit celle en cours. Si cette animation n'existe pas, continue. *
\*****************************************************************************/
bool C3DObjElement::ChangeAnimation(UINT curUID, UINT newUID)
{ 
  if (!m_enabled) return false;

  unsigned int i,j;
  bool res = false;
  for (i = 0; i < m_children_number; ++i)
  {
    res = m_children[i]->ChangeAnimation(curUID, newUID);
  }
  if (curUID == ANIMATION_INVALID_VALUE)
  {
    for (j = 0; j < m_anim_number ; ++j)
    {
      if (m_animations[j]->m_uid == newUID)
      {
        m_animations[m_anim_current_ind]->m_anim_next_ind = j;
        return true;
      }    
    }

  }
  else
  {
    for (i = 0; i < m_anim_number ; ++i)
    {
      if (m_animations[i]->m_uid == curUID)
      {
        for (j = 0; j < m_anim_number ; ++j)
        {
          if (m_animations[j]->m_uid == newUID)
          {
            m_animations[i]->m_anim_next_ind = j;
            return true;
          }    
        }
      }
    }
  }
  return res;
}

/* ChangeNextAnimation() *****************************************************\
 * Change les next_anim de toutes les animations                             *
 * Toutes les animations qui auront pour next anim oldUID sera changé en     *
 * newUID
\*****************************************************************************/
bool C3DObjElement::ChangeNextAnimation(UINT oldUID, UINT newUID)
{
  if (!m_enabled) return false;

  bool res=false;
  unsigned int i,j,k;
  for (i = 0; i < m_children_number; ++i)
  {
    res = m_children[i]->ChangeNextAnimation(oldUID, newUID);
  }
  for (i = 0; i < m_anim_number ; ++i)
  {
    if (m_animations[i]->m_uid == oldUID)
    {
      for (j = 0; j < m_anim_number ; ++j)
      {
        if (m_animations[j]->m_uid == newUID)
        {
          // a ce point on a trouvé les indexes des animations oldUID et
          // newUID
          // maintenant on parcours les animations encore un fois pour
          // effectivement remplacer.
          for (k = 0; k < m_anim_number ; ++k)
          {
            if (m_animations[k]->m_anim_next_ind == i) // if nextanim = old
            {
              m_animations[k]->m_anim_next_ind = j;   // nextanim = new
              res=true;
            }
          }
        }    
      }
    }
  }
  return res;
}




/* Load() ********************************************************************\
 * Charge les fichiers 3ds et textures nécessaire à l'animation de l'objet   *
 * des ses fils.                                                             *
 * Objet 3D et textures sont stoqués dans des listes "globales" pour éviter  *
 * les redondances en mémoire                                                *
 * szobjpath : chemin vers le dossier où sont stoqué les objets 3ds          *
 * sztexpath : chemin vers le dossier contenant les textures tga 24b         *
 * loadchildren : charger aussi les enfants?                                 *
 * texture_list : liste de textures déjà chargées                            *
 * objects_list : liste des fichiers 3ds déjà chargés                        *
\*****************************************************************************/
bool C3DObjElement::Load( char * szobjpath, char * sztexpath, 
                         bool loadchildren, t3DTextures *textures_list, 
                         t3DObjects *objects_list, tLightMaterial *light_material)
{
  char sztmp[256];

  m_loaded = false;
  if (loadchildren)
  {
    for (unsigned int i = 0; i < m_children_number; ++i)
    {
      if (!m_children[i]->Load(szobjpath, sztexpath, true, textures_list, 
                               objects_list, light_material))
      {
        cerr << "Chargement du fichier " <<  (m_str3dfile?m_str3dfile:"") << " échoué" << endl;
        return false;
      }
    }
  }
  cout << "Chargement du fichier 3ds" << (m_str3dfile?m_str3dfile:"") << endl;
  if (m_str3dfile && *m_str3dfile)
  {
    unsigned int i, j, r;
    m_obj3d = NULL;
    for (j = 0; j < objects_list->m_objects_number; ++j)
    {
        if (objects_list->m_objects_filenames[j] == m_str3dfile)
        {
          m_obj3d = objects_list->m_objects[j];
          break;
        }
    }
    if (!m_obj3d) // fichier non chargé
    {
      m_obj3d = new CLoad3DS;
      objects_list->m_objects.push_back(m_obj3d);
      strcpy(sztmp, szobjpath);
      strcat(sztmp, m_str3dfile);
      cout << "Importation..." << endl;
      if (!m_obj3d->Import3DS(sztmp))
      {
        cerr << "Erreur lors du chargement du fichier " << m_str3dfile << endl;
        return false;
      }
      else
      {
        if (light_material)
          m_obj3d->AssignMaterial(*light_material);
        cout << "Importation finie" << endl;
        objects_list->m_objects_filenames.push_back(m_str3dfile);
        objects_list->m_objects_number++;
      }
    }
    if (!m_obj3d)
    {
      cerr << "Erreur fatale très dangeureuse!!!!" << endl;
      return false;
    }

    cout << "Chargement des textures" << endl;

    // A PARTIR DE LA CE N'EST PLUS MON CODE, C'EST DU REPOMPE SUR L'EXEMPLE
    // DONNE AVEC LA LIBRAIRIE DE CHARGEMENT D'UN FICHIER 3ds

    // Depending on how many textures we found, load each one // (Assuming BMP)
    // If you want to load other files than bitmaps, you will need to adjust 
    // CreateTexture().
    // Below, we go through all of the materials and check if they have a 
    // texture map to load.
    // Otherwise, the material just holds the color information and we don't 
    // need to load a texture.

    // Go through all the materials
    for(i = 0; (int)i < m_obj3d->m_model3D.numOfMaterials; i++)
    {
      // Check to see if there is a file name to load in this material
      if(strlen(m_obj3d->m_model3D.pMaterials[i].strFile) > 0)
      {
        r=0;
        for (j = 0; j < textures_list->m_textures_number; ++j)
        {
            if (textures_list->m_textures_filenames[j] ==
                  m_obj3d->m_model3D.pMaterials[i].strFile)
            {
              m_obj3d->m_texture[i] = textures_list->m_textures[j];
              r=1;
              break;
            }
        }
        if (r==0)
        {
          // Use the name of the texture file to load the bitmap, with a 
          // texture ID (i).
          // We pass in our global texture array, the name of the texture,
          // and an ID to reference it. 
          if (glIsTexture(m_obj3d->m_texture[i]))
            glDeleteTextures(1, &m_obj3d->m_texture[i]);
          glGenTextures(1, &(m_obj3d->m_texture[i]));	// Create The Texture

          strcpy(sztmp, sztexpath);
          strcat(sztmp, m_obj3d->m_model3D.pMaterials[i].strFile);

          // PETITE MODIFICATION PERSO
          if (!tgaLoad(sztmp, m_obj3d->m_texture[i]) )
          {
            cerr << "Impossible de charger la textures " <<
                    m_obj3d->m_model3D.pMaterials[i].strFile << endl;
            return false;
          }
          //LoadJpg(m_model3D.pMaterials[i].strFile,m_texture[i]);
          textures_list->m_textures_filenames.push_back(
                                         m_obj3d->m_model3D.pMaterials[i].strFile
                                                     );
          textures_list->m_textures.push_back(m_obj3d->m_texture[i]);
          ++textures_list->m_textures_number;
        }
      }

      // Set the texture ID for this material
      m_obj3d->m_model3D.pMaterials[i].texureId = i;
    }
  }
  // FIN DU CODE PAS PERSO REPRISE DE MON SUPERBE CODE
  // si succès loaded=true sinon loaded = false;
  m_loaded = true;
  cout << "Fin du chargement de l'élément" << endl;

  return true;
}


/* Render() ******************************************************************\
 * Dessiner l'objet ainsi que tous ses fils affichable à la position         *
 * actuelle                                                                  *
 * (cette position devra être changé par des glTranslate, glRotate...)       *
\*****************************************************************************/
bool C3DObjElement::Render(bool force) // force = false par défaut => .h
{
  if (!m_enabled) return false;
  if (!force)
  {
    // mise à jour du compteur de gestion des animations
    Update();
  }
  if (m_displayed || force)
  {
    glPushMatrix();
     // probleme avec Lightwave et son orientation des axes:
     // pas en sens direct (x est inversé)
      glTranslatef(-m_position.x, m_position.y, m_position.z);
      glRotatef(-m_position.anglex, 1.0, 0.0, 0.0);
      glRotatef(m_position.angley, 0.0, 1.0, 0.0);
      glRotatef(-m_position.anglez, 0.0, 0.0, 1.0);
 
      glPushMatrix();
        // à cause du X inversé, il faut faire une rotation de 180°
        // selon xOz
        glRotatef(180.0, 0.0, 1.0, 0.0);
        if (m_obj3d)
          m_obj3d->Render(); // on render l'objet
        if (m_particles && m_particles->IsEnabled())
        {
          m_particles->Update(m_particle_timer.Getdt()/1000.0f);
          m_particle_timer.Update();
          m_particles->Render();
        }
        if (m_attached_object && m_attached_enabled)
        {
          glPushMatrix();
            glTranslatef( m_attached_position.x, 
                          m_attached_position.y,
                         -m_attached_position.z);
            glRotatef(m_attached_position.anglex, 1.0, 0.0, 0.0);
            glRotatef(m_attached_position.angley, 0.0, 1.0, 0.0);
            glRotatef(m_attached_position.anglez, 0.0, 0.0, 1.0);
            m_attached_object->Render(true);
          glPopMatrix();
        }
      glPopMatrix();

      // rendu des enfants aussi
      for (unsigned int i = 0; i < m_children_number; ++i)
      {
        m_children[i]->Render(force);
      }
    glPopMatrix();
  }
  return true;
}


/* RenderBoundingBox() *******************************************************\
 * Dessiner le bounding box de l'objet. Uniquement à des fins de test/debug  *
\*****************************************************************************/
bool C3DObjElement::RenderBoundingBox(bool recurse) // recurse = true par défaut
{
  if (m_displayed )
  {
    glPushMatrix();
     // probleme avec Lightwave et son orientation des axes:
     // pas en sens direct (x est inversé)
      glTranslatef(-m_position.x, m_position.y, m_position.z);
      glRotatef(-m_position.anglex, 1.0, 0.0, 0.0);
      glRotatef(m_position.angley, 0.0, 1.0, 0.0);
      glRotatef(-m_position.anglez, 0.0, 0.0, 1.0);
 
      if (m_boundingbox.m_active)
      {
        glPushMatrix();
        // bizarrement pour les bounding box (qui ne dépend donc pas 
        // du chargeur 3DS ni même du fichier LWO, il ne faut pas
        // faire une rotation de 180° mais une symétrie selon
        // le plan yOz (x -> -x))
        // TODO : appeler Mulder pour l'explication de cet étrange
        // phénomène
          glScalef(-1.0,1.0,1.0);
          glColor3f(1.0,0.0,0.0);
      
          glBegin(GL_LINE_LOOP);
            glVertex3fv(m_boundingbox.m_point[0]);
            glVertex3fv(m_boundingbox.m_point[1]);
            glVertex3fv(m_boundingbox.m_point[2]);
            glVertex3fv(m_boundingbox.m_point[3]);
          glEnd();
          glBegin(GL_LINE_LOOP);
            glVertex3fv(m_boundingbox.m_point[0]);
            glVertex3fv(m_boundingbox.m_point[4]);
            glVertex3fv(m_boundingbox.m_point[7]);
            glVertex3fv(m_boundingbox.m_point[3]);
          glEnd();
          glBegin(GL_LINE_LOOP);
            glVertex3fv(m_boundingbox.m_point[0]);
            glVertex3fv(m_boundingbox.m_point[1]);
            glVertex3fv(m_boundingbox.m_point[5]);
            glVertex3fv(m_boundingbox.m_point[4]);
          glEnd();
          glBegin(GL_LINE_LOOP);
            glVertex3fv(m_boundingbox.m_point[4]);
            glVertex3fv(m_boundingbox.m_point[5]);
            glVertex3fv(m_boundingbox.m_point[6]);
            glVertex3fv(m_boundingbox.m_point[7]);
          glEnd();
          glBegin(GL_LINE_LOOP);
            glVertex3fv(m_boundingbox.m_point[5]);
            glVertex3fv(m_boundingbox.m_point[1]);
            glVertex3fv(m_boundingbox.m_point[2]);
            glVertex3fv(m_boundingbox.m_point[6]);
          glEnd();
          glBegin(GL_LINE_LOOP);
            glVertex3fv(m_boundingbox.m_point[7]);
            glVertex3fv(m_boundingbox.m_point[6]);
            glVertex3fv(m_boundingbox.m_point[2]);
            glVertex3fv(m_boundingbox.m_point[3]);
          glEnd();

          // Mulder?
        glLineWidth(5);
        glBegin(GL_LINES);
          glColor3f  ( 1.0f, 0.0f, 0.0f); // axe X
          glVertex3f ( 0.0f, 0.0f, 0.0f); glVertex3f( 5000.0,   0.0f,  0.0f);
          glColor3f  ( 0.0f, 1.0f, 0.0f); // axe Y
          glVertex3f ( 0.0f, 0.0f, 0.0f); glVertex3f(  0.0f, 5000.0f,  0.0f);
          glColor3f  ( 0.0f, 0.0f, 1.0f); // axe Z
          glVertex3f ( 0.0f, 0.0f, 0.0f); glVertex3f(  0.0f,  0.0f, 5000.0f);
        glEnd();
        glLineWidth(1);


          if (m_attached_object && m_attached_enabled)
          {
            glPushMatrix();
              glTranslatef( m_attached_position.x, 
                            m_attached_position.y,
                            m_attached_position.z);
              glRotatef(m_attached_position.anglex, 1.0, 0.0, 0.0);
              glRotatef(m_attached_position.angley, 0.0, 1.0, 0.0);
              glRotatef(m_attached_position.anglez, 0.0, 0.0, 1.0);
              m_attached_object->RenderBoundingBox();
            glPopMatrix();
          }

        glPopMatrix();
      }
      if (recurse)
      {
        // rendu des enfants aussi
        for (unsigned int i = 0; i < m_children_number; ++i)
        {
          m_children[i]->RenderBoundingBox();
        }
      }

    glPopMatrix();
  }
  return true;
}

/* Update() ******************************************************************\
 * Mise à jour des compteurs de l'animation en cours. Boucle si l'animation  *
 * doit boucler, va à l'animation suivante si on a atteind la dernière frame *
 * et calcul aussi les fausses frames entre les KeyFrames (interpolation     *
 * linéaire)                                                                 *
\*****************************************************************************/
bool C3DObjElement::Update(bool recurse) // recurse == false par défaut
{
  if (!m_enabled) return false;
  int animation_finished_ind = -1;

  if (m_anim_number && m_anim_current_ind != -1 && m_animations[m_anim_current_ind]->m_frame_number)
  { // sélection de la bonne frame (position) dans la liste des
    // animations


    C3DObjAnim *panim;
    panim = m_animations[m_anim_current_ind];


    // premier cas : on a un frame rate == au frame rate de l'objet
    // chaque frame => on bouge l'objet
    if (panim->m_framerate_ratio == 0 || panim->m_framerate_ratio == 1 || 
        panim->m_framerate_ratio == -1
       )
    {
      if (panim->m_anim_next_ind != -1 && panim->m_frame_current == panim->m_frame_number - 1)
        // ne pas looper l'animation, mais aller sur la NEXT si finie
      {
        animation_finished_ind = m_anim_current_ind;

        m_anim_current_ind = panim->m_anim_next_ind;
        if (panim->m_loop == true)
          panim->m_anim_next_ind = -1;
        panim->m_frame_current = 0;
        panim = m_animations[m_anim_current_ind];
      }
      panim->m_frame_current = (panim->m_frame_current + 1) % panim->m_frame_number;
      int m_frame_current = m_animations[m_anim_current_ind]->m_frame_current;
      m_position = *(m_animations[m_anim_current_ind]->m_frames[m_frame_current]);
    }


    else if (panim->m_framerate_ratio > 1)
    { // ratio > 1 => il faut aller "moins" vite, donc rajouter
      // des frames (interpolation simple (linéaire) )
      
      int ratio = panim->m_framerate_ratio;
      float percent;

      if (panim->m_anim_next_ind != -1 && 
          panim->m_frame_current == ((panim->m_frame_number -1) * ratio)) 
        // ne pas looper l'animation, mais aller sur la NEXT si finie
      {
        animation_finished_ind = m_anim_current_ind;

        panim->m_frame_current = 0;
        m_anim_current_ind = panim->m_anim_next_ind;        
        if (panim->m_loop == true)
          panim->m_anim_next_ind = -1;
        panim = m_animations[m_anim_current_ind];
      }
      panim->m_frame_current = 
        (panim->m_frame_current + 1) 
                % 
        (panim->m_frame_number * ratio);

      int frame_number = panim->m_frame_number;

      int frame_current = panim->m_frame_current / ratio;
      int frame_next = (frame_current + 1) % frame_number;

      percent = ((float)(panim->m_frame_current % ratio) / ((float) ratio));

      m_position =
       (*(m_animations[m_anim_current_ind]->m_frames[frame_current])) * (1.0-percent)
        + 
       (*(m_animations[m_anim_current_ind]->m_frames[frame_next])) * (percent);        
    }

    

    else // là on a un frame rate < 1, donc il faut aller + vite que ce qui
    {    // est décrit dans l'objet => il faut DROPER des frames

      int ratio = - panim->m_framerate_ratio;

      if (panim->m_anim_next_ind != -1 && 
          panim->m_frame_current + ratio >= panim->m_frame_number - 1 ) 
        // ne pas looper l'animation, mais aller sur la NEXT si finie
      {
        animation_finished_ind = m_anim_current_ind;

        m_anim_current_ind = panim->m_anim_next_ind;        
        if (panim->m_loop == true)
          panim->m_anim_next_ind = -1;
        panim->m_frame_current = 0;
        panim = m_animations[m_anim_current_ind];
        ratio = - panim->m_framerate_ratio;
      }

      panim->m_frame_current = 
         (panim->m_frame_current + ratio) % panim->m_frame_number;


      m_position = *(m_animations[m_anim_current_ind]->m_frames[
                                    m_animations[m_anim_current_ind]->m_frame_current
                                                           ]
                  );


    }
  }


  // gestion des triggers

  if (m_attached_trigger != TRIGGER_NONE && animation_finished_ind != -1)
  {
    // je savais bien qu'il y avait un trigger installé sur cette animation
    // est-ce qu'il est activé sur une prise d'object?
    if (m_attached_trigger == TRIGGER_TAKE_OBJECT /*&&
                                    m_anim_takeobj_ind == animation_finished_ind*/)
    {
      // valider la prise de l'objet
      if (m_attached_object == NULL)
        cerr << "Trigger invalide : trigger installé mais pas d'objet "
                "spécifié" << endl;
      else
      {
        m_attached_object->m_displayed = false;
        m_attached_enabled = true;
        m_attached_trigger = TRIGGER_NONE;
      }
    }
    // peut etre l'est il sur une dépose d'objet?
    else if (m_attached_trigger == TRIGGER_DROP_OBJECT /* &&
                                    m_anim_dropobj_ind == animation_finished_ind*/)
    {
      // valider le drop de l'objet
      if (m_attached_object == NULL)
        cerr << "Trigger invalide : trigger installé mais pas d'objet "
                "spécifié" << endl;
      else
      {
        m_attached_object->m_displayed = true;
        m_attached_object  = NULL;
        m_attached_enabled = false;
        m_attached_trigger = TRIGGER_NONE;
        m_attached_object  = NULL;
      }
    }
  }

  if (recurse)
  {
    for (unsigned int i = 0; i < m_children_number; ++i)
    {
      m_children[i]->Update(true);
    }
  }


  return true;
}

/* ChangeFrameRateRatio() ****************************************************\
 * Changement du nombre de frame (calculée) entre 2 keyframes (stockée dans  *
 * le fichier .fm?)                                                          *
 * un newrate > 1 signifie que je dois interpoler de nouvelle frames pour    *
 * garder l'animation fluide                                                 *
 * un newrate == 1 signifie qu'on prendre les keyframes une par une          *
 * un newsrate < 1 signifie que je doit dropper une frames sur 1/newsrate    *
\*****************************************************************************/
bool C3DObjElement::ChangeFrameRateRatio( float newrate )
{
  if (!m_enabled) return false;

  if (newrate < 0)
    return false;
  unsigned int i;
  for (i = 0; i < m_children_number; ++i)
  {
    m_children[i]->ChangeFrameRateRatio(newrate);
  }

  C3DObjAnim *panim;
  for (i = 0; i < m_anim_number ; ++i)
  {
    panim = m_animations[i];

    float lastratio = panim->m_framerate_ratio;
    // ne rien faire si lastratio > 1.0
    if (lastratio < -1.0f)
      lastratio = - 1.0f / lastratio;
    else if (! (lastratio > 1.0f))// if lastratio == 0.0f
      lastratio = 1.0;

    if (newrate < 1.0)
      panim->m_framerate_ratio = - ((int) floor((1.0 / newrate)));
    else if (newrate > 1.0)
      panim->m_framerate_ratio = (int) floor(newrate);
    else // newrate == 1.0
      panim->m_framerate_ratio =0;

    // il faut quand même mettre à jour la nouvelle position courante
    // pour éviter de faire un trop gros "cassage" dans l'animation
    panim->m_frame_current = 
      (unsigned int) panim->m_frame_current * ((float) lastratio / (float)newrate);
  }
  return true;
}

/* RandomAnimationStart() ****************************************************\
 * Mise à un point aléatoire dans l'animation AnimUID                        *
 * (sur une keyframe quand même)                                             *
 * parametres : AnimUID : UID de l'animation à prendre en compte             *
 *              samepos : même position de départ pour toutes ces animations *
 *              gem     : utilisé en interne pour transmettre la valeur de   *
 *                        la position à sélectionner                         *
 * ATTENTION: n'utiliser samepos = true UNIQUEMENT pour les animations de    *
 *            MEME taille pour tous les objets!                              *
\*****************************************************************************/
bool C3DObjElement::RandomAnimationStart(UINT AnimUID, bool samepos, 
                                          int gem)
// default : samepos = false, gem = -1
{
  unsigned int i;
  bool res = true;
  for (i = 0; i < m_anim_number ; ++i)
  {
    if (m_animations[i]->m_uid == AnimUID)
    {
      if (m_animations[i]->m_anim_next_ind == -1)
      {
        if (samepos == false && gem == -1)
        {
          float f = (float) rand();
          f = f / ((float) RAND_MAX) * (float) m_animations[i]->m_frame_number;
          if (m_animations[i]->m_framerate_ratio > 1)
            f *= m_animations[i]->m_framerate_ratio;
          m_animations[i]->m_frame_current = (int) floor(f);
          gem = m_animations[i]->m_frame_current;
        }
        else
        {
          if (gem < m_animations[i]->m_frame_number * 
            (m_animations[i]->m_framerate_ratio>1?m_animations[i]->m_framerate_ratio:1)
             )
            m_animations[i]->m_frame_current = gem;
          else
            res = false;
        }
      }
      else
      {
        res=false;
      }
    }
  }
  for (i = 0 ; i < m_children_number ; ++i)
  {
    if (!m_children[i]->RandomAnimationStart(AnimUID, samepos, gem))
      res= false;
  }
  return res;
}

/* RebuilDisplayLists ********************************************************\
 * Force les display list des objets 3D a être recréer à partir des          *
 * définitions des objets toujours en mémoire.                               *
 * Par exemple quand on change de résolution, il faut recréer les            *
 * displaylist car opengl détruit le viewport                                *
\*****************************************************************************/
bool C3DObjElement::RebuilDisplayLists()
{
  for (unsigned int i = 0 ; i < m_children_number ; ++i)
  {
    m_children[i]->RebuilDisplayLists();
  }
  if (m_obj3d)
  {
    if (m_obj3d->m_displaylist != -1)
      glDeleteLists(m_obj3d->m_displaylist,1);
    m_obj3d->m_displaylist = -1;
  }
  return true;
}

/* TakeObject ****************************************************************\
 * Prendre un objet                                                          *
 * Rechniquement cela consiste à installer le trigger de l'animation prise   *
 * de l'objet (TRIGGER_TAKE_OBJECT) et à récupèrer le nom de l'animation     *
 *qui sera propargé partout                                                  *
\*****************************************************************************/
bool C3DObjElement::TakeObject(C3DObjElement *pObject, UINT uObjUID)
{
  UINT u;
  // j'appelle la fonction qui effectue la liaison
  if ((u=TakeObject_priv(pObject, uObjUID)) != ANIMATION_INVALID_VALUE)
  {
    // et je change automatiquement d'animation
    return ChangeAnimation(ANIMATION_INVALID_VALUE, u);
  }
  return true;
}

/* TakeObject_priv ***********************************************************\
 * C'est ici que le programme fait effectivement le traitement adéquat       *
\*****************************************************************************/
UINT C3DObjElement::TakeObject_priv(C3DObjElement *pObject, UINT uObjUID)
{
  if (pObject)
  {
    // on est sur l'élément qui peut prendre l'objet
    if ((unsigned int) m_attached_object_uid == uObjUID)
    {
      m_attached_object = pObject;
      // s'il n'y a pas d'animation associée à la prise de cet objet
      if (m_anim_takeobj_ind == -1)
      {
        // faire l'attachement directement
        pObject->m_displayed = false;
        m_attached_enabled   = true;
        return ANIMATION_INVALID_VALUE;
      }
      else
      {
        // il y a une animation associée, il faut donc installer un trigger
        m_attached_trigger = TRIGGER_TAKE_OBJECT;
        m_attached_enabled = false;
        if (m_anim_current_ind != -1)
          m_animations[m_anim_current_ind]->m_anim_next_ind = m_anim_takeobj_ind;
        // voilà le trigger est installé il va se déclancher à la fin de 
        // l'animation d'UID anim_takeobj
        // (voir dans Update)
        return m_animations[m_anim_takeobj_ind]->m_uid;
      }
    }
    else
    {  
      UINT u = ANIMATION_INVALID_VALUE;
      for (unsigned int i = 0 ; i < m_children_number ; ++i)
      {
        if ((u=m_children[i]->TakeObject_priv(pObject, uObjUID)) != ANIMATION_INVALID_VALUE)
        {
          return u;
        }
      }
      return u;
    }
  }
  else
    return ANIMATION_INVALID_VALUE;
}

/* DropObject ****************************************************************\
 * Jeter un objet                                                            *
\*****************************************************************************/
bool C3DObjElement::DropObject(UINT uObjUID)
{
  if (!m_enabled) return false;

  UINT u;
  if (( u=DropObject_priv(uObjUID)) != ANIMATION_INVALID_VALUE)
  {
    return ChangeAnimation(ANIMATION_INVALID_VALUE, u);
  }
  return true;
}



UINT C3DObjElement::DropObject_priv(UINT uObjUID)
{
  // on est sur l'élément qui peut prendre/dropper l'objet
  if ((UINT) m_attached_object_uid == uObjUID)
  {
    if (m_attached_object)
    {
      // s'il n'y a pas d'animation associée à au drop de cet objet
      if (m_anim_dropobj_ind == -1)
      {
        m_attached_object->m_displayed = true;
        m_attached_object = NULL;
        m_attached_enabled = false;
        return ANIMATION_INVALID_VALUE;
      }
      else
      {
        // il y a une animation associée, il faut donc installer un trigger
        m_attached_trigger = TRIGGER_DROP_OBJECT;
        if (m_anim_current_ind != -1)
          m_animations[m_anim_current_ind]->m_anim_next_ind = m_anim_dropobj_ind;
        // voilà le trigger est installé il va se déclancher à la fin de 
        // l'animation d'UID anim_dropobj
        // (voir dans Update)
        return m_animations[m_anim_dropobj_ind]->m_uid;
      }
    }
    else
      return ANIMATION_INVALID_VALUE;
  }
  else
  {  
    UINT u = ANIMATION_INVALID_VALUE;
    for (unsigned int i = 0 ; i < m_children_number ; ++i)
    {
      if ((u=m_children[i]->DropObject_priv(uObjUID)) != ANIMATION_INVALID_VALUE)
      {
        return u;
      }
    }
    return u;
  }
}

//TODO: corriger tous ses parametres, tout mettre dans une structure, 
// bref s'arranger pour éviter d'empiler tous ses parametres...
bool C3DObjElement::InitParticleSystem( unsigned int partuid, unsigned int number, float  debit, float life, float fade,
                                        float red, float green, float blue, float alpha, 
                                        bool textured, unsigned int gltextureuid,
                                        float xsize, float ysize,
                                        CVector3f &grav,
                                        CVector3f &randomplan, CVector3f randomspeed,
                                        CREATEPARTICLECALLBACK pfCreateCallBack,
                                        UPDATEPARTICLECALLBACK pfUpdateCallBack)
{
  if (m_particle_uid == partuid)
  {
    if (!m_particles)
      m_particles = new CCastParticleSystem;
    CVector3f v1(m_particle_position.x, m_particle_position.y, m_particle_position.z),
              v2(m_particle_position.anglex, m_particle_position.angley, m_particle_position.anglez);
    m_particles->Init(number, debit, life, fade,
              red, green, blue, alpha, 
              textured, gltextureuid,
              xsize, ysize,
              v1, 
              v2, grav,
              randomplan, randomspeed,
              pfCreateCallBack,
              pfUpdateCallBack);
    return true;
  }
  // TODO: parcourir en utilisant une pile
  for (unsigned int i=0; i < m_children_number; ++i)
  {
    if (m_children[i]->InitParticleSystem(partuid, number, debit, life, fade,
              red, green, blue, alpha, 
              textured, gltextureuid,
              xsize, ysize,
              grav,
              randomplan, randomspeed,
              pfCreateCallBack,
              pfUpdateCallBack))
      return true;
  }
  return false;

}
//TODO : gerer plusieur systeme de particule par objet
void C3DObjElement::StartParticleSystem()
{
  if (m_particles)
  {
    m_particles->Enable();
    m_particle_timer.Start();
  }
  for (unsigned int i=0; i < m_children_number; ++i)
  {
      m_children[i]->StartParticleSystem();
  }
}
void C3DObjElement::StopParticleSystem()
{
  if (m_particles)
  {
    m_particles->Disable();
    m_particles->Reset();
    m_particle_timer.Stop();
  }
  for (unsigned int i=0; i < m_children_number; ++i)
  {
      m_children[i]->StopParticleSystem();
  }
}
void C3DObjElement::DeleteParticleSystem()
{
  if (m_particles)
  {
    delete m_particles;
    m_particles = NULL;
    m_particle_timer.Stop();
  }
  for (unsigned int i=0; i < m_children_number; ++i)
  {
      m_children[i]->DeleteParticleSystem();
  }
}


/*****************************************************************************\
 * Fonctions génériques                                                      *
\*****************************************************************************/
// hypothese : on suppose qu'une ligne dans le fichier texte ne fait pas
// plus de 256 caractères.
#define LINE_CHAR_COUNT 256
#define REMOVELASTRETURN(a)        \
{                                  \
    int l = strlen(a);             \
	  if (a[l-1] == '\n' )           \
	    a[l-1]=0;                    \
    l = strlen(a);                 \
	  if (a[strlen(a)-1] == '\r' )   \
	    a[strlen(a)-1]=0;            \
}
/*
void strrmcomments(char *strbuf);
bool strtrim(char *str);
bool getValueString(char *strbuf, char *strdest);
bool getValueInt( char *strbuf, int *iValue);
bool getValueFloatVectorsList(char *strbuf, FILE *f, int *iline,
                              float point[][3], int maxVectCount);
bool getValueIntList(char *strbuf, FILE *f, int *iline, 
                     int point[], int maxCount);

*/
// suppression des commentaires (//) dans la ligne donnée
void C3DObjLoader::strrmcomments(char *strbuf)
{
  int len, i;
  // suppression de tous les commentaires de la ligne
  len = strlen(strbuf);
  for (i=0; i< len -1 ; ++i)
  {
    if (strbuf[i] == '/' && strbuf[i+1]=='/')
    {
      strbuf[i] = 0; // c ça la suppression
      break; // fini on s'en va sans dire au revoir
    }
  }
}

// suppression des espaces génant en début et fin de str
bool C3DObjLoader::strtrim(char *str)
{
  char *p=str, *strtmp=str;
  if (*str)
  {
    while (*p == ' ' || *p == '\t')
      ++p;
    if (p != strtmp)
    {
      while (*p)
      {
        *strtmp++=*p++;
      }
      *strtmp=0;
    }
    p = str + strlen(str) - 1;
    while (p >= str && (*p == ' ' || *p =='\t'))
      --p;
    if (p>=str && p != str + strlen(str))
      *(p+1)=0;
    // TODO: y faire un peu + propre ou trouver l'eqv dans la std lib
    return true;
  }
  return false;
}

// extrait un entier de la ligne courrant
bool C3DObjLoader::getValueInt( char *strbuf, int *iValue)
{
  int i, len;
  char *strval;

  strval = strrchr(strbuf, '=');
  if (*strval)
  {
    ++strval;
    if (*strval)
    {
      len = strlen(strval);
      for (i = 0 ; i < len ; ++i)
      {
        if (strval[i] == ';')
        {
          strval[i] = 0;
          break;
        }
      }
      if (*strval)
      {
        if (i)
        {
          strtrim(strval);
          *iValue = atoi(strval);
          return true;
        }
      }
    }
  }
  return false;
}

// strbuf sera modifier !!!!
// Extraction d'une liste de float.
// peut être répartie sur plusieurs ligne pour une meilleur lisibilité
// donc on doit pouvoir bouger dans le fichier => FILE *f et iline
bool C3DObjLoader::getValueFloatVectorsList(char *strbuf, ifstream *f, int *iline,
                              float point[][3], int maxVectCount)
{
  char * strval, *strnextval;
  char strtmp[LINE_CHAR_COUNT];

  strrmcomments(strbuf);
  strtrim(strbuf);
  getValueString(strbuf, strtmp);
  if (!strcmpi(strtmp, "NULL"))
    return true;
  if (*strbuf)
  {
    strval = strrchr(strbuf, '=');
    if (*strval)
    {
      ++strval;
      if (*strval) // chaine après le '='
      {
        int iVect;
        int iCoord;
        float fval;
        int fin = 0;
        iCoord= 0; // X
        iVect = 0;
        int counter =1;
        while(fin == 0 && counter <= maxVectCount)
        {
          strtrim(strval);
          strnextval = strchr(strval,' '); 
          if (!strnextval)
          {
            strnextval = strchr(strval, ';');
            if (!strnextval)
              strnextval = NULL;
            else
            {
              *strnextval = 0;
              fin=1;
            }
          }
          else
            *(strnextval++)=0;
          strcpy(strtmp, strval);
          strtrim(strtmp);
          if (*strtmp)
          {
            fval = (float) atof(strtmp);
            point[iVect][iCoord]=fval;
          }
          if (iCoord != 2) // != Z
            ++iCoord;
          else
          {
            ++iVect;
            iCoord = 0; // X
            ++counter;
          }
          if (!fin)
          {
            if (strnextval && *strnextval)
              strval = strnextval;
            else // == NULL => pas de fin de donnée 
            {    //    (';')=> lire la ligne suivante
              if (!f->eof())
              {
                f->getline(strbuf, LINE_CHAR_COUNT-1, '\n');
                REMOVELASTRETURN(strbuf);
                strrmcomments(strbuf);
                strtrim(strbuf);
                strval=strbuf;
                if (*iline) ++*iline;
              }
              else
                return false;
            }
          }
        }
        return true;
      }
    }
  }
  return false;
}

// pareil que getValueFloatList mais avec des entiers.
bool C3DObjLoader::getValueIntList(char *strbuf, ifstream *f, int *iline, 
                     int point[], int maxCount)
{
  char * strval, *strnextval;
  char strtmp[LINE_CHAR_COUNT];
  strrmcomments(strbuf);
  strtrim(strbuf);
  getValueString(strbuf, strtmp);
  if (!strcmpi(strtmp, "NULL"))
    return true;
  if (*strbuf)
  {
    strval = strrchr(strbuf, '=');
    if (*strval)
    {
      ++strval;
      if (*strval) // chaine après le '='
      {
        int ival;
        int fin = 0;
        int counter =0;
        while(fin == 0 && counter <= maxCount)
        {
          ++counter;
          strtrim(strval);
          strnextval = strchr(strval,' '); 
          if (!strnextval)
          {
            strnextval = strchr(strval, ';');
            if (!strnextval)
              strnextval = NULL;
            else
            {
              *strnextval = 0;
              fin=1;
            }
          }
          else
            *(strnextval++)=0;
          strcpy(strtmp, strval);
          strtrim(strtmp);
          if (*strtmp)
          {
            ival = atoi(strtmp);
            point[counter-1]=ival;
          }
          if (!fin)
          {
            if (strnextval && *strnextval)
              strval = strnextval;
            else // == NULL => pas de fin de donnée
            {    //    (';')=> lire la ligne suivante
              if (!f->eof())
              {
                f->getline(strbuf, LINE_CHAR_COUNT-1, '\n');
                REMOVELASTRETURN(strbuf);
                strrmcomments(strbuf);
                strtrim(strbuf);
                strval=strbuf;
                if (*iline) ++*iline;
              }
              else
                return false;
            }
          }
        }
        return true;
      }
    }
  }
  return false;
}

// extraction d'une chaine de caractère
bool C3DObjLoader::getValueString(char *strbuf, char *strdest)
{
  char *strval;
  if (*strbuf)
  {
    strval = strrchr(strbuf, '=');
    if (*strval)
    {
      ++strval;
      if (*strval) // chaine après le '='
      {
        strcpy(strdest, strval);
        strtrim(strdest);
        char * p = strchr(strdest, ';');
        if (p)
          *p=0;
        else
          return false;
        return true;
      }
    }
  }
  return false;
}

UINT C3DObjLoader::getUIDFromName(char *strName, UNKNOWN_NAMEUID *NameUIDList)
{
  UNKNOWN_NAMEUID *pa;
  pa = NameUIDList;
  while (pa->m_strName)
  {
    if (!strcmpi(pa->m_strName, strName))
      return pa->m_iUID;
    pa++;
  }
  return (UINT) -1;
}

/* Chargement d'un objet complet (à partir d'un fichier de définition) 
 * pObj doit être l'adresse d'un pointeur initialisé à NULL.
 * filename : nom du fichier contenant les information
 * p3DObject : recevra un pointeur vers un arbre contenant les informations
 *   stoqués dans le fichier sous forme hierarchique.
 * AnimNameUIDList: continent une liste de ANIMATIONS_NAMEUID (dont le
 *   dernier élément est (NULL, NULL), qui contient la correspondance
 *   entre nom d'animation et Unique IDentifier de celle là.
 */
bool C3DObjLoader::Load3d(string filename, C3DObjElement **p3DObject,
            ANIMATIONS_NAMEUID *AnimNameUIDList,
            OBJECTS_NAMEUID    *ObjNameUIDList
           )
{
  char strbuf[LINE_CHAR_COUNT];// buffer de ligne
  char strdest[LINE_CHAR_COUNT];

  objstruct       *pObjHead   =NULL;
  objstruct       *pCurrentObj=NULL;
  int iCurrentAnim  = -1;
  int iCurrentFrame = -1;
  int iCurrentChild = -1;
  int iline         = 0;
  int i;

  int iAnimCounter  = 0; /* compteur d'animation.. */

  int CurObjUID     = -1;
  char strobjectname[LINE_CHAR_COUNT];
  *strobjectname = 0;

  Assert(*(filename.c_str())!=0, "filename doit contenir un chemin valide");
  Assert(p3DObject!=NULL, "p3DObject ne doit pas être NULL");
  Assert(AnimNameUIDList!=NULL, "AnimNameUIDList ne doit pas être NULL");
  Assert(ObjNameUIDList!=NULL, "ObjNameUIDList ne doit pas être NULL");

  ifstream f(filename.c_str());
  //f = fopen(filename, "r");
  if (!f)
  {
    cerr << "Impossible d'ouvrir le fichier" << filename << endl;
    return false;
  }
  cout << "Traitement du fichier " << filename << endl;

  bool loading = true;
  while (loading &&!f.eof())
  {
    f.getline(strbuf, LINE_CHAR_COUNT-1, '\n') ;
    REMOVELASTRETURN(strbuf);
    ++iline; // compteur de ligne
    if (*strbuf) // if strbuf != ""
    {
      strrmcomments(strbuf);
      strtrim(strbuf);


      // gestion de chaque entrées dans le fichier, au cas par cas
      // => c'est un genre d'éclatement...

      if (!strncmp(strbuf, "ObjectUID", 9))
      {
        if (!getValueString(strbuf, strobjectname))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "ObjectUID non spécifié" << endl;
          loading = false;
        }
        else
        {
          CurObjUID = getUIDFromName(strobjectname, ObjNameUIDList);
          pObjHead->p3DObjElement->m_ObjectUID = CurObjUID;
        }
      }
      
      
      else if (!strncmp(strbuf, "ElementID", 8))
      {
        if (pCurrentObj == NULL)
        {
          pObjHead = new objstruct;
          pCurrentObj = pObjHead;
        }
        else
        {
          pCurrentObj->pnext = new objstruct;
          pCurrentObj = pCurrentObj->pnext;
          pCurrentObj->pnext = NULL;
        }
        pCurrentObj->p3DObjElement = new C3DObjElement;        
        if (!getValueInt(strbuf, (int *) &(pCurrentObj->objID)))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Pas un nombre" << endl;
          loading = false;
        }
        else
        {
          iCurrentAnim=-1;
          iCurrentFrame=-1;
        }
      }


      else if (!strncmp(strbuf, "BoundingBox", 11))
      { // attention : une liste, éventuellement sur plusieurs lignes
        getValueString(strbuf, strdest);
        if ( strcmpi(strdest, "NULL")) // if str != NULL
        {
          pCurrentObj->p3DObjElement->GetBoundingBox().m_active = true;
          if (!getValueFloatVectorsList(strbuf, &f, &iline, 
                            pCurrentObj->p3DObjElement->GetBoundingBox().m_point,
                                       8)
             )
          {
            cerr << "Ligne invalide ("<< iline << "): " << endl << 
              strbuf << endl << 
              "Liste de 8 valeurs float invalide" << endl;
            loading = false;
          }
          {
            // maintenant que j'ai le bounding box, je calcule le bounding 
            // sphere correpondante (en gros, le rayon de la sphere
            // contenant tout l'objet)
            float max;
            max = Norme3fv(pCurrentObj->p3DObjElement->GetBoundingBox().m_point[0]);
            float f;
            for (i = 1; i < 8; ++i)
            {
              if (max < (f=Norme3fv(pCurrentObj->p3DObjElement->GetBoundingBox().m_point[i])))
                max = f;
            }
            pCurrentObj->p3DObjElement->m_boundingsphere = max;
          }
        }
      }


      else if (!strncmp(strbuf, "Object3D", 8))
      {
        if (!getValueString(strbuf,strdest))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Chaine invalide" << endl;
          loading = false;
        }
        else
          if (strcmpi(strdest, "NULL")) // if strfilename != "NULL"
          {
            pCurrentObj->p3DObjElement->m_str3dfile = 
                              new char[sizeof(char) * (strlen(strdest) + 1)];
            strcpy(pCurrentObj->p3DObjElement->m_str3dfile, strdest);
            pCurrentObj->p3DObjElement->m_loaded = false;          
          }
      }

      else if (!strncmp(strbuf, "ParticlePos", 11))
      {
        float supervect[2][3];
        if (!getValueFloatVectorsList(strbuf, &f, &iline,
             supervect,
             2) // (x,y,z) et (anglex, angley, anglez)
           )
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Liste de 8 valeurs float invalide" << endl;
          loading = false;
        }
        else
          pCurrentObj->p3DObjElement->m_particle_position = supervect;
      }

      else if (!strncmp(strbuf, "ParticleName", 12))
      {
        if (!getValueString(strbuf,strdest))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Chaine invalide" << endl;
          loading = false;
        }
        else if (strcmpi(strdest, "NULL")) // if strfilename != "NULL"
        {
          pCurrentObj->p3DObjElement->m_particle_uid = getUIDFromName(strdest,
                ObjNameUIDList);
          if (pCurrentObj->p3DObjElement->m_particle_uid == -1)
          {
            cerr << "Ligne invalide ("<< iline << "): " << endl << 
              strbuf << endl << 
              "Objet inconnu : " << strdest << endl;
            loading = false;
          }
          else
            cout << "Cet objet peut prendre : " << strdest << endl;
        }
      }


      else if (!strncmp(strbuf, "ObjectAimPos", 12))
      {
        float supervect[2][3];
        if (!getValueFloatVectorsList(strbuf, &f, &iline,
             supervect,
             2) // (x,y,z) et (anglex, angley, anglez)
           )
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Liste de 8 valeurs float invalide" << endl;
          loading = false;
        }
        else
          pCurrentObj->p3DObjElement->m_attached_position = supervect;
      }


      else if (!strncmp(strbuf, "ObjectAimName", 13))
      {
        if (!getValueString(strbuf,strdest))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Chaine invalide" << endl;
          loading = false;
        }
        else if (strcmpi(strdest, "NULL")) // if strfilename != "NULL"
        {
          pCurrentObj->p3DObjElement->m_attached_object_uid = getUIDFromName(strdest,
                ObjNameUIDList);
          if (pCurrentObj->p3DObjElement->m_attached_object_uid == -1)
          {
            cerr << "Ligne invalide ("<< iline << "): " << endl << 
              strbuf << endl << 
              "Objet inconnu : " << strdest << endl;
            loading = false;
          }
          else
            cout << "Cet objet peut prendre : " << strdest << endl;
        }
      }


      else if (!strncmp(strbuf, "ObjectTakeAnim", 14))
      {
        if (!getValueString(strbuf,strdest))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Chaine invalide" << endl;
          loading = false;
        }
        else if (strcmpi(strdest, "NULL")) // if strfilename != "NULL"
        {
          /* anim_takeobj doit être un index (indice de l'animation take obj 
           * dans la liste des animation de l'élément courant), pour des
           * raisons d'efficacité. Hors on ne connais pas a priori cette
           * animation, on sauvegarde temporairement sont nom pour effectuer
           * la recherche quand toutes les animations seront chargée */
          pCurrentObj->p3DObjElement->m_anim_takeobjstr =
                                                new char [strlen(strdest) + 1];
          strcpy(pCurrentObj->p3DObjElement->m_anim_takeobjstr, strdest);
        }
      }


      else if (!strncmp(strbuf, "ObjectDropAnim", 14))
      {
        if (!getValueString(strbuf,strdest))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl << 
            "Chaine invalide" << endl;
          loading = false;
        }
        else if (strcmpi(strdest, "NULL")) // if strfilename != "NULL"
        {
          pCurrentObj->p3DObjElement->m_anim_dropobjstr =
                                                new char [strlen(strdest) + 1];
          strcpy(pCurrentObj->p3DObjElement->m_anim_dropobjstr, strdest);
        }
      }

      else if (!strncmp(strbuf, "AnimDefault", 11))
      {
        if (!getValueString(strbuf,strdest))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
                  strbuf << endl << 
                  "Chaine invalide" << endl;
          loading = false;
        }
        else if (strcmpi(strdest, "NULL")) // if strfilename != "NULL"
        {
          pCurrentObj->p3DObjElement->m_anim_default_str= 
                                                new char[strlen(strdest) + 1];
          strcpy(pCurrentObj->p3DObjElement->m_anim_default_str, strdest);
        }
      }


      else if (!strncmp(strbuf, "AnimNumber", 10))
      {
        int i;
        if (!getValueInt(strbuf, &i))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl;
          loading = false;
        }
        else
        {
          pCurrentObj->p3DObjElement->m_anim_number=i;
          // mise à jour du compteur d'animation
          iAnimCounter += i;
          if (i)
          {
            pCurrentObj->p3DObjElement->m_animations = new C3DObjAnim*[i];
            memset(pCurrentObj->p3DObjElement->m_animations,0, 
                   sizeof(C3DObjAnim*)*i);
          }
        }
      }


      else if (!strncmp(strbuf, "AnimName", 8))
      {
        if (!getValueString(strbuf,strdest))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl;
          loading = false;
        }
        else
          {
          ++iCurrentAnim;
          pCurrentObj->p3DObjElement->m_animations[iCurrentAnim] = new C3DObjAnim;
          pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_name = 
                                                         new char[strlen(strdest)+1];
          strcpy(pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_name,strdest);
          if (!strcmpi(strdest, "NULL"))
          {
            cerr << "Ligne invalide ("<< iline << "): " << endl << 
              strbuf << endl << 
              "NULL invalide ici" << endl;
            loading = false;
          }
          else
            pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_uid = 
                getUIDFromName(strdest, AnimNameUIDList);
        }
      }


      else if (!strncmp(strbuf, "AnimNext", 8))
      {
        if (!getValueString(strbuf, strdest))
        {
          cerr << "Ligne invalide ("<< iline <<"): " << strbuf << endl;
          loading = false;
        }
        else if (pCurrentObj->p3DObjElement->m_animations && iCurrentAnim != -1)
        {
          if (!strcmpi(strdest, "NULL"))
          {
            pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_anim_next_ind
                                                                          = -1;
            pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_loop = true;
          }
          else
          {
            pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_loop = false;
            // l'animation est inconnue pour le moment, on vera si on la
            // trouve après que tout l'élément soit chargé
            // mais avant, il faut sauvegarder le nom
            pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_anim_next_str
                                                = new char[strlen(strdest) + 1];
            strcpy(pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->
                                                        m_anim_next_str, strdest);
          }
        }
        else
        {
          cerr << "Erreur à la ligne " << iline << endl;
          loading = false;
        }
      }


      else if (!strncmp(strbuf, "AnimFrameNumber", 15))
      {
        int i;
        if (!getValueInt(strbuf, &i))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl;
          loading = false;
        }
        else if (i)
        {
          iCurrentFrame = 0;
          pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_frame_number=i;
          pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->m_frames = 
                                                             new CPosition*[i];
        }
        // si i==0, pas de frame => pas affiché
      }


      else if (!strncmp(strbuf, "AnimFramePosition", 17))
      {
        float supervect[2][3];
        if (!getValueFloatVectorsList(strbuf, &f, &iline,
             supervect,
             2) // (x,y,z) et (anglex, angley, anglez)
           )
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl;
          loading = false;
        }
        else if (iCurrentFrame != -1 && iCurrentAnim != -1 )
        {
          pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->
            m_frames[iCurrentFrame] = new CPosition;
          *(pCurrentObj->p3DObjElement->m_animations[iCurrentAnim]->
            m_frames[iCurrentFrame]) = supervect;
          ++iCurrentFrame;
        }
        else
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl;
          loading = false;
        }
      }


      else if (!strncmp(strbuf, "ChildrenNumber", 14))
      {
        int i;
        if (!getValueInt(strbuf, &i))
        {
          cerr << "Ligne invalide ("<< iline << "): " << endl << 
            strbuf << endl;
          loading = false;
        }
        else if (i)
        {
          pCurrentObj->children_number = i;
          if (i)
          {
            pCurrentObj->childrenID = new UINT[ i ];
            iCurrentChild = 0;
          }
          else
          {
            pCurrentObj->childrenID=NULL;
          }
        }
      }


      else if (!strncmp(strbuf, "Children", 8))
      {
        if (pCurrentObj->children_number)
        {
          int *megavect = new int[pCurrentObj->children_number];
          if (!getValueIntList(strbuf, &f, &iline, 
               megavect,
               pCurrentObj->children_number) 
               // (x,y,z) et (anglex, angley, anglez)
             )
          {
            cerr << "Ligne invalide ("<< iline << "): " << endl << 
              strbuf << endl;
            loading = false;
          }
          else
          {
            for (UINT i = 0 ; i < pCurrentObj->children_number ; ++i)
            {
              pCurrentObj->childrenID[i] = megavect[i];
            }
            delete[] megavect;
          }
        }
      }


      else if (!strcmpi(strbuf, "EndOfFile"))
      {
        break;
      }

    }
  }
  f.close();
  if (loading == false)
    return false;

  // posttraitement au niveau des animations take et drop object que l'on
  // ne connait pas, et aussi des next anim
  pCurrentObj = pObjHead;
  bool bf;
  while (  pCurrentObj  )
  {
    bf=false;

    // installer les indexes des animations de prise d'objet
    if (pCurrentObj->p3DObjElement->m_anim_takeobjstr != NULL)
    {
      for (int i=0; i < pCurrentObj->p3DObjElement->m_anim_number; ++i)
      {
        if (!strcmpi(pCurrentObj->p3DObjElement->m_animations[i]->m_name, 
                     pCurrentObj->p3DObjElement->m_anim_takeobjstr))
        {
          // on a trouvé l'animation
          pCurrentObj->p3DObjElement->m_anim_takeobj_ind = i;
          delete pCurrentObj->p3DObjElement->m_anim_takeobjstr;
          pCurrentObj->p3DObjElement->m_anim_takeobjstr = NULL;
          bf = true;
          break;
        }
      }
      if (bf==false)
      {
          cerr << "Erreur: l'animation " << 
                  pCurrentObj->p3DObjElement->m_anim_takeobjstr <<
                  "est inconnue pour le trigger TAKE_OBJECT" << endl;
        delete pCurrentObj->p3DObjElement->m_anim_takeobjstr;
        pCurrentObj->p3DObjElement->m_anim_takeobjstr = NULL;
        return false;
      }
    }

    // installer les indexes des animations de poses d'objet
    if (pCurrentObj->p3DObjElement->m_anim_dropobjstr != NULL)
    {
      bf=false;
      for (int i=0; i < pCurrentObj->p3DObjElement->m_anim_number; ++i)
      {
        if (!strcmpi(pCurrentObj->p3DObjElement->m_animations[i]->m_name, 
                     pCurrentObj->p3DObjElement->m_anim_dropobjstr))
        {
          // on a trouvé l'animation
          pCurrentObj->p3DObjElement->m_anim_dropobj_ind = i;
          delete pCurrentObj->p3DObjElement->m_anim_dropobjstr;
          pCurrentObj->p3DObjElement->m_anim_dropobjstr = NULL;
          bf = true;
          break;
        }
      }
      if (bf==false)
      {
          cerr << "Erreur: l'animation " << 
                  pCurrentObj->p3DObjElement->m_anim_takeobjstr <<
                  "est inconnue pour le trigger DROP_OBJECT" << endl;
        delete pCurrentObj->p3DObjElement->m_anim_takeobjstr;
        pCurrentObj->p3DObjElement->m_anim_takeobjstr = NULL;
        return false;
      }
    }


    // installer les next animations (résoudre les noms en indexes)
    for (int i=0; i < pCurrentObj->p3DObjElement->m_anim_number; ++i)
    {
      if (pCurrentObj->p3DObjElement->m_animations[i]->m_anim_next_str != NULL)
      {
        // une "next animation" dont l'indexe est inconnue:
        bf= false;
        for (int j=0; j < pCurrentObj->p3DObjElement->m_anim_number; ++j)
        {
          if (!strcmpi(
             pCurrentObj->p3DObjElement->m_animations[i]->m_anim_next_str,
             pCurrentObj->p3DObjElement->m_animations[j]->m_name))
          {
            delete pCurrentObj->p3DObjElement->m_animations[i]->m_anim_next_str;
            pCurrentObj->p3DObjElement->m_animations[i]->m_anim_next_str = NULL;
            pCurrentObj->p3DObjElement->m_animations[i]->m_anim_next_ind = j;
            bf = true;
            break;
          }
        }
        if (bf==false)
        {
          cerr << "Erreur: l'animation " << 
                  pCurrentObj->p3DObjElement->m_animations[i]->m_anim_next_str <<
                  " est inconnue en tant que NEXT_ANIMATION" << endl;
          delete pCurrentObj->p3DObjElement->m_anim_takeobjstr;
          pCurrentObj->p3DObjElement->m_anim_takeobjstr = NULL;
          return false;
        }
      }
    }

    // pareil pour les animations par défaut
    if ( pCurrentObj->p3DObjElement->m_anim_default_str != NULL)
    {
      bf = true;
      for (int i=0; i < pCurrentObj->p3DObjElement->m_anim_number; ++i)
      {
        if (!strcmpi(pCurrentObj->p3DObjElement->m_anim_default_str,
                              pCurrentObj->p3DObjElement->m_animations[i]->m_name))
        {
          bf= true;
          delete pCurrentObj->p3DObjElement->m_anim_default_str;
          pCurrentObj->p3DObjElement->m_anim_default_str = NULL;
          pCurrentObj->p3DObjElement->m_anim_default_ind = i;
          break;
        }
        if (bf==false)
        {
          cerr << "Erreur: l'animation " << 
                  pCurrentObj->p3DObjElement->m_anim_default_str << 
                  " est inconnue en tant que DEFAULT_ANIMATION" << endl;
                 
          delete pCurrentObj->p3DObjElement->m_anim_default_str;
          pCurrentObj->p3DObjElement->m_anim_default_str = NULL;
          return false;
        }
      }
    }

    

    pCurrentObj = pCurrentObj->pnext;
  }

  // maintenant on passe à un gros truc: les uid.
  // les animations qui ne sont pas géré par l'IA n'ont pas de UID, et
  // comme il est inconcevable de choisir les animations en fonction des
  // noms (chaine de caractère), on fixe nous même les uid...
  // et c'est là que iAnimCounter est très utile...
  // (le choix d'une telle animation peut arrivé dans le cas où elle est
  // "pointée" par une nextanim d'une autre animation, gérée par l'IA)
  pCurrentObj = pObjHead;
  char **pstr_animtraitees = new char *[iAnimCounter * sizeof(char**)];
  UINT *pUID_animtraitees = new UINT [iAnimCounter * sizeof(UINT)];
  memset(pstr_animtraitees,0, sizeof(iAnimCounter * sizeof(char *)));
  memset(pUID_animtraitees,0, sizeof(iAnimCounter * sizeof(UINT)));
  int nAnimTraitees=0;
  int j;

  // récupération du maximum des UID connus par l'IA
  UINT max_known_uid = 0;
  UNKNOWN_NAMEUID *pa;
  pa = AnimNameUIDList;
  while (pa->m_strName)
  {
    if (pa->m_iUID > max_known_uid)
      max_known_uid = pa->m_iUID;
    pa++;
  }
  max_known_uid += 500;
  
  // parcour de tous les objets pour trouver toute les animations
  while (  pCurrentObj  )
  {
    for (i=0; i < pCurrentObj->p3DObjElement->m_anim_number; ++i)
    {
      bf = false;
      for (j=0; j < nAnimTraitees; ++j)
      {
        if (!strcmpi(pstr_animtraitees[j], pCurrentObj->p3DObjElement->m_animations[i]->m_name))
        {
          bf = true;
          break;
        }
      }
      // animation pas encore ajouté dans la liste.
      if (bf == false)
      {
        // ajout du nom dans la liste des noms des animations traitées
        pstr_animtraitees[nAnimTraitees] = pCurrentObj->p3DObjElement->m_animations[i]->m_name;
        // le UTD est-il déjà définie (ie est-il dans la liste des animations connues)?
        if (pCurrentObj->p3DObjElement->m_animations[i]->m_uid != ANIMATION_INVALID_VALUE)
          pUID_animtraitees[nAnimTraitees++] = pCurrentObj->p3DObjElement->m_animations[i]->m_uid;
        else
        {    // non: on l'ajoute
          pUID_animtraitees[nAnimTraitees] = max_known_uid++;
          pCurrentObj->p3DObjElement->m_animations[i]->m_uid = pUID_animtraitees[nAnimTraitees++];
        }
      }
      else
        pCurrentObj->p3DObjElement->m_animations[i]->m_uid = pUID_animtraitees[j];

    }
    pCurrentObj = pCurrentObj->pnext;
  }
  delete[] pstr_animtraitees;
  delete[] pUID_animtraitees;





  
  cout << "Création de l'arbre des objets..." << endl;

//  Assert(p3DObject != NULL, "p3DObject ne doit pas être NULL");
//  Assert(pObjHead != NULL, "pObjHead ne doit pas être NULL");


  // tout est chargé dans une liste, reste plus qu'à recréer l'arbre
  // à partir de la liste et des childrenID

  if (p3DObject)
  {
    stack<C3DObjElement*> pile;
    // on utilise une pile pour éviter une fonction récursive inutile.
    // cette pile contiendra des pointeurs vers des C3DObjElement ET AUSSI
    // des entiers (on suppose que le CAST ne pose pas de probleme, on est
    // sur de vrai proc, alors int == void *)

    C3DObjElement *pCurrent3DObject;
    UINT current_child = 0;

    // traitement séparé de la tête
    pCurrent3DObject = pObjHead->p3DObjElement;
    *p3DObject = pCurrent3DObject;
    pCurrent3DObject->m_children_number = pObjHead->children_number;
//    Assert(pCurrent3DObject->m_children_number > 25,"Ne peut pas allouer plus de 25 fils");
    pCurrent3DObject->m_children = new C3DObjElement*
                                           [pCurrent3DObject->m_children_number];

    for (unsigned int i=0; i < pCurrent3DObject->m_children_number ; ++i)
    {
      pCurrent3DObject->m_children[i] = (C3DObjElement*) pObjHead->childrenID[i];
      // oui je sais je stocke les ID dans des pointeurs, mais
      // sizeof(int) = 4 et sizeof(C3DObjElement*) = 4 à la rigueur
      // 8 mais c pas grave!!!
    }


    current_child=0; // TRES IMPORTANT!! sinon les pointeurs seront invalides!!
    
    // traitement de tous les fils...
    while (1) // un while(1) volontaire...
    {         // normalement c while (pile non vide) :))))


      if (pCurrent3DObject->m_children_number != 0
          && 
          current_child != pCurrent3DObject->m_children_number)
      {
        // on empile que les éléments qui ont des enfants...
        if (pCurrent3DObject->m_children_number)
        {
          pile.push(pCurrent3DObject);
          pile.push((C3DObjElement*) current_child);
        }

        // TRAITEMENT 
        // recherche du current_child ème dans la liste
        pCurrentObj = pObjHead;
        while (  pCurrentObj 
               && 
                 pCurrentObj->objID !=
                       (unsigned int) pCurrent3DObject->m_children[current_child]
              )
        {
          pCurrentObj = pCurrentObj->pnext;
        }

        if (!pCurrentObj)
        {
          cerr << "Impossible de trouver l'enfant d'ID " << 
                  (unsigned int) pCurrent3DObject->m_children[current_child] << endl;
          return false;
        }
        pCurrent3DObject->m_children[current_child] = pCurrentObj->p3DObjElement;
        pCurrent3DObject = pCurrentObj->p3DObjElement;
        pCurrent3DObject->m_children_number = pCurrentObj->children_number;
        pCurrent3DObject->m_children =
                           new C3DObjElement*[pCurrent3DObject->m_children_number];
        for (unsigned int i=0; i < pCurrent3DObject->m_children_number ; ++i)
          pCurrent3DObject->m_children[i] = 
                                   (C3DObjElement*) pCurrentObj->childrenID[i];

        // FIN DU TRAITEMENT

        current_child = 0;
      }
      else
      {
        if (!pile.empty())
        {
          current_child = (int) pile.top() + 1;
          pile.pop();
          pCurrent3DObject = pile.top();
          pile.pop();
        }
        else // pile vide => travail fini
          break; 
      }
    }
  }


  // destruction des structures temporaires.
  if (pObjHead)
  {
    pCurrentObj = pObjHead;
    objstruct * ptmp;
    while (pCurrentObj)
    {
      ptmp = pCurrentObj->pnext;
      delete pCurrentObj;
      pCurrentObj = ptmp;
    }
  }

  cout << "Fichier chargé" << endl;

  // remarque : on a pas détruit les C3DObjElement alloué 
  // dans ces structures car normalement ils sont maintenant
  // collés dans l'arbres pointé par p3DObject.
  // Le probleme c pour les objets sans parent, ils sont
  // toujours en mémoire mais utilisés nul par 
  // REMARQUE : il ne devrait PAS y avoir de tels objets
  // TODO : coriger ce petit probleme.
  return true;
}
