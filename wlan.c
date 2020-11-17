#include <oslib/oslib.h>
#include <pspkernel.h>

#include "wlan.h"
#include "vars.h"

#define ADDRESS "www.google.com"

//Get Configs
void getConfigs(){
    numconfigs = oslGetNetConfigs(configs);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Connect to Access Point:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int connectAPCallback(int state){
    oslStartDrawing();
    sprintf(buffer, "%s.", buffer);
    oslDrawString(30, 200, buffer);
    oslEndDrawing();
    oslEndFrame();
    oslSyncFrame();

    return 0;
}


int connectToAP(int config){
    //oslStartDrawing();
    //oslDrawString(30, 200, "Connecting to AP...");
    //oslEndDrawing();
    //oslEndFrame();
    //oslSyncFrame();
	sprintf(buffer, "Connecting To WiFi.");
    int result = oslConnectToAP(config, 30, connectAPCallback);
    if (!result){
        sprintf(menuItem[1].Title, "Disconnect");
    }else{
        oslStartDrawing();
        sprintf(buffer, "Error connecting to AP!");
        oslDrawString(30, 200, buffer);
        oslEndDrawing();
        oslEndFrame();
        oslSyncFrame();
		sceKernelDelayThread(3*1000000);
    }
    return 0;
}
