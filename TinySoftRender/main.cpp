#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include"line.h"
#include"global.h"
#include"Value.h"


int main(int argc, char** argv) {
	const char*new_path = "obj/diablo3_pose/diablo3_pose.obj";
	model.clear();
	model.Update(new_path);
	DrawWireframe(model);
	image.write_tga_file("Lesson1Line/Wireframe/attempt/output.tga");
	return 0;
}