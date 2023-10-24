#ifndef CAMERA_H
#define CAMERA_H

        void PerspectiveCam(GLfloat fovY, GLfloat aspect, GLfloat zNear, GLfloat zFar) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            GLfloat f = 1.0f / tan(fovY / 2.0f);
            GLfloat m[16];
            m[0]  = f / aspect; m[4]  = 0.0f;
            m[1]  = 0.0f;       m[5]  = f;
            m[2]  = 0.0f;       m[6]  = 0.0f;
            m[3]  = 0.0f;       m[7]  = 0.0f;

            m[8]  = 0.0f;
            m[9]  = 0.0f;
            m[10] = (zFar + zNear) / (zNear - zFar);
            m[11] = -1.0f;

            m[12] = 0.0f;
            m[13] = 0.0f;
            m[14] = (2.0f * zFar * zNear) / (zNear - zFar);
            m[15] = 0.0f;
            glLoadMatrixf(m);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

        void OrthoCam(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar) {
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(left, right, bottom, top, zNear, zFar);
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
        }

#endif // CAMERA_H
