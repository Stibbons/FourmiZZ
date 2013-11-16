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


    File	: tga.c
    Date	: 05/05/1999
    Author	: Nate Miller
    Contact	: vandals1@home.com

    Change Log
    **********
    8/07/99 - made the code portable
    7/26/99 - added code to fclose () a file when there is an error
    6/11/99 - added support for 8bit images, changed commenting
*/
#include "tga.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>

GLenum texFormat;

/*
=============
checkSize

Make sure its a power of 2.
=============
*/
int tgaCheckSize (int x)
{
    if (x == 2	 || x == 4 || 
        x == 8	 || x == 16 || 
        x == 32  || x == 64 ||
        x == 128 || x == 256 || x == 512)
        return 1;
    else return 0;
}
/*
=============
getRGBA

Reads in RGBA data for a 32bit image. 
=============
*/
unsigned char *tgaGetRGBA (FILE *s, int size)
{
    unsigned char *rgba;
    unsigned char temp;
    int bread;
    int i;

    rgba = (unsigned char *) malloc (size * 4); 

    if (rgba == NULL)
        return 0;

    bread = fread (rgba, sizeof (unsigned char), size * 4, s); 

    /* TGA is stored in BGRA, make it RGBA */
    if (bread != size * 4)
    {
        free (rgba);
        return 0;
    }

    for (i = 0; i < size * 4; i += 4 )
    {
        temp = rgba[i];
        rgba[i] = rgba[i + 2];
        rgba[i + 2] = temp;
    }

    texFormat = GL_RGBA;
    return rgba;
}
/*
=============
getRGB

Reads in RGB data for a 24bit image. 
=============
*/
unsigned char *tgaGetRGB (FILE *s, int size)
{
    unsigned char *rgb;
    unsigned char temp;
    int bread;
    int i;

    //printf("Avant Malloc\n");
    rgb = (unsigned char *) malloc (size * 3); 

 

    if (rgb == NULL)
        return 0;

    bread = fread (rgb, sizeof (unsigned char), size * 3, s);

    if (bread != size * 3)
    {
        free (rgb);
        return 0;
    }

    /* TGA is stored in BGR, make it RGB */
    for (i = 0; i < size * 3; i += 3)
    {
        temp = rgb[i];
        rgb[i] = rgb[i + 2];
        rgb[i + 2] = temp;
    }

    texFormat = GL_RGB;

    return rgb;
}
/*
=============
getGray

Gets the grayscale image data.  Used as an alpha channel.
=============
*/
unsigned char *tgaGetGray (FILE *s, int size)
{
    unsigned char *grayData;
    int bread;

    grayData = (unsigned char *) malloc (size);

    if (grayData == NULL)
        return 0;

    bread = fread (grayData, sizeof (unsigned char), size, s);

    if (bread != size)
    {
        free (grayData);
        return 0;
    }

    //texFormat = GL_ALPHA;
   texFormat = GL_LUMINANCE;

    return grayData;
}
/*
=============
getData

Gets the image data for the specified bit depth.
=============
*/
unsigned char *tgaGetData (FILE *s, int sz, int iBits)
{
    if (iBits == 32)
        return tgaGetRGBA (s, sz);
    else if (iBits == 24)
        return tgaGetRGB (s, sz);	
    else if (iBits == 8)
        return tgaGetGray (s, sz);
    return NULL;
}
/*
=============
returnError
=============
Called when there is an error loading the .tga file.
*/
int tgaReturnError (FILE *s, int error)
{
    fclose (s);
    return error;
}
/*
=============
loadTGA

Loads up a targa file.  Supported types are 8,24 and 32 uncompressed images.  
id is the texture ID to bind too.
=============
*/
int tgaLoad (const char *name, int id)
{
    unsigned char type[4];
    unsigned char info[7];
    unsigned char *imageData = NULL;
    int imageWidth, imageHeight;
    int imageBits, size;
    FILE *s;

    if (!(s = fopen (name, "rbt")))
        return TGA_FILE_NOT_FOUND;

    fread (&type, sizeof (char), 3, s); // read in colormap info and image type, byte 0 ignored
    fseek (s, 12, SEEK_SET);			// seek past the header and useless info
    fread (&info, sizeof (char), 6, s);

    if (type[1] != 0 || (type[2] != 2 && type[2] != 3))
        return tgaReturnError (s, TGA_BAD_IMAGE_TYPE);

    imageWidth = info[0] + info[1] * 256; 
    imageHeight = info[2] + info[3] * 256;
    imageBits =	info[4]; 

    size = imageWidth * imageHeight; 

    /* make sure dimension is a power of 2 */
    if (!tgaCheckSize (imageWidth) || !tgaCheckSize (imageHeight))
        return tgaReturnError (s, TGA_BAD_DIMENSION);

    /* make sure we are loading a supported type */
    if (imageBits != 32 && imageBits != 24 && imageBits != 8)
        return tgaReturnError (s, TGA_BAD_BITS);

    imageData = tgaGetData (s, size, imageBits);

    /* no image data */
    if (imageData == NULL)
        return tgaReturnError (s, TGA_BAD_DATA);

    fclose (s);
/*
    glBindTexture (GL_TEXTURE_2D, id);
    gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imageWidth, imageHeight, texFormat, GL_UNSIGNED_BYTE, imageData);
     glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
*/
    
	glPixelStorei ( GL_UNPACK_ALIGNMENT, 1);
    
#define GL_CLAMP_TO_EDGE	0x812F	

  glBindTexture(GL_TEXTURE_2D, id);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, imageWidth, imageHeight, texFormat, GL_UNSIGNED_BYTE, imageData);

	// Lastly, we need to tell OpenGL the quality of our texture map.  GL_LINEAR_MIPMAP_LINEAR
	// is the smoothest.  GL_LINEAR_MIPMAP_NEAREST is faster than GL_LINEAR_MIPMAP_LINEAR, 
	// but looks blochy and pixilated.  Good for slower computers though.  
		
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	// The default GL_TEXTURE_WRAP_S and ""_WRAP_T property is GL_REPEAT.
	// We need to turn this to GL_CLAMP_TO_EDGE, otherwise it creates ugly seems
	// in our sky box.  GL_CLAMP_TO_EDGE does not repeat when bound to an object.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    /* release data, its been uploaded */
    free (imageData);

    return 1;
}

