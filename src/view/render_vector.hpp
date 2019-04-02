#pragma once
#include <FL/gl.h>
#include <rfftw.h>
#include <vector>

typedef std::pair<GLfloat, GLfloat> coord;

class RenderVector
{
  public:
    static void render_vector(std::vector<fftw_real> scalar_field,
                              std::vector<fftw_real> vector_field_x,
                              std::vector<fftw_real> vector_field_y,
                              GLfloat                height);
    static void controller(coord start, coord end, GLfloat height);
    static void render_hedgehog(coord start, coord end, GLfloat height);
    static void render_cone(coord start, coord end);
    static void render_arrow_2d(coord start, coord end, GLfloat height);

    static coord rotate_2d(coord center, GLfloat x_vertex, GLfloat y_vertex, GLfloat angle);
};