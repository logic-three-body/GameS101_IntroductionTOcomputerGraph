#pragma once
//reference:https://github.com/ssloy/tinyrenderer/wiki/Lesson-1:-Bresenham%E2%80%99s-Line-Drawing-Algorithm

//draw line
void lineNaiveSegment(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);
void lineBresenhambad(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);
void lineBresenham(int x0, int y0, int x1, int y1, TGAImage&img, const TGAColor&color);