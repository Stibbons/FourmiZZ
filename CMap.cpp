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
#include "CMap.h"
#include "lib/tga.h"
#include <time.h>
#include "lib/TVector.h"
#include <iostream>
#include "sounds.h"
#include "Objects.h"
#include "lib/CMotor.h"
#include "math.h"

/*-------------------------------------------------------------*/
/*  Initialisation de la matrice des hauteurs                  */
/*-------------------------------------------------------------*/


void CMap::Init(Map_Settings Settings)
{

// Recopie des settings passés en paramètres
  mSettings = Settings;
  
// Création du menu associé a la map en vue scrolling
  menu_down=new menu(mSettings,"Leftmenu.ini");
  
  glClear(((GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)));
  glMatrixMode(GL_MODELVIEW);
//  SDL_ShowCursor(false);
  glLoadIdentity();
  glColor4f(1,1,1,1);
  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0,800,600,0,0,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  
  
  int Iter = 0;
  int i,j;
  int yi,yj;
  // Plus petit et plus grand
  float Max;
  float Min;
  // Variable du centre de chaque bosse
  int CenterX;
  int CenterY;
  float Calcul;

  
  //  Rayon de la bosse
  int Radius;
  unsigned char texId;
  // Altitude au point actuel
  float iZ;
  ScrollLimit.x = 0;
  ScrollLimit.z = 0;
  
  printf("Saisie des parametres de la carte ...\n");
  glPrint(10,50,"Saisie des parametres...!",0);
  glFlush();
  SDL_GL_SwapBuffers();
  
// Allocation des tableaux
  printf("Allocation des tableaux...");
  HeightMatrix = new float *[mSettings.Mat_Size];
 // Normal = new CVector3f *[mSettings.Mat_Size];
 // TexMatrix = new unsigned char *[mSettings.Mat_Size];
  for (i=0 ; i<mSettings.Mat_Size ; ++i)
  {
    HeightMatrix[i] = new float[mSettings.Mat_Size];
    //Normal[i] = new CVector3f[mSettings.Mat_Size];
    //TexMatrix[i] = new unsigned char[mSettings.Mat_Size];
  }
  

  // Etendue des valeurs possible pour le centre
  int RangeX = mSettings.Mat_Size + 2*mSettings.Far_Max;
  int RangeY = mSettings.Mat_Size + 2*mSettings.Far_Max;
  
  srand(time(NULL));
  
  


// generation des bosses
  glPrint(10,70,"Generation des bosses...!",0);
  glFlush();
  SDL_GL_SwapBuffers();
        for (i = 0; i<mSettings.Mat_Size;i++)
  {
    for (j = 0; j<mSettings.Mat_Size;j++)
    {
      HeightMatrix[i][j]=0;
      }}
  for (Iter = 0; Iter < (int)mSettings.Nb_Hills; Iter ++)
  {
    // Generation du centre
    CenterX = (int) (ceil (floor((double) rand() *  RangeX / RAND_MAX ))) - mSettings.Far_Max;
    CenterY = (int) (ceil (floor((double) rand() *  RangeY / RAND_MAX ))) - mSettings.Far_Max;
    // Generation du rayon
    
    Radius = (int) (ceil (floor((double) rand() * (mSettings.Max_Radius - mSettings.Min_Radius) / RAND_MAX)))+ mSettings.Min_Radius;
    
    // Parcours de la HeightMap et changements des points concernés.
    
    // METHODE 1 : PARCOURS ENTIER
    /*
    for(i = 0; i < MAP_SIZE_X; i ++)
    {
    for(j = 0; j < MAP_SIZE_Y; j++)
    {
    // Calcul du changement d'altitude
    Z = Radius*Radius  - ((i-CenterX)*(i-CenterX) + (j-CenterY)*(j-CenterY));
    // On ne prend en compte que les valeurs positives
    if (Z > 0)
    {
    Map[i][j] +=  Z;
    }
    }
    }
    
    */
    // Calcul du coeff d'applatissement


    int rad = Radius / mSettings.Step_Size;
    // CETTE METHODE SEMBLE BEAUCOUP PLUS PERFORMANTE
    // METHODE 2 : PARCOURS DES POINTS AUX ALENTOURS DU CENTRE
    for (i = -3*rad; i<= 3*rad; i++)
    {
       
      if ((CenterX + i < mSettings.Mat_Size) && (CenterX + i >= 0)) yi = i;
      else if ((!mSettings.Flat)&&(CenterX + i >= mSettings.Mat_Size ) && (CenterX + i < 2 *mSettings.Mat_Size))
      {
         yi = i - mSettings.Mat_Size;
      }
      else if ((!mSettings.Flat) && (CenterX + i >= -mSettings.Mat_Size) && (CenterX + i < 0)) yi = i + mSettings.Mat_Size;
      else continue;
      
      for (j = -3*rad; j<= 3*rad; j++)
      {
        
        if ((CenterY + j <mSettings.Mat_Size) && (CenterY + j >= 0)) yj = j;
        else if ((!mSettings.Flat)&&(CenterY + j >=mSettings.Mat_Size )&& (CenterY + j < 2 *mSettings.Mat_Size))
        {
          yj = j - mSettings.Mat_Size;
        }
        else if ((!mSettings.Flat)&&(CenterY+j>=-mSettings.Mat_Size)&&(CenterY +j <0 )) yj = j + mSettings.Mat_Size;
        else continue;
        
        iZ = Radius* exp(-(float)((i*i*mSettings.Step_Size*mSettings.Step_Size + j*mSettings.Step_Size*mSettings.Step_Size*j)
          /(float)(Radius*Radius)));
        // On ne prend en compte que les valeurs positives
        
        if (iZ > 0.0f)
        {
          HeightMatrix[yi+CenterX][yj+CenterY] +=  iZ;
         }
        
        
      }
      
    }
    
  }



  Min = 255;
  Max = 0;
  glPrint(10,90,"Aplatissement...!",0);
  glFlush();
  SDL_GL_SwapBuffers();

  // On cherche maintenant le plus petit et le plus grand
  for(i = 0; i < mSettings.Mat_Size; i++)
  {
    for(j = 0; j< mSettings.Mat_Size;j ++)
    {
      
      if ( HeightMatrix[i][j] > Max ) Max = HeightMatrix[i][j];
      else if (HeightMatrix[i][j] < Min) Min = HeightMatrix[i][j];
      
    }
  }
  // faire une bosse pour la nest

  CenterX = mSettings.o_cpt.coord_nest.x / mSettings.Step_Size;
  CenterY = mSettings.o_cpt.coord_nest.z / mSettings.Step_Size;
  HeightMatrix[CenterX][CenterY] +=7*mSettings.o_cpt.nest_width;  
// Nombre de faces et nombre de vertex
  numofvertices = (mSettings.Mat_Size+1)*(mSettings.Mat_Size+1);
  numoffaces = 2* (mSettings.Mat_Size)*(mSettings.Mat_Size);

// Liste de faces et liste de vertex
  facetab = new face[numoffaces];
  vertextab = new vertex[numofvertices]; 


// On normalise pour avoir une matrice entre 0 et 1
  for(i = 0; i <= mSettings.Mat_Size; i++)
  {
    for(j = 0; j<= mSettings.Mat_Size;j ++)
    {
      
      int t = j % (mSettings.Mat_Size);
      int r = i % (mSettings.Mat_Size);
      if (( i <mSettings.Mat_Size)&&(j<mSettings.Mat_Size))
      {
        Calcul = (float) ( HeightMatrix[r][t] - Min) / (Max - Min);
      
      // On obtient une matrice de 0 a Map_Max_Height
        HeightMatrix[r][t] =  Calcul * mSettings.Map_Max_Height;
      }
      
      vertextab[i*(mSettings.Mat_Size+1)+j].pt = 
       vect3f(
       i*mSettings.Step_Size,
       HeightMatrix[r][t],
       j*mSettings.Step_Size);
          
    }
    
  }

// Génération des faces 
// Les faces sont groupés par paires, à ces paires correspond une texture
// Les faces sont stockés "dans l'ordre des vertex" pour la vue en scrolling

  int f = 0;
  int k= 0;
  for (i=0;i<numoffaces-1;i+=2)
  {
    facetab[i].v1 = k;
    facetab[i].v2 = k+1;
    facetab[i].v3 = k + mSettings.Mat_Size+2;

    facetab[i+1].v1 = mSettings.Mat_Size + k+2;
    facetab[i+1].v2 = mSettings.Mat_Size + k+1;
    facetab[i+1].v3 = k;

    k++;
    f++;

    if (!((f)%(mSettings.Mat_Size)))
    {
      k++;
      f=0;
    }
 
 
 
  
  }

// On tire les textures aléatoirement

  glPrint(10,130,"Generation des textures...!",0);
  glFlush();
  SDL_GL_SwapBuffers();
  for (i=0;i<NBTEX_MAP;i++)
  nbtex[i]=0;
  int *tex=new int[numoffaces /2];
  for(i = 0; i< numoffaces-1 ; i+=2)
  {
      texId = (unsigned char) (ceil (floor((double) rand() *  (EARTH_NB-1) / RAND_MAX )))+EARTH1_ID;
      nbtex[texId]++;
      tex[i/2] = texId;
      facetab[i].tex = texId;
      facetab[i+1].tex = texId;
  }

// Tri par textures
  sortbytex = new int*[NBTEX_MAP];
  for (i=0;i<NBTEX_MAP;i++)
  {
    
    sortbytex[i] = new int[nbtex[i]*2];
  }
  int tab[NBTEX_MAP] = {0};
  
  for(i = 0; i< numoffaces-1; i+=2)
  {
      sortbytex[tex[i/2]][tab[tex[i /2]]] = i;       
      sortbytex[tex[i/2]][tab[tex[i / 2]]+1] = i+1;       
      tab[tex[i/2]]+=2;
   }

  delete []tex;

// Calcul des normales de tous les vertex
  glPrint(10,110,"Calcul des normales...!",0);
  glFlush();
  SDL_GL_SwapBuffers();
  NormalizeMap();


// Chargement des textures
  printf("Map ; OK!\n");
  glPrint(10,150,"Chargement des textures...!",0);
  glFlush();
  SDL_GL_SwapBuffers();
  LoadTextures();
  printf("textures OK!\n");
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);



