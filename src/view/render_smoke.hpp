#pragma once

#include <Fl/gl.h>
#include <rfftw.h>
#include <vector>

class RenderSmoke
{
  public:
    static void render_smoke(std::vector<fftw_real> scalar_field, GLfloat height);
};