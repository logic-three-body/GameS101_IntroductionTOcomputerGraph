# GAMES101->assignment7 Path Tracing

项目地址：[here](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/%E4%BD%9C%E4%B8%9A1/Path%20Tracing%20Ref)

## castRay

```c++
Vector3f Scene::castRay(const Ray &ray, int depth) const
{
	auto hitObj = intersect(ray);
	if (!hitObj.happened) return {};
    return shade(hitObj,-ray.direction);
}
```

## shade

```c++
Vector3f Scene::shade(Intersection& hit_obj, Vector3f wo) const
{
    if (hit_obj.m->hasEmission())
    {
        return hit_obj.m->getEmission();
    }
    const float epsilon = 0.0005f;
    // 直接光照贡献
    Vector3f Lo_dir;
    {
		float light_pdf;
		Intersection hit_light;
		sampleLight(hit_light, light_pdf);
		Vector3f obj2Light = hit_light.coords - hit_obj.coords;
        Vector3f obj2LightDir = obj2Light.normalized();
       
        // 检查光线是否被遮挡
        auto t = intersect(Ray(hit_obj.coords, obj2LightDir));
        if (t.distance - obj2Light.norm() > -epsilon)
        {
			Vector3f f_r = hit_obj.m->eval(obj2LightDir, wo, hit_obj.normal);
			float r2 = dotProduct(obj2Light, obj2Light);
            float cosA = std::max(.0f, dotProduct(hit_obj.normal,obj2LightDir));
            float cosB = std::max(.0f, dotProduct(hit_light.normal,-obj2LightDir));
			Lo_dir = hit_light.emit * f_r * cosA * cosB / r2 / light_pdf;
        }
    }

    // 间接光照贡献
    Vector3f Lo_indir;
    {
		if (get_random_float() < RussianRoulette)
		{
			Vector3f dir2NextObj = hit_obj.m->sample(wo, hit_obj.normal).normalized();
            float pdf = hit_obj.m->pdf(wo, dir2NextObj, hit_obj.normal);
            if (pdf > epsilon)
            {
                Intersection nextObj = intersect(Ray(hit_obj.coords, dir2NextObj));
				if (nextObj.happened && !nextObj.m->hasEmission())
				{
					Vector3f f_r = hit_obj.m->eval(dir2NextObj, wo, hit_obj.normal); //BRDF
					float cos = std::max(.0f, dotProduct(dir2NextObj, hit_obj.normal));
					Lo_indir = shade(nextObj, -dir2NextObj) * f_r * cos / pdf / RussianRoulette;
				}
            }
		}
    }
	//return Lo_dir;
   // return Lo_dir + Lo_indir;
}
```

## 多线程

```c++
void Renderer::Render(const Scene& scene,int spp)
{
//...other code
    
	//多线程
	int numThreads = std::thread::hardware_concurrency();
	int lines = scene.height / numThreads + 1;
    std::vector<std::thread> wokers;
	for (int i = 0; i < numThreads; i++)
	{
        int y0 = i * lines;
        int y1 = std::min(y0 + lines,scene.height);
        std::cout << "id:" <<i << " " << y0 << "=>" << y1 << std::endl;
        wokers.push_back(std::thread(render_thread,std::ref(framebuffer),std::ref(scene),spp,y0,y1));
	}
    for (int i =0;i<wokers.size();i++)
    {
        wokers[i].join();
    }
    
   //... other code 
}
```

```c++
void render_thread(std::vector<Vector3f>& framebuffer, const Scene& scene,int spp, int y0, int y1)
{
	float scale = tan(deg2rad(scene.fov * 0.5f));
	float imageAspectRatio = scene.width / (float)(scene.height);
	Vector3f eye_pos(278.0f, 273.0f, -800.0f);
	for (int i = y0; i < y1; i++)
	{
		for (int j = 0; j < scene.width; j++)
		{
            int index = i * scene.width + j;
			for (int k = 0; k < spp; k++)
			{
				float x = get_random_float();
				float y = get_random_float();
				float _x = (2.0f * (j + x) / (float)scene.width - 1) *
					imageAspectRatio * scale;
				float _y = (1.0f - 2.0f * ((float)i + y) / (float)(scene.height)) * scale;
				Vector3f dir = normalize(Vector3f(-_x, _y, 1.0f));
				Ray ray = Ray(eye_pos, dir);
				framebuffer[index] += scene.castRay(ray, 0) / (float)spp;
			}
		}
		g_mutex.lock();
		g_completeTotals++;
        UpdateProgress(g_completeTotals / (float)scene.height);
		g_mutex.unlock();
	}
}
```

## 结果图

### ssp=8

![image-20210816181311500](https://i.loli.net/2021/08/16/ShklgKtuHfrU8d4.png)

### ssp=50

![image-20210816181333539](https://i.loli.net/2021/08/16/WwmN1IJUnSMtQpg.png)

### ssp=64

![image-20210816181446113](https://i.loli.net/2021/08/16/khUlKTur3ZLIVMa.png)

![image-20210816181506410](https://i.loli.net/2021/08/16/xQdvWXqZKhSzjLi.png)

龙前左右加两盏灯：

![image-20210816181541563](https://i.loli.net/2021/08/16/i7vLEB6P2lVFqhd.png)

### ssp=200

![image-20210816181109872](https://i.loli.net/2021/08/16/3TN5c4jZXanhzpo.png)

### ssp=400

![image-20210816181358490](https://i.loli.net/2021/08/16/czELbyBnuiGQTIW.png)