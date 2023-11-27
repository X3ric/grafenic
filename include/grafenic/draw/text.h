#ifndef TEXT_H
#define TEXT_H

    #define STB_RECT_PACK_IMPLEMENTATION
    #include <stb_rect_pack.h>
    #define STB_TRUETYPE_IMPLEMENTATION
    #include <stb_truetype.h>

    typedef struct {
        float x0, y0, x1, y1;  // coordinates of the glyph in the atlas
        float xoff, yoff;      // left/top offsets
        float xadvance;        // advance width
    } Glyph;

    typedef struct {
        stbtt_fontinfo fontInfo;     // Font information
        unsigned char* fontBuffer;   // Font data buffer
        unsigned char* atlasData;    // Atlas texture data
        int atlasWidth, atlasHeight; // Dimensions of the atlas
        Glyph glyphs[96];            // Glyph data for ASCII characters 32-127
        float fontSize;              // Font size for which glyphs were generated
    } Font;

    Font GenAtlas(Font font) {
        int atlasWidth = 512; // Fixed atlas width
        int atlasHeight = 512; // Fixed atlas height
        font.atlasWidth = atlasWidth;
        font.atlasHeight = atlasHeight;
        font.atlasData = (unsigned char*)calloc(1, atlasWidth * atlasHeight);
        if (!font.atlasData) {
            free(font.fontBuffer);
            return font;
        }
        stbtt_pack_context packContext;
        if (!stbtt_PackBegin(&packContext, font.atlasData, atlasWidth, atlasHeight, 0, 1, NULL)) {
            free(font.atlasData);
            free(font.fontBuffer);
            return font;
        }
        stbtt_PackSetOversampling(&packContext, 2, 2);
        stbtt_packedchar packedChars[96];
        font.fontSize = 24.0;//generated atlas font size
        if (!stbtt_PackFontRange(&packContext, font.fontBuffer, 0, font.fontSize, 32, 96, packedChars)) {
            stbtt_PackEnd(&packContext);
            free(font.atlasData);
            free(font.fontBuffer);
            return font;
        }
        for (int i = 0; i < 96; ++i) {
            font.glyphs[i].x0 = packedChars[i].x0;
            font.glyphs[i].y0 = packedChars[i].y0;
            font.glyphs[i].x1 = packedChars[i].x1;
            font.glyphs[i].y1 = packedChars[i].y1;
            font.glyphs[i].xoff = packedChars[i].xoff;
            font.glyphs[i].yoff = packedChars[i].yoff;
            font.glyphs[i].xadvance = packedChars[i].xadvance;
        }
        stbtt_PackEnd(&packContext);
        // stbi_write_png("font_atlas.png", atlasWidth, atlasHeight, 1, font.atlasData, atlasWidth);
        return font;
    }


    Font LoadFont(const char* fontPath) {
        Font font = {0};
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
                fclose(fp);
                return font;
            }
        }
        fclose(fp);
        font = GenAtlas(font);
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

    void DrawTextAtlas(int x, int y, Font font, float fontSize, const char* text, Color color) {
        if (fontSize <= 1) fontSize = 1;
        if (color.a == 0) color.a = 255;
        if (!font.fontBuffer) return;
        float scale = (fontSize / (font.fontSize*2.5));
        int ch_x = x;
        int baseline = SCREEN_HEIGHT - y; // This adjusts the baseline considering the screen height
        for (unsigned int i = 0; text[i] != '\0'; ++i) {
            char ch = text[i];
            if (ch < 32 || ch >= 128) continue; // Skip non-printable characters
            int index = ch - 32; // Assuming ASCII 32-127
            Glyph glyph = font.glyphs[index];
            for (int glyphY = glyph.y0; glyphY < glyph.y1; ++glyphY) {
                for (int glyphX = glyph.x0; glyphX < glyph.x1; ++glyphX) {
                    unsigned char alpha = font.atlasData[glyphX + glyphY * font.atlasWidth];
                    if (alpha > 0) {
                        int drawX = ch_x + (int)((glyphX - glyph.x0) * scale);
                        int drawY = baseline - (int)((glyphY - glyph.y0) * scale);
                        if (drawX < 0 || drawX >= SCREEN_WIDTH || drawY < 0 || drawY >= SCREEN_HEIGHT) {
                            continue;
                        }
                        DrawPixel(drawX, drawY, (Color){color.r, color.g, color.b, (color.a - alpha)/2});
                    }
                }
            }
            ch_x += (int)(glyph.xadvance * 2 * scale);
        }
    }

    void DrawTextRect(int x, int y, float imageWidth, float imageHeight, Font font, float fontSize, const char* text, Color color,int angleInDegrees) {
        if (fontSize <= 1) fontSize = 1;
        if (color.a == 0) color.a = 255;
        if (!font.fontBuffer) return;
        float scale = stbtt_ScaleForPixelHeight(&font.fontInfo, fontSize);
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&font.fontInfo, &ascent, &descent, &lineGap);
        unsigned char* bitmap = calloc(imageWidth * imageHeight * 4, sizeof(unsigned char));
        if (!bitmap) return;
        int ch_x = x, ch_y = y + imageHeight - ((int)((ascent + descent) * scale));
        int lineWidth = 0;
        for (size_t i = 0; text[i] != '\0'; ++i) {
            int advanceWidth, leftSideBearing;
            stbtt_GetCodepointHMetrics(&font.fontInfo, text[i], &advanceWidth, &leftSideBearing);
            if (text[i] == '\n' || (lineWidth + advanceWidth * scale > imageWidth && text[i] != ' ')) {
                ch_y -= (ascent - descent + lineGap) * scale;
                ch_x = x;
                lineWidth = 0;
            }
            if (text[i] == '\n') continue;
            int c_width, c_height, xoffset, yoffset;
            unsigned char* char_bitmap = stbtt_GetCodepointBitmap(&font.fontInfo, 0, scale, text[i], &c_width, &c_height, &xoffset, &yoffset);
            for (int j = 0; j < c_height; ++j) {
                for (int k = 0; k < c_width; ++k) {
                    int bitmap_x = ch_x + k + xoffset;
                    int bitmap_y = ch_y - j - yoffset;
                    if (bitmap_x < 0 || bitmap_x >= imageWidth || bitmap_y < 0 || bitmap_y >= imageHeight) continue;
                    int pixel = (bitmap_y * imageWidth + bitmap_x) * 4;
                    unsigned char alpha = char_bitmap[j * c_width + k];
                    bitmap[pixel] = color.r;
                    bitmap[pixel + 1] = color.g;
                    bitmap[pixel + 2] = color.b;
                    bitmap[pixel + 3] = (color.a * alpha) / 255;
                }
            }
            ch_x += roundf(advanceWidth * scale);
            lineWidth += roundf(advanceWidth * scale);
            stbtt_FreeBitmap(char_bitmap, NULL);
        }
        // Bind bitmap to OpenGL Created texture
        glBindTexture(GL_TEXTURE_2D, textTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bitmap);
        free(bitmap);
        // Draw textured rectangle
        glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBindTexture(GL_TEXTURE_2D, textTexture);
        ScreenCam(x, y, imageWidth, imageHeight,angleInDegrees,shaderdefault);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
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
