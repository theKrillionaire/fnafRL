#include <stdio.h>
#include <raylib.h>

enum STATESTUFF {
	ST_SCREEN,
	ST_NIGHT,
	ST_CAM,
	ST_POWER,
	ST_DOORL,
	ST_DOORR
};
enum SCREENS {
	SC_TITLE,
	SC_OFFICE,
	SC_CAMS,
	SC_DEAD,
	SC_WIN
};
enum CAMERAS {
	C_STAGE,
	C_DININGROOM,
	C_PARTSNSERVICE,
	C_LEFTHALL1,
	C_LEFTHALLCLOSET,
	C_LEFTHALL2,
	C_LEFTDOORWAY,
	C_RIGHTHALL1,
	C_RIGHTHALL2,
	C_RIGHTDOORWAY
}
void update(int* bonTimer) {
	
}

void drawScreen(int* state) {
	BeginDrawing();
		char string[15];
		snprintf(string, 15, "%i, %i, %i, %i", state[ST_SCREEN], state[ST_NIGHT], state[ST_CAM],  state[ST_POWER]); 
		DrawText(string, 10, 10, 20, WHITE);
	EndDrawing();
}

int main() {
	InitWindow(640,480,"FNaF");
	//the enum STATESTUFF is to easier index this ↓↓ ☻
	int state[4] = { SC_TITLE, 0, C_STAGE, 100, false, false };
	int bonSTATE[2] = { C_STAGE, 100 };
	while(!WindowShouldClose()) {
		drawScreen(state);
	}
}