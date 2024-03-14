# SHMTU-CAS

上海海事大学统一认证平台(及学校微信平台)接口**研究**记录，从提出问题到解决问题，完整记录。

本项目为`孔昊旻`同学的一个课程设计，从头开始提出问题，分析问题，尝试解决问题，训练模型，部署模型到各种设备，**仅用于学习用途**，请勿用于非法用途。

## 项目介绍


## 项目组成

- Train: 基于PyTorch的深度学习训练及推理
- Deploy/SHMTU_CAS_OCR_Demo_Windows: 用于部署的Windows版本的GUI界面
- Deploy/SHMTU_CAS_OCR_Demo_Android: 用于部署的Android版本的GUI界面
- Deploy/QT: 用于部署的Qt版本(跨平台)的GUI界面
- Application/SHMTU-CAS-Server: 用于部署的模型推理服务器
- Application/SHMTU-CAS-Login: 基于自动识别验证码的登录流程模拟
- Homework/LaTeX: 全过程记录的LaTeX文档

项目部署时采用的推理框架为腾讯优图的[NCNN](https://github.com/Tencent/ncnn)

## 模型的训练

具体请参考:
[Train/README.md](Train/README.md)

## Windows平台部署

包括3种在Windows平台的实现:

- WIN32 API
- .Net WPF
- Qt

## Android平台部署

## Qt跨平台解决方案

## C++ 推理服务器

## Kotlin统一认证登录流程

### 示例项目

[![smu_course_election_helper](https://github-readme-stats.vercel.app/api/pin/?username=EricHongXDD&repo=smu_course_election_helper)](https://github.com/EricHongXDD/smu_course_election_helper)
