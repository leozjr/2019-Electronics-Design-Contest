# 2019年全国大学生电子设计竞赛 无人机组 北京市第八名

目标任务：
    无人机从定点起飞，面前有一个模拟的电线杆，即两根柱子+一个电线。电线上某位置挂有一黄色异物，无人机需要巡检整条电线找到异物并拍摄异物图片。然后绕回降落至定点。

![(Docs\detect_pole.jpg)](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/detect_pole.jpg)

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/capture_tag.jpg)

## 方案简介

使用匿名科创拓空者Pro飞控作为主控，加入OPENMV摄像头模块，与飞控双工通信完成任务。

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/taskflow.jpg)

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/board_front.png)

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/board_back.png)

## 开发现场

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/debug2.jpg)

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/debug1.jpeg)

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/drone.jpg)

## 日志


20190801

实现：
- openmv正常通信
- 修改了循迹的速度、角速度、标定系数、转弯速度等参数，仍需完善
- 增加CAU_task_Ctrl文件，用来统一管理我们所有的飞行任务
- 测试了一键起飞和程序控制的函数，为后面的控制做了一些准备

bug:
- 上位机数传无法起飞，但是可以控制其他

20190803

实现：
- 修复数传起飞bug，更改openmv模式发送函数的位置到CAU_task_Ctrl()中，计划之后使用开关进行同步控制
- 加入Msp430.c文件，负责与430通信，接受430发来的开关数据及超声数据


20190807

实现：
- 加入2个OPMV,一个对地获取航向角和高度，另一个专注于拍照和巡线
- 大幅删减代码，移除无用的部分

## 证书

![](https://github.com/leozjr/2019-Electronics-Design-Contest/blob/master/Docs/certificate.jpg)
