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
#include  "AI_renders_in_menu.h"


/*****************\
* classe aiObject *
\*****************/
void aiObject::render_in_menu()
{
  // ... {
  
    glPrint_chsize(400,650,"Type : ",0,1024,768);

  // } ...
}

/***************\
* classe aiRock *
\***************/
void aiRock::render_in_menu()
{
  // ... {
 glColor4f(1.0f,1.0f,1.0f,1.0f);
  glPrint_chsize(500,650,"Rock",0,1024,768);
  // } ...
  aiObject::render_in_menu() ;
}

/***************\
* classe aiFood *
\***************/

void aiFood::render_in_menu()
{
  // ... {

  // } ...
  aiObject::render_in_menu() ;
}

/***************\
* classe aiSeed *
\***************/

void aiSeed::render_in_menu()
{
  // ... {
 glColor4f(1.0f,1.0f,1.0f,1.0f);
  glPrint_chsize(500,650,"Seed",0,1024,768);
  // } ...
  aiFood::render_in_menu() ;
}


/**********************\
* classe aiLivingBeing *
\**********************/

void aiLivingBeing::render_in_menu()
{
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  char buffer[255];
  sprintf(buffer,"%d",get_hp());	

  glPrint_chsize(400,670,"HP : ",0,1024,768);
  glPrint_chsize(500,670,buffer,0,1024,768);

  aiObject::render_in_menu() ;
}

/**************\
* classe aiAnt *
\**************/

void aiAnt::render_in_menu()
{
  aiLivingBeing::render_in_menu() ;
}

/****************\
* classe aiMinor *
\****************/

void aiMinor::render_in_menu()
{
    
  aiAnt::render_in_menu() ;
}

/********************\
* classe aiHarvester *
\********************/

void aiHarvester::render_in_menu()
{
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glPrint_chsize(500,650,"Harvester",0,1024,768);
  aiMinor::render_in_menu() ;
}

/****************\
* classe aiScout *
\****************/

void aiScout::render_in_menu()
{
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glPrint_chsize(500,650,"Scout",0,1024,768);
  aiMinor::render_in_menu() ;
}

/*****************\
* classe aiSpider *
\*****************/

void aiSpider::render_in_menu()
{
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glPrint_chsize(500,650,"Spider",0,1024,768);
  aiLivingBeing::render_in_menu() ;
}

/********************\
* classe aiVegetable *
\********************/

void aiVegetable::render_in_menu()
{
}