// Creation de la minimap
 mmap = new minimap(this,mSettings.Map_Size/180.0f,12,581,192,761);
 CreateMinimap();
  

  
}





/*-------------------------------------------------------------*/
/*  Calcul des normales                                        */
/*-------------------------------------------------------------*/


void CMap::NormalizeMap()
{
  CVector3f v0,v1,v2,v3,v4,v5,v6,v7,v8,f1,f2,f3,f4,f6,f7,f5,f8,tmp;
  CVector3f nul(0,0,0);
  for (int i=0; i<=mSettings.Mat_Size; i++)
  {
    for (int j=0; j<=mSettings.Mat_Size; j++)
    {

// Les vertex sont enregistrés dans cet ordre, on enregistre leurs normales
// de la manière suivante : on calcule les normales des 8 faces alentours que 
// l'on ajoute puis on normalise le tout.

// En calculant d'abord toutes les normales de faces ce serait plus rapide, mais
// faute de temps voila l'algorithme
      v0 = CVector3f(0,
        (float)(Height(i%mSettings.Mat_Size  ,j%mSettings.Mat_Size )), 
        (0  ));

      v1 = CVector3f((-mSettings.Step_Size),
        (float)(Height((i-1)%mSettings.Mat_Size,(j-1)%mSettings.Mat_Size)-Height(i  ,j )),
        -mSettings.Step_Size);

      v2 = CVector3f((0.0f  ),
        (float)(Height((i)%mSettings.Mat_Size,(j-1)%mSettings.Mat_Size  )-Height(i  ,j )),
        -mSettings.Step_Size);

      v3 = CVector3f(+mSettings.Step_Size,
        (float)(Height((i+1)%mSettings.Mat_Size,(j-1)%mSettings.Mat_Size)-Height(i%mSettings.Mat_Size  ,j%mSettings.Mat_Size )),
        -mSettings.Step_Size);

      v4 = CVector3f(
        +mSettings.Step_Size,
        (float)(Height((i+1)%mSettings.Mat_Size,j%mSettings.Mat_Size)-Height(i%mSettings.Mat_Size  ,j%mSettings.Mat_Size )),
        0.0f);

      v5 = CVector3f(mSettings.Step_Size,
        (float)(Height((i+1)%mSettings.Mat_Size,(j+1)%mSettings.Mat_Size)-Height(i%mSettings.Mat_Size  ,j%mSettings.Mat_Size )),
        mSettings.Step_Size);

      v6 = CVector3f(0.0f  ,
        (float)(Height(i,j +1 )-Height(i%mSettings.Mat_Size  ,j%mSettings.Mat_Size )),
        mSettings.Step_Size);

      v7 = CVector3f(-mSettings.Step_Size,
        (float)(Height(i-1,j+1)-Height(i%mSettings.Mat_Size  ,j%mSettings.Mat_Size )),
        mSettings.Step_Size);

      v8 = CVector3f(-mSettings.Step_Size,
        (float)(Height(i-1  ,j)-Height(i%mSettings.Mat_Size  ,j%mSettings.Mat_Size )),
        0.0f  );
      
      
      /*
      V1   V2   V3
      \  f1 | f2 /
      f8\   |  / f3
      V8---V0----V4
      f7/ |   \ f4
      / f6| f5  \
      V7   V6    V5
      */
      v1.Normalize();
      v2.Normalize();
      v3.Normalize();
      v4.Normalize();
      v5.Normalize();
      v8.Normalize();
      v6.Normalize();
      v7.Normalize();
      
      
      f1 = cross_product((v2),(v1));      
      f2 = cross_product((v3),(v2));      
      f3 = cross_product((v4),(v3));      
      f4 = cross_product((v5),(v4));      
      f5 = cross_product((v6),(v5));      
      f6 = cross_product((v7),(v6));     
      f7 = cross_product((v8),(v7));      
      f8 = cross_product((v1),(v8));
      
      f1.Normalize();
      f2.Normalize();
      f3.Normalize();
      f4.Normalize();
      f5.Normalize();
      f8.Normalize();
      f6.Normalize();
      f7.Normalize();
      v0.Normalize();
      tmp = (f1 + f2 + f3 +f4 +f5 + f6 + f7+ f8);
    /*  Normal[i][j].x = tmp.x;
      Normal[i][j].y = tmp.y;
      Normal[i][j].z = tmp.z;
      Normal[i][j].Normalize();*/
      vertextab[i*(mSettings.Mat_Size+1) + j].n = tmp.Normalize();
    }
  }

  printf("Normales OK!");
}

