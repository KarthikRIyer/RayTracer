# RayTracer

This is a simple ray tracer based on Peter Shirley's RayTracing in One Weekend series and the book PBRT.

Features currently implemented
- [x] Analytical Sphere, Rectangle and Triangle Geometry
- [x] Mesh loading using assimp
- [x] Simple BVH
- [x] Gradient and Black Sky
- [x] Lambertial, Metal, Dielectric and Emission materials
- [x] Random and Stratified sampling
- [x] Importance sampling materials
- [x] Solid, Checker and Image Textures
- [x] Multithreaded Rendering
- [x] Integrated Intel's OpenImageDenoise

## Building

The builds have been tested on Windows with MSVC 14.2 and on Ubuntu 18.04LTS GNU C++ Compiler.

### Windows

To build install CMake create a `build` directory in the root directory of the project. From the `build` directory execute the following command in `cmd`:
<br>
```console
cmake -DCMAKE_BUILD_TYPE=Release ..
```
<br>

You should now get a file `RayTracer.soln` in the `build` directory. Open the file and build.

To test the project, from the root directory of the project run:
<br>

```console
Release/RayTracer.exe
```
or
```console
Release/viewer.exe
```

### Linux

You can use CMake to build on Linux.

```console
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

To run (from root directory of project):

```console
./Release/RayTracer
```
or
```console
./Release/viewer
```

## Dependencies

- [assimp](https://github.com/assimp/assimp) for model loading.
- Intel's [OpenImageDenoise](https://github.com/OpenImageDenoise/oidn) for image denoising.
- OpenImageDenoise needs [TBB](https://github.com/oneapi-src/oneTBB) as a dependency.
- [PCG Random Number Generator](https://github.com/wjakob/pcg32) by [Wenzel Jakob](https://github.com/wjakob).
- [JSON for Modern C++](https://github.com/nlohmann/json) by [Niels Lohmann](https://github.com/nlohmann) for scene file format.
- [GLFW](https://www.glfw.org/) and [GLAD](https://glad.dav1d.de/) for OpenGL.
- [Dear ImGui](https://github.com/ocornut/imgui) for Immediate Mode GUI to view the rendered image.

## Example Images

![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_monkey_scale_1.png)
![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_monkey_scale_2.png)
![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_dragon.png)
![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_rest_of_your_life_denoised.png)
