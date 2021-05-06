#pragma once
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue = TGAColor(0, 0, 255, 255);
const auto path = "obj/african_head/african_head.obj";
const auto path1 = "obj/shark.obj";
const auto path2 = "obj/Jelle.obj";
const auto path3 = "obj/floor.obj";
const int width = 800;
const int height = 800;
const int depth = 255;
Vec3f light_dir(0,0,-1);
Vec3f eye(0,0,0);
//Vec3f eye(-4.074089, 0.647881, -0.122805);
Vec3f center(0.1, 0.1, 0.1);
//Vec3f center(-3.798896, 1.250536, 0.070385);
static Model model(path2);
static TGAImage image(width, height, TGAImage::GRAYSCALE);