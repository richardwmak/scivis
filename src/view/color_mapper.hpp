#pragma once

class ColorMapper
{
  public:
    // various functions that visualize() uses to draw
    static void rainbow(float value, float RGB[3], int index = 0);
    static void red_to_white(float value, float RGB[3], int index = 0);
    static void direction_to_color(float x, float y, bool method);
    static void set_colormap(float vy);
};