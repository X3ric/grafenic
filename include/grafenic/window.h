#ifndef WINDOW_H
#define WINDOW_H

    const char* TITLE = "\0";
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;
    int WIDTH;
    int HEIGHT;

    GLFWwindow* window;

    GLubyte* framebuffer;

    double frametime;
    int fpslimit;
    double fps;

    #include <stdbool.h>
    typedef struct {
        bool input;
        bool wiredframe;
        bool fps;
    } Debug;
    Debug debug;

    #include <grafenic/input.h>

    #include <grafenic/utils.h>

    #include <grafenic/audio.h>

    bool pixels = false;

    #include <grafenic/draw/init.h>

    bool cursor = true;
    bool oldcursor = true;

    bool vsync = false;
    bool oldvsync = false;

    bool fullscreen = false;
    bool oldfullscreen = false;

    bool visible = true;
    bool oldvisible = true;

    bool floating = true;
    bool transparent = false;
    bool decorated = true;

    void WindowFrames(){
        static double previousframetime = 0.0;
        double targetTime = 1.0 / fpslimit;
        frametime = glfwGetTime();
        double elapsedTime = frametime - previousframetime;
        if (fpslimit != 0) {
            while (elapsedTime < targetTime) {
                frametime = glfwGetTime();
                elapsedTime = frametime - previousframetime;
            }
        }
        if (elapsedTime != 0) {
            fps = 1.0 / elapsedTime;
        } else {
            fps = 0.0;
        }
        previousframetime = frametime;
        if(debug.fps){
            //ClearOutput();
            print("FPS: %.0f\n",fps);
        }
    }

    void WindowClear() {
        if(framebuffer){free(framebuffer);}
        if(pixels){framebuffer = (GLubyte*)calloc(SCREEN_WIDTH * SCREEN_HEIGHT, 4 * sizeof(GLubyte));}
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        WindowFrames();
    }
    
    void WindowProcess() {
        MouseInit();
        if (fullscreen != oldfullscreen) {
            if (fullscreen) {
                glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, SCREEN_WIDTH, SCREEN_WIDTH, 0);
            } else {
                glfwSetWindowMonitor(window, NULL, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
            }
            oldfullscreen = fullscreen;
        }
        if (cursor != oldcursor) {
            if (cursor) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            }
            oldcursor = cursor;
        }
        if (vsync != oldvsync) {
            if (vsync) {
                glfwSwapInterval(1);
            } else {
                glfwSwapInterval(0);
            }
            oldvsync = vsync;
        } 
        if (visible != oldvisible) {
            if (visible) {
                glfwShowWindow(window);
            } else {
                glfwHideWindow(window);
            }
            oldvisible = visible;
        } 
        if(framebuffer){DrawPixels(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);}
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    const int MIN_PIXEL = 1;
    void window_buffersize_callback(GLFWwindow* window, int width, int height)
    {
        SCREEN_WIDTH = width < MIN_PIXEL ? MIN_PIXEL : width;
        SCREEN_HEIGHT = height < MIN_PIXEL ? MIN_PIXEL : height;
        if(!pixels){glfwSetWindowAspectRatio(window, SCREEN_WIDTH, SCREEN_HEIGHT);}
        glViewport(0, 0, (GLsizei)SCREEN_WIDTH, (GLsizei)SCREEN_HEIGHT);
    }

    void update(void);
    void window_refresh_callback(GLFWwindow* window)
    {
        WindowClear();
        update();
        WindowProcess();   
    }

    int WindowInit(int width, int height, const char* title)
    {
        TITLE = title;
        WIDTH = width;
        HEIGHT = height;
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
        glfwSetErrorCallback(ErrorCallback);
        if (!glfwInit()) {
            printf("Failed to initialize GLFW\n");
            return -1;
        }
        //glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
        //glEnable(GL_MULTISAMPLE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        if (transparent) {
            glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GL_TRUE); 
        }
        if (!decorated) {
            glfwWindowHint(GLFW_DECORATED, GL_FALSE); 
        }
        if (floating) {
            glfwWindowHint(GLFW_RESIZABLE , GLFW_FALSE);
        } else {
            glfwWindowHint(GLFW_RESIZABLE , GLFW_TRUE);
        }
        //glfwWindowHint(GLFW_REFRESH_RATE, 0);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);//forced new opengl version
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, title, NULL, NULL);
        if (!window) {
            printf("Failed to open GLFW window.\n");
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        glfwSetCharCallback(window, CharCallback);
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetFramebufferSizeCallback(window, window_buffersize_callback);
        glfwSetWindowRefreshCallback(window, window_refresh_callback);
        printf("Renderer: %s\n", glGetString(GL_RENDERER));
	    printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
        //glewExperimental=true;
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            fprintf(stderr, "Error initializing GLEW: %s\n", glewGetErrorString(err));
        }
        if (vsync) {
            glfwSwapInterval(1);
        } else {
            glfwSwapInterval(0);
        }
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);

        //glEnable(GL_DEPTH_TEST);//depth
        //glDepthFunc(GL_LEQUAL);
        //glDisable(GL_CULL_FACE);
        //glCullFace(GL_BACK);

        InitializeOpenGL();

        print("Loaded\n");
        return 0;
    }

    int WindowState() {
        return glfwWindowShouldClose(window);
    }

    void WindowStateSet(bool state) {
        if (state) {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        } else  {
            glfwSetWindowShouldClose(window, GLFW_FALSE);            
        }
    }

    void WindowClose()
    {
        glfwSetKeyCallback(window, NULL);
        print("Exit\n");
        AudioStop();
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(2, PBO);
        glDeleteProgram(shaderdefault.Program);
        stbi_image_free(img.data);
        free(framebuffer);
        framebuffer = NULL;
        glfwDestroyWindow(window);
        glfwTerminate();
    }

#endif // WINDOW_H
