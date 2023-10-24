#ifndef SHADER_H
#define SHADER_H

        GLuint shaderProgram; // Shader program should be compiled and linked already
        GLuint VAO;           // Vertex Array Object
        GLuint VBO;           // Vertex Buffer Object
        GLuint EBO;           // Element Buffer Object

        GLuint CompileShader(const char* shaderSource, GLenum type) {
            GLuint shader = glCreateShader(type);
            glShaderSource(shader, 1, &shaderSource, NULL);
            glCompileShader(shader);
            GLint success;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(shader, 512, NULL, infoLog);
                printf("ERROR::SHADER::COMPILATION_FAILED\n%s\n", infoLog);
            }
            return shader;
        }

        char* LoadShader(const char* filepath) {
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

        GLuint LinkShaders(const char* vertexSource, const char* fragmentSource) {
            GLuint vertexShader = CompileShader(vertexSource, GL_VERTEX_SHADER);
            GLuint fragmentShader = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);
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
            }
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
            return program;
        }

        void InitializeOpenGL() {
            //// Load and compile shaders and link program
            //const char* vertexSource = LoadShader("./res/shaders/pixel.vert");
            //const char* fragmentSource = LoadShader("./res/shaders/pixel.frag");
            //shaderProgram = LinkShaders(vertexSource, fragmentSource);
            //free((void*)vertexSource);
            //free((void*)fragmentSource);
            //// Define vertices for a full-screen quad
            //GLfloat vertices[] = {
            //    // Positions    // Texture Coords
            //    1.0f,  1.0f,    1.0f, 1.0f,   // Top Right
            //    1.0f, -1.0f,    1.0f, 0.0f,   // Bottom Right
            //    -1.0f, -1.0f,   0.0f, 0.0f,   // Bottom Left
            //    -1.0f,  1.0f,   0.0f, 1.0f    // Top Left 
            //};
            //// Define indices for drawing the two triangles that make up the quad
            //unsigned int indices[] = {
            //    0, 1, 3,   // First Triangle
            //    1, 2, 3    // Second Triangle
            //};
            //// Generate and bind VAO
            //glGenVertexArrays(1, &VAO);
            //glBindVertexArray(VAO);
            //// Generate VBO, bind it, and upload vertices
            //glGenBuffers(1, &VBO);
            //glBindBuffer(GL_ARRAY_BUFFER, VBO);
            //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
            //// Generate EBO, bind it, and upload indices
            //glGenBuffers(1, &EBO);
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
            //// Set vertex attribute pointers for position and texture coordinates
            //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
            //glEnableVertexAttribArray(0);
            //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
            //glEnableVertexAttribArray(1);
            //// Unbind VAO
            //glBindVertexArray(0);
            //// Unbind VBO and EBO
            //glBindBuffer(GL_ARRAY_BUFFER, 0);
            //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            // Generate texture for frame buffer
            glGenTextures(1, &framebufferTexture);
            glBindTexture(GL_TEXTURE_2D, framebufferTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

#endif // SHADER_H
