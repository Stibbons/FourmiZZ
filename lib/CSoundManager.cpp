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
#include <stdio.h>
#include <iostream.h>
#include "CSoundManager.h"
#include "CustomAssert.h"

bool CSoundManager::InitAudio()
{
  return (Mix_OpenAudio(44100,
                MIX_DEFAULT_FORMAT,
                MIX_DEFAULT_CHANNELS, 4096)==0? true:false);
}

CSoundManager::~CSoundManager()
{
  for (zicMapIter itor = m_musics.begin(); itor != m_musics.end(); ++itor)
  {
    Mix_FreeMusic(itor->second);
  }
  for (sndMapIter iter = m_sounds.begin(); iter != m_sounds.end(); ++iter)
  {
    Mix_FreeChunk(iter->second);
  }
}


bool CSoundManager::AddMusic(char* szfilename, unsigned int uid)
{
  Mix_Music *music;
//  Assert(szfilename,"szfilename == NULL");
  if (!(music = Mix_LoadMUS(szfilename)))
    return false;
  m_musics.insert(zicMapType(uid, music));
  return true;
}

bool CSoundManager::PlayMusic(unsigned int uid, bool loop)
{
  if (m_enabled)
  {
    zicMapIter pos = m_musics.find(uid);
    if (pos != m_musics.end())
    {
      if (Mix_PlayMusic(pos->second, (loop?-1:0)) == -1)
      {
        cerr << "Impossible de lire la musiq d'id" << pos->first << endl;
      return false;
      }
      return true;
    }
  }
  return false;
}

void CSoundManager::StopMusic()
{
  if (m_enabled)
  {
    Mix_HaltMusic();
  }
}



bool CSoundManager::AddSound(char* szfilename, unsigned int uid)
{
  Mix_Chunk *sound;
  Assert(szfilename,"szfilename == NULL");
  if (!(sound = Mix_LoadWAV(szfilename)))
    return false;
  m_sounds.insert(sndMapType(uid, sound));
  return true;
}

int CSoundManager::PlaySound(unsigned int uid, bool loop)
{
  if (m_enabled)
  {
    int channel;
    sndMapIter pos = m_sounds.find(uid);

    if (pos != m_sounds.end())
    {
      if ((channel=Mix_PlayChannel(-1, pos->second, (loop?-1:0))) == -1)
      {
        cerr << "Impossible de lire le son d'id" << pos->first << endl;
        return -1;
      }
      return channel;
    }
  }
  return -1;
}

void CSoundManager::StopSound(int channel)
{ 
  Mix_HaltChannel(channel);
}
