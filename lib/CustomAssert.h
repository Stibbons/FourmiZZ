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
#ifndef __CUSTOMASSERT_H__HEADER__
#define __CUSTOMASSERT_H__HEADER__


#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus */

#ifdef  _DEBUG

/* Assert est une version améliorée de assert.
 * Assert permet de vérifier qu'une hypothèse est bien vérifié à un moment
 * donné. Si l'hypothèse (booléenne) est fausse, alors le programme s'arrete
 * et le débuggeur est appellé par l'intéruption 3.
 * Mais seulement en version DEBUG, en version RELEASE, l'erreur est sensée être 
 * corrigée, donc cette fonction ne fait RIEN du tout.
 * Lors de l'erreur, une boite de dialogue est affichée avant d'appeller le
 * debuggeur (si par exemple on a lancer le programme dans Visual Studio), 
 * celle ci affiche diverses informations quant à la location et le type
 * d'erreur.
 */

extern int CustomAssertFunction( char *exp, char* descr, int line, char* file, int * ignoreAlways );

#define Assert(exp, description) {\
    static int ignoreAlways = 0;\
    if (!ignoreAlways) { \
      if (!exp && CustomAssertFunction(#exp, description, __LINE__, __FILE__, &ignoreAlways )) {\
        _asm{ int 3 } \
        }}}

#else  /* _DEBUG */

#define Assert(exp, description)


#endif /* _DEBUG */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __CUSTOMASSERT_H__HEADER__ */
