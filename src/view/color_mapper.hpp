#pragma once

class ColorMapper
{
  public:
    // various functions that visualize() uses to draw
    static void greyscale(float value, float RGB[3], int index = 0);
    static void rainbow(float value, float RGB[3], int index = 0);
    static void red_white(float value, float RGB[3], int index = 0);
    static void set_colormap(float value, float RGB[3], int index = 0);
    static void direction_to_color(float RGB[3], float x, float y, int index = 0);
};