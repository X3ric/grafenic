typedef struct {
    float x;
    float y;
    float z;
    GLfloat angle;
} Camera;

Camera camera = {0.0f, 0.0f , 0.0f, (GLfloat)0.0f};

void RenderShaderOrtho(Shader shader, GLfloat vertices[], GLfloat vertSize, GLuint indices[], GLfloat indexSize, int count) {
    if(shader.hotreloading){ // regenerate shader on file change
        shader = ShaderHotReload(shader);
    }
    GLdouble left = 0;
    GLdouble right = SCREEN_WIDTH;
    GLdouble bottom = SCREEN_HEIGHT;
    GLdouble top = 0;
    GLdouble zNear = -1;
    GLdouble zFar = 1;
    GLfloat middle_x = SCREEN_WIDTH / 2;
    GLfloat middle_y = SCREEN_HEIGHT / 2;
    GLfloat pivot_x = camera.x;
    GLfloat pivot_y = camera.y;
    GLfloat zoom = camera.z;
    GLfloat orthoMatrix[16] = {
        2.0f * zoom / SCREEN_WIDTH, 0.0f, 0.0f, 0.0f,
        0.0f, -2.0f * zoom / SCREEN_HEIGHT, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f / (zFar - zNear), 0.0f,
        -(right + left) / SCREEN_WIDTH * zoom + pivot_x * 2.0f * zoom / SCREEN_WIDTH,
        (top + bottom) / SCREEN_HEIGHT * zoom - pivot_y * 2.0f * zoom / SCREEN_HEIGHT,
        -(zFar + zNear) / (zFar - zNear), 1.0f
    };
    if (debug.wireframe) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    // Bind VAO
        glBindVertexArray(VAO);
    // Bind VBO and update with new vertex data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, vertSize, vertices);
    // Bind EBO and update with new index data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indexSize, indices);
    // Use the shader program
        glUseProgram(shader.Program);
    // Set uniforms
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "viewMatrix"), 1, GL_FALSE, orthoMatrix);
        glUniform3f(glGetUniformLocation(shader.Program, "camera"), camera.x, camera.y, camera.z);
        glUniform1f(glGetUniformLocation(shader.Program, "angle"), camera.angle);
        glUniform1f(glGetUniformLocation(shader.Program, "iTime"), glfwGetTime());
        glUniform2f(glGetUniformLocation(shader.Program, "iResolution"), SCREEN_WIDTH, SCREEN_HEIGHT);
        glUniform2f(glGetUniformLocation(shader.Program, "iMouse"), mouse.x, mouse.y);
    // Draw using indices
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
    // Unbind shader program
        glUseProgram(0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Triangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat angle, Shader shader) {
    // Calculate angle
    GLfloat angleInDegrees = angle;
    GLfloat angleInRadians = angle * (M_PI / 180.0f);
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
    // Define vertices
    GLfloat vertices[] = {
        // Positions          // Texture Coords        
        rotatedX1, rotatedY1, 0.0f, 0.0f,  // Vertex 0 
        rotatedX2, rotatedY2, 1.0f, 0.0f,  // Vertex 1 
        rotatedX3, rotatedY3, 0.5f, 1.0f,  // Vertex 2 
    };
    /*
        0
        | \ 
        |   \ 
        |     \ 
        1-------2 
    */
    GLuint indices[] = {
        0,1,2
    };
    GLuint verts = sizeof(vertices);
    GLuint indexs = sizeof(indices);
    RenderShaderOrtho(shader,vertices ,verts ,indices ,indexs , 3);
}

void Rect(float x, float y, float width, float height, GLfloat angle,Shader shader) {
    // Calculate angle
    GLfloat angleInDegrees = angle;
    GLfloat angleInRadians = angleInDegrees * (M_PI / 180.0f);
    GLfloat cosAngle = cos(angleInRadians);
    GLfloat sinAngle = sin(angleInRadians);
    //Camera Movement
        x += camera.x;
        y += camera.y;
    // Calculate the center of the rect for rotation
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
    /*
        0-------1/4
        |     / |
        |   /   |
        | /     |
      2/5-------3 
    */                 
    GLuint indices[] = {   
        0,1,2,             
        3,4,5              
    };                                                                                        
    GLuint verts = sizeof(vertices);
    GLuint indexs = sizeof(indices);
    RenderShaderOrtho(shader,vertices ,verts , indices, indexs, 6);
}

