#pragma once
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const auto path = "obj/african_head/african_head.obj";
const int width = 800;
const int height = 800;
const int depth = 255;
Vec3f light_dir(0,0,-1);
Vec3f camera(0, 0, 3);
static Model model(path);
static TGAImage image(width, height, TGAImage::GRAYSCALE);