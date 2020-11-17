#define PR_MAIN_MENU 0
#define PR_GAME 1
#define PR_CREDIT 2
#define PR_SETT_MENU 3
#define PR_WIFI_MENU 4
#define PR_ONLINE_SET 5

#define false 0
#define true 1
#define FALSE 0
#define TRUE 1

#define MENU_MAX 20

#define PINK RGBA(255, 120, 187, 255)

#define SCREEN_HEIGHT 272
#define SCREEN_WIDTH 480

typedef struct Menu{
	char Title[100];
} Menu;

typedef struct Pigs{
	OSL_IMAGE *pig;
	int x;
	int y;
} Pigs;

extern int runningFlag;
extern struct timeval rVal;
extern OSL_IMAGE *grass;
extern OSL_IMAGE *fence;
extern OSL_IMAGE *finish;
extern OSL_IMAGE *BG;
extern char message[100];
extern char buffer[100];
extern int numconfigs;
extern struct oslNetConfig configs[OSL_MAX_NET_CONFIGS];
extern int enabled;
extern int show;
extern int width;
Menu menuItem[MENU_MAX];
Pigs Pig[4];
extern int menuLen;
extern int tmpNum;
extern int tmpNum1;
extern int selItem;
extern OSL_FONT *font;
extern int selectedConfig;
extern int runRace;
extern int winPig;
extern int wifiAuto;
extern char macAddr[12];
extern char finalMac[20];
extern int pigCoins;
extern int betPig;
extern int betAmount;

//Online
extern char playerName[15];
extern int onlineCoins;

//Functions
extern int GetRandNum(int min, int max, int tmpNumVal);
extern void readSettings();
extern void getName();
extern void getMoney();
extern void updateName(char *newName);
extern void formMacAddr();
//extern void setupPigs();
