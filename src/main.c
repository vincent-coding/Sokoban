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

char *version = "0.1b";
int level = 1;
int maxlevel = 6;
bool selectMenuOpen = false; 
bool devModEnable = false;
int currentMenuPosition;

int r_crate1, g_crate1, b_crate1;
int r_crate2, g_crate2, b_crate2;

int x, y;
int crateX1, crateY1, crateX2, crateY2;
int goalX1, goalY1, goalX2, goalY2;
int lavaX1, lavaY1, lavaX2, lavaY2, lavaX3, lavaY3, lavaX4, lavaY4, lavaX5, lavaY5, lavaX6, lavaY6, lavaX7, lavaY7, lavaX8, lavaY8, lavaX9, lavaY9, lavaX10, lavaY10;

void drawFixedText() {
	char title[64];
	char msg[64];
	__os_snprintf(title, 64, "-- Sokoban v%s - By VCoding --", version);
	__os_snprintf(msg, 64, "Level: %i", level);
	drawString(0, 0, title);
	if(devModEnable == true) {
		drawString(50, 0, msg);
	} else {
		drawString(42, 0, msg);
	}
	drawString(0, 16, "Press HOME to exit...");
	drawString(32, 16, "Use the directional arrows to move");
	drawString(0, 17, "Press SELECT to switch levels");
	drawString(32, 17, "Press START to reset the level");
}

void drawEndGames() {
	fillScreen(52, 152, 219, 255);
	char title[64];
	__os_snprintf(title, 64, "-- Sokoban v%s - By VCoding --", version);
	drawString(0, 0, title);
	drawString(0, 7, "Oh no, you have finished all the levels!");
	drawString(0, 8, "Stay tuned for new updates!");
	drawString(0, 10, "You can redo levels by pressing SELECT!");
	drawString(0, 17, "Press HOME to exit...");
	flipBuffers();
}

