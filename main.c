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
	SPR_TOGGLE0,
	SPR_TOGGLE1,
	SPR_BEGIN,
	SPR_CURSOR,
	SPR_QUIT,
	SPR_BLU,
	SPR_STAGE,
	SPR_LHALL1,
	SPR_PARTS,
	SPR_DEADPIXELS,
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
	ST_FLASHER,
	ST_MILITARYTIME,
	STATECOUNT
};
enum SCREENS {
	SC_TITLE,
	SC_OFFICE,
	SC_DEAD,
	SC_bluebJUMP,
	SC_WIN
};
enum playCAMERAS {
	P_PARTSNSERVICE,
	P_STAGE,
	P_LEFTHALL1,
	P_LEFTHALLCLOSET,
	P_LEFTHALL2
};
enum animCAMERAS {
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
	state[ST_FLASHER]++;
	if(state[ST_FLASHER] >= 30) state[ST_FLASHER] = 0;
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
			if(state[ST_MILITARYTIME]) DrawTexture(images[SPR_TOGGLE1],570,438,WHITE);
			else DrawTexture(images[SPR_TOGGLE0],570,438,WHITE);
			DrawText("Military\n  Time",584,443,10,WHITE);
		}
		else if(state[ST_SCREEN] == SC_OFFICE) {
			char string[64];
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
			if(state[ST_CAMTOG]) {
				switch (state[ST_CAM]) {
					case P_STAGE:
						DrawTexture(images[SPR_STAGE],0,0,WHITE);
						if(state[ST_FLASHER] < 15) {
							if(bluebStates[0] == C_STAGE) DrawTexture(images[SPR_BLU],172,42,WHITE);
							else if(bluebStates[0] == C_DININGROOM) DrawTexture(images[SPR_BLU],72,180,WHITE);
						}
						break;
					case P_LEFTHALL1:
						DrawTexture(images[SPR_LHALL1],0,0,WHITE);
						if(state[ST_FLASHER] < 15) {
							if(bluebStates[0] == C_LEFTHALL1) DrawTexture(images[SPR_BLU],280,29,WHITE);
						}
						break;
					case P_PARTSNSERVICE:
						DrawTexture(images[SPR_PARTS],0,0,WHITE);
						if(state[ST_FLASHER] < 15) {
							if(bluebStates[0] == C_PARTSNSERVICE) DrawTexture(images[SPR_BLU],350,180,WHITE);
						}
						break;
				}
				DrawTexture(images[SPR_DEADPIXELS],0,0,WHITE);
			}
			if(state[ST_MILITARYTIME]) {
				snprintf(string, 64, "0%i:00", state[ST_TIME] / 30 / 60); 
			} else {
				if(state[ST_TIME] / 300 == 0) snprintf(string, 64, "12 AM"); 
				else snprintf(string, 64, "%i AM", state[ST_TIME] / 30 / 60); 
			}
			DrawText(string, 575, 10, 20, WHITE);
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
		LoadTexture("sprites/SPR_TOGGLE0.png"),
		LoadTexture("sprites/SPR_TOGGLE1.png"),
		LoadTexture("sprites/SPR_BEGIN.png"),
		LoadTexture("sprites/SPR_CURSOR.png"),
		LoadTexture("sprites/SPR_QUIT.png"),
		LoadTexture("sprites/SPR_BLU.png"),
		LoadTexture("sprites/SPR_STAGE.png"),
		LoadTexture("sprites/SPR_LHALL1.png"),
		LoadTexture("sprites/SPR_PARTS.png"),
		LoadTexture("sprites/SPR_DEADPIXELS.png")
	};
	Sound sounds[SOUNDCOUNT] = {
		LoadSound("sounds/recording-electric-hum-whining.wav"),
		LoadSound("sounds/SND_DOORSLAM.wav"),
		LoadSound("sounds/SND_DOOROPEN.wav")
	};
	int mouseX;
	int mouseY;
	//the enum STATESTUFF is to easier index this ↓↓ ☻
	int state[STATECOUNT] = { SC_TITLE, 0, P_STAGE, 100, false, false, false, false, 0, 0, true };
	
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
			} else if(IsMouseButtonPressed(0) && mouseX >= 570 && mouseX <= 630 && mouseY >= 438 && mouseY <= 470) {
				state[ST_MILITARYTIME] = !state[ST_MILITARYTIME];
			}
		}
		else if(state[ST_SCREEN] == SC_OFFICE) {
			if(state[ST_TIME] < 10800) state[ST_TIME]++;
			if(!IsSoundPlaying(sounds[SND_ELECHUM])) PlaySound(sounds[SND_ELECHUM]);
			if(!state[ST_CAMTOG]) {
				if(IsKeyPressed(KEY_LEFT)) {
					if(state[ST_DOORL]) PlaySound(sounds[SND_DOORSLAM]);
					else PlaySound(sounds[SND_DOOROPEN]);
					state[ST_DOORL] = !state[ST_DOORL];
				} else if(IsKeyPressed(KEY_RIGHT)) {
					if(state[ST_DOORR]) PlaySound(sounds[SND_DOORSLAM]);
					else PlaySound(sounds[SND_DOOROPEN]);
					state[ST_DOORR] = !state[ST_DOORR];
				} else if(IsKeyPressed(KEY_F)) {
					state[ST_LIGHT] = !state[ST_LIGHT];
				}
			} else {
				if(IsKeyPressed(KEY_LEFT) && state[ST_CAM] > P_PARTSNSERVICE) {
					state[ST_CAM]--;
				} else if(IsKeyPressed(KEY_RIGHT)) {
					state[ST_CAM]++;
				}
				
			}
			if(IsKeyPressed(KEY_SPACE)) {
				state[ST_CAMTOG] = !state[ST_CAMTOG];
			}
			update(bluebStates, state);
		}
		drawScreen(state, bluebStates, images);
	}
	CloseAudioDevice();
	CloseWindow();
	return 0;
}