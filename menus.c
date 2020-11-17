#include <oslib/oslib.h>

#include "menus.h"
#include "vars.h"

void addMenuItem(char *Name){
	sprintf(menuItem[menuLen].Title, Name);
	menuLen++;
}

void clearMenu(){
	for(tmpNum = 0; tmpNum < MENU_MAX; tmpNum++){
		sprintf(menuItem[tmpNum].Title, "\0");
	}
}

void setupMenu(int menuID){
	show = menuID;
	menuLen = 0;
	selItem = 0;
	if(show == PR_MAIN_MENU){
		addMenuItem("Play Online");
		addMenuItem("Play Offline");
		addMenuItem("Settings");
		addMenuItem("Credits");
		addMenuItem("Exit Game");
	}else if(show == PR_SETT_MENU){
		addMenuItem("WiFi Settings");
		addMenuItem("Online Settings");
		addMenuItem("Save Settings");
	}else if(show == PR_WIFI_MENU){
		sprintf(buffer, "Connect To: %s", configs[selectedConfig].name);
		addMenuItem(buffer);
		if(oslGetAPState() == 4){
			addMenuItem("Disconnect");
		}else{
			addMenuItem("Connect");
		}
	}else if(show == PR_CREDIT){
		addMenuItem("Coded by Disconnected on Se7ensins!");
		addMenuItem("Check out my websites!");
		addMenuItem("isonation.net");
		addMenuItem("projectapex.org");
		addMenuItem("budgetwebhost.us");
	}else if(show == PR_ONLINE_SET){
		sprintf(buffer, "Name: %s", playerName);
		addMenuItem(buffer);
		addMenuItem("Change Name");
		sprintf(buffer, "Auto Connect: %s", wifiAuto?"On":"Off");
		addMenuItem(buffer);
	}else if(show == PR_GAME){
		sprintf(buffer, "Bet Amount: %d", betAmount);
		addMenuItem(buffer);
		sprintf(buffer, "Pig: %d", betPig + 1);
		addMenuItem(buffer);
		addMenuItem("Race!");
	}
}

void showMainMenu(){
	for(tmpNum = 0; tmpNum < menuLen; tmpNum++){
		if(tmpNum == selItem)
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
		else
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), NULL);
		width = oslGetStringWidth(menuItem[tmpNum].Title);
		oslDrawString((SCREEN_WIDTH - width) / 2, 50 + (20*tmpNum), menuItem[tmpNum].Title);
	}
}

void playGame(){
	for(tmpNum = 0; tmpNum < menuLen; tmpNum++){
		if(tmpNum == selItem)
			oslIntraFontSetStyle(font, 0.6, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
		else
			oslIntraFontSetStyle(font, 0.6, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), NULL);
		width = oslGetStringWidth(menuItem[tmpNum].Title);
		oslDrawString((SCREEN_WIDTH - width) / 2, 20 + (15*tmpNum), menuItem[tmpNum].Title);
	}
	oslIntraFontSetStyle(font, 0.6, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), NULL);
	sprintf(buffer, "Pig Coins: %d", pigCoins);
	width = oslGetStringWidth(buffer);
	oslDrawString((SCREEN_WIDTH - width) / 2, 20 + (15*menuLen), buffer);
	//Print winning pig!
	if(winPig > -1){
		sprintf(buffer, "Pig %d won the race!", winPig + 1);
		width = oslGetStringWidth(buffer);
		oslDrawString((SCREEN_WIDTH - width) / 2, 110 - osl_curFont->charHeight, buffer);
	}
	oslDrawImageXY(grass, 0, 110);
	oslDrawImageXY(finish, 435, 170);
	oslDrawImageXY(fence, 0, 120);
	oslDrawImageXY(Pig[0].pig, Pig[0].x, Pig[0].y);
	oslDrawImageXY(Pig[1].pig, Pig[1].x, Pig[1].y);
	oslDrawImageXY(Pig[2].pig, Pig[2].x, Pig[2].y);
	oslDrawImageXY(Pig[3].pig, Pig[3].x, Pig[3].y);
}

void showCredits(){
	for(tmpNum = 0; tmpNum < menuLen; tmpNum++){
		if(tmpNum == selItem)
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
		else
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), NULL);
		width = oslGetStringWidth(menuItem[tmpNum].Title);
		oslDrawString((SCREEN_WIDTH - width) / 2, 50 + (20*tmpNum), menuItem[tmpNum].Title);
	}
}

void showSettingsMenu(){
	for(tmpNum = 0; tmpNum < menuLen; tmpNum++){
		if(tmpNum == selItem)
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
		else
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), NULL);
		width = oslGetStringWidth(menuItem[tmpNum].Title);
		oslDrawString((SCREEN_WIDTH - width) / 2, 50 + (20*tmpNum), menuItem[tmpNum].Title);
	}
}

void showMenu(){
	for(tmpNum = 0; tmpNum < menuLen; tmpNum++){
		if(tmpNum == selItem)
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
		else
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), NULL);
		width = oslGetStringWidth(menuItem[tmpNum].Title);
		oslDrawString((SCREEN_WIDTH - width) / 2, 50 + (20*tmpNum), menuItem[tmpNum].Title);
	}
}

void showWifiMenu(){
	for(tmpNum = 0; tmpNum < menuLen; tmpNum++){
		if(tmpNum == selItem)
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 120, 187, 255), RGBA(0, 0, 0, 0), NULL);
		else
			oslIntraFontSetStyle(font, 0.7, RGBA(255, 255, 255, 255), RGBA(0, 0, 0, 0), NULL);
		width = oslGetStringWidth(menuItem[tmpNum].Title);
		oslDrawString((SCREEN_WIDTH - width) / 2, 50 + (20*tmpNum), menuItem[tmpNum].Title);
	}
}

int isMenu(){
	if(show == PR_MAIN_MENU || show == PR_SETT_MENU || show == PR_WIFI_MENU || show == PR_CREDIT || show == PR_ONLINE_SET || show == PR_GAME)
		return 1;
	return 0;
}
