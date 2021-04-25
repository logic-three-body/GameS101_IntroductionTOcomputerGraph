#pragma once
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
//const char* path= "obj/african_head/african_head.obj";
//#define path "obj/african_head/african_head.obj"
const auto path = "obj/african_head/african_head.obj";
const int width = 800;
const int height = 800;
static Model model(path);
static TGAImage image(width, height, TGAImage::RGB);