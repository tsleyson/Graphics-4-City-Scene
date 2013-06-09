/* path_points.hpp */


const size_t path_points_length = 15;
float path_points[][3] = 
{
    {-230, 0, 0}
    {-230, 0, 800}
    // Starting to turn. 850 comes just to the intersection edge.
    {-210, 0, 900}
    {-150, 0, 950}
    {-120, 0, 950}
    /* control points for first Bezier curve.*/
    {950, 0, 950}
    {1000, 0, 900}
    {1000, 0, -395}
    {950, 0, -450}
    /* controls for second Bezier curve. */
    {-170, 0, -450}
    {-230, 0, -400}
    {-230, 0, 0}
    /* controls for third Bezier curve. */
};
