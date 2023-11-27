#ifndef CAMERA_H
#define CAMERA_H

        typedef struct {
            float x;
            float y;
            float z;
            GLfloat angle;
        } Camera;

        Camera camera = {0.0f, 0.0f , 0.0f, (GLfloat)0};

        void RenderShaderOrtho(Shader shader,GLfloat vertices[],GLfloat verticessize,int verts){
            shader = HotReloading(shader);//regenerate shader on file change
            GLdouble left = 0;
            GLdouble right = SCREEN_WIDTH;
            GLdouble bottom = SCREEN_HEIGHT;
            GLdouble top = 0;
            GLdouble zNear = -1;
            GLdouble zFar = 1;
            GLfloat orthoMatrix[16] = {
                2.0f / (right - left), 0.0f , 0.0f, 0.0f,
                0.0f, 2.0f / (top - bottom), 0.0f , 0.0f,
                0.0f, 0.0f, -2.0f / (zFar - zNear) , 0.0f ,
                -(right + left) / (right - left) , -(top + bottom) / (top - bottom), -(zFar + zNear) / (zFar - zNear), 1.0f
            };
            if(debug.wiredframe){glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);}else{glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);}
            // Bind VAO
            glBindVertexArray(VAO);
            // Bind VBO and update with new vertex data
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, verticessize, vertices);
            // Use the shader program
            glUseProgram(shader.Program);
                // Set uniforms
                    GLumatrix4fv(shader,orthoMatrix,"viewMatrix");
                    GLuint1f(shader,camera.z,"z");
                    GLuint1f(shader,glfwGetTime(),"iTime");
                    GLuint2f(shader,SCREEN_WIDTH,SCREEN_HEIGHT,"iResolution");
                    GLuint2f(shader,mouse.x, mouse.y,"iMouse");
            // Draw the quad
            glDrawArrays(GL_TRIANGLES, 0, verts);
            // Unbind VAO and shader program
            glBindVertexArray(0);
            glUseProgram(0);
        }

        void ScreenCam(float x, float y, float width, float height, GLfloat angle,Shader shader) {
            shader = HotReloading(shader);//regenerate shader on file change
            // Calculate angle
            GLfloat angleInDegrees = camera.angle + angle;
            GLfloat angleInRadians = angleInDegrees * (M_PI / 180.0f);
            GLfloat cosAngle = cos(angleInRadians);
            GLfloat sinAngle = sin(angleInRadians);
            //Camera Movement
                x += camera.x;
                y += camera.y;
            // Calculate the center of the quad for rotation
            float centerX = x + width / 2.0f;
            float centerY = y + height / 2.0f;
            // Define the rotated vertices directly in the vertices array
            GLfloat vertices[] = {
                // First Triangle
                (x - centerX) * cosAngle - (y + height - centerY) * sinAngle + centerX, (x - centerX) * sinAngle + (y + height - centerY) * cosAngle + centerY, 0.0f, 0.0f,  // Bottom Left
                (x + width - centerX) * cosAngle - (y - centerY) * sinAngle + centerX, (x + width - centerX) * sinAngle + (y - centerY) * cosAngle + centerY, 1.0f, 1.0f,  // Top Right
                (x - centerX) * cosAngle - (y - centerY) * sinAngle + centerX, (x - centerX) * sinAngle + (y - centerY) * cosAngle + centerY, 0.0f, 1.0f,  // Top Left
                // Second Triangle
                (x - centerX) * cosAngle - (y + height - centerY) * sinAngle + centerX, (x - centerX) * sinAngle + (y + height - centerY) * cosAngle + centerY, 0.0f, 0.0f,  // Bottom Left
                (x + width - centerX) * cosAngle - (y + height - centerY) * sinAngle + centerX, (x + width - centerX) * sinAngle + (y + height - centerY) * cosAngle + centerY, 1.0f, 0.0f,  // Bottom Right
                (x + width - centerX) * cosAngle - (y - centerY) * sinAngle + centerX, (x + width - centerX) * sinAngle + (y - centerY) * cosAngle + centerY, 1.0f, 1.0f,  // Top Right
            };
            RenderShaderOrtho(shader,vertices,sizeof(vertices),6);
        }

        void Triangle(Shader shader, GLfloat angle, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3) {
            shader = HotReloading(shader);
            GLfloat angleInDegrees = camera.angle + angle;
            GLfloat angleInRadians = angleInDegrees * (M_PI / 180.0f);
            GLfloat cosAngle = cos(angleInRadians);
            GLfloat sinAngle = sin(angleInRadians);
            //Camera Movement
                x1 += camera.x;
                y1 += camera.y;
                x2 += camera.x;
                y2 += camera.y;
                x3 += camera.x;
                y3 += camera.y;
            // Calculate the center of the tinagle for rotation
            float centerX = (x1 + x2 + x3) / 3.0f;
            float centerY = (y1 + y2 + y3) / 3.0f;
            GLfloat rotatedX1 = cosAngle * (x1 - centerX) - sinAngle * (y1 - centerY) + centerX;
            GLfloat rotatedY1 = sinAngle * (x1 - centerX) + cosAngle * (y1 - centerY) + centerY;
            GLfloat rotatedX2 = cosAngle * (x2 - centerX) - sinAngle * (y2 - centerY) + centerX;
            GLfloat rotatedY2 = sinAngle * (x2 - centerX) + cosAngle * (y2 - centerY) + centerY;
            GLfloat rotatedX3 = cosAngle * (x3 - centerX) - sinAngle * (y3 - centerY) + centerX;
            GLfloat rotatedY3 = sinAngle * (x3 - centerX) + cosAngle * (y3 - centerY) + centerY;
            GLfloat vertices[] = {
                // Positions       // Texture Coords 
                rotatedX1, rotatedY1, 0.0f, 0.0f,  // Vertex 1
                rotatedX2, rotatedY2, 1.0f, 0.0f,  // Vertex 2
                rotatedX3, rotatedY3, 0.5f, 1.0f,  // Vertex 3
            };
            RenderShaderOrtho(shader, vertices, sizeof(vertices), 3);
        }

#endif // CAMERA_H
