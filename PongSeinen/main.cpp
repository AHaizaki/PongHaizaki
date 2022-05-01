#include "raylib.h"
#include <math.h>
using namespace std;

struct Ball
{
	float x, y;
	float speedX, speedY;
	float size;

	Rectangle getRect() {
		return Rectangle{ x, y, size, size };
	}
	void Draw()
	{
		DrawRectangleRec(getRect(), WHITE);
	}

	void resetBall() {
		x = GetScreenWidth() / 2 - 10;
		y = GetScreenHeight() / 2 - 10;
		speedX = 300;
		speedY = 0;
	}
};

struct Paddle
{
	float x, y;
	float speed;
	float width, height;

	Rectangle getRect() {
		return Rectangle{ x, y, width, height };
	}

	void Draw()
	{
		DrawRectangleRec(getRect(), WHITE);
	}
};

int main() 
{
	InitWindow(800, 600, "PongSeinen");
	SetWindowState(FLAG_VSYNC_HINT);
	SetWindowIcon(LoadImage("../assets/images/icon.png"));

	const char* puntos[11] = { "0","1","2","3","4","5","6","7","8","9","10" };

	//Puntacion
	int player1Score = 0;
	int player2Score = 0;
	bool isPause = false;
	const char* pauseText = nullptr;

	//Pelota
	Ball ball;
	ball.x = GetScreenWidth() / 2 - 10;
	ball.y = GetScreenHeight() / 2 - 10;
	ball.size = 20;
	ball.speedX = 300;
	ball.speedY = 0;
	//Raquetas
	Paddle leftPaddle;
	leftPaddle.x = 50;
	leftPaddle.y = GetScreenHeight() / 2 - 50;
	leftPaddle.width = 20;
	leftPaddle.height = 100;
	leftPaddle.speed = 500;

	Paddle rightPaddle;
	rightPaddle.x = GetScreenWidth() - 70;
	rightPaddle.y = GetScreenHeight() / 2 - 50;
	rightPaddle.width = 20;
	rightPaddle.height = 100;
	rightPaddle.speed = 600;

	const char* winnerText = nullptr;

	while (!WindowShouldClose()) {

		if (!isPause) {
			ball.x += ball.speedX * GetFrameTime();
			ball.y += ball.speedY * GetFrameTime();
		}

		if (ball.y < 0) {
			ball.y = 0;
			ball.speedY *= -1;
		}
		if (ball.y > GetScreenHeight() - 20) 
		{
			ball.y = GetScreenHeight() - 20;
			ball.speedY *= -1;
		}
		if (ball.x < 35) {
			if (player2Score < 10)
				player2Score++;
			if (player2Score == 10) {
				winnerText = "CPU gana el pong";
			}
			if (!winnerText) {
				ball.resetBall();
			}
		}
		if (ball.x > GetScreenWidth() - 55)
		{
			if(player1Score < 10)
				player1Score++;
			if (player1Score == 10) {
				winnerText = "El jugador gana el pong";
			}
			if (!winnerText) {
				ball.resetBall();
			}
		}

		if (IsKeyDown(KEY_W) && !isPause) {
			leftPaddle.y -= leftPaddle.speed * GetFrameTime();
			if (leftPaddle.y < 0) {
				leftPaddle.y = 0;
			}
		}
		if (IsKeyDown(KEY_S) && !isPause) {
			leftPaddle.y += leftPaddle.speed * GetFrameTime();
			if (leftPaddle.y > GetScreenHeight() - 100) {
				leftPaddle.y = GetScreenHeight() - 100;
			}
		}
		if (ball.y + 10 > rightPaddle.y + 50 && rightPaddle.y < GetScreenHeight() - 100 && !winnerText && !isPause) {
			rightPaddle.y += rightPaddle.speed * GetFrameTime();
			if (rightPaddle.y < 0) {
				rightPaddle.y = 0;
			}
		}
		if (ball.y + 10 < rightPaddle.y + 50 && rightPaddle.y > 0 && !winnerText && !isPause) {
			rightPaddle.y -= rightPaddle.speed * GetFrameTime();
			if (rightPaddle.y > GetScreenHeight() - 100) {
				rightPaddle.y = GetScreenHeight() - 100;
			}
		}


		if (CheckCollisionRecs(ball.getRect(),leftPaddle.getRect())) {
			if (ball.speedX < 0) {
				ball.speedX *= -1.1f;
				ball.speedX = fminf(700.0, ball.speedX);
				ball.speedY = (ball.y + 5 - leftPaddle.y - 50) / (leftPaddle.height / 2) * ball.speedX;
			}
		}
		if (CheckCollisionRecs(ball.getRect(), rightPaddle.getRect())) {
			if (ball.speedX > 0) {
				ball.speedX *= -1.1f;
				ball.speedX = fminf(700.0, ball.speedX);
				ball.speedY = (ball.y + 5 - rightPaddle.y - 50) / (rightPaddle.height / 2) * -ball.speedX;
			}
		}

		if (winnerText && IsKeyPressed(KEY_ENTER)) {

			ball.resetBall();

			leftPaddle.x = 50;
			leftPaddle.y = GetScreenHeight() / 2 - 50;

			rightPaddle.x = GetScreenWidth() - 70;
			rightPaddle.y = GetScreenHeight() / 2 - 50;

			player1Score = 0;
			player2Score = 0;
			winnerText = nullptr;
		}

		if (IsKeyPressed(KEY_P) && !winnerText) {
			isPause = !isPause;
			if (isPause) {
				pauseText = "PAUSADO";
			}
			else {
				pauseText = nullptr;
			}
		}

		BeginDrawing();
			ClearBackground(BLACK);

			DrawRectangle(0, 0, 35, GetScreenHeight(), RED); //Pared Izquierda
			DrawRectangle(GetScreenWidth() - 35, 0, 35, GetScreenHeight(), SKYBLUE); //Pared Derecha
			for (int i = 0; i < GetScreenHeight() / 40; i++)
			{
				DrawRectangle(GetScreenWidth() / 2 - 10, i * 40 + 10,ball.size, ball.size, DARKGRAY);
			}
			DrawText(puntos[player1Score], GetScreenWidth() / 3, 50, 100, WHITE);
			DrawText(puntos[player2Score], GetScreenWidth() - GetScreenWidth() / 3 - MeasureText(puntos[player2Score], 100), 50, 100, WHITE);
			ball.Draw();
			DrawRectangle(0, 0, 35, GetScreenHeight(), RED); //Pared Izquierda
			DrawRectangle(GetScreenWidth() - 35, 0, 35, GetScreenHeight(), SKYBLUE); //Pared Derecha
			leftPaddle.Draw();
			rightPaddle.Draw();

			if (winnerText) 
			{
				int textWidth = MeasureText(winnerText,40);
				DrawText(winnerText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 4, 40, WHITE);
				DrawText("Presiona Enter para reiniciar", GetScreenWidth() / 2 - MeasureText("Presiona Enter para reiniciar", 30) / 2, GetScreenHeight() / 2, 30, WHITE);
			}
			if (pauseText) {
				int textWidth = MeasureText(pauseText, 40);
				DrawText(pauseText, GetScreenWidth() / 2 - textWidth / 2, GetScreenHeight() / 4, 40, WHITE);
			}
		EndDrawing();
	}

	CloseWindow();
	return 0;
}