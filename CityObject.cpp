#include "CityObject.h"
#include <cassert>
#include <iostream>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
using std::cout;
using std::endl;

// Hold off on implementing these until there is actually a use for them.
// We don't want another situation like the accessors on the AST for
// the compilers semantic analyzer.
// Good thing I waited; GLM can do all of these things. Too bad about
// the frustum function, though; it can do that too. So if I need
// any of those things, just make these like accessors to the object's
// matrix that allow those transformations to be performed. Or just
// make the transformation matrix public.
namespace City
{
    void CityObject::rotate(float xi, float gamma, float zeta)
    {
        
    }
    
    void CityObject::translate(float byx, float byy, float byz)
    {
    
    }


  float* CityObject::flatten_coords(vector<vec3> coords)
  {
    vector<float>* glCoords = new vector<float>();
    for (unsigned int i = 0; i < coords.size(); i++)
    {
        vec3 point = this->coordinates.at(i);
        assert(!(isnan(point[0]) || isnan(point[1]) || isnan(point[2])));
        glCoords->push_back(point[0]);
        glCoords->push_back(point[1]);
        glCoords->push_back(point[2]);
    }
    return glCoords->data();
  }
  
  void CityObject::flatten_3darray(float array3d[][3][3], const size_t len)
  {
    for (int d1 = 0; d1 < len; ++d1)
    {
        for (int d2 = 0; d2 < 3; ++d2)
        {
            for (int d3 = 0; d3 < 3; ++d3)
            {
                this->float_coords.push_back(array3d[d1][d2][d3]);
            }
        }
    }
  }
  
/* Building class's functions */
  void Building::push_line(vec3 first, float hinc, unsigned int height, 
    vector<vector<vec3> >& main)
  {
    vector<vec3> line;
    line.push_back(first);
    for (unsigned int i = 0; i < height; i++)
    {
        //cout << i << endl;
        line.push_back(vec3(first[0], first[1]+i*hinc, first[2]));
    }
    assert(line.size() > 2);
    main.push_back(line);
    assert(main[main.size()-1].size() > 2);
  }
  
  /* The mesh built by this function is based on ideas from the web site
   * http://www.gamedev.net/topic/208950-help-vert-array-mesh-with-triangle_strip/
   */
  void Building::build_mesh(vector<vector<vec3> >& starters)
  {
      
  }
  
  void Building::calculate_normals()
  {
    // Calculates an averaged normal for each vertex by finding all
    // the triangles a vertex is part of and averaging their standard
    // normals.
    for (vector<vec3>::iterator ptit = this->coordinates.begin();
         ptit != this->coordinates.end();
         ++ptit)
    {
        vector<vec3> adj_normals;
        for (vector<Triangle>::iterator trit = this->triangles.begin();
             trit != this->triangles.end();
             ++trit)
        {
            if (trit->contains(*ptit))
                adj_normals.push_back(trit->standard_normal);
        }
        
        vec3 avgd = vec3(0.0, 0.0, 0.0);
        for (vector<vec3>::iterator nit = adj_normals.begin();
             nit != adj_normals.end();
             ++nit)
        {
            avgd += *nit;
        }
        avgd /= vec3(adj_normals.size(), adj_normals.size(), adj_normals.size());
        avgd = glm::normalize(avgd);
        this->average_normals.push_back(avgd);
    }
  }
  
  Building::Building(float vertices[][3][3], size_t len)
  {
    this->len3d = len;
    this->flatten_3darray(vertices, len);
    /* debugging 
    this->coordinates.push_back(vec3(-0.5, -0.5, 0.0));
    this->coordinates.push_back(vec3(-0.5, 0.5, 0.0));
    this->coordinates.push_back(vec3(0.5, -0.5, 0.0));
    this->coordinates.push_back(vec3(0.5, 0.5, 0.0));*/
    // Initialize object coordinate matrix and vertex buffer that holds
    // vertices for this object.
    this->object_transform = mat4(1.0);
    glGenBuffers(1, &this->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    //float* glCoords = this->flatten_coords(this->coordinates);
    // debugging
    for (int i = 0; i < float_coords.size(); ++i)
        cout << float_coords[i] << endl;
    // end debugging
    //float three[] = {0.0, 0.5, 0.0, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0};
    //float three[] = {0.0, 1200.0, 0.0, -1200.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    glBufferData(GL_ARRAY_BUFFER, float_coords.size()*sizeof(float), 
        float_coords.data(), GL_STATIC_DRAW);
  }
  
  
  /* Expects shader program to be in use when called. */
  void Building::render(map<string, unsigned int>& attributes, 
            map<string, unsigned int>& uniforms)
  {
    // Precondition on this method:
    assert(attributes.count("position") && attributes.count("normal"));

    // Send the normals into the vertex shader.
    /*glBindBuffer(GL_ARRAY_BUFFER, this->normal_buffer);
    glEnableVertexAttribArray(attributes["normal"]);
    glVertexAttribPointer(attributes["normal"], 3, GL_FLOAT, GL_TRUE,
        0, 0);  // Watch out for that GL_TRUE.*/
    // Send vertices into vertex shader and draw mesh.
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    glEnableVertexAttribArray(attributes["position"]);
    glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_TRUE,
        0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, float_coords.size()/3);
  }

}  // End namespace City.
