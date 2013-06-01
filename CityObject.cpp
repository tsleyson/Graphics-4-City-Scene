#include "CityObject.h"
#include <cassert>
#include <iostream>
#include <math.h>
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


  float* CityObject::flatten_coords()
  {
    vector<float>* glCoords = new vector<float>();
    for (unsigned int i = 0; i < this->coordinates.size(); i++)
    {
        vec3 point = this->coordinates.at(i);
        assert(!(isnan(point[0]) || isnan(point[1]) || isnan(point[2])));
        glCoords->push_back(point[0]);
        glCoords->push_back(point[1]);
        glCoords->push_back(point[2]);
    }
    return glCoords->data();
  }
  
/* Building class's functions */
  void Building::push_line(vec3 first, float hinc, unsigned int height, 
    vector<vector<vec3> >& main)
  {
    vector<vec3> line;
    line.push_back(first);
    for (unsigned int i = 0; i < height; i++)
        line.push_back(vec3(first[0], first[1]+i*hinc, first[2]));
    main.push_back(line);
  }
  
  void Building::build_mesh(vector<vector<vec3> >& starters)
  {
      /* The starters vector contains four lines; each line is a
       * corner of a building with a square base. This function
       * builds a set of points for a triangle strip mesh, using
       * degenerate triangles to stitch everything together; it
       * also saves the points that make up an individual triangle
       * so we can calculate an averaged normal for each vertex and
       * do Gouraud shading.
       */
    for (int i = 1; i < 4; ++i)
    {
        vector<vec3>::iterator left = starters.at(i-1).begin();
        vector<vec3>::iterator right = starters.at(i).begin();
        assert(starters.at(i-1).size() > 2 && starters.at(i).size() > 2);
        
        // For making degenerate triangles; duplicate first point of
        // new second curve.
        if (i > 1)
            this->coordinates.push_back(*right);
            
        // Push the first two points so that every point in the loop
        // forms a new triangle.
        this->coordinates.push_back(*left);
        this->coordinates.push_back(*right);
        ++left; ++right;
        this->coordinates.push_back(*left);
        this->coordinates.push_back(*right);
        ++left; ++right;
        for (; left != starters.at(i-1).end() && 
                right != starters.at(i).end();
                ++left, ++right)
        {
            // Add the point from the left line, then add the triangle 
            // formed by it and the previous two points. Do the same for
            // the right line.
            this->coordinates.push_back(*left);
            size_t size = coordinates.size();
            this->triangles.push_back(Triangle(coordinates.at(size-3),
                coordinates.at(size-2), coordinates.at(size-1)));
            this->coordinates.push_back(*right);
            ++size;
            this->triangles.push_back(Triangle(coordinates.at(size-3),
                coordinates.at(size-2), coordinates.at(size-1)));
        }
        // We're finished with these two lines; duplicate last
        // point on second curve for degenerate triangles. By the way,
        // we don't bother adding degenerate triangles to the triangle
        // list since they have no area.
        size_t last = starters.at(i).size() - 1;
        this->coordinates.push_back(starters.at(i).at(last));
    }
  }
  
  void Building::calculate_normals()
  {
      
  }
  
  Building::Building(vec3 bottom_right, float side_length, size_t height)
  {
    // Remember: y = 0 is the ground. Assume that y = 0 in the top_left
    // vec. bottom_right is the bottom right point of the square that forms the
    // base of the building.
    float height_increment = height / side_length;
    vector<vector<vec3> > starters;
    this->push_line(bottom_right, side_length, height, starters);
    
    vec3 top_right = vec3(bottom_right[0], 0.0, bottom_right[2]+side_length);
    this->push_line(top_right, side_length, height, starters);
    
    vec3 top_left = vec3(top_right[0]-side_length, 0.0, top_right[2]);
    this->push_line(top_left, side_length, height, starters);
    
    vec3 bottom_left = vec3(bottom_right[0]-side_length, 0.0, 
        bottom_right[2]);
    this->push_line(bottom_left, side_length, height, starters);
    
    this->build_mesh(starters);
    /* debugging
    this->coordinates.push_back(vec3(-0.5, 0.5, 0.0));
    this->coordinates.push_back(vec3(-0.5, -0.5, 0.0));
    this->coordinates.push_back(vec3(0.5, -0.5, 0.0));*/
    // Initialize object coordinate matrix and vertex buffer that holds
    // vertices for this object.
    this->object_transform = mat4(1.0);
    glGenBuffers(1, &this->vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    float* glCoords = this->flatten_coords();
    // debugging
    for (int i = 0; i < this->coordinates.size()*3; ++i)
        cout << glCoords[i] << endl;
    glBufferData(GL_ARRAY_BUFFER, this->coordinates.size()*3*sizeof(float),
        glCoords, GL_STATIC_DRAW);/*
    glBufferData(GL_ARRAY_BUFFER, starters[0].size()*3*sizeof(float),
        starters[0].data(), GL_STATIC_DRAW);*/
    delete [] glCoords;
  }
  
  
  /* Expects shader program to be in use when called. */
  void Building::render(map<string, unsigned int>& attributes, 
            map<string, unsigned int>& uniforms)
  {
    glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer);
    assert(attributes.count("position"));    // Ensure position in the map.
    glEnableVertexAttribArray(attributes["position"]);
    glVertexAttribPointer(attributes["position"], 3, GL_FLOAT, GL_FALSE,
        0, 0);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->coordinates.size());
  }

}  // End namespace City.
