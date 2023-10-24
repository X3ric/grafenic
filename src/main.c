#include "libs/grafenic.h"

Font font;
Image img;
float ballX;
float ballY;

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
        Mouse mouse = GetMousePos();
        DrawCenteredTextRows( font,0,texts, text("Mouse = X: %.0f Y: %.0f", mouse.x, mouse.y));
    // Scroll info
        MouseScroll scrollData = GetScroll();
        DrawCenteredTextRows( font,1,texts, text("Scroll = X: %.0f Y: %.0f", scrollData.x, scrollData.y));
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
            isKeyReset("Esc"); // inverts isKey("Esc")
        }
    }
}

void TextInfo(){ // Text info
    int texts = 12;
    DrawCenteredTextColumn(font,5,texts, text("M = mouse state: %s", cursor ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,6,texts, text("V = vsync state: %s", vsync ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,7,texts, text("M1 = circle state: %s", isMouseButton(0) ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,8,texts, text("Esc = exitbar state: %s", isKey("Esc") ? "ON" : "OFF"));
    DrawCenteredTextColumn(font,9,texts, text("Space = bar state: %s", !isKey("Space") ? "ON" : "OFF"));
}

float timer;

void Update(){
    //DrawImage
        DrawImage(img,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,0);
    //Input Example
        //      Key                    MouseButton                      Info
        // IsKey,isKeyReset  // isMouseButton,isMouseButtonReset  // key Press
        // IsKeyDown,IsKeyUp // isMouseButtonDown,isMouseButtonUp // key Mantain/Relase 
        if(isKey("M")){ // Change Cursor state
            cursor = false;
        } else {
            cursor = true;
        }
        if(isKey("V")){ // Change Vsync state
            vsync = true;
        } else {
            vsync = false;
        }
        if (!isMouseButton(0)) { //DrawCircle Examples "Mouse Circle"
            // gradually reset ball to center pos when not visible
                const int speed = 3;
                float lerpOutSpeed = (0.0001*speed) * GetTime();
                ballX = Lerp(ballX, SCREEN_WIDTH / 2,  lerpOutSpeed);
                ballY = Lerp(ballY, SCREEN_HEIGHT / 2, lerpOutSpeed);
                timer = 0;
        } else {
            Mouse mouse = GetMousePos();
            float lerpInSpeed = 0.05;
            ballX = Lerp(ballX, mouse.x, lerpInSpeed);
            ballY = Lerp(ballY, mouse.y, lerpInSpeed);
            const float speed = 0.85;
            timer += GetTime()/(speed*1000);
            float circleSize = Lerp(0, (float)Scaling(25), timer);
            //if (IsInside(mouse.x,mouse.y,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 12)) { // Simple 2d collision Mouse based
            if(IsInside(ballX,ballY,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 12)) { // Simple 2d collision X and Y based
                DrawCircle(ballX, ballY, circleSize, PURPLE);
                DrawCircleBorder(ballX, ballY, circleSize, 2, VIOLET);
            } else {
                DrawCircle(ballX, ballY, circleSize, VIOLET);
                DrawCircleBorder(ballX, ballY, circleSize, 2, PURPLE);
            }
        }
    //DrawLine Examples "Cross Screen"
        //DrawLine(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 5, BLACK);
        //DrawLine(0, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 5, BLACK);
    //DrawTriangle Examples
        //int x = SCREEN_WIDTH/2;
        //int y = SCREEN_HEIGHT/2;
        //static int size = 50;
        //    if(isKeyDown("+")){
        //        size += 1;
        //    }
        //    if(isKeyDown("-")){
        //        size -= 1;
        //    }
        //DrawTriangle(x - size, y, x + size, y, x, y + size, PURPLE);
    // Modular Utils Funcitons
        if(!isKey("Space")){DrawBar(font);}//Hides bar on key press
        TextInfo();
        Fps(0, SCREEN_HEIGHT, font, Scaling(50));
        ExitPromt(font);
}

int main(void)
{
    // Built-in "!default"
      // floating = false;
      // fullscreen = true;
      // vsync = true;
      // cursor = false;
      // transparent = true;
      // decorated = false;
      // visible = false;
      // fpslimit = 60;
      // fps = ReadOnly; 
      // frametime = ReadOnly; 
    WindowInit(1920, 1080, "Grafenic");
    font = LoadFont("./res/fonts/Monocraft.ttf");
    img = LoadImage("./res/images/Arch.png");
    ClearColor((Color){75, 75, 75});
    while (!WindowState())
    {
        WindowClear();
            Update();
        WindowProcess();
    }
    WindowClose();
    return 0;
}
