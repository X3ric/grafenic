#ifndef TEXT_H
#define TEXT_H

    #define STB_TRUETYPE_IMPLEMENTATION
    #include <stb_truetype.h>

    typedef struct {
        stbtt_fontinfo fontInfo;
        unsigned char* fontBuffer;
    } Font;

    Font LoadFont(const char* fontPath) {
        Font font;
        long size;
        FILE* fp = fopen(fontPath, "rb");
        if (!fp) return font;
        fseek(fp, 0, SEEK_END);
        size = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        font.fontBuffer = (unsigned char*)malloc(size);
        
        if (font.fontBuffer) {
            fread(font.fontBuffer, size, 1, fp);
            if (!stbtt_InitFont(&font.fontInfo, font.fontBuffer, stbtt_GetFontOffsetForIndex(font.fontBuffer, 0))) {
                free(font.fontBuffer);
            }
        }
        fclose(fp);
        return font;
    }

    void DrawText(int x, int y, Font font, float fontSize, const char* text, Color color) {
        if (fontSize <= 1) fontSize = 1;
        if (color.a == 0) color.a = 255;
        if (!font.fontBuffer) return;
        float scale = stbtt_ScaleForPixelHeight(&font.fontInfo, fontSize);
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&font.fontInfo, &ascent, &descent, &lineGap);
        y -= (int)((ascent + descent) * scale);
        int ch_x = x;
        for (size_t i = 0; text[i] != '\0'; ++i) {
            int width, height, xoffset, yoffset;
            unsigned char* bitmap = stbtt_GetCodepointBitmap(&font.fontInfo, 0, scale, text[i], &width, &height, &xoffset, &yoffset);
            for (int j = 0; j < height; ++j) {
                for (int k = 0; k < width; ++k) {
                    int pixel = j * width + k;
                    unsigned char alpha = bitmap[pixel];
                    if (alpha > 0) {
                        DrawPixel(ch_x + k + xoffset, y - j - yoffset, (Color){color.r, color.g, color.b, color.a / (alpha*3)}); // blended color with font alpha
                        //DrawPixel(ch_x + k + xoffset, y - j - yoffset, (Color){color.r, color.g, color.b, color.a}); // only setted  font color
                        //DrawPixel(ch_x + k + xoffset, y - j - yoffset, (Color){color.r, color.g, color.b, alpha}); // only alpha of the font
                    }
                }
            }
            int advanceWidth, leftSideBearing;
            stbtt_GetCodepointHMetrics(&font.fontInfo, text[i], &advanceWidth, &leftSideBearing);
            ch_x += (int)((advanceWidth + (text[i+1] ? stbtt_GetCodepointKernAdvance(&font.fontInfo, text[i], text[i+1]) : 0)) * scale);
            stbtt_FreeBitmap(bitmap, NULL);
        }
    }
    
    typedef struct {
        int width;
        int height;
    } TextSize;

    TextSize GetTextSize(Font font, float fontSize, const char* text) {
        if (!font.fontBuffer) return (TextSize){0, 0};
        TextSize size = {0, 0};
        float scale = stbtt_ScaleForPixelHeight(&font.fontInfo, fontSize);
        int ascent, descent;
        stbtt_GetFontVMetrics(&font.fontInfo, &ascent, &descent, 0); 
        int max_height = (int)((ascent) * scale);
        for (size_t i = 0; text[i] != '\0'; ++i) {
            int advanceWidth, leftSideBearing;
            stbtt_GetCodepointHMetrics(&font.fontInfo, text[i], &advanceWidth, &leftSideBearing);
            size.width += (int)(advanceWidth * scale);
        }
        size.height = max_height;
        return size;
    }

#endif // TEXT_H
