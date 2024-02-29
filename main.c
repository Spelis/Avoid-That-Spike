#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Variables
float playerX = 30;
float playerY = 30;
float velY = 0;
int velX = 5;
int playerS = 20;
float delta;
int menu = 0;
int level = 0;
int spikes[10];
int coins = 0;

// Sound FXs
Sound die;
Sound jump;
Sound levelup;
Sound coin;

bool CheckButton(Rectangle location) {
  if (CheckCollisionPointRec(GetMousePosition(), location) &&
      IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    PlaySound(jump);
    return true;
  } else {
    return false;
  }
}
void DrawButton(const char *string, Vector2 pos, Vector2 size) {
  DrawRectangle(pos.x, pos.y, size.x, size.y, GRAY);
  DrawText(string, pos.x + 2, pos.y + 2, size.y - 4, WHITE);
}
void reSpike() {
  int c;
  int i;
  bool g = false;
  for (i = 0; i < 10; i++) {
    c = rand() % 3;
    if (c == 2 && g == false) {
      g = true;
    } else if (c == 2 && g == true) {
      c = 0;
    } else {
      c = c;
    }
    spikes[i] = c;
  }
}

void Reset(void) {
  playerX = 30;
  playerY = 30;
  velY = 0;
  velX = 5;
  playerS = 20;
  coins = 0;
  level = 0;
  reSpike();
}

void DrawSpikes() {
  int i;
  int spike;
  for (i = 0; i < 10; i++) {
    spike = spikes[i];
    Vector2 x;
    Vector2 y;
    Vector2 z;
    if (velX == -5) {
      y.x = 0;
      y.y = i * 60;
      x.x = 30;
      x.y = (i * 60) + 30;
      z.x = 0;
      z.y = (i * 60) + 60;
      if (spike == 1) {
        DrawTriangle(x, y, z, RED);
      } else if (spike == 2) {
        DrawRectangleV((Vector2){20, (i * 60) + 20}, (Vector2){10, 10}, YELLOW);
      }

    } else if (velX == 5) {
      x.x = 600;
      x.y = i * 60;
      y.x = 570;
      y.y = (i * 60) + 30;
      z.x = 600;
      z.y = (i * 60) + 60;
      if (spike == 1) {
        DrawTriangle(x, y, z, RED);
      } else if (spike == 2) {
        DrawRectangleV((Vector2){580, (i * 60) + 20}, (Vector2){10, 10},
                       YELLOW);
      }
    }
  }
}
void CollideSpikes() {
  int i;
  for (i = 0; i < 10; i++) {
    Rectangle spike = {0, (i * 60) + 10, 10, 40};
    Rectangle player = {playerX, playerY, playerS, playerS};

    if (velX == -5) {
      if (spikes[i] != 0) {
        if (spikes[i] == 2 &&
            CheckCollisionRecs((Rectangle){25, (i * 60) + 25, 10, 10},
                               player)) {
          spikes[i] = 0;
          coins++;
          PlaySound(coin);
        }

        if (CheckCollisionRecs(spike, player) && spikes[i] == 1) {
          menu = 2;
          PlaySound(die);
        }
      }
    } else if (velX == 5) {
      if (spikes[i] != 0) {
        if (spikes[i] == 2 &&
            CheckCollisionRecs((Rectangle){575, (i * 60) + 25, 10, 10},
                               player)) {
          spikes[i] = 0;
          coins++;
          PlaySound(coin);
        }

        spike.x = 590;
        if (CheckCollisionRecs(spike, player) && spikes[i] == 1) {
          menu = 2;
          PlaySound(die);
        }
      }
    }
  }
}

int main(void) {
  srand(time(NULL));
  InitWindow(600, 600, "Avoid That Spike!");
  InitAudioDevice();
  SetTargetFPS(360);
  SetExitKey(0);
  HideCursor();

  die = LoadSound("die.wav");
  jump = LoadSound("jump.wav");
  levelup = LoadSound("level.wav");
  coin = LoadSound("coin.wav");
  reSpike();

  while (!WindowShouldClose()) {
    delta = GetFrameTime() * (60 + level);
    // Game Loop
    if (menu == 1) {
      if (IsKeyPressed(KEY_SPACE) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        velY = -6.0;
        PlaySound(jump);
      }
      if (IsKeyPressed(KEY_ESCAPE)) {
        menu = 3;
      }
      if (playerX < 0) {
        PlaySound(levelup);
        velX = 5;
        reSpike();
        level++;
      } else if (playerX > 580) {
        PlaySound(levelup);
        velX = -5;
        reSpike();
        level++;
      }
      if (playerY < 0 || playerY > 580) {
        menu = 2;
        PlaySound(die);
      }
      playerX += velX * delta;
      playerY += velY * delta;
      velY += 0.2 * delta;
    }

    // Drawing
    BeginDrawing();
    ClearBackground(BLACK);

    if (menu == 1 || menu == 3 || menu == 2) {
      char LVstr[50];
      float g;

      sprintf(LVstr, "%d", level);
      g = MeasureText(LVstr, 70);

      float tw = 300.0f - (g / 2.0f);
      DrawText(LVstr, tw, 20, 70, GRAY);

      sprintf(LVstr, "%d", coins);
      g = MeasureText(LVstr, 20);

      tw = 300.0f - (g / 2.0f);
      DrawText(LVstr, tw, 80, 20, YELLOW);
      DrawFPS(0, 0);
      DrawRectangle(playerX, playerY, playerS, playerS, WHITE);
      DrawSpikes();
      if (menu != 2) {
        CollideSpikes();
      }
    }
    if (menu == 0) {
      DrawText("Avoid That Spike!", 0, 0, 64, WHITE);
      DrawButton("Play", (Vector2){0.0f, 72.0f}, (Vector2){300.0f, 40.0f});
      if (CheckButton((Rectangle){0, 72, 300, 40})) {
        menu = 1;
      }
      DrawButton("Quit :(", (Vector2){0.0f, 122.0f}, (Vector2){300.0f, 40.0f});
      if (CheckButton((Rectangle){0, 122, 300, 40})) {
        break;
      }
    }
    if (menu == 2) {
      DrawText("You Died.", 0, 0, 64, RED);

      DrawButton("Restart", (Vector2){0.0f, 72.0f}, (Vector2){300.0f, 40.0f});
      if (CheckButton((Rectangle){0, 72, 300, 40})) {
        Reset();
        menu = 1;
      }
      DrawButton("Main Menu", (Vector2){0.0f, 122.0f},
                 (Vector2){300.0f, 40.0f});
      if (CheckButton((Rectangle){0, 122, 300, 40})) {
        Reset();
        menu = 0;
      }
    }
    if (menu == 3) {
      DrawText("Paused", 0, 0, 64, WHITE);
      DrawButton("Resume", (Vector2){0.0f, 72.0f}, (Vector2){300.0f, 40.0f});
      if (CheckButton((Rectangle){0, 72, 300, 40})) {
        menu = 1;
      }
      DrawButton("Main Menu", (Vector2){0.0f, 122.0f},
                 (Vector2){300.0f, 40.0f});
      if (CheckButton((Rectangle){0, 122, 300, 40})) {
        menu = 0;
        Reset();
      }
    }

    DrawRectangle(GetMouseX() - 5, GetMouseY() - 5, 10, 10, WHITE);
    EndDrawing();
  }

  return 0;
}