/*-------------------------------------------------------------*/
/*  Rendu de la carte en vue 'abeille'                         */
/*-------------------------------------------------------------*/

void CMap::Render()
{
  // Variables de parcours
  int i;
  float x, y, z; 

//////////////////////////////////////////
// RENDER SELON LA MATRICE DES HAUTEURS //
//////////////////////////////////////////
  
  //float time_color[3];
  //CVector3f Face_Normal;
  //Get_time_color(time_color);
  //CVector3f SunNorme = vSunDirection*-1;


  
  // SkyBox(SKYCENTER_X,SKYCENTER_Y,SKYCENTER_Z,SKY_WIDTH,SKY_HEIGHT, SKY_LARGE);
  /// debut  version TRIANGLE_STRIP
 
  

 // glBegin(GL_TRIANGLE_STRIP);
 //  for (i = 0; i < (int)mSettings.Mat_Size; i ++)
 // {
    // Parcours de la matrice des hauteurs de Step_Size en Step_Size sur les colonnes
 //   for (j = 0; j < (int)mSettings.Mat_Size; j++)
 //     RenderQuad(i,j,time_color);
 // }
 //  glEnd();
  

 // glBegin(GL_TRIANGLES);

//////////////////////////////////////////
// RENDER SELON LES FACES               //
//////////////////////////////////////////
  Render_every_faces();


// Rendu des bord de la map
  glColor3f(0.5f,0.5f,0.5f);
  glBegin(GL_TRIANGLE_STRIP);
  
  /* on est obliger d'utiliser ce sens de dessin pour le GL culling */
  for (i = mSettings.Mat_Size; i >=0 ; i-= 1)
  {
    x = (float)0 ;
    y = (float)Height(0,i) ;
    z = (float)(i*mSettings.Step_Size);
    glVertex3f(x,(float)mSettings.Floor_Size,z);
    glVertex3f(x ,y,z);
    
  }
  glEnd();
  
  glBegin(GL_TRIANGLE_STRIP);
  
  for (i = 0; i <=mSettings.Mat_Size; i+= 1)        {
    x = (float)(mSettings.Map_Size);
    y = (float)Height(0,i) ;
    z = (float)(i*mSettings.Step_Size);
    glVertex3f(x,(float)mSettings.Floor_Size,z);
    glVertex3f(x ,y,z);
  }
  
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  
  for (i = 0; i <= mSettings.Mat_Size; i+= 1)
  {
    x = (float)(i*mSettings.Step_Size);
    y = (float)Height(i,0);
    z = (float)0;
    glVertex3f(x,(float)mSettings.Floor_Size,z);
    glVertex3f(x ,y,z);
  }
  
  glEnd();
  glBegin(GL_TRIANGLE_STRIP);
  for (i = mSettings.Mat_Size; i >= 0; i-= 1)
  {
    x = (float)(i*mSettings.Step_Size);
    y = (float)Height(i,0) ;
    z = (float)mSettings.Map_Size;
    glVertex3f(x,(float)mSettings.Floor_Size,z);
    glVertex3f(x ,y,z);
  }
  glEnd();
  
  glBegin(GL_TRIANGLE_STRIP);
  glVertex3f(0.0f,(float)mSettings.Floor_Size,(float)mSettings.Map_Size);
  glVertex3f(0.0f,(float)mSettings.Floor_Size,0.0f);
  glVertex3f((float)mSettings.Map_Size,(float)mSettings.Floor_Size,(float)mSettings.Map_Size);
  glVertex3f((float)mSettings.Map_Size,(float)mSettings.Floor_Size,0.0f);
  glEnd();
  glColor3f(0.5f,0.5f,0.5f);
  
  // On affiche le Skybox
 SkyBox(SKYCENTER_X,SKYCENTER_Y,SKYCENTER_Z,SKY_WIDTH,SKY_HEIGHT, SKY_LARGE);
  
}

