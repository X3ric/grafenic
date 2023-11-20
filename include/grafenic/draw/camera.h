#ifndef CAMERA_H
#define CAMERA_H

        typedef struct {
            float x;
            float y;
            float z;
            GLfloat angle;
        } Camera;

        Camera camera = {0.0f, 0.0f , 0.0f, (GLfloat)0};

        void ScreenOrthoCam(float x, float y, float width, float height, GLfloat angle,Shader shader) {
            shader = HotReloading(shader);//regenerate shader on file change
            GLdouble left = 0;
            GLdouble right = SCREEN_WIDTH;
            GLdouble bottom = SCREEN_HEIGHT;
            GLdouble top = 0;
            GLdouble zNear = -1;
            GLdouble zFar = 1;
            GLfloat angleInDegrees = camera.angle + angle;
            GLfloat angleInRadians = angleInDegrees * (M_PI / 180.0f);
            // Calculate the center of the quad for rotation
            x += camera.x;
            y += camera.y;
            float centerX = x + width / 2.0f;
            float centerY = y + height / 2.0f;
            // Calculate sine and cosine of the rotation angle
            GLfloat cosAngle = cos(angleInRadians);
            GLfloat sinAngle = sin(angleInRadians);
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
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            // Use the shader program
            glUseProgram(shader.Program);
            // Set the orthographic matrix as a uniform
            GLint orthoMatrixLoc = glGetUniformLocation(shader.Program, "viewMatrix");
            glUniformMatrix4fv(orthoMatrixLoc, 1, GL_FALSE, orthoMatrix);
            // zoom
            GLint zoomLoc = glGetUniformLocation(shader.Program, "z");
            glUniform1f(zoomLoc, camera.z); // Set the desired zoom factor
            // Set the time uniform
            GLint timeLoc = glGetUniformLocation(shader.Program, "iTime");
            glUniform1f(timeLoc, glfwGetTime());
            // Set resolution vector2
            GLint resLoc = glGetUniformLocation(shader.Program, "iResolution");
            glUniform2f(resLoc, (float)width, (float)height);
            // Set mouse vector2
            GLint msLoc = glGetUniformLocation(shader.Program, "iMouse");
            glUniform2f(msLoc, mouse.x, mouse.y);
            // Draw the quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // Unbind VAO and shader program
            glBindVertexArray(0);
            glUseProgram(0);
        }

#endif // CAMERA_H
