# README

### Brief
This repo contains source code for my second OpenGL project. I hope to render a bobbing square at first, and then I'll aim to implement more.

### Helpful References
 1. Manually include OpenGL API function link code with [GLAD Setup](https://rpxomi.github.io/).
 2. Get started with GLFW and OpenGL with [OpenGL Tutorial](https://learnopengl.com/Getting-started/Creating-a-window)
 3. Dependency List:
    - GLFW from the GLFW site
    - OpenGL 3.3 (Core Profile) from GLAD

### Setup
 1. Ensure you have VSCode, Git, and GNU Make installed.
 2. Follow the first 3 chapters of the _OpenGL Tutorial_ in the 2nd reference above to manually setup the project dependencies: GLAD and GLFW.
   - On the GLAD file generation step, note that the service is down. Follow the red notice box carefully in the tutorial!
 3. Create the `bin`, `build`, `include`, and `lib` folders.
 4. Follow the 1st reference to manually organize the code into `include`. Then compile the GLFW and then the GLAD code into dynamic libraries. These should go into the `lib` folder.
 5. Run the command `chmod +x build.zsh` to allow running `build.zsh` if you have ZSH. If you use another shell, rename it to `build.sh` and add the same run permissions.
 6. Run the build script. This should prompt for a debug build or optimized build.
 7. Run `./bin/myglapp`. No arguments are needed for it to run.
