#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <string>
#include <GL/glew.h>
#include "OpenGLGeneral.h"
using std::vector;
using std::map;
using std::string;
using glm::vec3;
using glm::mat4;

#ifndef CITY_OBJ_H
#define CITY_OBJ_H
namespace City
{
    struct Triangle
    {
        vec3 first_point;
        vec3 second_point;
        vec3 third_point;
        vec3  standard_normal; // i.e., non-averaged.
      
      Triangle(vec3 p1, vec3 p2, vec3 p3) :
        first_point(p1), second_point(p2), third_point(p3) 
        {
            this->standard_normal = glm::cross(third_point - first_point,
                second_point - first_point);
        };
    
      bool contains(vec3 v) const
      {
        return this->first_point == v ||
                this->second_point == v ||
                this->third_point == v;
      }
    };

    class CityObject
    {
        protected:
        glm::mat4 object_transform;
        vector<glm::vec3> coordinates;  // The points defining the mesh.
        unsigned int vertex_buffer;
        
        float* flatten_coords();
        virtual void build_mesh(vector<vector<vec3> >&) = 0;
        virtual void calculate_normals() = 0;
            /* If it doesn't want to use anything nonlocal, just
             * make a local array and stick all the coordinates
             * in it upon rendering. In fact we can just store
             * everything as vec3s and then convert them before
             * rendering.
             */
        
        public:
        // Implement a constructor for the derived classes and make
        // it take some basic set of points (like for a cube, the
        // four corners of the bottom face and a height). Then have
        // the constructor call build_mesh to make a mesh out of those
        // points. Also initialize object_transform to the identity.
        void rotate(float xi, float gamma, float zeta);
        void translate(float byx, float byy, float byz);
        /* rotate and translate affect the object coordinates */
        
        virtual void render(map<string, unsigned int>&, 
            map<string, unsigned int>&) = 0;
    };
    
    class Building : public CityObject
    {
      private:
      void push_line(vec3 first, float hinc, unsigned int height, 
        vector<vector<vec3> >& main);
      unsigned int texture;
      vector<Triangle> triangles;
      
      protected:
      virtual void build_mesh(vector<vector<vec3> >& starters);
      virtual void calculate_normals();
      
      public:
      Building(vec3 top_left, float side_length, unsigned int height);
        // The base must be square. No rectangles.
      virtual void render(map<string, unsigned int>&, 
        map<string, unsigned int>&);
    };
}
#endif
