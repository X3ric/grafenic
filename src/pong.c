#include "grafenic/init.c"

#define PADDLE_SPEED 10
#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 125
#define BALL_SIZE 20
#define BALL_SPEED 12
#define LERP 0.75

typedef struct {
    float x, y, width, height, speed;
} Paddle;
typedef struct {
    float x, y, size, speedX, speedY;
} Ball;

Font font;
Paddle leftPaddle, rightPaddle;
Ball ball;
int leftScore = 0, rightScore = 0;

void resetBall() {
    ball.x = SCREEN_WIDTH / 2 - BALL_SIZE / 2;
    ball.y = SCREEN_HEIGHT / 2 - BALL_SIZE / 2;
    ball.speedX = BALL_SPEED * ((rand() % 2 == 0) ? 1 : -1);
    ball.speedY = BALL_SPEED * ((rand() % 2 == 0) ? 1 : -1);
}

void AIPaddle(const float lerp, Paddle *paddle) {
    if (ball.y + ball.size / 2 < paddle->y + paddle->height / 2) {
        paddle->y = Lerp(paddle->y, (paddle->y - paddle->speed), lerp);
    } else if (ball.y + ball.size / 2 > paddle->y + paddle->height / 2) {
        paddle->y = Lerp(paddle->y, (paddle->y + paddle->speed), lerp);
    }
}

void update(void) {
    // Force Screen at 1920x1080
        SCREEN_WIDTH = 1920;
        SCREEN_HEIGHT = 1080;
    // Enemy Ai
        AIPaddle(LERP, &rightPaddle);
    // Move paddles based on user input
        if (isKeyDown("w")) leftPaddle.y = Lerp(leftPaddle.y, (leftPaddle.y + leftPaddle.speed), LERP);
        if (isKeyDown("s")) leftPaddle.y = Lerp(leftPaddle.y, (leftPaddle.y - leftPaddle.speed), LERP);
    // Ensure paddles stay within screen bounds
        leftPaddle.y = fminf(fmaxf(leftPaddle.y, 0), SCREEN_HEIGHT - leftPaddle.height);
        rightPaddle.y = fminf(fmaxf(rightPaddle.y, 0), SCREEN_HEIGHT - rightPaddle.height);
    // Move ball
        ball.x = Lerp(ball.x, (ball.x + ball.speedX), LERP);
        ball.y = Lerp(ball.y, (ball.y + ball.speedY), LERP);
    // Check collision with top and bottom walls
        if (ball.y <= 0 || ball.y >= SCREEN_HEIGHT - ball.size) ball.speedY = -ball.speedY;
    // Check collision with paddles
        if ((ball.x <= leftPaddle.x + leftPaddle.width) && (ball.y + ball.size >= leftPaddle.y) && (ball.y <= leftPaddle.y + leftPaddle.height)) {
            ball.speedX = -ball.speedX;
        }
        if ((ball.x + ball.size >= rightPaddle.x) && (ball.y + ball.size >= rightPaddle.y) && (ball.y <= rightPaddle.y + rightPaddle.height)) {
            ball.speedX = -ball.speedX;
        }
    // Check if the ball is out of bounds
        if (ball.x < 0) {
            rightScore++; // Increment adversary's score
            resetBall();
        } 
        if (ball.x > SCREEN_WIDTH - ball.size) {
            leftScore++; // Increment player's score
            resetBall();
        }
    // Draw ball
        DrawCircle((ball.x + (ball.size / 2)), (SCREEN_HEIGHT - ball.y - (ball.size / 2)), (ball.size / 2), (Color){255, 255, 255, 255});
    // Draw paddles
        DrawRect(leftPaddle.x, leftPaddle.y, Scaling(leftPaddle.width), Scaling(leftPaddle.height), (Color){255, 255, 255, 255});
        DrawRect(rightPaddle.x, rightPaddle.y, Scaling(rightPaddle.width), Scaling(rightPaddle.height), (Color){255, 255, 255, 255});
    // Draw scores
        const char* scoreText = text("%d - %d", leftScore, rightScore);
        const int fontSize = Scaling(35);
        const TextSize textSize = GetTextSize(font, fontSize, scoreText);
        const int textX = (SCREEN_WIDTH - textSize.width) / 2;
        const int textY = (SCREEN_HEIGHT - textSize.height);
        DrawText(textX, textY, font, fontSize, scoreText, (Color){255, 255, 255, 255});
}

int main(int argc, char** argv) {
    fpslimit = 60;
    WindowInit(1920, 1080, "Grafenic");
    font = LoadFont("./res/fonts/Monocraft.ttf");
    ClearColor((Color){0, 0, 0, 100});
    leftPaddle = (Paddle){0,SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED};
    rightPaddle = (Paddle){SCREEN_WIDTH - (PADDLE_WIDTH), SCREEN_HEIGHT / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT, PADDLE_SPEED};
    ball = (Ball){SCREEN_WIDTH/2,SCREEN_HEIGHT/2, BALL_SIZE, BALL_SPEED, BALL_SPEED};
    while (!WindowState()) {
        WindowClear();
        update();
        WindowProcess();
    }
    WindowClose();
    return 0;
}