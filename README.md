# Software Rasterizer

This project is a minimalistic software renderer developed to understand the fundamentals of 3D graphics and how modern graphics APIs like OpenGL operate under the hood. It was implemented following the [TinyRenderer tutorial](https://github.com/ssloy/tinyrenderer), which guides the creation of a simple renderer from scratch using approximately 500 lines of C++ code.

## Features

* **Triangle Rasterization**: Implements the core algorithm for drawing triangles on the screen.
* **Z-Buffering**: Adds depth testing to handle visibility of surfaces.
* **Phong Shading**: Applies Phong illumination model for realistic lighting.
* **Texture Mapping**: Supports applying textures to surfaces.
* **Perspective Projection**: Projects 3D vertices onto a 2D plane.
* **Model Loading**: Capable of loading and rendering 3D models in the `.obj` format.

This will generate a `output.tga` file containing the rendered image.

How to Build && Run
1. download microsoft visual c++ compiler
2. code/build.bat
3. code/run.bat
4. data/output.tga

## Result
[Final Rendered Image](./data/output.png)

## Learning Resources

* [TinyRenderer Tutorial](https://github.com/ssloy/tinyrenderer/wiki): Step-by-step lessons on implementing each component of the renderer.