// SetShadow : calcul l'ombre 

//float CMap::SetShadow(int i, int j,float *time_color)
//{
//  float ambient = 00.76f;
//  float diff = 0.7f;
//  float s = ambient+diff*dot_product((Normal[i% mSettings.Mat_Size][j% mSettings.Mat_Size]),vSunDirection));
//  if (s<0) s = 0.0f;
//  else if (s >1.0f) s = 1.0f;
//  glColor3f(s*time_color[0],s*time_color[1],s*time_color[2]);
//  return s;
//}



// Rendu de toutes les faces de la map
void CMap::Render_every_faces()
{
  glEnable(GL_TEXTURE_2D);
  glColor4f(1.0f,1.0f,1.0f,1.0f);

  for (int f = 0; f < NBTEX_MAP ; f++ )
  {
    glBindTexture(GL_TEXTURE_2D,Textures[f]);
    glBegin(GL_TRIANGLES);
    for (int t = 0; t < nbtex[f]; t+=1)
    {
      Renderface(sortbytex[f][2*t],0);
      Renderface(sortbytex[f][2*t+1],1);
    }
    glEnd();
  }
}
    

// Rendu des faces en scroll_view
void CMap::Render_faces_in_scroll_view()
{
  glEnable(GL_TEXTURE_2D);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
 
  for (int k = 0 ; k< (int)mSettings.Frame_Size.x; k++)
  {
    if (((int)ScrollLimit.x+k>=mSettings.Mat_Size ))
       glTranslatef(mSettings.Map_Size,0,0);
    for (int l = 0 ; l < (int)mSettings.Frame_Size.z ; l++)
    {
      int i = (int)((k+(int)ScrollLimit.x)*(mSettings.Mat_Size) );
      i+=(int)((int)ScrollLimit.z+l);
      i*=2; 
      if ((l+(int)ScrollLimit.z>=mSettings.Mat_Size))
      {
        glTranslatef(0,0,mSettings.Map_Size);
        i = i-2*mSettings.Mat_Size;
      }

      glBindTexture(GL_TEXTURE_2D,Textures[facetab[i%numoffaces].tex]);
      glBegin(GL_TRIANGLES);
        Renderface(i%numoffaces,0);
        Renderface((i+1)%numoffaces,1);
      glEnd();
  
     if ((l+(int)ScrollLimit.z>=mSettings.Mat_Size))
         glTranslatef(0,0,-mSettings.Map_Size);
      
    }

       if (((int)ScrollLimit.x+k>=mSettings.Mat_Size ))
          glTranslatef(-mSettings.Map_Size,0,0);
        
  }
}




// Affiche un rectangle 

// void CMap::RenderQuad(int i, int j,float *time_color)
// {
  
// float x,y,z;
 // glBindTexture(GL_TEXTURE_2D,  Textures[TexMatrix[i][j]]);
//   glBegin(GL_TRIANGLE_STRIP);
//   x = (float)(i*mSettings.Step_Size + mSettings.Step_Size);
//   y = (float)Height((i+1)%mSettings.Mat_Size,(j))/255 * mSettings.Map_Max_Height;
//   z= (float)j*mSettings.Step_Size;
  
//   glTexCoord2f(1.0f,0.0f);
//   SetShadow(i+1,j,time_color);
//   glNormal3f(Normal[(i+1)%mSettings.Mat_Size][j].x,Normal[(i+1)%mSettings.Mat_Size][j].y,Normal[(i+1)%mSettings.Mat_Size][j].z);
//   glVertex3f(x,y,z);
//   
//   x = (float)i*mSettings.Step_Size;
//   y = (float)Height(i,j)/255 * mSettings.Map_Max_Height;
//   z = (float)j*mSettings.Step_Size;
  
//   glTexCoord2f(1.0f,1.0f);
//   SetShadow(i,j,time_color);
//   glNormal3f(Normal[i][j].x,Normal[i][j].y,Normal[i][j].z);
//   glVertex3f(x,y,z);
  
//   x = (float)(i*mSettings.Step_Size + mSettings.Step_Size);
//   y = (float)Height((i+1)%mSettings.Mat_Size,(j+1)%mSettings.Mat_Size)/255 * mSettings.Map_Max_Height;
//   z= (float)(j*mSettings.Step_Size+mSettings.Step_Size);
  
//   glTexCoord2f(0.0f,0.0f);
//   SetShadow(i+1,j+1,time_color);
//   glNormal3f(Normal[(i+1)%mSettings.Mat_Size][(j+1)%mSettings.Mat_Size].x,Normal[(i+1)%mSettings.Mat_Size][(j+1)%mSettings.Mat_Size].y,Normal[(i+1)%mSettings.Mat_Size][(j+1)%mSettings.Mat_Size].z);
//   glVertex3f(x,y,z);
  
//   x = (float)(i*mSettings.Step_Size);
//   y = (float)Height(i,(j+1)%mSettings.Mat_Size)/255 * mSettings.Map_Max_Height;
//   z = (float)(j*mSettings.Step_Size+ mSettings.Step_Size);
  
//   glTexCoord2f(0.0f,1.0f);
//   SetShadow(i,j+1,time_color);
//   glNormal3f(Normal[i][(j+1)%mSettings.Mat_Size].x,Normal[i][(j+1)%mSettings.Mat_Size].y,Normal[i][(j+1)%mSettings.Mat_Size].z);
//   glVertex3f(x,y,z);
//   glEnd();
// }


/*-------------------------------------------------------------*/
/*  Chargement des textures pour la map                        */
/*-------------------------------------------------------------*/

