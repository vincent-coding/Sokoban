#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>

#include <unistd.h>
#include <fcntl.h>
#include "dynamic_libs/os_functions.h"
#include "dynamic_libs/fs_functions.h"
#include "dynamic_libs/gx2_functions.h"
#include "dynamic_libs/sys_functions.h"
#include "dynamic_libs/vpad_functions.h"
#include "dynamic_libs/padscore_functions.h"
#include "dynamic_libs/socket_functions.h"
#include "dynamic_libs/ax_functions.h"
#include "fs/fs_utils.h"
#include "fs/sd_fat_devoptab.h"
#include "system/memory.h"
#include "utils/logger.h"
#include "utils/utils.h"
#include "common/common.h"

#include "draw.h"

char *version = "0.1a";
int level = 1;
int maxlevel = 4;

int x; int y;
int crateX1; int crateY1;
int crateX2; int crateY2;
int goalX1; int goalY1;
int goalX2; int goalY2;

void drawFixedText() {
	char title[64];
	char msg[64];
	__os_snprintf(title, 64, "-- Sokoban v%s - By VCoding --", version);
	__os_snprintf(msg, 64, "Level: %i", level);
	drawString(0, 0, title);
	drawString(42, 0, msg);
	drawString(32, 16, "Press + to reset the level");
	drawString(0, 17, "Press HOME to exit...");
	drawString(32, 17, "Use the directional arrows to move");
}

void drawEndGames() {
	fillScreen(52, 152, 219, 255);
	char title[64];
	__os_snprintf(title, 64, "-- Sokoban v%s - By VCoding --", version);
	drawString(0, 0, title);
	drawString(0, 8, "Oh no, you have finished all the levels!");
	drawString(0, 9, "Stay tuned for new updates!");
	drawString(0, 17, "Press HOME to exit...");
	flipBuffers();
}

void levelObjectPosition() {
	if(level <= maxlevel) {
		if(level == 1) {
			x = 140; y = 240;
			crateX1 = 260; crateY1 = 280;
			crateX2 = 600; crateY2 = 160;
			goalX1 = 200; goalY1 = 200;
			goalX2 = 360; goalY2 = 340;
		} else if(level == 2) {
			x = 200; y = 340;
			crateX1 = 280; crateY1 = 160;
			crateX2 = 460; crateY2 = 260;
			goalX1 = 500; goalY1 = 320;
			goalX2 = 660; goalY2 = 140;
		} else if(level == 3) {
			x = 700; y = 120;
			crateX1 = 560; crateY1 = 280;
			crateX2 = 140; crateY2 = 140;
			goalX1 = 140; goalY1 = 340;
			goalX2 = 140; goalY2 = 320;
		} else if(level == 4) {
			x = 640; y = 320;
			crateX1 = 680; crateY1 = 320;
			crateX2 = 700; crateY2 = 300;
			goalX1 = 700; goalY1 = 120;
			goalX2 = 140; goalY2 = 120;
		}
		drawLevelDesign();
	} else {
		drawEndGames();
	}
}

void changeLevel() {
	if(level <= maxlevel) {
		fillScreen(52, 152, 219, 255);
		drawFixedText();
		drawString(0, 8, "Wow, congratulations, you've gone to the next level!");
		flipBuffers();
		os_sleep(2);
		level = level + 1;
		levelObjectPosition();
	} else {
		drawEndGames();
	}
};