void Line(float x0, float y0, float x1, float y1, int thickness, Shader shader) {
    // Calculate angle
    GLfloat angleInDegrees = atan2(y1 - y0, x1 - x0);
    GLfloat angleInRadians = angleInDegrees * (M_PI / 180.0f);
    GLfloat cosAngle = cos(angleInRadians);
    GLfloat sinAngle = sin(angleInRadians);
    // Calculate the half thickness
    float halfThickness = thickness / 2.0f;
    // Calculate the length of the line
    float length = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    // Calculate the vertices of the rectangle for the line
    float x2 = x0 - halfThickness * sinAngle;
    float y2 = y0 + halfThickness * cosAngle;
    float x3 = x1 - halfThickness * sinAngle;
    float y3 = y1 + halfThickness * cosAngle;
    float x4 = x1 + halfThickness * sinAngle;
    float y4 = y1 - halfThickness * cosAngle;
    float x5 = x0 + halfThickness * sinAngle;
    float y5 = y0 - halfThickness * cosAngle;
    // Define the vertices for drawing the rectangle
    GLfloat vertices[] = {
        x2, y2, 0.0f, 0.0f,  // Bottom left
        x3, y3, 1.0f, 0.0f,  // Top left
        x4, y4, 1.0f, 1.0f,  // Top right
        x5, y5, 0.0f, 1.0f   // Bottom right
    };
    /*
        0---1/4
        |  /|
        | / |
        |/  |
      2/5---3 
    */       
    // Indices for the rectangle (two triangles)
    GLuint indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    // Render the line as a rectangle
    RenderShaderOrtho(shader, vertices, sizeof(vertices), indices, sizeof(indices), 6);
}

void Zelda(Shader shader, GLfloat angle, GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3) {
    // Calculate angle
    GLfloat angleInDegrees = angle;
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
    // Calculate the center of the triangle for rotation
    float centerX = (x1 + x2 + x3) / 3.0f;
    float centerY = (y1 + y2 + y3) / 3.0f;
    // Apply rotation to each vertex of the outer triangle
    GLfloat rotatedX1 = cosAngle * (x1 - centerX) - sinAngle * (y1 - centerY) + centerX;
    GLfloat rotatedY1 = sinAngle * (x1 - centerX) + cosAngle * (y1 - centerY) + centerY;
    GLfloat rotatedX2 = cosAngle * (x2 - centerX) - sinAngle * (y2 - centerY) + centerX;
    GLfloat rotatedY2 = sinAngle * (x2 - centerX) + cosAngle * (y2 - centerY) + centerY;
    GLfloat rotatedX3 = cosAngle * (x3 - centerX) - sinAngle * (y3 - centerY) + centerX;
    GLfloat rotatedY3 = sinAngle * (x3 - centerX) + cosAngle * (y3 - centerY) + centerY;
    // Calculate midpoints for the inner triangle
    GLfloat midX12 = (rotatedX1 + rotatedX2) / 2.0f;
    GLfloat midY12 = (rotatedY1 + rotatedY2) / 2.0f;
    GLfloat midX23 = (rotatedX2 + rotatedX3) / 2.0f;
    GLfloat midY23 = (rotatedY2 + rotatedY3) / 2.0f;
    GLfloat midX31 = (rotatedX3 + rotatedX1) / 2.0f;
    GLfloat midY31 = (rotatedY3 + rotatedY1) / 2.0f;
    // Define vertices for both triangles (outer and inner)
    GLfloat vertices[] = {
        // Outer Triangle      // Texture Coords 
            rotatedX3, rotatedY3,  0.5f, 1.0f,  // 0 Bottom Left
            rotatedX2, rotatedY2,  1.0f, 0.0f,  // 1 Bottom right
            rotatedX1, rotatedY1,  0.0f, 0.0f,  // 2 Top middle
        // Inner Inverted Triangle
            midX31, midY31,        0.0f, 1.0f,  // 3 Top Left
            midX12, midY12,        0.5f, 0.0f,  // 4 Top right
            midX23, midY23,        1.0f, 1.0f,  // 5 Bottom middle
    };
    /*
             2 
            / \ 
           /   \  
          4-----3 
         / \   / \  
        /   \ /   \ 
       1-----5-----0
    */
    // Test to use index buffers
    // Indices for both triangles         
    GLuint indices[] = {        
        0, 3, 5, // Inner Triangle 1 bottom left
        3, 2, 4, // Inner Triangle 2 bottom right     
        5, 4, 1  // Inner Triangle 3 top middle 
    };                         
    GLuint verts = sizeof(vertices);
    GLuint indexs = sizeof(indices);
    RenderShaderOrtho(shader,vertices ,verts ,indices ,indexs, 9);
}

void Framebuffer(float x, float y, float width, float height) {
    glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    CreateTextureWithPBO(framebuffer,framebufferTexture);
    Rect(x, y, width, height,0,pixelshaderdefault);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