void CMap::LoadTextures()
{
  
  // Texture du skybox en BEE_MODE
  glGenTextures(1, &(Textures[BACK_ID]));
  tgaLoad("textures"PATHDELIM_S"background-back.tga",Textures[BACK_ID]);
  glGenTextures(1, &(Textures[FRONT_ID]));
  tgaLoad("textures"PATHDELIM_S"background-front.tga",Textures[FRONT_ID]);
  glGenTextures(1, &(Textures[TOP_ID]));
  tgaLoad("textures"PATHDELIM_S"background-top.tga",Textures[TOP_ID]);
  glGenTextures(1, &(Textures[BOTTOM_ID]));
  tgaLoad("textures"PATHDELIM_S"background-bottom.tga",Textures[BOTTOM_ID]);
  glGenTextures(1, &(Textures[LEFT_ID]));
  tgaLoad("textures"PATHDELIM_S"background-left.tga",Textures[LEFT_ID]);
  glGenTextures(1, &(Textures[RIGHT_ID]));
  tgaLoad("textures"PATHDELIM_S"background-right.tga",Textures[RIGHT_ID]);
  
  // Texture du sol
  //CreateTexture(Textures,"Textures/Terre2.bmp",EARTH_ID);
  glGenTextures(1, &(Textures[EARTH1_ID]));
  tgaLoad("textures"PATHDELIM_S"earth1.tga",Textures[EARTH1_ID]);
  glGenTextures(1, &(Textures[EARTH2_ID]));
  tgaLoad("textures"PATHDELIM_S"earth2.tga",Textures[EARTH2_ID]);
  glGenTextures(1, &(Textures[EARTH3_ID]));
  tgaLoad("textures"PATHDELIM_S"earth3.tga",Textures[EARTH3_ID]);
  glGenTextures(1, &(Textures[EARTH4_ID]));
  tgaLoad("textures"PATHDELIM_S"earth4.tga",Textures[EARTH4_ID]);
  glGenTextures(1, &(Textures[EARTH5_ID]));
  tgaLoad("textures"PATHDELIM_S"earth5.tga",Textures[EARTH5_ID]);
  glGenTextures(1, &(Textures[EARTH6_ID]));
  tgaLoad("textures"PATHDELIM_S"earth6.tga",Textures[EARTH6_ID]);
  glGenTextures(1, &(Textures[EARTH7_ID]));
  tgaLoad("textures"PATHDELIM_S"earth7.tga",Textures[EARTH7_ID]);
}


// Detruit toutes les textures de la map
void CMap::DeleteTextures()
{
  glDeleteTextures(1, &(Textures[BACK_ID]));
  
  glDeleteTextures(1, &(Textures[FRONT_ID]));
  
  glDeleteTextures(1, &(Textures[TOP_ID]));
  
  glDeleteTextures(1, &(Textures[BOTTOM_ID]));
  
  glDeleteTextures(1, &(Textures[LEFT_ID]));
  
  glDeleteTextures(1, &(Textures[RIGHT_ID]));
  
  
  // Texture du sol
  //CreateTexture(Textures,"Textures/Terre2.bmp",EARTH_ID);
  glDeleteTextures(1, &(Textures[EARTH1_ID]));
  
  glDeleteTextures(1, &(Textures[EARTH2_ID]));
  
  glDeleteTextures(1, &(Textures[EARTH3_ID]));
  
  glDeleteTextures(1, &(Textures[EARTH4_ID]));
  
  glDeleteTextures(1, &(Textures[EARTH5_ID]));
  
  glDeleteTextures(1, &(Textures[EARTH6_ID]));
  
  glDeleteTextures(1, &(Textures[EARTH7_ID]));
  
}

/*-------------------------------------------------------------*/
/* Affichage du Skybox                                         */
/*-------------------------------------------------------------*/
void CMap::SkyBox(float x, float y, float z,
                  float width, float height, float large)
{
  
  x = x - width/2;
  y = y - height/2;
  z = z - large/2;
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glBindTexture(GL_TEXTURE_2D, Textures[BACK_ID]);
  glBegin(GL_QUADS);
		glTexCoord2i(1,1); glVertex3f(x        , y         , z);
    glTexCoord2i(0,1); glVertex3f(x + width, y         , z);
    glTexCoord2i(0,0); glVertex3f(x + width, y + height, z);
    glTexCoord2i(1,0); glVertex3f(x        , y + height, z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, Textures[FRONT_ID]);
    glBegin(GL_QUADS);
    glTexCoord2i(0,0); glVertex3f(x, y, z + large);
    glTexCoord2i(0,1); glVertex3f(x , y+height, z+ large);
    glTexCoord2i(1,1); glVertex3f(x +width, y + height, z+large);
    glTexCoord2i(1,0); glVertex3f(x+width, y, z+large);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, Textures[TOP_ID]);
    glBegin(GL_QUADS);
    glTexCoord2i(1,0); glVertex3f(x , y+height, z+large);
    glTexCoord2i(1,1); glVertex3f(x, y+height, z);
    glTexCoord2i(0,1); glVertex3f(x+width, y +height, z);
    glTexCoord2i(0,0); glVertex3f(x +width, y + height, z+large);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, Textures[BOTTOM_ID]);
    glBegin(GL_QUADS);
    glTexCoord2i(1,0); glVertex3f(x, y, z);
    glTexCoord2i(1,1); glVertex3f(x , y, z +large);
    glTexCoord2i(0,1); glVertex3f(x +width, y, z+large);
    glTexCoord2i(0,0); glVertex3f(x +width, y, z);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, Textures[LEFT_ID]);
    glBegin(GL_QUADS);
    glTexCoord2i(1,1); glVertex3f(x, y, z+large);
    glTexCoord2i(0,1); glVertex3f(x, y, z);
    glTexCoord2i(0,0); glVertex3f(x, y +height, z);
    glTexCoord2i(1,0); glVertex3f(x, y +height , z+large);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, Textures[RIGHT_ID]);
    glBegin(GL_QUADS);
    glTexCoord2i(1,1); glVertex3f(x+width, y, z);
    glTexCoord2i(0,1); glVertex3f(x + width, y, z+large);
    glTexCoord2i(0,0); glVertex3f(x +width, y + height, z+large);
    glTexCoord2i(1,0); glVertex3f(x+width, y +height, z);
    glEnd();
}


