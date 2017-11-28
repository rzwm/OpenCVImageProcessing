# 一、简介

实现CSDN专栏：[OpenCV图像处理](http://blog.csdn.net/column/details/opencv-image.html?&page=3)所介绍的所有算法。

当前已实现(0/50)：

# 二、开发环境

Win10 + Visual Studio Community 2017 + OpenCV 3.3.0(64位)

# 三、编译方法

1. 使用Visual Studio 2017打开“OpenCVImageProcessing.sln”；
2. 打开“视图->其他窗口->属性管理器”；
3. 任选一项目，在“OpenCV330_x64_Debug”上右击，然后点击“属性”；
4. 然后分别在“C/C++->常规->附加包含目录”和“链接器->常规->附加库目录”中添加你自己的OpenCV头文件目录和库目录；
5. 任选一项目，在“OpenCV330_x64_Release”上右击，然后点击“属性”；
6. 然后分别在“C/C++->常规->附加包含目录”和“链接器->常规->附加库目录”中添加你自己的OpenCV头文件目录和库目录；
7. 回到Visual Studio 2017主界面，更改解决方案平台为“x64”；
8. 点击“生成->生成解决方案”。