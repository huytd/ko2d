ko2d
====

Cross-platform OpenGL ES 2 Game Framework

Ko2D is a cross-platform game engine writen in C/C++ using OpenGL ES 2. 
It's name is 2D but it can do anything including 3D games.
There are 2 template for Windows and Android version located in Build folder.
For Android version, build using NDK and ANT. 
Then copy Resource folder to /mnt/sdcard/KO2D/Resource unless you wanna see how fun is it when the game got crash.

It's not a completed engine but it has some extremely-basic and cool feature for you to make games.
Ko2D has some built-in feature:
- 3D Math struct (Vector2, Vector3, Vector4, Matrix)
- File System
- Game State Manager
- Camera
- Input Manager
- 3D Model Loader (OBJ files)
- Texture Loader (TGA files)
- Shader Loader
- Resources Manager
- Particle System
- 2D Drawing system 
	+ 3D Billboard
	+ Draw sprite sheet
	+ Draw static 2D elements (HUD,...)
	+ Bitmap Font rendering