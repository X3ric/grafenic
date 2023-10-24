#ifndef UTILS_H
#define UTILS_H

    #include <stdarg.h>
    #include <math.h>

    const char* text(const char* format, ...) {
        static char buffer[100];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format, args);
        va_end(args);
        return buffer;
    }

    void print(const char* format, ...) {
        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);
    }

    int RandomInt(int min, int max)
    {
        if (min > max) {
            int tmp = max;
            max = min;
            min = tmp;
        }
        return (rand()%(abs(max-min)+1) + min);
    }

    float Lerp(float start, float end, float t) {
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        return start + (end - start) * t;
    }

    double GetTime() {
        return glfwGetTime();
    }

    void SetTime(double time) {
        glfwSetTime(time);
    }

    void ErrorCallback(int error, const char* description) {
        fprintf(stderr, "Error: %s\n", description);
    }

    int Scaling(int fontsize) { // Ratio scaling
        float widthScale = (float)SCREEN_WIDTH / WIDTH;
        float heightScale = (float)SCREEN_HEIGHT / HEIGHT;
        float scale = fmin(widthScale, heightScale);
        int scaledFontSize = (int)(fontsize * scale);
        return (fontsize > 0) ? fmax(scaledFontSize, 1) : 0;
    }

#endif // UTILS_H