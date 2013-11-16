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





#include "AI_renders.h"
#include "AI_tokens.h"
#include "../lib/C3DObject.h"
#include "../lib/CMotor.h"
//#include "../objects.h"

AnimsCell* anims_links_[ACTION_TYPE_NB] ;

/****************************\
* méthodes de aiGRAPHICWORLD *
\****************************/

aiGraphicWorld::aiGraphicWorld(aiWorld& w)
: world_(w), rendered_token_(-1), highlighted_object_(0)
{
  glGenTextures(1,&token_texture_id_) ;
  tgaLoad("textures"PATHDELIM_S"token.tga",token_texture_id_);
  set_token_RGBA_color(found_food, colorRGBA(DEFAULT_FOUND_FOOD_TOKEN_COLOR)) ;
  set_token_RGBA_color(path      , colorRGBA(DEFAULT_PATH_TOKEN_COLOR)) ;
  create_animations_links() ;
}

void aiGraphicWorld::render()
{
  switch (cCam.ViewMode())
  {
  case SCROLL_VIEW :
    world_.nest_->render_in_scroll_view() ;
    world->render_visible_objects_in_scroll_view() ;
    render_tokens_in_scroll_view() ;
    if (highlighted_object_)   
    {  
      render_highlighted_object() ;       
    }
    world_.map_->menu_down->Render() ;
    world_.map_->mmap->Render() ;
    render_every_object_on_minimap() ;
    if (highlighted_object_) 
      highlighted_object_->render_in_menu();
    break ;
  case BEE_VIEW :
    world_.nest_->render_in_bee_view() ;
    world->render_visible_objects_in_bee_view() ;
    break ;
  }
}

void aiGraphicWorld::render_highlighted_object()
{
  glPushMatrix() ;
    vect2f pos(highlighted_object_->get_coords2d()) ;
    vect2f dec;

    if (cCam.ViewMode()!=BEE_VIEW) 
    {
      dec = world_.map_->ScrollingOffset(pos) ;
    }

    GLUquadricObj* qobj = gluNewQuadric() ;
    gluQuadricDrawStyle(qobj, GLU_SILHOUETTE) ;
    glEnable(GL_BLEND) ;
    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA) ;
    glTranslatef(pos.x-dec.x, world_.map_->GetMapHeight(pos), pos.z-dec.z) ;
    glColor4f(0.0f, 1.0f, 0.0, 0.75f) ;
    gluSphere(qobj, highlighted_object_->g_obj_->objs3d_.m_hiRes->GetBoundingSphere(), 20.0f, 6.0f) ;
    gluDeleteQuadric(qobj) ;
    glColor3f(1.0f, 1.0f, 1.0f) ;
    glDisable(GL_BLEND) ;
  glPopMatrix() ;
  glColor4f(1.0f, 1.0f, 1.0, 1.0f) ;
}


const float PhPt1[]={ TOKEN_SIZE,0, TOKEN_SIZE},
            PhPt2[]={ TOKEN_SIZE,0,-TOKEN_SIZE},
            PhPt3[]={-TOKEN_SIZE,0, TOKEN_SIZE},
            PhPt4[]={-TOKEN_SIZE,0,-TOKEN_SIZE};


void aiGraphicWorld::render_tokens_in_scroll_view()
{
  vect2f vect ;
  int ti = get_rendered_token() ;
 // int ti=3;
  if (ti!=-1) {
    float d ;
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, get_token_texture_id());
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    glDepthMask(GL_FALSE);

    token_t tt = static_cast<token_t>(ti) ;
    colorRGBA RGBA_base_color = get_rendered_token_RGBA_color() ;
    colorRGBA RGBA_color ;

    for (uint i(0) ; i<world_.get_number_of_tokens(tt) ; ++i, world_.next_token(tt)) {
      if (world_.visible_in_scrolling_view(world_.current_token(tt).where_)) {
        glPushMatrix() ;
          glColorMask(1,1,1,1);

          vect = world_.map_->ScrollingOffset(world_.current_token(tt).where_) ;
          glTranslatef(-vect.x,0,-vect.z) ;

          world_.map_->PutOnTheMap((float*)PhPt1,(float*)PhPt2,(float*)PhPt3,(float*)PhPt4, 0,
                                    world_.current_token(tt).where_);

          RGBA_color = RGBA_base_color ;
          RGBA_color.alpha *= world_.current_token(tt).amount_ / 5000.0f ;
          glColor4fv(reinterpret_cast<float *>(&RGBA_color)) ;

          d = 1 / 10.0f ;
         glBegin(GL_QUAD_STRIP) ;
            glTexCoord2f(0.0f,0.0f);
            glVertex3f(TOKEN_SIZE, 0.5f+d, TOKEN_SIZE);
            glTexCoord2f(0.0f,1.0f);
            glVertex3f(TOKEN_SIZE, 0.5f+d, -TOKEN_SIZE);
            glTexCoord2f(1.0f,0.0f);
            glVertex3f(-TOKEN_SIZE, 0.5f+d, TOKEN_SIZE);
            glTexCoord2f(1.0f,1.0f);
            glVertex3f(-TOKEN_SIZE, 0.5f+d, -TOKEN_SIZE);
          glEnd();
        glPopMatrix() ;
      }
    }
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
  }
}

