// $Id: interp.cpp,v 1.3 2019-03-19 16:18:22-07 - - $

#include <memory>
#include <string>
#include <vector>
using namespace std;

#include <GL/freeglut.h>

#include "debug.h"
#include "interp.h"
#include "shape.h"
#include "util.h"

unordered_map<string,interpreter::interpreterfn>
interpreter::interp_map {
   {"define" , &interpreter::do_define },
   {"draw"   , &interpreter::do_draw   },
   {"border" , &interpreter::do_border },
   {"moveby" , &interpreter::do_move   },
};

unordered_map<string,interpreter::factoryfn>
interpreter::factory_map {
   {"text"       , &interpreter::make_text       },
   {"ellipse"    , &interpreter::make_ellipse    },
   {"circle"     , &interpreter::make_circle     },
   {"polygon"    , &interpreter::make_polygon    },
   {"rectangle"  , &interpreter::make_rectangle  },
   {"square"     , &interpreter::make_square     },
   {"diamond"    , &interpreter::make_diamond    },
   {"triangle"   , &interpreter::make_triangle   },
   {"equilateral", &interpreter::make_equilateral},
};

interpreter::shape_map interpreter::objmap;

interpreter::~interpreter() {
   for (const auto& itor: objmap) {
      cout << "objmap[" << itor.first << "] = "
           << *itor.second << endl;
   }
}

void interpreter::interpret (const parameters& params) {
   DEBUGF ('i', params);
   param begin = params.cbegin();
   string command = *begin;
   auto itor = interp_map.find (command);
   if (itor == interp_map.end()) throw runtime_error ("syntax error");
   interpreterfn func = itor->second;
   func (++begin, params.cend());
}

void interpreter::do_define (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin < 3) throw runtime_error ("syntax error");
   string name = *begin;
   objmap.emplace (name, make_shape (++begin, end));
}


void interpreter::do_draw (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 4) throw runtime_error ("syntax error");
   string name = begin[1];
   shape_map::const_iterator itor = objmap.find (name);
   if (itor == objmap.end()) {
      throw runtime_error (name + ": no such shape");
   }
   rgbcolor color {begin[0]};
   vertex where {from_string<GLfloat> (begin[2]),
                 from_string<GLfloat> (begin[3])};
   window::push_back (object (itor->second, where, color));
}

void interpreter::do_border (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 2) throw runtime_error ("syntax error");

   // Assign the border properties
   shape::borderColor = rgbcolor(*begin++);
   shape::borderWidth = stof(*begin);
}

void interpreter::do_move (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 1) throw runtime_error ("syntax error");

   // Assign the specified pixels
   window::moveByPixels = stof(*begin);
}

shape_ptr interpreter::make_shape (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   string type = *begin++;
   auto itor = factory_map.find(type);
   if (itor == factory_map.end()) {
      throw runtime_error (type + ": no such shape");
   }
   factoryfn func = itor->second;
   return func (begin, end);
}

shape_ptr interpreter::make_text (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin < 2) throw runtime_error ("syntax error");
   string font = *begin++;

   // Check if font exists
   const auto itor = fontcode.find(font);
   if (itor == fontcode.end()) {
      throw runtime_error (font + ": no such font");
   }
   
   // Store the string data
   string data = "";
   string space = "";
   while (begin != end) {
      data += space + *begin++;
      space = " ";
   }

   return make_shared<text> (itor->second, data);
}

shape_ptr interpreter::make_ellipse (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 2) throw runtime_error ("syntax error");

   // Read the parameters
   GLfloat width = stof(*begin++) ;
   GLfloat height = stof(*begin) ;
   return make_shared<ellipse> (width, height);
}

shape_ptr interpreter::make_circle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 1) throw runtime_error ("syntax error");
   GLfloat diameter = stof(*begin) ;
   return make_shared<circle> (diameter);
}

shape_ptr interpreter::make_polygon (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin < 2) throw runtime_error ("syntax error");
   vertex_list vertices;

   // Store the vertices
   while (begin != end){
      GLfloat xPos = stof(*begin++);
      
      // Check if the y coordinate is not specified
      if (begin == end) throw runtime_error ("vertices mismatch");

      GLfloat yPos = stof(*begin++);

      // store and push the vertice to the vertex_list
      vertex vertice;
      vertice.xpos = xPos;
      vertice.ypos = yPos;
      vertices.push_back(vertice);
   }
   
   // To store the average of the coordinates
   float avgX = 0 ;
   float avgY = 0 ;

   // Calculate the averages
   for (auto itor = vertices.cbegin(); 
        itor != vertices.cend(); ++itor) {
      avgX += itor->xpos;
      avgY += itor->ypos;
   }
   avgX /= vertices.size();
   avgY /= vertices.size();

   // To normalize the vertices
   for (auto itor = vertices.begin(); 
        itor != vertices.end(); ++itor) {
      itor->xpos -= avgX;
      itor->ypos -= avgY;
   }

   return make_shared<polygon> (vertices);
}

shape_ptr interpreter::make_rectangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 2) throw runtime_error ("syntax error");
   GLfloat width = stof(*begin++) ;
   GLfloat height = stof(*begin) ;
   return make_shared<rectangle> (width, height);
}

shape_ptr interpreter::make_square (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 1) throw runtime_error ("syntax error");
   GLfloat width = stof(*begin) ;
   return make_shared<square> (width);
}

shape_ptr interpreter::make_diamond (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 2) throw runtime_error ("syntax error");
   GLfloat width = stof(*begin++) ;
   GLfloat height = stof(*begin) ;
   return make_shared<diamond> (width, height);
}

shape_ptr interpreter::make_triangle (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 6) throw runtime_error ("syntax error");
   vertex_list vertices;
   while (begin != end){
      GLfloat xPos = stof(*begin++);
      if (begin == end) throw runtime_error ("vertices mismatch");
      GLfloat yPos = stof(*begin++);
      vertex vertice;
      vertice.xpos = xPos;
      vertice.ypos = yPos;
      vertices.push_back(vertice);
   }

   float avgX = 0 ;
   float avgY = 0 ;

   for (auto itor = vertices.cbegin(); 
        itor != vertices.cend(); ++itor) {
      avgX += itor->xpos;
      avgY += itor->ypos;
   }
   avgX /= vertices.size();
   avgY /= vertices.size();

   for (auto itor = vertices.begin(); 
        itor != vertices.end(); ++itor) {
      itor->xpos -= avgX;
      itor->ypos -= avgY;
   }
   return make_shared<triangle> (vertices);
}

shape_ptr interpreter::make_equilateral (param begin, param end) {
   DEBUGF ('f', range (begin, end));
   if (end - begin != 1) throw runtime_error ("syntax error");
   GLfloat width = stof(*begin) ;
   return make_shared<equilateral> (width);
}
