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

The builds have been tested on VisualStudio 2019 with MSVC 14.2 and on Ubuntu 18.04LTS GNU C++ Compiler.

### Windows

#### Using VisualStudio
You can build the project on Windows directly using VisualStudio by opening the `raytracer.soln` file.

#### Using CMake
To build using CMake create a `build` directory in the root directory of the project. From the `build` directory execute the following command in `cmd`:
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

## Dependencies

- [assimp](https://github.com/assimp/assimp) for model loading.
- Intel's [OpenImageDenoise](https://github.com/OpenImageDenoise/oidn) for image denoising.
- OpenImageDenoise needs [TBB](https://github.com/oneapi-src/oneTBB) as a dependency

## Example Images

![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_monkey_scale_1.png)
![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_monkey_scale_2.png)
![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_dragon.png)
![](https://github.com/KarthikRIyer/RayTracer/blob/master/img_rest_of_your_life_denoised.png)
