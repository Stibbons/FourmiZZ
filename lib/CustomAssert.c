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
#include "CustomAssert.h"
#include <stdio.h>
#include <malloc.h>
#ifdef WIN32
#include <windows.h>
#endif

int CustomAssertFunction( char *expr, char* descr, int line, char* file, int *ignoreAlways)
{
  int r=1;
  #ifdef WIN32
    char *sztxt = "Erreur d'assertion !!\n"
                  "Fichier: %s\n"
                  "Ligne %d\n\n"
                  "Expression: %s\n"
                  "Description: %s\n\n"
                  "Voulez-vous arreter le déroulement du programme pour debugger?\n"
                  "Annuler : quitte le programme\n"
                  "Recommencer : continue, mais arrete sur la prochaine erreur semblable\n"
                  "Ignorer : ignore toutes ces erreurs";
    char *szbuf;
    szbuf = (char*) malloc(sizeof(char) * (strlen(expr) + strlen(sztxt) + 3 + 30 + strlen(descr) + strlen(file)));
    if (szbuf)
    {
      sprintf(szbuf, sztxt, file, line, expr, descr);
      switch (MessageBox(NULL, szbuf, "Assertion failure", MB_ICONERROR |MB_ABORTRETRYIGNORE))
      {
        case IDABORT:
          r=1;
          break;
        case IDIGNORE:
          *ignoreAlways = 1;
        case IDRETRY:
        default:
          r=0;
          break;
      }   
      free(szbuf);
    }
  #endif
  return r;
}
