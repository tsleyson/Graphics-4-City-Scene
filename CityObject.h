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
{/*
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
    };*/

    class CityObject
    {
        protected:
        glm::mat4 object_transform;
        vector<glm::vec3> coordinates;  // The points defining the mesh.
        vector<float> float_coords, tex_coords;
        unsigned int vertex_buffer, normal_buffer;
        
        float* flatten_coords(vector<vec3> coords);
        void flatten_3darray(float a[][3][3], size_t);
        void build_mesh(vector<vector<vec3> >&);
        void calculate_normals();
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

          unsigned int len3d;
          /*
          vector<Triangle> triangles;
          vector<vec3> average_normals;*/

          CityObject(float a[][3][3], size_t len);
          ~CityObject() { glDeleteBuffers(1, &this->vertex_buffer); 
                         glDeleteBuffers(1, &this->normal_buffer); }
        void render(map<string, unsigned int>&, map<string, unsigned int>&);
    };
}
#endif
