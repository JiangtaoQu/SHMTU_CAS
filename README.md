# SHMTU-CAS

上海海事大学统一认证平台(及学校微信平台)接口**研究**记录，从提出问题到解决问题，完整记录。

本项目为 `孔昊旻` 同学的一个课程设计，从头开始提出问题，分析问题，尝试解决问题，训练模型，部署模型到各种设备，**仅用于学习用途**，请勿用于非法用途。

## 项目介绍

## 项目组成

* Train: 基于PyTorch的深度学习训练及推理
* Deploy/SHMTU_CAS_OCR_Demo_Windows: 用于部署的Windows版本的GUI界面
* Deploy/SHMTU_CAS_OCR_Demo_Android: 用于部署的Android版本的GUI界面
* Deploy/QT: 用于部署的Qt版本(跨平台)的GUI界面
* Application/SHMTU-CAS-Server: 用于部署的模型推理服务器
* Application/SHMTU-CAS-Login: 基于自动识别验证码的登录流程模拟
* Homework/LaTeX: 全过程记录的LaTeX文档

项目部署时采用的推理框架为腾讯优图的[NCNN](https://github.com/Tencent/ncnn)
下面所有部署的核心代码均为`CAS_OCR.cpp`与`CAS_OCR.h`。

## 模型的训练

具体请参考:
[Train/README.md](Train/README.md

### 数据集

我训练模型所使用的数据集已经公开在下面的网址：

https://huggingface.co/datasets/a645162/shmtu_cas_validate_code

https://ai.gitee.com/datasets/a645162/shmtu_cas_validate_code

任选一个下载即可！

数据集的具体制作方式请参考LaTeX文档。

## Windows平台部署

包括3种在Windows平台的实现:

* WIN32 API
* . Net WPF
* Qt

第三方库：

- NCNN：C++推理框架
- OpenCV：C++图像处理库

## Android平台部署

使用Java+JNI编写了一个Module方便移植到其他项目。

识别Demo采用Kotlin编写。

## Qt跨平台解决方案

将Win32 API移植到Qt，实现了一个简单的GUI界面。
核心代码与VC++版本的实现一致。构建脚本采用CMake。

在Windows、Linux以及macOS平台上均可以正常运行。

## C++ 推理服务器

推理代码与VC++版本的实现一致。

TCP服务器使用Poco库实现，命令行解析使用Tclap，fmt用于格式化字符串。

构建脚本采用CMake，具体请参考对应目录中的Build.md。

在Windows、Linux以及macOS平台上均可以正常运行。

## Kotlin统一认证登录流程

使用Kotlin以及OkHttp实现。

模拟上海海事大学统一认证平台的登录流程，登录过程中，验证码使用C++推理服务器进行推理。

### 示例项目

[![smu_course_election_helper](https://github-readme-stats.vercel.app/api/pin/?username=EricHongXDD&repo=smu_course_election_helper)](https://github.com/EricHongXDD/smu_course_election_helper)