// Rendu en mode scrolling

void CMap::RenderScrolling()
{
    
  
 // VERSION AVEC SEULEMENT LA MATRICE 
 //float time_color[3];
 // Get_time_color(time_color);
 // SDL_ShowCursor(true);
 // Variables qui détermine la limite haut gauche	de ce qui sera affiché à l'écran
 // int Limitx = (int)this->ScrollLimitX();
 // int Limitz = (int)this->ScrollLimitZ();
 // float modifx = (this->ScrollLimitX() - Limitx) * mSettings.Step_Size;
 // float modifz = (this->ScrollLimitZ() - Limitz) *mSettings.Step_Size;
 // i = Limitx;
 // 
 //  for (int k = 0 ; k< (int)mSettings.Frame_Size.x; k++)
 // {
    
 //   j = Limitz;			
 //   if ((i>= mSettings.Mat_Size	)) i -= mSettings.Mat_Size ;
 //   x= (float)k*mSettings.Step_Size-modifx;
 //   
 //   
 //   for (int l = 0 ; l < (int)mSettings.Frame_Size.z ; l++)
 //   {
 //    z= (float)l*mSettings.Step_Size-modifz;
      
      // On vérifie si on tente d'afficher en dehors de la matrice auquel cas 
      // on affiche l'autre bord de la matrice
      
 //     if ((j >= mSettings.Mat_Size ))	j -= mSettings.Mat_Size ;
    //  else if (j < 0 ) j += mSettings.Mat_Size  ; 
      
      
 //     glBindTexture(GL_TEXTURE_2D,Textures[TexMatrix[i][j]]);
 //     glBegin(GL_TRIANGLE_STRIP);
      
 //       SetShadow(i+1,j,time_color);
 //       glNormal3f(Normal[(i+1)%mSettings.Mat_Size][j].x,Normal[(i+1)%mSettings.Mat_Size][j].y,Normal[(i+1)%mSettings.Mat_Size][j].z);
 //       y = (float)this->Height(i+1,j)/255 * mSettings.Map_Max_Height;
 //       glTexCoord2f(1.0f,0.0f);
 //       glVertex3f(x+mSettings.Step_Size,y,z);
      
        // On affiche le second vertex en lui associant sa normale et ses coordonnées sur la texture
 //       SetShadow(i,j,time_color);
 //       y = (float)this->Height(i,j)/255 * mSettings.Map_Max_Height;
 //      glNormal3f(Normal[i][j].x,Normal[i][j].y,Normal[i][j].z);
 //       glTexCoord2f(1.0f,1.0f);
 //       glVertex3f(x,y,z);
      
      
 //       SetShadow(i+1,j+1,time_color);
 //      y = (float)this->Height(i+1,j+1)/255 * mSettings.Map_Max_Height;
 //       glNormal3f(Normal[(i+1)%mSettings.Mat_Size][(j+1)%mSettings.Mat_Size].x,Normal[(i+1)%mSettings.Mat_Size][(j+1)%mSettings.Mat_Size].y,Normal[(i+1)%mSettings.Mat_Size][(j+1)%mSettings.Mat_Size].z);
 //       glTexCoord2f(0.0f,0.0f);
 //       glVertex3f(x+mSettings.Step_Size,y,z+mSettings.Step_Size);
      
        // On affiche le second vertex en lui associant sa normale et ses coordonnées sur la texture
      
 //       SetShadow(i,j+1,time_color);
 //       y = (float)this->Height(i,j+1)/255 * mSettings.Map_Max_Height;
 //       glNormal3f(Normal[i][(j+1)%mSettings.Mat_Size].x,Normal[i][(j+1)%mSettings.Mat_Size].y,Normal[i][(j+1)%mSettings.Mat_Size].z);
 //       glTexCoord2f(0.0f,1.0f);
 //       glVertex3f(x,y,z+mSettings.Step_Size);
 //     glEnd();
      
 //     ++j;
      
      
      
 //   }
    //face = 1-face;
   // ++i;
  //}
  
  
// RENDU EN SELECTIONNANT LES FACES  
  glPushMatrix();
    glTranslatef(-ScrollLimitXf(),0,-ScrollLimitZf());
    Render_faces_in_scroll_view();
  glPopMatrix();

// Rendu du skybox
  this->SkyBox(SKYCENTER_X,SKYCENTER_Y,SKYCENTER_Z,SKY_WIDTH,SKY_HEIGHT, SKY_LARGE);
  
  
}

/////// DEBUT INSERTION CODE GAETAN DANS CODE FLORENT ///////

