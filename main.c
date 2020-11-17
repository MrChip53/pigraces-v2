#include <pspkernel.h>
#include <oslib/oslib.h>
#include <sys/time.h>
#include <openssl/rsa.h>

#include "wlan.h"
#include "vars.h"
#include "menus.h"

PSP_MODULE_INFO("Pig Races", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER | THREAD_ATTR_VFPU);
PSP_HEAP_SIZE_KB(6*1024);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TO-DO:
////Save Settings File -- Implemented; Adding Auto Connect Toggle
////Make game like PS3 with its online community
//////--"Social Hub" so to speak for homebrew
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Callbacks:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Exit callback */
int exit_callback(int arg1, int arg2, void *common) {
    runningFlag = 0;
    return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp) {
    int cbid;

    cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
    sceKernelRegisterExitCallback(cbid);
    sceKernelSleepThreadCB();
    return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void) {
    int thid = 0;
    thid = sceKernelCreateThread("update_thread", CallbackThread, 0x11, 0xFA0, PSP_THREAD_ATTR_USER, 0);
    if(thid >= 0)
        sceKernelStartThread(thid, 0, 0);
    return thid;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init OSLib:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int initOSLib(){
    oslInit(0);
    oslInitGfx(OSL_PF_8888, 1);
    oslInitAudio();
    oslSetQuitOnLoadFailure(1);
    oslSetKeyAutorepeatInit(40);
    oslSetKeyAutorepeatInterval(10);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Main:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(){
    SetupCallbacks();

    initOSLib();
    oslIntraFontInit(INTRAFONT_CACHE_MED);
    oslNetInit();

	sprintf(playerName, "*Offline*");

	readSettings();
	if(wifiAuto)
		connectToAP(selectedConfig + 1);

	//Get MAC
	formMacAddr();
	
    //Loads image:
    BG = oslLoadImageFilePNG("BG.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    grass = oslLoadImageFilePNG("grass.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
    finish = oslLoadImageFilePNG("finish.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	fence = oslLoadImageFilePNG("fence.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	Pig[0].pig = oslLoadImageFilePNG("pig1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	Pig[1].pig = oslLoadImageFilePNG("pig1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	Pig[2].pig = oslLoadImageFilePNG("pig1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	Pig[3].pig = oslLoadImageFilePNG("pig1.png", OSL_IN_RAM | OSL_SWIZZLED, OSL_PF_8888);
	Pig[0].x = 0;
	Pig[1].x = 0;
	Pig[2].x = 0;
	Pig[3].x = 0;
	Pig[0].y = 170;
	Pig[1].y = 195;
	Pig[2].y = 220;
	Pig[3].y = 245;
	
    //Load font:
    font = oslLoadFontFile("flash0:/font/ltn0.pgf");
    
    oslSetFont(font);
	
	getConfigs();
	
	setupMenu(PR_MAIN_MENU);
	
	//Get Name(online)
	getName();
	
    while(runningFlag && !osl_quit){
		oslStartDrawing();
		oslDrawImageXY(BG, 0, 0);
		//Error message box
		if(strlen(message) > 0){
			int tmpNum = oslMessageBox(message, "Warning", oslMake3Buttons(OSL_KEY_SQUARE, OSL_MB_OK, OSL_KEY_TRIANGLE, OSL_MB_CANCEL, 0, 0));
			if(tmpNum == OSL_MB_CANCEL){
				setupMenu(PR_MAIN_MENU);
			}
			memset(message, 0, sizeof(message));
			for(tmpNum = 0; tmpNum < 5; tmpNum++){
				oslFlushKey();
			}
		}
		//End error box
		
		while(oslOskIsActive()){
			oslStartDrawing();
			oslClearScreen(RGBA(0,0,0,0));
			oslDrawOsk();
			if (oslGetOskStatus() == PSP_UTILITY_DIALOG_NONE){
				if (oslOskGetResult() == OSL_OSK_CANCEL){
					sprintf(message, "Cancel");
				}else{
					char userText[100] = "";
					oslOskGetText(userText);
					if((strlen(userText) < 16) && (strcmp(userText, playerName) != 0)){
						updateName(userText);
					}else if(strlen(userText) >= 16){
						sprintf(message, "Name too long!");
					}
				}
				oslEndOsk();
			}
			oslEndDrawing();
			oslEndFrame();
			oslSyncFrame();
		}
		
		//Draw menu/game graphics
        switch(show){
			case PR_MAIN_MENU:
				showMainMenu();
				break;
			case PR_CREDIT:
				showCredits();
				break;
			case PR_SETT_MENU:
				showSettingsMenu();
				break;
			case PR_WIFI_MENU:
				if(!numconfigs){
					sprintf(message, "No WiFi Configs!");
				}else if (!oslIsWlanPowerOn()){
					sprintf(message, "Please turn on the WLAN.");
				}else{
					showWifiMenu();
				}
				break;
			case PR_ONLINE_SET:
				showMenu();
				break;
			case PR_GAME:
				playGame();
				break;
		} 
		
		//Race logic
		if(runRace){
			for(tmpNum = 0; tmpNum < 4; tmpNum++){
				if(!runRace)
					continue;
				//srand(rVal.tv_usec * rVal.tv_sec);
				//srand(rand()%300);
				//tmpNum1 = rand()%6;
				tmpNum1 = GetRandNum(-1, 9, tmpNum);
				if(tmpNum1 < 0)
					tmpNum1 = 0;
				Pig[tmpNum].x += tmpNum1;
				if(Pig[tmpNum].x >= 435){
					winPig = tmpNum;
					if(winPig == betPig)
						pigCoins += (betAmount*2);
					updateMoney();
					runRace = false;
				}
			}
		}
		
		//Credits
		if(show != PR_GAME){
			width = oslGetStringWidth("(c) 2014 ProjectApex");
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
			oslDrawString((SCREEN_WIDTH - width) / 2, SCREEN_HEIGHT - osl_curFont->charHeight, "(c) 2014 ProjectApex");
		}
		
		sprintf(buffer, "Welcome, %s!", playerName);
		width = oslGetStringWidth(buffer);
		oslIntraFontSetStyle(font, 0.7, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
		oslDrawString((SCREEN_WIDTH - width) / 2, 2, buffer);
		
		//Handle buttons
		oslReadKeys();
		if(!runRace){
			if (osl_keys->released.cross){
				if(show == PR_MAIN_MENU){
					if(strcmp(menuItem[selItem].Title, "Play Online") == 0){
						if(oslGetAPState() == 4){
							getMoney();
							setupMenu(PR_GAME);
						}else{
							sprintf(message, "You need to be online!");
						}
					}else if(strcmp(menuItem[selItem].Title, "Play Offline") == 0){
						sprintf(message, "Not yet implemented!");	
					}else if(strcmp(menuItem[selItem].Title, "Settings") == 0){
						setupMenu(PR_SETT_MENU);
					}else if(strcmp(menuItem[selItem].Title, "Credits") == 0){
						setupMenu(PR_CREDIT);
					}else if(strcmp(menuItem[selItem].Title, "Exit Game") == 0){
						runningFlag = 0;
					}
				}else if(show == PR_SETT_MENU){
					if(strcmp(menuItem[selItem].Title, "WiFi Settings") == 0){
						setupMenu(PR_WIFI_MENU);
					}else if(strcmp(menuItem[selItem].Title, "Online Settings") == 0){
						setupMenu(PR_ONLINE_SET);
					}else if(strcmp(menuItem[selItem].Title, "Save Settings") == 0){
						saveSettings();
					}
				}else if(show == PR_WIFI_MENU){
					if(selItem == 1){
						if(oslGetAPState() == 4){
							oslDisconnectFromAP();
							sprintf(menuItem[1].Title, "Connect");
						}else{
							connectToAP(selectedConfig + 1);
							if(oslGetAPState() == 4){
								getName();
							}
						}
					}
				}else if(show == PR_GAME){
					if(strcmp(menuItem[selItem].Title, "Race!") == 0){
						pigCoins -= betAmount;
						Pig[0].x = 0;
						Pig[1].x = 0;
						Pig[2].x = 0;
						Pig[3].x = 0;
						winPig = -1;
						runRace = true;
					}
				}else if(show == PR_ONLINE_SET){
					if(strcmp(menuItem[selItem].Title, "Change Name") == 0){
						oslInitOsk("Type your desired name!", playerName, 128, 1, -1);
					}
				}
			}else if (osl_keys->released.circle){
				if(show == PR_GAME){
					setupMenu(PR_MAIN_MENU);
				}else if(show == PR_SETT_MENU){
					setupMenu(PR_MAIN_MENU);
				}else if(show == PR_CREDIT){
					setupMenu(PR_MAIN_MENU);
				}else if(show == PR_WIFI_MENU){
					setupMenu(PR_SETT_MENU);
				}else if(show == PR_ONLINE_SET){
					setupMenu(PR_SETT_MENU);
				}
			}else if (osl_keys->released.up){
				if(isMenu()){
					selItem--;
					if(selItem < 0)
						selItem = menuLen-1;
				}
			}else if (osl_keys->released.down){
				if(isMenu()){
					selItem++;
					if(selItem > menuLen-1)
						selItem = 0;
				}
			}else if(osl_keys->released.left){
				if(show == PR_WIFI_MENU && selItem == 0){
					if (--selectedConfig < 0)
						selectedConfig = numconfigs - 1;
					sprintf(menuItem[selItem].Title, "Connect To: %s", configs[selectedConfig].name);
				}else if(show == PR_GAME){
					if(selItem == 0){
						betAmount -= 10;
						if(betAmount < 0)
							betAmount = 0;
						sprintf(menuItem[selItem].Title, "Bet Amount: %d", betAmount);
					}else if(selItem == 1){
						betPig -= 1;
						if(betPig < 0)
							betPig = 0;
						sprintf(menuItem[selItem].Title, "Pig: %d", betPig + 1);
					}
				}
			}else if(osl_keys->released.right){
				if(show == PR_WIFI_MENU && selItem == 0){
					if (++selectedConfig >= numconfigs)
						selectedConfig = 0;
					sprintf(menuItem[selItem].Title, "Connect To: %s", configs[selectedConfig].name);
				}else if(show == PR_GAME){
					if(selItem == 0){
						betAmount += 10;
						if(betAmount > pigCoins)
							betAmount = pigCoins;
						sprintf(menuItem[selItem].Title, "Bet Amount: %d", betAmount);
					}else if(selItem == 1){
						betPig += 1;
						if(betPig > 3)
							betPig = 3;
						sprintf(menuItem[selItem].Title, "Pig: %d", betPig + 1);
					}
				}
			}
		}
		
		//End frame
		oslEndDrawing();
		
		oslEndFrame();
        oslSyncFrame();
    }
    //oslNetSocketClose(Socket);
    //Quit OSL:
    oslNetTerm();
    oslEndGfx();
    sceKernelExitGame();
    return 0;

}
