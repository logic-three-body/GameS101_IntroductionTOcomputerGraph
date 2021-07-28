# GAMES101->assignment8 质点弹簧系统

项目地址：

## 1.1连接绳子约束

![image-20210728165322003](https://i.loli.net/2021/07/28/i59bzfp6V8gkeNd.png)

## 1.2.1显式欧拉法 
非常**不稳定**

1024 step per frame

![显式欧拉](https://i.loli.net/2021/07/28/XZBgjWxQkmwYlKi.gif)

4096 step per frame

![显式欧拉4096](https://i.loli.net/2021/07/28/CpUY7AiJ81DkB4w.gif)

8192 step per frame

![显式欧拉8192](https://i.loli.net/2021/07/28/Viyho4fBecwZPKF.gif)

加入阻力(即使步长很小，显式欧拉系统最终还是会崩溃)：

![显式欧拉8192S](https://i.loli.net/2021/07/28/CztguFGwT5YSEUD.gif)

## 1.2.2半隐式欧拉法

8192 step per frame

 ![隐式欧拉8192](https://i.loli.net/2021/07/28/GstOJi6Be8gTlpY.gif)

加入阻力后：

![隐式欧拉8192S](https://i.loli.net/2021/07/28/UYJkxsAQv6ztW57.gif)

## 1.3 显式 Verlet

8192 step per frame

![显式 Verlet8192](https://i.loli.net/2021/07/28/eOFE7JDBzLtnKxr.gif)
## 1.4 阻尼
512 step per frame

![显式 Verlet512](https://i.loli.net/2021/07/28/IGVQlqEmRuUBjHT.gif)

## 综合



![综合](https://i.loli.net/2021/07/28/BHnfVDSxq9eEGZ8.gif)