void levelObjectPosition() {
	if(level <= maxlevel) {
		r_crate1 = 245; g_crate1 = 176; b_crate1 = 65;
		r_crate2 = 245; g_crate2 = 176; b_crate2 = 65;
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
		} else if(level == 5) {
			x = 420; y = 220;
			crateX1 = 160; crateY1 = 140;
			crateX2 = 160; crateY2 = 320;
			goalX1 = 420; goalY1 = 240;
			goalX2 = 700; goalY2 = 220;
			lavaX1 = 420; lavaY1 = 120;
			lavaX2 = 420; lavaY2 = 140;
			lavaX3 = 420; lavaY3 = 160;
			lavaX4 = 420; lavaY4 = 180;
			lavaX5 = 420; lavaY5 = 200;
			lavaX6 = 420; lavaY6 = 260;
			lavaX7 = 420; lavaY7 = 280;
			lavaX8 = 420; lavaY8 = 300;
			lavaX9 = 420; lavaY9 = 320;
			lavaX10 = 420; lavaY10 = 340;
		} else if(level == 6) {
			x = 160; y = 320;
			crateX1 = 160; crateY1 = 300;
			crateX2 = 180; crateY2 = 320;
			goalX1 = 180; goalY1 = 260;
			goalX2 = 220; goalY2 = 280;
			lavaX1 = 140; lavaY1 = 260;
			lavaX2 = 200; lavaY2 = 340;
			lavaX3 = 160; lavaY3 = 260;
			lavaX4 = 200; lavaY4 = 320;
			lavaX5 = 180; lavaY5 = 240;
			lavaX6 = 200; lavaY6 = 260;
			lavaX7 = 220; lavaY7 = 260;
			lavaX8 = 220; lavaY8 = 300;
			lavaX9 = 240; lavaY9 = 280;
			lavaX10 = 140; lavaY10 = 320;
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

void drawGameOver() {
	fillScreen(52, 152, 219, 255);
	drawFixedText();
	drawString(0, 8, "Lmao, you lost! How can you lose on a game like that?");
	flipBuffers();
	os_sleep(2);
	levelObjectPosition();
}

void updateGame(int index) {
	if(selectMenuOpen == false) {
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
			// Prevents collision between the two boxes
			if(crateX1 == crateX2 && crateY1 == crateY2) {
				crateY1 = crateY1;
				crateY2 = crateY2 + 20;
				y = y + 20;
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
			// Prevents collision between the two boxes
			if(crateX1 == crateX2 && crateY1 == crateY2) {
				crateY1 = crateY1;
				crateY2 = crateY2 - 20;
				y = y - 20;
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
			// Prevents collision between the two boxes
			if(crateX1 == crateX2 && crateY1 == crateY2) {
				crateX1 = crateX1;
				crateX2 = crateX2 + 20;
				x = x + 20;
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
			// Prevents collision between the two boxes
			if(crateX1 == crateX2 && crateY1 == crateY2) {
				crateX1 = crateX1;
				crateX2 = crateX2 - 20;
				x = x - 20;
			}
		}
		
		// Lava collisions
		if(level >= 5) {
			if(x == lavaX1 && y == lavaY1) {drawGameOver();}
			if(x == lavaX2 && y == lavaY2) {drawGameOver();}
			if(x == lavaX3 && y == lavaY3) {drawGameOver();}
			if(x == lavaX4 && y == lavaY4) {drawGameOver();}
			if(x == lavaX5 && y == lavaY5) {drawGameOver();}
			if(x == lavaX6 && y == lavaY6) {drawGameOver();}
			if(x == lavaX7 && y == lavaY7) {drawGameOver();}
			if(x == lavaX8 && y == lavaY8) {drawGameOver();}
			if(x == lavaX9 && y == lavaY9) {drawGameOver();}
			if(x == lavaX10 && y == lavaY10) {drawGameOver();}

			if(crateX1 == lavaX1 && crateY1 == lavaY1) {drawGameOver();}
			if(crateX1 == lavaX2 && crateY1 == lavaY2) {drawGameOver();}
			if(crateX1 == lavaX3 && crateY1 == lavaY3) {drawGameOver();}
			if(crateX1 == lavaX4 && crateY1 == lavaY4) {drawGameOver();}
			if(crateX1 == lavaX5 && crateY1 == lavaY5) {drawGameOver();}
			if(crateX1 == lavaX6 && crateY1 == lavaY6) {drawGameOver();}
			if(crateX1 == lavaX7 && crateY1 == lavaY7) {drawGameOver();}
			if(crateX1 == lavaX8 && crateY1 == lavaY8) {drawGameOver();}
			if(crateX1 == lavaX9 && crateY1 == lavaY9) {drawGameOver();}
			if(crateX1 == lavaX10 && crateY1 == lavaY10) {drawGameOver();}

			if(crateX2 == lavaX1 && crateY2 == lavaY1) {drawGameOver();}
			if(crateX2 == lavaX2 && crateY2 == lavaY2) {drawGameOver();}
			if(crateX2 == lavaX3 && crateY2 == lavaY3) {drawGameOver();}
			if(crateX2 == lavaX4 && crateY2 == lavaY4) {drawGameOver();}
			if(crateX2 == lavaX5 && crateY2 == lavaY5) {drawGameOver();}
			if(crateX2 == lavaX6 && crateY2 == lavaY6) {drawGameOver();}
			if(crateX2 == lavaX7 && crateY2 == lavaY7) {drawGameOver();}
			if(crateX2 == lavaX8 && crateY2 == lavaY8) {drawGameOver();}
			if(crateX2 == lavaX9 && crateY2 == lavaY9) {drawGameOver();}
			if(crateX2 == lavaX10 && crateY2 == lavaY10) {drawGameOver();}
		}

		// Change crate color
			// Crate 1
		if(crateX1 == goalX1 && crateY1 == goalY1) {
			r_crate1 = 40; g_crate1 = 180; b_crate1 = 99;
		} else if(crateX1 == goalX2 && crateY1 == goalY2) {
			r_crate1 = 40; g_crate1 = 180; b_crate1 = 99;
		}
			// Crate 2
		if(crateX2 == goalX1 && crateY2 == goalY1) {
			r_crate2 = 40; g_crate2 = 180; b_crate2 = 99;
		} else if(crateX2 == goalX2 && crateY2 == goalY2) {
			r_crate2 = 40; g_crate2 = 180; b_crate2 = 99;
		}


		// Check End Level
		if(crateX1 == goalX1 && crateY1 == goalY1 && crateX2 == goalX2 && crateY2 == goalY2) {
			os_sleep(1);
			changeLevel();
		} else if(crateX1 == goalX2 && crateY1 == goalY2 && crateX2 == goalX1 && crateY2 == goalY1) {
			os_sleep(1);
			changeLevel();
		} else {
			drawLevelDesign();
			os_usleep(250000);
		}
	} else {
		if(index == 0) {
			currentMenuPosition = currentMenuPosition - 1;
			if(currentMenuPosition < 1) {currentMenuPosition = 1;}
		} else if(index == 1) {
			currentMenuPosition = currentMenuPosition + 1;
			if(currentMenuPosition > maxlevel){currentMenuPosition = maxlevel;}
		} else if(index == 2 || index == 3) {
			return;
		}
		drawSelectMenu();
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

	if(devModEnable == true) {
		char coordmessage[64];
		__os_snprintf(coordmessage, 64, "x: %i / y: %i", x, y);
		drawString(34, 0, coordmessage);
	}

	// Player
	drawFillRect(x, y, x + 20, y + 20, 142, 68, 173, 255);

	// Goal
	drawRect(goalX1, goalY1, goalX1 + 20, goalY1 + 20, 231, 76, 60, 255);
	drawRect(goalX2, goalY2, goalX2 + 20, goalY2 + 20, 231, 76, 60, 255);
	
	// Crate
	drawFillRect(crateX1, crateY1, crateX1 + 20, crateY1 + 20, r_crate1, g_crate1, b_crate1, 255);
	drawFillRect(crateX2, crateY2, crateX2 + 20, crateY2 + 20, r_crate2, g_crate2, b_crate2, 255);
	
	if(level >= 5) {
		// Lava
		drawFillRect(lavaX1, lavaY1, lavaX1 + 20, lavaY1 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX2, lavaY2, lavaX2 + 20, lavaY2 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX3, lavaY3, lavaX3 + 20, lavaY3 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX4, lavaY4, lavaX4 + 20, lavaY4 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX5, lavaY5, lavaX5 + 20, lavaY5 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX6, lavaY6, lavaX6 + 20, lavaY6 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX7, lavaY7, lavaX7 + 20, lavaY7 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX8, lavaY8, lavaX8 + 20, lavaY8 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX9, lavaY9, lavaX9 + 20, lavaY9 + 20, 231, 76, 60, 255);
		drawFillRect(lavaX10, lavaY10, lavaX10 + 20, lavaY10 + 20, 231, 76, 60, 255);
	}

	flipBuffers();
}

void clearFrameBuffers() {
	for(int i = 0; i < 2; i++) {
		fillScreen(0,0,0,0);
		flipBuffers();
	}
}

void drawSelectMenu() {
	fillScreen(52, 152, 219, 255);
	char title[64];
	__os_snprintf(title, 64, "-- Sokoban v%s - By VCoding --", version);
	drawString(0, 0, title);

	if(currentMenuPosition > maxlevel || currentMenuPosition < 1) {
		currentMenuPosition = 1;
	}
	
	for(int i = 1; i <= maxlevel; i++) {
		if(i == currentMenuPosition) {
			char levelMsg[64];
			__os_snprintf(levelMsg, 64, "-> Level %i", i);
			drawString(0, i + 1, levelMsg);
		} else {
			char levelMsg[64];
			__os_snprintf(levelMsg, 64, "   Level %i", i);
			drawString(0, i + 1, levelMsg);
		}
	}
	
	drawString(0, 17, "Press SELECT to close this menu");
	drawString(42, 17, "Press A to change levels");
	flipBuffers();
};

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
	
	levelObjectPosition();

	int vpadError = -1;
	VPADData vpad_data;

	while(1) {
		VPADRead(0, &vpad_data, 1, &vpadError);

		u32 pressedBtns = vpad_data.btns_d | vpad_data.btns_h;

		if(pressedBtns & VPAD_BUTTON_UP) {updateGame(0);}
		if(pressedBtns & VPAD_BUTTON_DOWN) {updateGame(1);}
		if(pressedBtns & VPAD_BUTTON_LEFT) {updateGame(2);}
		if(pressedBtns & VPAD_BUTTON_RIGHT) {updateGame(3);}
		if(pressedBtns & VPAD_BUTTON_PLUS) {levelObjectPosition();}

		if(pressedBtns & VPAD_BUTTON_A) {
			if(selectMenuOpen == true) {
				level = currentMenuPosition;
				selectMenuOpen = false;
				levelObjectPosition();
			}
		}

		if(pressedBtns & VPAD_BUTTON_MINUS) {
			if(selectMenuOpen == false) {
				currentMenuPosition = level;
				drawSelectMenu();
				selectMenuOpen = true;
			} else {
				levelObjectPosition();
				selectMenuOpen = false;
			}
			os_usleep(250000);
		}

		if(pressedBtns & VPAD_BUTTON_TV) {
			if(devModEnable == false) {
				devModEnable = true;
				drawLevelDesign(level);
			} else {
				devModEnable = false;
				drawLevelDesign();
			}
			os_usleep(250000);
		}

		if(pressedBtns & VPAD_BUTTON_HOME) {break;}
	}

	clearFrameBuffers();

	return 0;
}