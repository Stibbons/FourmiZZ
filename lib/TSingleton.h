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
#ifndef __SINGLETON_H__HEADER__
#define __SINGLETON_H__HEADER__



#include "CustomAssert.h"

/* Singleton.
 * Le singleton est utilisé pour gérer de manière transparente un objet
 * qui doit avoir une portée globale dans toute l'application, et qui
 * doit être instanciée qu'une seule et unique fois.
 * Par exemple, un gestionnaire de ressource ou de message
 * C'est un template de classe, il faut dériver le gestionnaire de ce
 * template pour que cela fonctionne bien.
 */

template <typename T> class TSingleton
{
  static T* ms_Singleton; // membre static, c'est la clé du singleton
public:
  TSingleton ( void )
  {
    // cette ligne est très importante, elle est la substantifique moelle
    // du singleton: empecher de créer 2 fois le même objet...
    Assert(!ms_Singleton, "Erreur: tentative de création du singleton alors qu'il existe déjà");
    // on récupère l'adresse (violent!!) de la classe dérivée
    // ie la différence d'adresse entre un objet de 2 types différents
    // situé à la même adresse 0x01
    int offset = (int) (T*)0x01 - (int) (TSingleton <T>*)(T*)0x01;
    // on positionne correctement le pointeur vers la classe...
    ms_Singleton = (T*)((int) this + offset);
  }
  ~TSingleton( void )
  {
    Assert( ms_Singleton, "Singleton non initialisé" );
    ms_Singleton = 0;
  }
  static T& GetSingleton ( void )
  { 
    Assert( ms_Singleton, "Singleton non initialisé" );
    return *ms_Singleton;
  }
  static T* GetSingletonPtr ( void )
  { 
    return ms_Singleton;
  }
};

// c'est cette ligne qui initialise le ms_Singleton à 0 "avant"
// le constructeur de la classe...
template <typename T> T* TSingleton <T>::ms_Singleton = 0;

/* comment faire pour faire fonctionner le singleton:
 * - dériver la classe du manager (car les singletons sont typiquement
 *   utiliser pour les manager :=)  ) de l'instance Singleton<NomDeLaClasse>
 * - construire une instance de NomDeLaClasse quelque part, mais il faut le 
 *   faire (c'est pas une chose que le singleton gère)
 * Acces: 
 * NomDeLaClasse::GetSingleton()
 *
 *
 *
 * conseil: ajouter un define 
 * Exemple:
 * #define GetTextureManager() TextureMgr::GetSingleton()
 * ou plus violent:
 * #define g_TextureMgr TextureMgr::GetSingleton()
 * 
 * ::GetSingleton() existera toujours car elle est static.
 */





#endif //__SINGLETON_H__HEADER__
