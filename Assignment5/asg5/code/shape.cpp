// $Id: shape.cpp,v 1.2 2019-02-28 15:24:20-08 - - $

#include <typeinfo>
#include <unordered_map>
#include <cmath>
using namespace std;

#include "shape.h"
#include "util.h"
#include "graphics.h"

static unordered_map<void*,string> fontname {
   {GLUT_BITMAP_8_BY_13       , "Fixed-8x13"    },
   {GLUT_BITMAP_9_BY_15       , "Fixed-9x15"    },
   {GLUT_BITMAP_HELVETICA_10  , "Helvetica-10"  },
   {GLUT_BITMAP_HELVETICA_12  , "Helvetica-12"  },
   {GLUT_BITMAP_HELVETICA_18  , "Helvetica-18"  },
   {GLUT_BITMAP_TIMES_ROMAN_10, "Times-Roman-10"},
   {GLUT_BITMAP_TIMES_ROMAN_24, "Times-Roman-24"},
};

unordered_map<string,void*> fontcode {
   {"Fixed-8x13"    , GLUT_BITMAP_8_BY_13       },
   {"Fixed-9x15"    , GLUT_BITMAP_9_BY_15       },
   {"Helvetica-10"  , GLUT_BITMAP_HELVETICA_10  },
   {"Helvetica-12"  , GLUT_BITMAP_HELVETICA_12  },
   {"Helvetica-18"  , GLUT_BITMAP_HELVETICA_18  },
   {"Times-Roman-10", GLUT_BITMAP_TIMES_ROMAN_10},
   {"Times-Roman-24", GLUT_BITMAP_TIMES_ROMAN_24},
};

rgbcolor shape::borderColor = rgbcolor(255,0,0);
GLfloat shape::borderWidth = 4;

ostream& operator<< (ostream& out, const vertex& where) {
   out << "(" << where.xpos << "," << where.ypos << ")";
   return out;
}

shape::shape() {
   DEBUGF ('c', this);
}

text::text (void* glut_bitmap_font_, const string& textdata_):
      glut_bitmap_font(glut_bitmap_font_), textdata(textdata_) {
   DEBUGF ('c', this);
}

ellipse::ellipse (GLfloat width, GLfloat height):
dimension ({width, height}) {
   DEBUGF ('c', this);
}

circle::circle (GLfloat diameter): ellipse (diameter, diameter) {
   DEBUGF ('c', this);
}


polygon::polygon (const vertex_list& vertices_): vertices(vertices_) {
   DEBUGF ('c', this);
}

rectangle::rectangle (GLfloat width, GLfloat height):
            polygon({{0, 0}, {0, height}, 
            {width, height}, {width, 0}}) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

square::square (GLfloat width): rectangle (width, width) {
   DEBUGF ('c', this);
}

diamond::diamond (GLfloat width, GLfloat height): 
            polygon({{-width/2, 0}, {0, -height/2},
            {width/2, 0}, {0, height/2}}) {
   DEBUGF ('c', this << "(" << width << "," << height << ")");
}

triangle::triangle (const vertex_list& vertices_): polygon(vertices_){
   DEBUGF ('c', this);
}

equilateral::equilateral (GLfloat width): 
            triangle({{0,0},{width,0}, 
            {width/2, (width/2) * static_cast<float>(sqrt(3))} }) {
   DEBUGF ('c', this);
}

void text::draw (const vertex& center, const rgbcolor& color,
               const int position) const {
   DEBUGF ('d', this << "(" << center << "," << color <<
              "," << position << ")");

   // To draw the text
   const GLubyte* data =
         reinterpret_cast<const GLubyte*> (textdata.c_str());
   glColor3ubv(color.ubvec);
   glRasterPos2f (center.xpos, center.ypos);
   glutBitmapString (glut_bitmap_font, data);

   // To draw the text border if it is selected
   if (window::isSelected) {
      GLfloat width = glutBitmapLength(glut_bitmap_font, data);
      GLfloat height = glutBitmapHeight(glut_bitmap_font);
      vertex_list borderVer {{0, 0}, {0, height}, 
                 {width, height}, {width, 0}};
      glClear (GL_COLOR_BUFFER_BIT);
      glLineWidth(shape::borderWidth);
      glBegin(GL_LINE_LOOP);
      glColor3ubv(rgbcolor(shape::borderColor).ubvec);
      for (auto itor = borderVer.begin(); 
           itor != borderVer.end(); ++itor){
         glVertex2f(itor->xpos + center.xpos, itor->ypos + center.ypos);
      }
   }
   glEnd();
}

