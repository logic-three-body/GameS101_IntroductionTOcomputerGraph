#pragma once
//draw line
void lineNaiveSegment(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);
void lineInterpolateSegment(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);