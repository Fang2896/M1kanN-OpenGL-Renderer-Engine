# Mikann Renderer Engine

Author : [LIU Fang]([Fang2896 (Fang LIU) (github.com)](https://github.com/Fang2896))

---

## Introduction

Mikann Renderer Engine is a OpenGL based renderer engine for rendering algorithm showcases.



## Features

### Basic Features

* [x] Camera Movement
* [x] Using `Assimp` load model
* [ ] Skybox loading
* [ ] Scene Manger (using scene tree)

* [ ] Text Rendering




### Functions

* [x] Basic shapes
  (Cube, Sphere, Capsule, Plane, Quad and so on.)
* [x] Lightings and Objects transform configure system
* [x] Visualizing **Depth Buffer**
* [x] Using **Stencil Testing** to draw objects’ outlining
* [x] Transparent Object Rendering : Clipping and Blending 
  (draw a plane of grass and half transparent glass)
* [ ] Using Frame Buffer to renderer scene to a texture
  Then Post Processing
* [ ] High level GLSL (half-side rendering etc.)
* [ ] Using Geometry shader to visualize normal
* [ ] Anti Aliasing
* [ ] Occlusion Culling and Frustum Culling



### Rendering Features

* [x] Blinn-Phong lighting 
* [x] Basic Transparency Rendering and Sort
* [ ] Reflection and Refraction shader
* [ ] Gamma Correction
* [ ] Shadow
* [ ] Normal Mapping and Displacement Mapping
* [ ] HDR
* [ ] Blooming
* [ ] SSAO
* [ ] Deferred shading
* [ ] PBR


## Bugs
1. 透明度物体选择轮廓问题
2. 物体轮廓的深度测试问题
3. 部分多材质模型（nanosuit）选择后轮廓不显现问题（仅物体增大）
4. 【已解决】部分多材质模式（nanosuit）仅显示一半的部分贴图问题
   * 原因及其解决方法: 
     部分模型的uv会超过1来重复利用对称材质，因此texture wrap mode应该设为repeat
5. 【已解决】改变剔除模式后，模型会变成全黑
   * 原因及其解决方法:
     原因未知。解决方法：每次绘制的时候重新传入Material。


## References

[1] [Learn OpenGL, extensive tutorial resource for learning Modern OpenGL](https://learnopengl.com/)

[2] [Fundamentals of Computer Graphics, Fourth Edition (darmajaya.ac.id)](http://repo.darmajaya.ac.id/4242/1/Fundamentals of Computer Graphics%2C Fourth Edition ( PDFDrive ).pdf)

[3] [Real-Time Rendering Resources (realtimerendering.com)](https://www.realtimerendering.com/)

[4] [GPU Gems](https://developer.nvidia.com/gpugems/gpugems/contributors)