void aiGraphicWorld::render_every_object_on_minimap()
{
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
      glLoadIdentity();

      glOrtho(0,1024,768,0,0,1);
  
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glBegin(GL_POINTS);
      for(uint i(0) ; i < world_.get_number_of_objects() ; ++i, world_.next_object())
      {
        if (world_.current_object().is_active()) {
          colorRGBA color(world_.current_object().color_on_minimap()) ;
          glColor4fv(reinterpret_cast<float *>(&color)) ;
          glVertex2i(world_.map_->mmap->ulx + world_.current_object().get_coords2d().z / world_.map_->mmap->ratio,
                     world_.map_->mmap->lry - world_.current_object().get_coords2d().x / world_.map_->mmap->ratio);
        }
      }
      glEnd();

      glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
  glPopMatrix();
  //glLoadIdentity();

  //glEnable(GL_DEPTH_TEST);
}

void aiGraphicWorld::reload_token_texture()
{
  glDeleteTextures(1,&token_texture_id_) ;
  glGenTextures(1,&token_texture_id_) ;
  tgaLoad("textures"PATHDELIM_S"token.tga",token_texture_id_);
}

aiGraphicWorld::~aiGraphicWorld()
{
  delete_animations_links() ;
}

/************************\
* classe aiGraphicObject *
\************************/

void aiGraphicObject::render()
{

  float *p1,*p2,*p3,*p4;
  p1 = objs3d_.m_hiRes->GetBoundingBox().m_point[0];
  p2 = objs3d_.m_hiRes->GetBoundingBox().m_point[1];
  p3 = objs3d_.m_hiRes->GetBoundingBox().m_point[2];
  p4 = objs3d_.m_hiRes->GetBoundingBox().m_point[3];

  glPushMatrix() ;
    vect2f vect = coords2d() ;

    if (cCam.ViewMode()!=BEE_VIEW) 
    {
      vect = map_.ScrollingOffset(vect) ;
      glTranslatef(-vect.x,0,-vect.z) ;
    }

    map_.PutOnTheMap((float*)p1,(float*)p2,(float*)p3,(float*)p4, angles3d_.y,
                      coords2d());

    // il faut toujours mettre à jour l'objet hires, pour garder des animations cohérentes.
    if ((cCam.ViewMode()==BEE_VIEW) && (objs3d_.m_lowRes) &&
       (get_distance_from_bee_camera() > DIST_LOWRES_OBJECT))
    {
      // si on est en mode Abeille et que l'objet est loin on affiche l'objet basse résolution,
      // s'il existe
      // mais il faut mettre à jour l'objet hires, pour garder des animations cohérentes.
      objs3d_.m_hiRes->Update() ;
      objs3d_.m_lowRes->Render() ;
    }
    else 
    { // sinon c'est l'objet haute résolution qu'on affiche
      objs3d_.m_hiRes->Render() ;
    }
    /*
    if (highlighted_)
    {
      GLUquadricObj* qobj = gluNewQuadric();
      gluQuadricDrawStyle(qobj, GLU_FILL);
      glEnable(GL_BLEND);
      glBlendFunc(GL_ONE_MINUS_SRC_ALPHA,GL_SRC_ALPHA);
      glPushMatrix();
        glTranslatef(0.0f, 0.2f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor4f(0.0f, 1.0f, 0.0, 0.75f);
        gluSphere(qobj, objs3d_.m_hiRes->GetBoundingSphere(), 20.0f, 6.0f);
        gluDeleteQuadric(qobj);
        glColor3f(1.0f, 1.0f, 1.0f);
      glPopMatrix();
      glDisable(GL_BLEND);
    }*/

  glPopMatrix() ;
}

