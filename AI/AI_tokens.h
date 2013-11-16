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
#ifndef __AI_TOKENS_H__
#define __AI_TOKENS_H__

// les types de token
// il est important de ne pas donner de valeurs à ces constantes.
// remarque : cet enum influence la macro-définition TOKEN_T_NB
enum token_t { found_food, path } ;

// nombre de types de token
// il doit être égal au cardinal de l'enum token_t
#define TOKEN_T_NB 2

#endif /* __AI_TOKEN_H__ */
