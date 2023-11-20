#ifndef SHADER_H
#define SHADER_H

        typedef struct {
            GLuint Program;
            const char* vertex;
            const char* fragment;
        } Shader;

        Shader shaderdefault; // Shader program default

        GLuint CompileShader(const char* shaderSource, GLenum type) {
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &shaderSource, NULL);
            glCompileShader(shader);
            GLint success;
            glValidateProgram(shader);
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
                glDeleteShader(shader);
                return 0;
            }
            return shader;
        }

        char* LoadShaderText(const char* filepath) {
            FILE* file = fopen(filepath, "rb");
            if (!file) {
                printf("Failed to open %s\n", filepath);
                return NULL;
            }
            fseek(file, 0, SEEK_END);
            long length = ftell(file);
            fseek(file, 0, SEEK_SET);
            char* buffer = (char*)malloc(length + 1);
            if (!buffer) {
                fclose(file);
                printf("Failed to allocate memory for %s\n", filepath);
                return NULL;
            }
            fread(buffer, 1, length, file);
            buffer[length] = '\0';
            fclose(file);
            return buffer;
        }

        GLuint LinkShaders(const char* vertex, const char* fragment) {
            GLuint vertexShader = CompileShader(vertex, GL_VERTEX_SHADER);
            GLuint fragmentShader = CompileShader(fragment, GL_FRAGMENT_SHADER);
            GLuint program = glCreateProgram();
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);
            GLint success;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetProgramInfoLog(program, 512, NULL, infoLog);
                printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
                glDeleteProgram(program);
                return 0;
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return program;
        }

        Shader LoadShader(const char* vertex, const char* fragment) {
            const char* fragmentsrc = fragment;
            const char* vertexsrc = vertex;
            if (FileExists(vertex)) {
                vertexsrc = LoadShaderText(vertex);
            }
            if (FileExists(fragment)) {
                fragmentsrc = LoadShaderText(fragment);
            }
            return (Shader){LinkShaders(vertexsrc, fragmentsrc), vertex, fragment};
        }

        time_t lastVertexShaderModTime = 0;
        time_t lastFragmentShaderModTime = 0;
        bool hotreloading = true;

        Shader HotReloading(Shader shader){
            if (hotreloading) {
                time_t currentVertexModTime = GetFileModificationTime(shader.vertex);
                time_t currentFragmentModTime = GetFileModificationTime(shader.fragment);
                if (currentVertexModTime != lastVertexShaderModTime || currentFragmentModTime != lastFragmentShaderModTime) {
                    shader = LoadShader(shader.vertex, shader.fragment);
                    lastVertexShaderModTime = currentVertexModTime;
                    lastFragmentShaderModTime = currentFragmentModTime;
                    
                }
            }
            return shader;
        }

        GLuint framebufferTexture;
        GLuint textTexture;
        GLuint VAO;           // Vertex Array Object
        GLuint VBO;           // Vertex Buffer Object
        GLuint PBO[2];        // Pixel Buffer Object
        int currentPboIndex = 0;

        void GenArrays(){
            // Generate VAO and VBO
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);
            // Bind VAO
                glBindVertexArray(VAO);
            // Bind VBO
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Allocate memory for VBO
                glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 24, NULL, GL_DYNAMIC_DRAW); // 6 vertices * 4 attributes per vertex (x, y, u, v)
            // Vertex positions attribute
                glEnableVertexAttribArray(0); // Matches aPos in your shader
                glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
            // Texture coordinates attribute
                glEnableVertexAttribArray(1); // Matches aTexCoords in your shader
                glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
            // Unbind VBO and VAO
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glBindVertexArray(0);
            // Generate PBOs for asynchronous data transfer
                glGenBuffers(2, PBO);
            // Initialize PBOs
                for (int i = 0; i < 2; ++i) {
                    glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO[i]);
                    // Allocate memory for the PBO
                    glBufferData(GL_PIXEL_UNPACK_BUFFER, SCREEN_WIDTH * SCREEN_HEIGHT * 4, NULL, GL_STREAM_DRAW);
                }
            // Unbind PBO
                glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
        }

        void CreateTextureWithPBO(unsigned char* framebuffer, GLuint framebufferTexture) {
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO[currentPboIndex]);
            glBufferData(GL_PIXEL_UNPACK_BUFFER, SCREEN_WIDTH * SCREEN_HEIGHT * 4, NULL, GL_STREAM_DRAW);
            GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
            if (ptr) {
                memcpy(ptr, framebuffer, SCREEN_WIDTH * SCREEN_HEIGHT * 4);
                glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
            }
            glBindTexture(GL_TEXTURE_2D, framebufferTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
            currentPboIndex = (currentPboIndex + 1) % 2;
        }
        
        void InitializeOpenGL() {
            // Generate Shader default
                shaderdefault = LoadShader("./res/shaders/pixel.vert","./res/shaders/pixel.frag");
                GenArrays();
            // Generate Texture for the TextRect 
                glGenTextures(1, &textTexture);
            // Generate texture for frame buffer
                glGenTextures(1, &framebufferTexture);
                glBindTexture(GL_TEXTURE_2D, framebufferTexture);
                glTexImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, 0);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
                glBindTexture(GL_TEXTURE_2D, 0);
        }

        void DeleteShaderProgram(Shader shader) {
            glDeleteProgram(shader.Program);
        }

#endif // SHADER_H