void add_anims_link(const action_type oa, const action_type na, const uint ci, const uint ti, const uint ni)
{
  anims_links_[oa] = new AnimsCell(na,AnimsLink(ci,ti,ni),anims_links_[oa]) ;
}

void add_anims_link(const action_type oa, const action_type na, const uint ci, const uint ni)
{
  anims_links_[oa] = new AnimsCell(na,AnimsLink(ci,ni,ni),anims_links_[oa]) ;
}


const AnimsLink& get_anims_link(const action_type oa, const action_type na)
{
  static const AnimsLink invalid_AnimsLink_ ;

  AnimsCell* p = anims_links_[oa] ;
  while (p) {
    if (p->next_action_==na)
      break ;
    else
      p = p->next_ ;
  }
  return (p) ?  p->val_ : invalid_AnimsLink_ ;
}

void create_animations_links()
{
  for ( uint i(0); i<ACTION_TYPE_NB ; ++i)
    anims_links_[i]=0;

  // IDLE
  add_anims_link(IDLE, WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE,
                 ANIMATION_STAY_UID, ANIMATION_WALK_UID) ;

  add_anims_link(IDLE, FOLLOW_FOUND_FOOD_PHEROMONE_TO_FIND_FOOD,
                 ANIMATION_STAY_UID, ANIMATION_WALK_UID) ;

  add_anims_link(IDLE, LOOK_FOR_A_NEW_SOURCE_OF_FOOD,
                 ANIMATION_STAY_UID, ANIMATION_WALK_UID) ;

  add_anims_link(IDLE, GO_BACK_TO_THE_NEST_LOOKING_FOR_FOOD,
                 ANIMATION_STAY_UID, ANIMATION_WALK_UID) ;

  add_anims_link(IDLE, RETURN_TO_NEST_WITH_INFO_ABOUT_FOOD,
                 ANIMATION_STAY_UID, ANIMATION_WALK_UID) ;

  add_anims_link(IDLE, BRING_FOOD_TO_THE_NEST,
                 ANIMATION_STAY_UID, ANIMATION_WALKWITHSEED_UID) ;

  
  add_anims_link(IDLE, HUNT_ANTS,
                 ANIMATION_STAY_UID, ANIMATION_WALK_UID) ;


  // WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE

  add_anims_link(WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE, IDLE,
                 ANIMATION_WALK_UID, ANIMATION_STAY_UID) ;

  add_anims_link(WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE, BRING_FOOD_TO_THE_NEST,
                 ANIMATION_WALK_UID, ANIMATION_WALKWITHSEED_UID) ;

  // BRING_FOOD_TO_THE_NEST

  add_anims_link(BRING_FOOD_TO_THE_NEST, IDLE,
                 ANIMATION_WALKWITHSEED_UID, ANIMATION_STAYWITHSEED_UID) ;

  add_anims_link(BRING_FOOD_TO_THE_NEST, WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE,
                 ANIMATION_WALKWITHSEED_UID, ANIMATION_WALK_UID) ;

  // FOLLOW_FOUND_FOOD_PHEROMONE_TO_FIND_FOOD

  add_anims_link(FOLLOW_FOUND_FOOD_PHEROMONE_TO_FIND_FOOD, WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE,
                 ANIMATION_WALKWITHSEED_UID, ANIMATION_WALK_UID) ;
  add_anims_link(FOLLOW_FOUND_FOOD_PHEROMONE_TO_FIND_FOOD, RETURN_TO_NEST_WITH_INFO_ABOUT_FOOD,
                 ANIMATION_WALKWITHSEED_UID, ANIMATION_WALK_UID) ;

  // LOOK_FOR_A_NEW_SOURCE_OF_FOOD

  add_anims_link(LOOK_FOR_A_NEW_SOURCE_OF_FOOD, WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE,
                 ANIMATION_WALKWITHSEED_UID, ANIMATION_WALK_UID) ;

  // RETURN_TO_NEST_WITH_INFO_ABOUT_FOOD

  add_anims_link(RETURN_TO_NEST_WITH_INFO_ABOUT_FOOD, WANDER_AROUND_NEST_WAITING_FOR_FOUND_FOOD_PHEROMONE,
                 ANIMATION_WALKWITHSEED_UID, ANIMATION_WALK_UID) ;

}


