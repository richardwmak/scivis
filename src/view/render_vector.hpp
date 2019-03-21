#pragma once
#include <FL/gl.h>
#include <vector>

typedef std::pair<GLfloat, GLfloat> coord;

class RenderVector
{
  public:
    static void render_vector(coord start, coord end);
    static void render_hedgehog(coord start, coord end);
    static void render_cone(coord start, coord end);
    static void render_arrow_2d(coord start, coord end);

    static coord rotate_2d(coord center, GLfloat x_vertex, GLfloat y_vertex, GLfloat angle);
};