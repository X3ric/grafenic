#include <grafenic/init.h>

Font font;
Image img;
Image box;

float mousecursorx;
float mousecursory;
float timer;

Color pixelColor;
Shader shaderx;

const char* fragmentsh = 
"#version 330 core\n"
"\n"
"uniform vec2 iMouse;\n"
"uniform float iTime;\n"
"uniform vec2 iResolution;\n"
"uniform sampler2D screenTexture;\n"
"\n"
"in vec2 texCoord;\n"
"out vec4 fragColor;\n"
"\n"
"void mainImage(in vec2 texCoord, in vec2 fragCoord, out vec4 fragColor) {\n"
"    fragColor = vec4(1.0,1.0,1.0,1.0);\n"
"}\n"
"\n"
"void main() {\n"
"    mainImage(texCoord, gl_FragCoord.xy, fragColor);\n"
"}\n";

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
void update(void){
        float speed = 1.0f;
        if (isKeyDown("LeftShift")) {
            speed = 2.0F;
        }
        if (isKeyDown("w")) {
            camera.y += speed;
        }
        if (isKeyDown("s")) {
            camera.y -= speed;
        }
        if (isKeyDown("a")) {
            camera.x += speed;
        }
        if (isKeyDown("d")) {
            camera.x -= speed;
        }
        camera.z = mousescroll.y;
        //camera.angle = (sin(glfwGetTime())*50.0f);
    // DrawImage
        DrawImageShader(img,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,0,shaderx);
        //DrawImage(img,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,0);
    // Input Example
        //      Key                    MouseButton                      Info
        // isKey,isKeyReset  // isMouseButton,isMouseButtonReset  // key Bool
        // isKeyDown,isKeyUp // isMouseButtonDown,isMouseButtonUp // key Press/Relase 
        // isKeyPressed      //                                   // key Mantain 
        //if (isKeyPressed("A", 0.06f)) { // Repeat key every 0.06 seconds
        //    DrawCircle(mouse.x,mouse.y, Scaling(35), (Color){0, 0, 0, 75});
        //}
        //if(isKey("M")){ // Change Cursor state
        //    cursor = false;
        //} else {
        //    cursor = true;
        //}
        //if(isKey("V")){ // Change Vsync state
        //    vsync = true;
        //} else {
        //    vsync = false;
        //}
        // cursor
            //if (!isMouseButton(0)) { 
            //    // gradually reset ball to center pos when not visible
            //        const int speed = 3;
            //        float lerpOutSpeed = (0.0001*speed) * GetTime();
            //        mousecursorx = Lerp(mousecursorx, SCREEN_WIDTH / 2,  lerpOutSpeed);
            //        mousecursory = Lerp(mousecursory, SCREEN_HEIGHT / 2, lerpOutSpeed);
            //        timer = 0;
            //} else {
            //    float lerpInSpeed = 0.05;
            //    mousecursorx = Lerp(mousecursorx, mouse.x, lerpInSpeed);
            //    mousecursory = Lerp(mousecursory, mouse.y, lerpInSpeed);
            //    const float speed = 0.85;
            //    timer += GetTime()/(speed*1000);
            //    float circleSize = Lerp(0, (float)Scaling(25), timer);
            //    //if (IsInside(mouse.x,mouse.y,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 12)) { // Simple 2d collision Mouse based
            //    if(IsInside(mousecursorx,mousecursory,0, 0, SCREEN_WIDTH, SCREEN_HEIGHT / 12)) { // Simple 2d collision X and Y cursor based
            //        DrawCircle(mousecursorx, mousecursory, circleSize, PURPLE);
            //        DrawCircleBorder(mousecursorx, mousecursory, circleSize, 2, VIOLET);
            //    } else {
            //        DrawCircle(mousecursorx, mousecursory, circleSize, VIOLET);
            //        DrawCircleBorder(mousecursorx, mousecursory, circleSize, 2, PURPLE);
            //    }
            //}
    // DrawLine Examples "Cross Screen"
        //DrawLine(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 5, BLACK);
        //DrawLine(0, SCREEN_HEIGHT, SCREEN_WIDTH, 0, 5, BLACK);
    // DrawTriangle Examples
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
    // Drawing Pixel Pointed Color 
        // Uncomment "GetPixel Pointed color" in main
        //int fontsize = Scaling(50);
        //const char* textContent = text("R:%d G:%d B:%d A:%d", pixelColor.r, pixelColor.g, pixelColor.b, pixelColor.a);
        //TextSize textSize = GetTextSize(font, fontsize, textContent);
        //DrawText(SCREEN_WIDTH/2-(textSize.width/2), SCREEN_HEIGHT/6-(textSize.height/2), font, fontsize, textContent, WHITE);
    // Modular Utils Funcitons
        //if(!isKey("Space")){DrawBar(font);}
        //TextInfo();
        //Fps(0, SCREEN_HEIGHT, font, Scaling(50));
        //ExitPromt(font);
    // Experimental WorkInProgress
        //DrawTextAtlas(0, 0, font, Scaling(15), text("FPS: %.0f", fps), WHITE);
        //DrawTextRect(0,0,SCREEN_WIDTH, SCREEN_HEIGHT,font, Scaling(50), text("FPS: %.0f", fps), WHITE,0);
        
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
      // mouse = {x,y} ReadOnly; 
      // mousescroll = {x,y} ReadOnly;
      // mousemoving = Bool ReadOnly; 
      // camera.x = float;
      // camera.y = float;
      // camera.z = float;
      // camera.angle = angle in degres;
      // debug.input = true;
      // debug.wiredframe = true;
    WindowInit(1920, 1080, "Grafenic");
    shaderx = LoadShader("./res/shaders/pixel.vert","./res/shaders/custom.frag");
    //shaderx = LoadShader("./res/shaders/pixel.vert",fragmentsh);//example to load raw "not convinient" not have hotreloading
    font = LoadFont("./res/fonts/Monocraft.ttf");
    img = LoadImage("./res/images/Arch.png");
    ClearColor((Color){75, 75, 75});
    // Saving how many times you boot
        //FileLoadAll(); // Load all data from file "init"
        //char File[FILE_LENGTH]; // file var
        //FileLoad("boot", File); // load boot var in File
        //int bootCount = textint(File); // Convert string to integer
        //FileSave("boot", text("%d", bootCount++)); // Save updated boot count + 1 converted to str
        //print("Booted times: %d\n", bootCount); // debug on console
        ////FileClear(); // clear all data
    while (!WindowState())
    {
        WindowClear();
            update();
        WindowProcess();
        // After Drawing Screen Process
            // GetPixel Pointed color "slow"
                //pixelColor = GetPixel(mouse.x, mouse.y);
            // Screenshot
                //if(isKey("F11")){
                //    SaveScreenshot("Screenshot.png", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
                //    isKeyReset("F11");
                //}
    }
    WindowClose();
    DeleteShaderProgram(shaderx);
    return 0;
} 
