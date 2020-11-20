#include <oslib/oslib.h>
#include <openssl/sha.h>

struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
struct timeval rVal;
int numconfigs = 0;
int runningFlag = 1;
OSL_IMAGE *grass = NULL;
OSL_IMAGE *fence = NULL;
OSL_IMAGE *finish = NULL;
OSL_IMAGE *BG = NULL;
char message[100] = "";
char buffer[100] = "";
int enabled = 1;
int show = 0;
int width = 0;
int menuLen = 0;
int tmpNum;
int tmpNum1;
OSL_FONT *font;
int selItem = 0;
int selectedConfig = 0;
int runRace = 0;
int winPig = -1;
int wifiAuto = 0;
char macAddr[12];
char myMac[6][10];
char finalMac[20];
int pigCoins = 0;
int betPig = -1;
int betAmount = 0;

//Online Variables
char playerName[15];
int onlineCoins = 0;
//End Online

int GetRandNum(int min, int max, int tmpNumVal)
{
    static SceKernelUtilsMt19937Context ctx;
    //static int count = -1;
    //if(count == -1)
    //{
		gettimeofday(&rVal, NULL);
        sceKernelUtilsMt19937Init(&ctx, (rVal.tv_usec * rVal.tv_sec) + (tmpNumVal * 10));
   // }
    //else if(count > 10)
    //{
        // should probably "free" the ctx resource, if it's possible
     //   gettimeofday(&rVal, NULL);
    //    sceKernelUtilsMt19937Init(&ctx, (rVal.tv_usec * rVal.tv_sec) + (tmpNumVal * 10));
    //    count = -1;
    //}
    //count++;
    u32 randNum = sceKernelUtilsMt19937UInt(&ctx);
    return (int)(min + randNum % max);
}

void readSettings(){
	FILE *settingsFile = fopen("Settings.txt", "r");
	int settingsLevel = 0;
	char * string1;
	char * string2;
	int charI = 0;
	if ( settingsFile != NULL ){
		char line [64];
		while(fgets(line, sizeof line, settingsFile) != NULL ){
			
			for(tmpNum = 0; tmpNum < 64; tmpNum++){
				if(line[tmpNum] == '\n' || line[tmpNum] == '\r'){
					line[tmpNum] = '\0';
				}
			}
			
			if(settingsLevel == 0){
				if(strcmp(line, "[WIFI]") == 0){
					settingsLevel = 1;
				}
			}else if(settingsLevel == 1){
				string1 = strchr(line, ':');
				if (string1 != NULL){
					charI = (int)(string1-line);
					if(strncmp(line, "WiFiAuto", charI-1)==0){
						strcpy(string1, string1+1);
						string2 = strchr(string1, ':');
						charI = (int)(string2-string1);
						strcpy(string2, string2+1);
						selectedConfig = atoi(string2);
						sprintf(string2, "\0");
						strncpy(string2, string1, charI);
						wifiAuto = atoi(string1+1);
					}
				}
			}
		}
		fclose(settingsFile);
	}else{
		sprintf(message, "Could not open settings file: Settings.txt");
	}
}

void getName(){
	if(oslGetAPState() == 4){
		char response[255];
		unsigned char hash[SHA_DIGEST_LENGTH];
		SHA1(finalMac, sizeof(finalMac), hash);
		sprintf(buffer, "getUser=%s", hash);
		oslNetPostForm("http://simoni.dev/Projects/PigRaces", buffer, response, 255);
		sprintf(playerName, "%s", response);
	}
}

void updateName(char *newName){
	if(oslGetAPState() == 4){
		char response[255];
		unsigned char hash[SHA_DIGEST_LENGTH];
		SHA1(finalMac, sizeof(finalMac), hash);
		sprintf(buffer, "updateName=%s&Name=%s", hash, newName);
		oslNetPostForm("http://simoni.dev/Projects/PigRaces", buffer, response, 255);
		if(strcmp(response, "Taken") == 0){
			sprintf(message, "Sorry but that name is taken!");
		}else{
			sprintf(playerName, "%s", response);
			sprintf(message, "Name changed to: %s!", response);
		}
	}
}

void getMoney(){
	if(oslGetAPState() == 4){
		char response[255];
		unsigned char hash[SHA_DIGEST_LENGTH];
		SHA1(finalMac, sizeof(finalMac), hash);
		sprintf(buffer, "getMoney=%s", hash);
		oslNetPostForm("http://simoni.dev/Projects/PigRaces", buffer, response, 255);
		pigCoins = atoi(response);
	}
}

void updateMoney(){
	if(oslGetAPState() == 4){
		char response[255];
		unsigned char hash[SHA_DIGEST_LENGTH];
		SHA1(finalMac, sizeof(finalMac), hash);
		sprintf(buffer, "updateMoney=%s&Money=%d", hash, pigCoins);
		oslNetPostForm("http://simoni.dev/Projects/PigRaces", buffer, response, 255);
		//sprintf(message, "%s", response);
	}
}

void formMacAddr(){
	sceWlanGetEtherAddr(&macAddr);
	unsigned short* saddr = (unsigned short*)macAddr;
	sprintf(buffer, "%2X", saddr[0]);
	sprintf(myMac[0], "%c%c:%c%c", buffer[2], buffer[3], buffer[0], buffer[1]);
	sprintf(buffer, "%2X", saddr[1]);
	sprintf(myMac[1], "%c%c:%c%c", buffer[2], buffer[3], buffer[0], buffer[1]);
	sprintf(buffer, "%2X", saddr[2]);
	sprintf(myMac[2], "%c%c:%c%c", buffer[2], buffer[3], buffer[0], buffer[1]);
	sprintf(finalMac, "%s:%s:%s", myMac[0], myMac[1], myMac[2]);
}

void saveSettings(){
	

	FILE *settingsFile = fopen("Settings.txt", "w+");
	if (settingsFile == NULL){
		sprintf(message, "Error opening Settings file!");
	}

	//Wifi Section
	const char *title = "[WIFI]";
	fprintf(settingsFile, "%s\n", title);
	fprintf(settingsFile, "WiFiAuto:%d:%d", wifiAuto, selectedConfig);

	fclose(settingsFile);


}
