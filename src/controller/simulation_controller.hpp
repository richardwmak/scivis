#pragma once

#include <utility>

class SimulationController
{
  public:
    std::pair<float, float> force_from_drag(int x_pixel_curr, int y_pixel_curr);
};