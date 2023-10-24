#ifndef IMAGE_H
#define IMAGE_H

    #define STB_IMAGE_IMPLEMENTATION
    #include <stb_image.h>

    typedef struct {
        GLuint raw;         
        unsigned char* data;
        int width, height;  
        int channels;       
    } Image;

    Image img;

    Image LoadImage(const char *filename) {
        img.data = stbi_load(filename, &img.width, &img.height, &img.channels, STBI_rgb_alpha);
        if (img.data == NULL) {
            img.raw = 0;
            return img;
        }
        glGenTextures(1, &img.raw);
        glBindTexture(GL_TEXTURE_2D, img.raw);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        stbi_image_free(img.data);
        img.data = NULL;
        return img;
    }

    void DrawImage(Image img, float x, float y, float width, float height, GLfloat angleInDegrees) {
        glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, img.raw);
        glPushMatrix();
        float centerX = x + width * 0.5f;
        float centerY = y + height * 0.5f;
        glTranslatef(centerX, centerY, 0);
        glRotatef(angleInDegrees, 0, 0, 1);
        glTranslatef(-centerX, -centerY, 0);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
            glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);
            glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height);
            glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);
        glEnd();
        glPopMatrix();
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY); 
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
    }

#endif // IMAGE_H
