/*******************************************************************************************
*
*   raylib gamejam template
*
*   Code licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2026 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>      // Emscripten library
#endif

#include <stdio.h>                          // Required for: printf()
#include <stdlib.h>                         // Required for: 
#include <string.h>                         // Required for:

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
#define LOG(...) printf(__VA_ARGS__)
#else
#define LOG(...)
#endif

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
typedef enum {
	SCREEN_LOGO,
	SCREEN_GAMEPLAY,
	SCREEN_EXIT
} GameScreen;

// TODO: Define your custom data types here

//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screenWidth = 720;
static const int screenHeight = 720;
static GameScreen currentScreen;

static RenderTexture2D target = { 0 };  // Render texture to render our game
static int frameCounter = 0;

// TODO: Define global variables here, recommended to make them static

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

static void DrawMenu(void);

static void DrawGame(void);


int RandInt(int min, int max);
int RandFloat(int min, int max);


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
	SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messages
#endif

	// Initialization
	//--------------------------------------------------------------------------------------
	InitWindow(screenWidth, screenHeight, "Hexcore Operator");

	SetExitKey(KEY_Q);
	currentScreen = SCREEN_LOGO;

	// TODO: Load resources / Initialize variables at this point

	// Render texture to draw, enables screen scaling
	// NOTE: If screen is scaled, mouse input should be scaled proportionally
	target = LoadRenderTexture(screenWidth, screenHeight);
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
	emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
	SetTargetFPS(60);     // Set our game frames-per-second
	//--------------------------------------------------------------------------------------


	// Main game loop
	while (!WindowShouldClose() && currentScreen != SCREEN_EXIT)    // Detect window close button
	{
		switch (currentScreen) {
		case SCREEN_LOGO:
			if (IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP))
			{
				currentScreen = SCREEN_GAMEPLAY;
			}
			break;
		case SCREEN_GAMEPLAY:
			currentScreen = SCREEN_GAMEPLAY;
			if (IsKeyPressed(KEY_Q) || IsGestureDetected(GESTURE_TAP)) currentScreen = SCREEN_EXIT;
			break;
		}
		UpdateDrawFrame();
	}
#endif

	// De-Initialization
	//--------------------------------------------------------------------------------------
	UnloadRenderTexture(target);

	// TODO: Unload all loaded resources at this point

	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}

//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
	// Update
	//----------------------------------------------------------------------------------
	// TODO: Update variables / Implement example logic at this point

	frameCounter++;
	//----------------------------------------------------------------------------------

	// Draw
	//----------------------------------------------------------------------------------

	// Render to screen (main framebuffer)
	BeginDrawing();
	// Render game screen to a texture, 
	// it could be useful for scaling or further shader postprocessing
	BeginTextureMode(target);
	switch (currentScreen) {
	case SCREEN_LOGO:
		DrawMenu();
		break;
	case SCREEN_GAMEPLAY:
		DrawGame();
		break;
	}
	EndTextureMode();

	// Draw render texture to screen, scaled if required
	DrawTexturePro(target.texture, (Rectangle) { 0, 0, (float)target.texture.width, -(float)target.texture.height },
		(Rectangle) {
		0, 0, (float)target.texture.width, (float)target.texture.height
	}, (Vector2) { 0, 0 }, 0.0f, WHITE);

	// TODO: Draw everything that requires to be drawn at this point, maybe UI?

	EndDrawing();
	//----------------------------------------------------------------------------------  
}

void DrawMenu(void) {
	ClearBackground(WHITE); // thank you @mapleredleaf99 for the fix
	printf("Drawing Menu\n");
	DrawText("Hexcore Operator", 70, 90 + 210, 60, MAROON);
	DrawText("a game by Blaise!", 460, 140 + 210, 20, BLACK);

	if ((frameCounter / 20) % 5) DrawText("ENTER TO START GAME", 40, 500, 50, BLACK);
	DrawText("Q TO QUIT GAME", 180, 550, 40, RED);

	DrawRectangleLinesEx((Rectangle) { 0, 0, screenWidth, screenHeight }, 16, BLACK);

}
void DrawGame(void) {
	ClearBackground(RAYWHITE);
	printf("Drawing Game\n");

	//Draw outer polygon -- this is what the player can't control, tries to matchs polygon count before it touches the player... 
	// merges if correct (gains point), looses point if not correct
	int outerPolyCount = RandInt(3, 7);
	int innerPolyCount = RandInt(3, 7);
	
	if (IsKeyPressed(KEY_SPACE)) innerPolyCount++;
	if (IsKeyPressed(KEY_SPACE) && IsKeyPressed(KEY_RIGHT_SHIFT)) innerPolyCount--;
	
	int outerRadius = (300 - (frameCounter));
	if (outerRadius < 0) outerRadius = 0; // reset radius of outer polygon go 0 to stop regrowth.
	
	int innerRadius = 80;
	DrawPoly((Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 2 }, outerPolyCount, outerRadius, 0, MAGENTA);


	// player block
	{
		//Draw inner polygon -- This is what the player controls
		DrawPoly((Vector2) { GetScreenWidth() / 2, GetScreenHeight() / 2 }, innerPolyCount, innerRadius, 0, RED);

		//draw player
		DrawCircle(GetScreenWidth() / 2, GetScreenHeight() / 2, 30, BLACK);
	}

	//DrawRectangleLinesEx((Rectangle) { 0, 0, screenWidth, screenHeight }, 16, BLACK);
}

int RandInt(int min, int max)
{
	int value = rand() % (max - min + 1) + min;
	return value;
}

int RandFloat(int min, int max)
{
	return 0;
}