void ellipse::draw (const vertex& center, const rgbcolor& color,
                  const int position) const {
   DEBUGF ('d', this << "(" << center << "," << color <<
              "," << position << ")");

   // To draw the ellipse
   glBegin(GL_POLYGON) ;
   glColor3ubv(color.ubvec) ;
   const GLfloat delta = 2 * M_PI / 64;
   const GLfloat width = dimension.xpos;
   const GLfloat height = dimension.ypos;
   for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
      GLfloat xpos = width * cos (theta) + center.xpos;
      GLfloat ypos = height * sin (theta) + center.ypos;
      glVertex2f (xpos, ypos);
   }
   glEnd();

   const GLubyte* pos =
      reinterpret_cast<const GLubyte*> 
      (to_string(position).c_str());
   glColor3f(0.5f, 0.0f, 1.0f);
   glRasterPos2f (center.xpos, center.ypos);
   glutBitmapString (GLUT_BITMAP_HELVETICA_10, pos);

   // To draw the ellipse outline if it is selected
   if (window::isSelected) {
      glLineWidth (shape::borderWidth);
      glBegin (GL_LINE_LOOP);
      glColor3ubv (rgbcolor(shape::borderColor).ubvec);
      for (GLfloat theta = 0; theta < 2 * M_PI; theta += delta) {
         GLfloat xpos = width * cos (theta) + center.xpos;
         GLfloat ypos = height * sin (theta) + center.ypos;
         glVertex2f (xpos, ypos);
      }
      glEnd();
   }
}

void polygon::draw (const vertex& center, const rgbcolor& color,
                   const int position) const {
   DEBUGF ('d', this << "(" << center << "," << color <<
              "," << position << ")");

   // To draw the polygon
   glBegin(GL_POLYGON);
   glColor3ubv(color.ubvec);
   for (auto itor = vertices.begin(); itor != vertices.end(); ++itor){
      glVertex2f(itor->xpos + center.xpos, itor->ypos + center.ypos);
   }
   glEnd();

   const GLubyte* pos =
      reinterpret_cast<const GLubyte*> 
      (to_string(position).c_str());
   glColor3f(0.5f, 0.0f, 1.0f);
   glRasterPos2f (center.xpos, center.ypos);
   glutBitmapString (GLUT_BITMAP_HELVETICA_10, pos);

   // To draw the polygon outline if it is selected
   if (window::isSelected) {
      glLineWidth (shape::borderWidth);
      glBegin (GL_LINE_LOOP);
      glColor3ubv (rgbcolor(shape::borderColor).ubvec);
      for (auto itor = vertices.begin(); 
           itor != vertices.end(); ++itor){
         glVertex2f(itor->xpos + center.xpos, itor->ypos + center.ypos);
      }
      glEnd();
   }
}

void shape::show (ostream& out) const {
   out << this << "->" << demangle (*this) << ": ";
}

void text::show (ostream& out) const {
   shape::show (out);
   out << glut_bitmap_font << "(" << fontname[glut_bitmap_font]
       << ") \"" << textdata << "\"";
}

void ellipse::show (ostream& out) const {
   shape::show (out);
   out << "{" << dimension << "}";
}

void polygon::show (ostream& out) const {
   shape::show (out);
   out << "{" << vertices << "}";
}

ostream& operator<< (ostream& out, const shape& obj) {
   obj.show (out);
   return out;
}

