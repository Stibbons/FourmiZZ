/*
   Vertex Array & Lightmap Demo
   Copyright (C) 1999 Nate Miller nkmiller@calpoly.edu

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

   See gpl.txt for more information regarding the GNU General Public License.

   See descr.txt for more information about this demo.  
*/
#ifndef __TGA_LOADER__HEADER__
#define __TGA_LOADER__HEADER__

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

/* Error Codes */
#define TGA_FILE_NOT_FOUND 13 /* file was not found */
#define TGA_BAD_IMAGE_TYPE 14 /* color mapped image or image is not uncompressed */
#define TGA_BAD_DIMENSION	15 /* dimension is not a power of 2 */
#define TGA_BAD_BITS       16 /* image bits is not 8, 24 or 32 */
#define TGA_BAD_DATA       17 /* image data could not be loaded */

int tgaLoad (const char *name, int id); /* id is the texture id to bind too */

#ifdef __cplusplus
}
#endif /*__cplusplus*/


#endif /* __TGA_LOADER__HEADER__ */
