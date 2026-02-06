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
	SC_BONJUMP,
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
};
void update(int* bonStates, int* state) {
	
	// bonnie shit
	if(state[ST_SCREEN] == SC_BONJUMP) {
		printf("holy jumpscare..");
		state[ST_SCREEN] = SC_DEAD;
	}
	else if(bonStates[1] > 0) {
		bonStates[1] -= 1;
	} else {
		bonStates[1] = GetRandomValue(100,1000);
		switch (bonStates[0]) {
			case C_STAGE:
				bonStates[0] = C_DININGROOM;
				break;
			case C_DININGROOM:
				bonStates[0] = GetRandomValue(C_PARTSNSERVICE, C_LEFTHALL1);
				break;
			case C_PARTSNSERVICE:
				bonStates[0] = C_DININGROOM;
				break;
			case C_LEFTHALL1:
				bonStates[0] = GetRandomValue(C_LEFTHALLCLOSET, C_LEFTHALL2);
				break;
			case C_LEFTHALLCLOSET:
				bonStates[0] = GetRandomValue(C_LEFTHALL1, C_LEFTHALL2);
				break;
			case C_LEFTHALL2:
				bonStates[0] = C_LEFTDOORWAY;
				break;
			case C_LEFTDOORWAY:
				if(state[ST_DOORL]) bonStates[0] = GetRandomValue(C_STAGE,C_LEFTHALL1);
				else state[ST_SCREEN] = SC_BONJUMP;
				break;
		}
	}
}

void drawScreen(int* state, int* bonStates) {
	BeginDrawing();
		ClearBackground(BLACK);
		if(state[ST_SCREEN] == SC_TITLE) {
			DrawText("FNaF Raylib Thing!!\n\n\n\n  Press Enter to Begin Night 1!", 10,10,20, WHITE);
		}
		else if(state[ST_SCREEN] == SC_OFFICE) {
			char string[24];
			snprintf(string, 24, "cam %i, tim %i, doorl %i", bonStates[0], bonStates[1], state[ST_DOORL]); 
			DrawText(string, 10, 10, 20, WHITE);
		}
	EndDrawing();
}

int main() {
	InitWindow(640,480,"FNaF");
	SetTargetFPS(30);
	
	//the enum STATESTUFF is to easier index this ↓↓ ☻
	int state[6] = { SC_TITLE, 0, C_STAGE, 100, false, false };
	
	//bonStates[0] is the current camera, bonStates[1] is his timer
	int bonStates[2] = { C_STAGE, 100 };
	while(!WindowShouldClose()) {
		if(state[ST_SCREEN] == SC_TITLE) {
			if(IsKeyPressed(KEY_ENTER)) state[ST_SCREEN] = SC_OFFICE;
		}
		else if(state[ST_SCREEN] == SC_OFFICE) {
			if(IsKeyPressed(KEY_LEFT)) {
				state[ST_DOORL] = !state[ST_DOORL];
			} else if(IsKeyPressed(KEY_RIGHT)) {
				state[ST_DOORR] = !state[ST_DOORR];
			}
			update(bonStates, state);
		}
		drawScreen(state, bonStates);
	}
}