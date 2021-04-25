#include"FrameWork.h"

int main(int argc, char** argv) {
	///const char*new_path = "obj/diablo3_pose/diablo3_pose.obj";
	///model.clear();
	///model.Update(new_path);
	rasterizer r(800,800);
	//DrawWireframe(model);
	r.DrawWireFrame(model,image,green);
	image.write_tga_file("Lesson1Line/Wireframe/attempt/rasterizer/output.tga");
	return 0;
}