void updateGame(int index) {
	if(index == 0) {
		x = x;
		y = y - 20;
		if(y < 120) {
			y = y + 20;
		}
		if(x == crateX1 && y == crateY1) {
			if(crateX1 == goalX1 && crateY1 == goalY1) {
				y = y + 20;
			} else if(crateX1 == goalX2 && crateY1 == goalY2) {
				y = y + 20;
			} else {
				// Walls Collisions
				crateY1 = crateY1 - 20;
				if(crateY1 < 120) {
					crateY1 = crateY1 + 20;
					y = y + 20;
				}
			}
		}
		if(x == crateX2 && y == crateY2) {
			if(crateX2 == goalX1 && crateY2 == goalY1) {
				y = y + 20;
			} else if(crateX2 == goalX2 && crateY2 == goalY2) {
				y = y + 20;
			} else {
				// Walls Collisions
				crateY2 = crateY2 - 20;
				if(crateY2 < 120) {
					crateY2 = crateY2 + 20;
					y = y + 20;
				}
			}
		}
	}
	if(index == 1) {
		x = x;
		y = y + 20;
		if(y > 340) {
			y = y - 20;
		}
		if(x == crateX1 && y == crateY1) {
			if(crateX1 == goalX1 && crateY1 == goalY1) {
				y = y - 20;
			} else if(crateX1 == goalX2 && crateY1 == goalY2) {
				y = y - 20;
			} else {
				// Walls Collisions
				crateY1 = crateY1 + 20;
				if(crateY1 > 340) {
					crateY1 = crateY1 - 20;
					y = y - 20;
				}
			}
		}
		if(x == crateX2 && y == crateY2) {
			if(crateX2 == goalX1 && crateY2 == goalY1) {
				y = y - 20;
			} else if(crateX2 == goalX2 && crateY2 == goalY2) {
				y = y - 20;
			} else {
				// Walls Collisions
				crateY2 = crateY2 + 20;
				if(crateY2 > 340) {
					crateY2 = crateY2 - 20;
					y = y - 20;
				}
			}
		}
	}
	if(index == 2) {
		x = x - 20;
		y = y;
		if(x < 140) {
			x = x + 20;
		}
		if(x == crateX1 && y == crateY1) {
			if(crateX1 == goalX1 && crateY1 == goalY1) {
				x = x + 20;
			} else if(crateX1 == goalX2 && crateY1 == goalY2) {
				x = x + 20;
			} else {
				// Walls Collisions
				crateX1 = crateX1 - 20;
				if(crateX1 < 140) {
					crateX1 = crateX1 + 20;
					x = x + 20;
				}
			}
		}
		if(x == crateX2 && y == crateY2) {
			if(crateX2 == goalX1 && crateY2 == goalY1) {
				x = x + 20;
			} else if(crateX2 == goalX2 && crateY2 == goalY2) {
				x = x + 20;
			} else {
				// Wall Collisions
				crateX2 = crateX2 - 20;
				if(crateX2 < 140) {
					crateX2 = crateX2 + 20;
					x = x + 20;
				}
			}
		}
	}
	if(index == 3) {
		x = x + 20;
		y = y;
		if(x > 700) {
			x = x - 20;
		}
		if(x == crateX1 && y == crateY1) {
			if(crateX1 == goalX1 && crateY1 == goalY1) {
				x = x - 20;
			} else if(crateX1 == goalX2 && crateY1 == goalY2) {
				x = x - 20;
			} else {
				// Walls Collisions
				crateX1 = crateX1 + 20;
				if(crateX1 > 700) {
					crateX1 = crateX1 - 20;
					x = x - 20;
				}
			}
		}
		if(x == crateX2 && y == crateY2) {
			if(crateX2 == goalX1 && crateY2 == goalY1) {
				x = x - 20;
			} else if(crateX2 == goalX2 && crateY2 == goalY2) {
				x = x - 20;
			} else {
				// Walls Collisions
				crateX2 = crateX2 + 20;
				if(crateX2 > 700) {
					crateX2 = crateX2 - 20;
					x = x - 20;
				}
			}
		}
	}
	if(crateX1 == goalX1 && crateY1 == goalY1 && crateX2 == goalX2 && crateY2 == goalY2) {
		changeLevel();
	} else if(crateX1 == goalX2 && crateY1 == goalY2 && crateX2 == goalX1 && crateY2 == goalY1) {
		changeLevel();
	} else {
		drawLevelDesign();
		os_usleep(250000);
	}
}

void drawGameScene() {
	// Horizontal Walls
	drawFillRect(100, 80, 760, 120, 255, 255, 255, 255);
	drawFillRect(100, 360, 760, 400, 255, 255, 255, 255);
	// Vertical Walls
	drawFillRect(100, 80, 140, 360, 255, 255, 255, 255);
	drawFillRect(720, 80, 760, 360, 255, 255, 255, 255);
	// Game Background
	drawFillRect(140, 120, 720, 360, 214, 219, 223, 255);
}

void drawLevelDesign() {
	fillScreen(52, 152, 219, 255);
	drawFixedText();
	drawGameScene();

	char coordmessage[64];
	__os_snprintf(coordmessage, 64, "x: %i / y: %i", x, y);
	drawString(0, 16, coordmessage);

	drawFillRect(x, y, x + 20, y + 20, 142, 68, 173, 255);
	drawRect(goalX1, goalY1, goalX1 + 20, goalY1 + 20, 231, 76, 60, 255);
	drawRect(goalX2, goalY2, goalX2 + 20, goalY2 + 20, 231, 76, 60, 255);
	drawFillRect(crateX1, crateY1, crateX1 + 20, crateY1 + 20, 211, 84, 0, 255);
	drawFillRect(crateX2, crateY2, crateX2 + 20, crateY2 + 20, 211, 84, 0, 255);

	flipBuffers();
}

void clearFrameBuffers() {
	for(int i = 0; i < 2; i++) {
		fillScreen(0,0,0,0);
		flipBuffers();
	}
}

int Menu_Main(void)
{
	InitOSFunctionPointers();
	InitVPadFunctionPointers();
	memoryInitialize();

	OSScreenInit();

	VPADInit();

	int buf0_size = OSScreenGetBufferSizeEx(0);

	OSScreenSetBufferEx(0, (void *)0xF4000000);
	OSScreenSetBufferEx(1, (void *)0xF4000000 + buf0_size);

	OSScreenEnableEx(0, 1);
	OSScreenEnableEx(1, 1);

	clearFrameBuffers();
	// loadPreviousBackup();
	levelObjectPosition();

	int vpadError = -1;
	VPADData vpad_data;

	//drawLevelDesign();

	while(1) {
		VPADRead(0, &vpad_data, 1, &vpadError);

		u32 pressedBtns = vpad_data.btns_d | vpad_data.btns_h;

		if(pressedBtns & VPAD_BUTTON_UP) {
			updateGame(0);
		}
		if(pressedBtns & VPAD_BUTTON_DOWN) {
			updateGame(1);
		}
		if(pressedBtns & VPAD_BUTTON_LEFT) {
			updateGame(2);
		}
		if(pressedBtns & VPAD_BUTTON_RIGHT) {
			updateGame(3);
		}
		if(pressedBtns & VPAD_BUTTON_PLUS) {
			levelObjectPosition();
			//drawLevelDesign();
		}
		if(pressedBtns & VPAD_BUTTON_HOME) {
			break;
		}
	}

	clearFrameBuffers();
	// saveCurrentLevel();

	return 0;
}