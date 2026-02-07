#include <stdio.h>
#include <raylib.h>
#include <string.h>

enum SND_STUFF {
	SND_ELECHUM,
	SND_DOORSLAM,
	SND_DOOROPEN,
	SOUNDCOUNT
};
enum SPR_STUFF {
	SPR_OFFICE,
	SPR_TITLE,
	SPR_BEGIN,
	SPR_CURSOR,
	SPR_QUIT,
	SPRITECOUNT
};
enum STATESTUFF {
	ST_SCREEN,
	ST_NIGHT,
	ST_CAM,
	ST_POWER,
	ST_DOORL,
	ST_DOORR,
	ST_CAMTOG,
	ST_LIGHT,
	ST_TIME,
	STATECOUNT
};
enum SCREENS {
	SC_TITLE,
	SC_OFFICE,
	SC_DEAD,
	SC_bluebJUMP,
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
int getPowerUsage(int* state) {
	int powerUsage = 0 + ST_LIGHT + ST_DOORR + ST_DOORL + ST_CAMTOG;
	return powerUsage;
}

void update(int* bluebStates, int* state) {
	
	// BlueBeary shit
	if(state[ST_SCREEN] == SC_bluebJUMP) {
		printf("holy jumpscare..");
		state[ST_SCREEN] = SC_DEAD;
	}
	else if(bluebStates[1] > 0) {
		bluebStates[1] -= 1;
	} else {
		bluebStates[1] = GetRandomValue(100,1000);
		switch (bluebStates[0]) {
			case C_STAGE:
				bluebStates[0] = C_DININGROOM;
				break;
			case C_DININGROOM:
				bluebStates[0] = GetRandomValue(C_PARTSNSERVICE, C_LEFTHALL1);
				break;
			case C_PARTSNSERVICE:
				bluebStates[0] = C_DININGROOM;
				break;
			case C_LEFTHALL1:
				bluebStates[0] = GetRandomValue(C_LEFTHALLCLOSET, C_LEFTHALL2);
				break;
			case C_LEFTHALLCLOSET:
				bluebStates[0] = GetRandomValue(C_LEFTHALL1, C_LEFTHALL2);
				break;
			case C_LEFTHALL2:
				bluebStates[0] = C_LEFTDOORWAY;
				break;
			case C_LEFTDOORWAY:
				if(state[ST_DOORL]) bluebStates[0] = GetRandomValue(C_STAGE,C_LEFTHALL1);
				else state[ST_SCREEN] = SC_bluebJUMP;
				break;
		}
	}
}

void drawScreen(int* state, int* bluebStates, Texture2D* images) {
	BeginDrawing();
		ClearBackground(BLACK);
		if(state[ST_SCREEN] == SC_TITLE) {
			DrawTexture(images[SPR_TITLE],25,20,WHITE);
			DrawTexture(images[SPR_BEGIN],35,185,WHITE);
			DrawTexture(images[SPR_QUIT],35,220,WHITE);
		}
		else if(state[ST_SCREEN] == SC_OFFICE) {
			char string[40];
			DrawTexture(images[SPR_OFFICE],0,0,WHITE);
			char room[24];
			switch (bluebStates[0]) {
				case C_STAGE:
					strcpy(room, "C_STAGE");
					break;
				case C_DININGROOM:
					strcpy(room, "C_DININGROOM");
					break;
				case C_PARTSNSERVICE:
					strcpy(room, "C_PARTSNSERVICE");
					break;
				case C_LEFTHALL1:
					strcpy(room, "C_LEFTHALL1");
					break;
				case C_LEFTHALL2:
					strcpy(room, "C_LEFTHALL2");
					break;
				case C_LEFTDOORWAY:
					strcpy(room, "C_LEFTDOORWAY");
					break;
			}
			if(state[ST_LIGHT]) {
				DrawText("light", GetMouseX() - MeasureText("light",20) / 2,GetMouseY() - 10,20,WHITE);
			}
			snprintf(string, 40, "cam %s, tim %i, doorl %i, %i:00", room, bluebStates[1], state[ST_DOORL], state[ST_TIME] / 300); 
			DrawText(string, 10, 10, 20, WHITE);
		}
		DrawTexture(images[SPR_CURSOR], GetMouseX(), GetMouseY(), WHITE);
	EndDrawing();
}

int main() {
	InitWindow(640,480,"FNaF");
	InitAudioDevice();
	SetTargetFPS(30);
	HideCursor();
	Texture2D images[SPRITECOUNT] = {
		LoadTexture("sprites/SPR_OFFICE.png"),
		LoadTexture("sprites/SPR_TITLE.png"),
		LoadTexture("sprites/SPR_BEGIN.png"),
		LoadTexture("sprites/SPR_CURSOR.png"),
		LoadTexture("sprites/SPR_QUIT.png")
	};
	Sound sounds[SOUNDCOUNT] = {
		LoadSound("sounds/recording-electric-hum-whining.wav"),
		LoadSound("sounds/SND_DOORSLAM.wav"),
		LoadSound("sounds/SND_DOOROPEN.wav")
	};
	int mouseX;
	int mouseY;
	//the enum STATESTUFF is to easier index this ↓↓ ☻
	int state[STATECOUNT] = { SC_TITLE, 0, C_STAGE, 100, false, false, false, false, 0 };
	
	//bluebStates[0] is the current camera, bluebStates[1] is his timer
	int bluebStates[2] = { C_STAGE, 100 };
	while(!WindowShouldClose()) {
		mouseX = GetMouseX();
		mouseY = GetMouseY();
		if(state[ST_SCREEN] == SC_TITLE) {
			//if(IsKeyPressed(KEY_ENTER)) state[ST_SCREEN] = SC_OFFICE;
			if(IsMouseButtonPressed(0) && mouseX >= 35 && mouseX <= 53 + 35 && mouseY >= 185 && mouseY <= 185 + 27) {
				state[ST_SCREEN] = SC_OFFICE;
			} else if(IsMouseButtonPressed(0) && mouseX >= 35 && mouseX <= 53 + 35 && mouseY >= 220 && mouseY <= 220 + 27) {
				CloseAudioDevice();
				CloseWindow();
				return 0;
			}
		}
		else if(state[ST_SCREEN] == SC_OFFICE) {
			if(state[ST_TIME] < 1800) state[ST_TIME]++;
			if(!IsSoundPlaying(sounds[SND_ELECHUM])) PlaySound(sounds[SND_ELECHUM]);
			if(IsKeyPressed(KEY_LEFT)) {
				if(state[ST_DOORL]) PlaySound(sounds[SND_DOORSLAM]);
				else PlaySound(sounds[SND_DOOROPEN]);
				state[ST_DOORL] = !state[ST_DOORL];
			} else if(IsKeyPressed(KEY_RIGHT)) {
				if(state[ST_DOORR]) PlaySound(sounds[SND_DOORSLAM]);
				else PlaySound(sounds[SND_DOOROPEN]);
				state[ST_DOORR] = !state[ST_DOORR];
			} else if(IsKeyPressed(KEY_SPACE)) {
				state[ST_CAMTOG] = !state[ST_CAMTOG];
			} else if(IsKeyPressed(KEY_F)) {
				state[ST_LIGHT] = !state[ST_LIGHT];
			}
			update(bluebStates, state);
		}
		drawScreen(state, bluebStates, images);
	}
	CloseAudioDevice();
	CloseWindow();
	return 0;
}