float CMap::GetMapHeight ( CVector2f& pos )
{
  float height;
  
  int curi = (int) floor(pos.x/(float) mSettings.Step_Size) % mSettings.Mat_Size;
  int curj = (int) floor(pos.z/(float) mSettings.Step_Size) % mSettings.Mat_Size;
  
  if (pos.x>mSettings.Map_Size)
    pos.x -= mSettings.Map_Size;
  if (pos.z>mSettings.Map_Size)
    pos.z -= mSettings.Map_Size;
  
  // détermination de la hauteur
  // petite explication de ce qui va être calculé (c très simple)
  // notre carré :
  //
  // B (i+1,j)               D (i+1, j+1)
  //  + ------------------- +
  //  |                  /  |
  //  |      A         /    |
  //  |     x        /      |
  //  |          O /        |
  //  |         x/          |
  //  |        /            |
  //  |      /              |
  //  |T1  /                |
  //  |  /                  |
  //  |/ T2                 |                i                       posx
  //  + ------------------- + (i, j+1)     ^                       ^
  //  C                      E             |      * STEP_SIZE =>   |
  //  (i,j)                                o-->  j                 o--> posz
  //
  // Remarque: i et j ne sont pas dans le sens "habituel": i est vers le
  // haut, j vers la droite.
  
  // produit scalaire entre le vecteur OA et (-1, 1), i.e. la normale à la
  // diagonale se dirigeant vers E
  // (curi+1) * STEP_SIZE => position réelle sur les x du point i+1 de la
  //                         matrice
  // (curi+1) * STEP_SIZE - posx) => position sur les x           -> ->
  //                                 du point A dans le repere (B,BC,BD)
  // (curi+0.5) * STEP_SIZE) : position sur les x du point O dans ce repere.
  //
  // donc si ce produit scalaire est négatif, on est dans le triangle T1 (du haut)
  float a = ((curi + 0.5f) * mSettings.Step_Size) - pos.x;
  float b = (float)((pos.z - (curj + 0.5f) * mSettings.Step_Size) * 1);
  if ( a + b < 0)
  { // triangle T1          -> ->              B
    // Soit le repere : (B, BC,BD)              x ---->
    //                                          |
    //                                   ->     V
    // principe: on décompose le vecteur BA selon les vecteurs de la base
    // (on travaille dans le plan uniquement)
    // on divise par la taille du coté du coté du carré => on a ainsi alpha
    // pour les x et beta pour les y:
    //          /            \      /
    // alpha = |   x   - x    |   /    STEP_SIZE
    //          \   i+1   A  /  /
    //          /            \      /
    // beta  = |   z  - z     |   /    STEP_SIZE
    //          \   A    j+1 /  /
    // on a en fait le % des vecteurs de la base "utilisé".
    // en gros on fait         ->          ->   ->
    //                 alpha * BC + beta * BD = BA
    //
    // reste plus qu'à utiliser ces vecteurs dans l'espace 3D pour déterminer
    // la hauteur :                   /         \            /        \ 
    //        hauteur = y  + alpha * |  y  - y   | + beta * | y  - y   |
    //                   B            \  C    B /            \ D    B /
    // Rappel: le sol est défini par xOz et la HAUTEUR est selon l'axe Oz
    //
    // simplifié
    height = Height(curi+1,curj) +
      ( (float) (curi+1) - (float) pos.x / (float) mSettings.Step_Size) *
      ((float) Height(curi,curj) - (float) Height(curi+1,curj) )
      +
      ( (float) pos.z / (float) mSettings.Step_Size - (float) curj ) *
      ( (float) Height(curi+1,curj+1) - (float) Height(curi+1,curj) );
  }
  else
  { //                                                       ->  ->
    // même principe mais avec le triangle T2 et la base (E, EC, ED)
    height = Height(curi,curj+1) +
      ( (float) pos.x / (float) mSettings.Step_Size - (float) (curi)) *
      ((float) Height(curi+1,curj+1) - (float) Height(curi,curj+1) )
      +
      ( (float) pos.z / (float) mSettings.Step_Size - (float) (curj+1) ) *
      ( (float) Height(curi,curj+1) - (float) Height(curi,curj) );
  }
  
  
  return height;
}


void CMap::PutOnTheMap(float *p1, float *p2, float *p3, float *p4, float theta, CVector2f pos)
{
  //
  //              sizex
  //   <-------------------------->
  //  D ------------------------- C
  //  |                           |    ^
  //  |                           |    |
  //  |                           |    |
  //  |             x---->        |    | sizez
  //  |                           |    |            ^ Z
  //  |                           |    |            |
  //  |                           |    V            x---> X
  //  A ------------------------- B                Y
  //
#define X 0
#define Y 1
#define Z 2
  
  float sizex, sizez, sizexd2, sizezd2;
  float bbposx, bbposz;
  
  sizex = p3[X] - p1[X];
  sizez = p3[Z] - p2[Z];
  if ( sizex < 0 )
    sizex = -sizex;
  if ( sizez < 0 )
    sizez = - sizez;
  
  sizexd2 = sizex / 2;
  sizezd2 = sizez / 2;
  
  bbposx = p1[X] + sizexd2;
  bbposz = p1[Z] + sizezd2;
  
  CVector3f pta, ptb, ptc, ptd;
  CVector3f *ptmax1,*ptmax2,*ptmax3;
  
  float thetaRAD = -theta * DEGTORAD;
  float costhetaRAD = (float) cos(thetaRAD),
    sinthetaRAD = (float) sin(thetaRAD);

  
  pta.x = pos.x + bbposx + costhetaRAD * (-sizexd2) - sinthetaRAD * (sizezd2);
  pta.z = pos.z + bbposz + sinthetaRAD * (-sizexd2) + costhetaRAD * (sizezd2);
  pta.y = GetMapHeight(pta);
  
  ptb.x = pos.x + bbposx + costhetaRAD * (-sizexd2) - sinthetaRAD * (-sizezd2);
  ptb.z = pos.z + bbposz + sinthetaRAD * (-sizexd2) + costhetaRAD * (-sizezd2);
  //ptb.y = GetMapHeight(ptb.x, ptb.z);
  
  ptc.x = pos.x + bbposx + costhetaRAD * (sizexd2) - sinthetaRAD * (sizezd2);
  ptc.z = pos.z + bbposz + sinthetaRAD * (sizexd2) + costhetaRAD * (sizezd2);
  ptc.y = GetMapHeight(ptc);
  
  ptd.x = pos.x + bbposx + costhetaRAD * (sizexd2) - sinthetaRAD * (-sizezd2);
  ptd.z = pos.z + bbposz + sinthetaRAD * (sizexd2) + costhetaRAD * (-sizezd2);
  //ptd.y = GetMapHeight(ptd.x, ptd.z);
  
  ptb = (ptb + ptd) / 2.0f;
  ptb.y = GetMapHeight(ptb);

  
  ptmax1 = &pta;
  ptmax2 = &ptc;
  ptmax3 = &ptb;
  
#if 0
  glPushMatrix();
  glColor3f(0.0,0.0,1.0);
  glLineWidth(5.0);
  //glTranslatef(0.0f,0.1f,0.0f);
  
  glBegin(GL_TRIANGLES);
  glVertex3f(ptmax1->x, ptmax1->y, ptmax1->z);
  glVertex3f(ptmax2->x, ptmax2->y, ptmax2->z);
  glVertex3f(ptmax3->x, ptmax3->y, ptmax3->z);
  glEnd();
  
  glLineWidth(1.0);
  glPopMatrix();
#endif
  
  CVector3f pla;
  CVector3f v1,v2;

  // produit vectoriel entre vec(ptmax1, ptmax2) et vec(ptmax1,ptmax3)
  v1 = *ptmax2 - *ptmax1;
  
  v2 = *ptmax3 - *ptmax1;

  
  pla = v1.CrossProd(v2);
  // pla est le vecteur normal au plan
  
  CVector3f rot(-pla.z, 0, pla.x); // vector de rotation !!!
  // produit scalaire entre pla et le vecteur Oy (0,1,0)
  // on a l'axe de rotation... manque plus que l'angle => produit scalaire!
  // ->  ->                            ->    ->
  // u . v = x * x + y * y + z * z = ||x||.||y||.cos(an)
  //          u   v   u   v   u   v
  float an = (float)acos((pla.y) / (sqrt(pla.x * pla.x + pla.y * pla.y + pla.z * pla.z) * 1));
  
  /////////////////////////////////////////////////////////////////////////////////////////////
  
  float height = GetMapHeight(pos);
   

  glTranslatef(pos.x, height, pos.z) ;

#if 0
  glLineWidth(5.0f);
  glColor3f(0.0,0.0,1.0);
  glBegin(GL_LINES      );
  glVertex3f(0, 0, 0);
  glVertex3f(pla.x, pla.y, pla.z);
  glEnd();
  glColor3f(0.0,1.0,0.0);
  glBegin(GL_LINES      );
  glVertex3f(0, 0, 0);
  glVertex3f(rot.x*20, rot.y*20, rot.z*20);
  glEnd();
  glColor3f(1.0,0.0,0.0);
  glBegin(GL_LINES      );
  glVertex3f(0,-2000, 0);
  glVertex3f(0, 2000, 0);
  glEnd();
  glLineWidth(1.0f);
  
#endif
  
  glRotatef(-an * RADTODEG, rot.x, rot.y, rot.z);
  
  glRotatef(theta,0.0,1.0,0.0) ;
  
#undef X
#undef Y
#undef Z

}


