# GAMES101->assignment8 质点弹簧系统

项目地址：[GameS101_IntroductionTOcomputerGraph/MassSpringSystem at 作业1 · logic-three-body/GameS101_IntroductionTOcomputerGraph (github.com)](https://github.com/logic-three-body/GameS101_IntroductionTOcomputerGraph/tree/作业1/MassSpringSystem)

## 1.1连接绳子约束

```c++
Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass, float k, vector<int> pinned_nodes)
{
    // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and containing `num_nodes` nodes.
    if (0 == num_nodes || 1 == num_nodes)
        return;
    Vector2D CurPos = start;
    //treat rope as a list
    Mass *p = new Mass(start, node_mass, false);
    masses.push_back(p);
    Mass *l = nullptr;
    Spring *presentSpring = nullptr;
    for (int i = 1; i <= num_nodes - 1; i++)
    {
        std::cout << i;
        if (i == num_nodes - 1)
        {
            CurPos = end;
        }
        else
        {
            CurPos = start + i * (end - start) / (num_nodes - 1);
        }
        l = p;
        p = new Mass(CurPos, node_mass, false);
        presentSpring = new Spring(l, p, k);
        masses.push_back(p);
        springs.push_back(presentSpring);
    }
    //        Comment-in this part when you implement the constructor
    for (auto &i : pinned_nodes)
    {
        masses[i]->pinned = true;
    }
}
```

![image-20210728165322003](https://i.loli.net/2021/07/28/i59bzfp6V8gkeNd.png)

## 1.2 欧拉法

```C++
void Rope::simulateEuler(float delta_t, Vector2D gravity)
{
    for (auto &s : springs)
    {
        // TODO (Part 2): Use Hooke's law to calculate the force on a node
        Vector2D ab = s->m2->position - s->m1->position;
        Vector2D f = s->k * (ab / ab.norm()) * (ab.norm() - s->rest_length);
        s->m1->forces += f;
        s->m2->forces -= f;
    }

    for (auto &m : masses)
    {
        if (!m->pinned)
        {
            // TODO (Part 2): Add the force due to gravity, then compute the new velocity and position
            m->forces += gravity * m->mass;

            // TODO (Part 2): Add global damping

            float k_d_global = 0.01;
            m->forces += -k_d_global * m->velocity;

            Vector2D a = m->forces / m->mass;

            //semi-implicit method
            m->velocity += a * delta_t;
            m->position += m->velocity * delta_t;

            //Explicit method
            //                m->position += m->velocity * delta_t;
            //                m->velocity += a * delta_t;
        }
        // Reset all forces on each mass
        m->forces = Vector2D(0, 0);
    }
}
```

### 1.2.1显式欧拉法 

非常**不稳定**

```c++
m->position += m->velocity * delta_t;
m->velocity += a * delta_t;
```

1024 step per frame

![显式欧拉](https://i.loli.net/2021/07/28/XZBgjWxQkmwYlKi.gif)

4096 step per frame

![显式欧拉4096](https://i.loli.net/2021/07/28/CpUY7AiJ81DkB4w.gif)

8192 step per frame

![显式欧拉8192](https://i.loli.net/2021/07/28/Viyho4fBecwZPKF.gif)

加入阻力(即使步长很小，显式欧拉系统最终还是会崩溃)：

```c++
float k_d_global = 0.01;
m->forces += -k_d_global * m->velocity;
```

![显式欧拉8192S](https://i.loli.net/2021/07/28/CztguFGwT5YSEUD.gif)

### 1.2.2半隐式欧拉法

```c++
m->velocity += a * delta_t;
m->position += m->velocity * delta_t;
```

8192 step per frame

 ![隐式欧拉8192](https://i.loli.net/2021/07/28/GstOJi6Be8gTlpY.gif)

加入阻力后：

![隐式欧拉8192S](https://i.loli.net/2021/07/28/UYJkxsAQv6ztW57.gif)

## 1.3 Verlet法

```C++
void Rope::simulateVerlet(float delta_t, Vector2D gravity)
{
    for (auto &s : springs)
    {
        // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet （solving constraints)
        Vector2D ab = s->m2->position - s->m1->position;
        Vector2D f = s->k * (ab / ab.norm()) * (ab.norm() - s->rest_length);
        s->m1->forces += f;
        s->m2->forces -= f;
    }

    for (auto &m : masses)
    {
        if (!m->pinned)
        {
            m->forces += gravity * m->mass;
            Vector2D a = m->forces / m->mass;
            // TODO (Part 3.1): Set the new position of the rope mass
            Vector2D lastposition = m->position;
            // TODO (Part 4): Add global Verlet damping
            float dampfactor = 0.00005;
            m->position = m->position + (1 - dampfactor) * (m->position - m->last_position) + a * delta_t * delta_t;
            //                m->position = m->position +  (m->position - m->last_position) + a * delta_t *delta_t;
            m->last_position = lastposition;
        }
        m->forces = Vector2D(0, 0);
    }
}
```

### 1.3 显式 Verlet

```c++
m->forces += gravity * m->mass;
Vector2D a = m->forces / m->mass;
// TODO (Part 3.1): Set the new position of the rope mass
Vector2D lastposition = m->position;
// TODO (Part 4): Add global Verlet damping
float dampfactor = 0.00005;
m->position = m->position + (m->position - m->last_position) + a * delta_t * delta_t;
m->last_position = lastposition;
```

8192 step per frame

![显式 Verlet8192](https://i.loli.net/2021/07/28/eOFE7JDBzLtnKxr.gif)
### 1.4 阻尼

```C++
m->forces += gravity * m->mass;
Vector2D a = m->forces / m->mass;
// TODO (Part 3.1): Set the new position of the rope mass
Vector2D lastposition = m->position;
// TODO (Part 4): Add global Verlet damping
float dampfactor = 0.00005;
m->position = m->position + (1 - dampfactor) * (m->position - m->last_position) + a * delta_t * delta_t;
m->last_position = lastposition;
```

512 step per frame

![显式 Verlet512](https://i.loli.net/2021/07/28/IGVQlqEmRuUBjHT.gif)

## 综合

配置：

```c++
AppConfig()
{
    // Rope config variables
    mass = 1;
    ks = 100;
    // Environment variables
    gravity = Vector2D(0, -1);
    //    steps_per_frame = 64;
    steps_per_frame = 256;
}
```

```c++
int node_num = 8;
ropeEuler = new Rope(Vector2D(0, 200), Vector2D(-400, 200), node_num * 2, config.mass,
                     config.ks, {0});
ropeVerlet = new Rope(Vector2D(0, 200), Vector2D(-400, 200), node_num, config.mass,
                      config.ks, {0});
```

![综合](https://i.loli.net/2021/07/28/BHnfVDSxq9eEGZ8.gif)