void delete_animations_links()
{
  AnimsCell* p, *temp ;
  for(uint i(0) ; i<ACTION_TYPE_NB ; ++i) {
    p = anims_links_[i] ;
    while (p) {
      temp = p ;
      p = p->next_ ;
      delete temp ;
    }
  }
}


void aiGraphicObject::update_animation(const action_type ol, const action_type ne)
{
  AnimsLink al = get_anims_link(ol,ne);
  objs3d_.m_hiRes->ChangeNextAnimation(al.current_anim_id_, al.next_anim_id_);
  objs3d_.m_hiRes->ChangeAnimation(al.current_anim_id_, al.transition_anim_id_);
}

/*****************\
* classe aiObject *
\*****************/

// void aiObject::render() const
// fait tout simplement
// void aiGraphicObject::render("sur la CMap")

/***************\
* classe aiRock *
\***************/
void aiRock::graphic_update()
{
  // ... {

  // } ...
  aiObject::graphic_update() ;
}

/***************\
* classe aiFood *
\***************/

void aiFood::graphic_update()
{
  // ... {

  // } ...
  aiObject::graphic_update() ;
}

/***************\
* classe aiSeed *
\***************/

void aiSeed::graphic_update()
{
  // ... {

  // } ...
  aiFood::graphic_update() ;
}


/**********************\
* classe aiLivingBeing *
\**********************/

void aiLivingBeing::graphic_update()
{
  // ... {
  // } ...
  aiObject::graphic_update() ;
}

/**************\
* classe aiAnt *
\**************/

void aiAnt::graphic_update()
{
  // ... {

  // } ...
  aiLivingBeing::graphic_update() ;
}

/****************\
* classe aiMinor *
\****************/

void aiMinor::graphic_update()
{
  // ... {
  destination_.Normalize() ;
  g_obj_->angles3d_.y = deg_angle(get_destination()) ;

  // fixe...
  vect2f new_coords = coords2d() + get_destination() * get_speed() ;
  world_.fix_coords(new_coords) ;
  teleport_to(new_coords);
  //...fixe

  // } ...
  aiAnt::graphic_update() ;
}

/********************\
* classe aiHarvester *
\********************/

void aiHarvester::graphic_update()
{
  aiMinor::graphic_update() ;
  // ... {

  // } ...
}

/****************\
* classe aiScout *
\****************/

void aiScout::graphic_update()
{
  aiMinor::graphic_update() ;
  // ... {

  // } ...
}

/*****************\
* classe aiSpider *
\*****************/

void aiSpider::graphic_update()
{
  aiLivingBeing::graphic_update() ;
  // ... {
  destination_.Normalize() ;
  g_obj_->angles3d_.y = deg_angle(get_destination()) ;

  // fixe...
  vect2f new_coords = coords2d() + get_destination() * get_speed() ;
  world_.fix_coords(new_coords) ;
  teleport_to(new_coords);
  // } ...
}

/********************\
* classe aiVegetable *
\********************/

void aiVegetable::graphic_update()
{}


void aiNest::render_in_scroll_view()
{
 for(uint i(0) ; i < get_number_of_entrances() ; ++i, next_entrance()) 
 {
   vect2f vect = world_.map_->ScrollingOffset(current_entrance().get_coords2d()) ;
   glPushMatrix();
     vect2f tv(current_entrance().get_coords2d());
     glTranslatef(-vect.x + current_entrance().get_coords2d().x,
        world_.map_->GetMapHeight(tv),
        -vect.z + current_entrance().get_coords2d().z) ;
     glLineWidth(4);
     glBegin(GL_LINES);
       glVertex3f(0,-10,0);
       glVertex3f(0,1000,0);
     glEnd();
     glLineWidth(1);
   glPopMatrix();
 }
}

void aiNest::render_in_bee_view()
{
}