/////// FIN INSERTION CODE GAETAN DANS CODE FLORENT ///////


// Fonction qui permet de bouger le soleil

//void CMap::MoveSun(float move)
//{
//  static int nightsound_playing = -1;
  
//  float cosT = (float)cos(move);
//  float sinT = (float)sin(move);
//  CVector3f sun;
//  static CVector3f vect = cross_product(vSunDirection*-1,CVector3f(0,1,0));
//  vect.Normalize();
  // La direction vers laquelle on regarde
//  CVector3f vWay = vSunDirection;
  
  
  // On applique la matrice de rotation
  
//  sun.x = (cosT + (1 - cosT) *vect.x *vect.x) * vWay.x;
//  sun.x += ((1-cosT)*vect.x*vect.y -vect.z*sinT) * vWay.y;
//  sun.x += ((1 - cosT) *vect.x *vect.z +vect.y*sinT) * vWay.z;
  
  
//  sun.y = ((1 - cosT)*vect.y * vect.x + vect.z * sinT) * vWay.x;
//  sun.y += (cosT + (1 - cosT) * vect.y * vect.y) * vWay.y;
//  sun.y += ((1 - cosT) * vect.y * vect.z - vect.x * sinT) * vWay.z;
  
  
//  sun.z = ((1 - cosT) * vect.x * vect.z - vect.y * sinT) * vWay.x;
//  sun.z += ((1 - cosT) * vect.y * vect.z + vect.x * sinT) * vWay.y;
//  sun.z += (cosT + (1 - cosT) * vect.z * vect.z) * vWay.z;
  
  
 // if ( dot_product(sun, CVector3f(0,-1,0)) < 0 )
 // {
 //   if (nightsound_playing==-1)
 //   {
 //     nightsound_playing  = GetSoundManager().PlaySound(NIGHT_ID, true);
 //   }
 // }
 // else
 // {
 //   if (nightsound_playing!=-1)
 //   {
 //     GetSoundManager().StopSound(nightsound_playing);
 //     nightsound_playing = -1;
 //   }
 // }
  
  // On tourne la vue seulement
 // sun.Normalize();
 // vSunDirection = sun;
  
//}


// Fonction qui devait donner la couleur a afficher en fonction de la direction
// du soleil

//void CMap::Get_time_color(float color[])
//{
  
  //		if (mSettings.vSunDirection.y < 0 )/
  //	{
  
  
  //	color[0] = (vSunDirection.y*-1 + 1.0f)/2;
  
  //	}
  //	else color[0] = 0.2;
  //	if (mSettings.vSunDirection.y< 0)
  //		color[1] = (vSunDirection.y*-1+1.0f) /2;
  //	else color[1] = 0.2;
  //	if (mSettings.vSunDirection.y <0)
  //		color[2] = (vSunDirection.y*-1+1.0f)/2;
  //	else color[2] = 0.6;
  
  //	if (color[0]>1) color[0]=1.0f;
  //else if (color[0]<0.3f) color[0]=0.8f;
  //if (color[1]>1) color[1]=1.0f;
 // else if (color[1]<0.3f) color[1]=0.8f;
  //if (color[2]>1) color[2]=1.0f;
  //else if (color[2]<1.0f) color[2]=0.50f;
  
//}


// Creer la minimap en fonction des hauteurs
void CMap::CreateMinimap()
{

  	
	unsigned char *pTexture = NULL;											
	pTexture = new unsigned char [mSettings.Mat_Size * mSettings.Mat_Size ];
  pTexture[0] = 1;
  
  for (int i = 0;i <mSettings.Mat_Size;i++)
    for (int j = 0; j<mSettings.Mat_Size;j++)
    {
      pTexture[i*mSettings.Mat_Size+j] = (unsigned char)(HeightMatrix[i][j]/mSettings.Map_Max_Height*255);
    }
  glPixelStorei ( GL_UNPACK_ALIGNMENT, 1);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &(mmap->tex));								
	glBindTexture(GL_TEXTURE_2D, mmap->tex);					
	
	// Creation de la texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, mSettings.Mat_Size, mSettings.Mat_Size, GL_LUMINANCE, GL_UNSIGNED_BYTE, pTexture);						
		
	// Qualité de la texture
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  #define GL_CLAMP_TO_EDGE	0x812F	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	delete [] pTexture;

}
