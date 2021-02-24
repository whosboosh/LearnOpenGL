#!/bin/bash

g++ -x c++ ./src/glad.c ./src/main.cpp ./src/Camera.cpp ./src/DirectionalLight.cpp ./src/Light.cpp ./src/Material.cpp ./src/Mesh.cpp ./src/PointLight.cpp ./src/Shader.cpp ./src/ShadowMap.cpp ./src/Texture.cpp ./src/Window.cpp -Iinclude -lGL -lGLU -lglfw3 -lX11 -lXxf86vm -lXrandr -pthread -lXi -ldl -lXinerama -lXcursor 
