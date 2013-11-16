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
#ifndef __SDLMUSIC_SOUND_H__HEADER__
#define __SDLMUSIC_SOUND_H__HEADER__
#ifdef WIN32
#pragma warning(disable:4786)
#endif
#include <stdio.h>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#ifdef WIN32
#include <windows.h>
/* remarque: grgrgrgr 
 * je suis obliger d'inclure Windows.h, car sinon il recherchera (ailleurs) la fonction
 * CSoundManager::PlaySoundA.
 * En effet, PlaySound est une fonction Windows. En fait, c'est un alias
 * vers une fonction Unicode ou ANSI (PlaySoundW ou PlaySoundA)
 * il y a : #define PlaySound PlaySoundA ou #define PlaySound PlaySoundW
 * selon qu'on compile le programme en UNICODE ou non.
 * C'était juste une remarque comme ça...
 */ 
#endif

#include <map>
#include <algorithm>
#include "TSingleton.h"

typedef std::map<unsigned int, Mix_Music*> zicMap;
typedef zicMap::value_type zicMapType;
typedef zicMap::iterator zicMapIter;

typedef std::map<unsigned int, Mix_Chunk*> sndMap;
typedef sndMap::value_type sndMapType;
typedef sndMap::iterator sndMapIter;

/* Gestionnaire de son: object instancié qu'une fois dans le programme
 * (vive le singleton) gerant le chargement des musics et des sons.
 * Il ne peut y avoir qu'une musique en même temps.
 * Par contre, on peut jouer autant de son en même temps que le systeme 
 * le permet (un "channel" par son)
 */
class CSoundManager: public TSingleton<CSoundManager>
{
  zicMap m_musics;
  sndMap m_sounds;
  bool m_enabled;

public:
  // constructeur. Fait rien
  CSoundManager(): m_enabled(true) { m_musics.empty(); m_sounds.empty(); };
  // destructeur: détruit tout
  ~CSoundManager();
  // active le son
  void Enable() { m_enabled = true; }
  // désactive le son
  void Disable() { m_enabled = false; }
  // initialise le gestionnaire de son
  bool InitAudio();
  // charge une musique du disque dur et lui assigne une clé
  bool AddMusic(char*, unsigned int);
  // joue la musique qui a telle clée. Le bool est pour savoir si elle doit boucler.
  bool PlayMusic(unsigned int, bool);
  // arrete la musique
  void StopMusic();

  // charge un son en lui assignant une clé
  bool AddSound(char*, unsigned int);
  // PlaySound retourne le channel utilisé pour jouer ce son
  // sinon il retourne -1
  // cette valeur sera utilisée pour arreter le son avec
  // StopSound
  int PlaySound(unsigned int, bool);
  // arrete le son qui joue sur le channel donné en parametre
  void StopSound(int channel);
};

// "fonction" d'acces au manager
#define GetSoundManager() CSoundManager::GetSingleton()

#endif //__SDLMUSIC_SOUND_H__HEADER__
