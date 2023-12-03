void DrawCenteredTextRows(Font font,int section,int rows, const char* textContent) { // Text Rows
    int y = SCREEN_HEIGHT / 12;
    int sectionWidth = SCREEN_WIDTH / rows;
    int fontsize = Scaling(50);
    TextSize textSize = GetTextSize(font, fontsize, textContent);
    int textX = section * sectionWidth + (sectionWidth - textSize.width) / 2;
    int textY = y/2 + textSize.height/2;
    DrawText(textX, textY , font, fontsize, textContent, WHITE);
} 
void DrawCenteredTextColumn(Font font, int section, int totalSections, const char* textContent) { // Text Collumns
    int sectionHeight = SCREEN_HEIGHT / totalSections;
    int fontsize = Scaling(40);
    TextSize textSize = GetTextSize(font, fontsize, textContent);
    int textX = 10;
    int textY = section * sectionHeight + (textSize.height);
    DrawText(textX, textY, font, fontsize, textContent, WHITE);
} 
void DrawBar(Font font) { // Bottom Bar with info
    int y = SCREEN_HEIGHT / 12;
    DrawRect(0, 0, SCREEN_WIDTH, y, (Color){50, 50, 50,100});
    DrawRectBorder(0, 0, SCREEN_WIDTH, y, 5, (Color){0, 0, 0,175});
    int texts = 3;
    // Mouse info
        DrawCenteredTextRows( font,0,texts, text("Mouse = X: %.0f Y: %.0f", mouse.x, mouse.y));
    // Scroll info
        DrawCenteredTextRows( font,1,texts, text("Scroll = X: %.0f Y: %.0f", mousescroll.x, mousescroll.y));
    // Window info 
        DrawCenteredTextRows( font,2,texts, text("Size = X: %d Y: %d", SCREEN_WIDTH, SCREEN_HEIGHT));
} 
void DrawPopUp(const char* title, Font font,int fontsize, int width, int height) { // PopUp with centered text
    int x = (SCREEN_WIDTH / 2) - (width / 2);
    int y = (SCREEN_HEIGHT / 2) - (height / 2);
    DrawRect(x, y, width, height, (Color){50, 50, 50,100});
    DrawRectBorder(x, y, width, height, 5, (Color){0, 0, 0});
    TextSize text = GetTextSize(font, fontsize, title);
    DrawText((SCREEN_WIDTH / 2) - (text.width / 2) , (SCREEN_HEIGHT / 2) + (text.height / 2) , font, fontsize, title, WHITE);
} 
void Fps(int x , int y, Font font, int size) { // FPS info
    const char* title = text("FPS: %.0f", fps);
    DrawText( x, y, font, size, title, WHITE);
} 
void ExitPromt(Font font) { // Escape PopUp
    if (isKey("Esc")) {
        DrawPopUp("Quit?Yes/No",font,Scaling(17),SCREEN_WIDTH/18, SCREEN_HEIGHT/35);
        if (isKeyDown("Y")) {
            WindowStateSet(true);
        }
        if (isKeyDown("N")) {
            isKeyReset("Esc"); // inverts isKey("Esc") return
        }
    }
} 
void TextInfo(){ // Text info
    int texts = 12;
    DrawCenteredTextColumn(font,4,texts, text("Mouse moving: %s", mousemoving ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,5,texts, text("M = mouse state: %s", cursor ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,6,texts, text("V = vsync state: %s", vsync ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,7,texts, text("M1 = circle state: %s", isMouseButton(0) ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,8,texts, text("Esc = exitbar state: %s", isKey("Esc") ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,9,texts, text("Space = bar state: %s", !isKey("Space") ? "ON" : "OFF"));
} 