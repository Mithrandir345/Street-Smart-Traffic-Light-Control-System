/**
Source Code: Street Smart Traffic Light Control System GUI
Created By: Anthony Dillon, Dayanara Serges, Christopher Ali
Course Project: ECET 494L
Date:10/18/2020
**/

#include <windows.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 640
#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3
HWND handle3;
SDL_Window * window;
SDL_Renderer * renderer;
TTF_Font * font;
SDL_Surface * textSurface;
SDL_Texture * textTexture;

// Begin Text Box For The North Light
std::string northCarCount = "0";
std::string northCarTimeStamp ="0";
SDL_Surface * northTextSurface[3];
SDL_Texture * northTextTexture[3];
SDL_Rect northTextRectSrc[3];
SDL_Rect northTextRectDst[3];
SDL_Rect northTextBox;
std::string northText[3];
std::string northColorState;
int nTextWidth,nTextHeight;
// End Text Box For The North Light
// Begin Text Box For The South Light
std::string southCarCount = "0";
std::string southCarTimeStamp ="0";
SDL_Surface * southTextSurface[3];
SDL_Texture * southTextTexture[3];
SDL_Rect southTextRectSrc[3];
SDL_Rect southTextRectDst[3];
SDL_Rect southTextBox;
std::string southText[3];
std::string southColorState;
int sTextWidth,sTextHeight;
// End Text Box For The South Light

// Begin Text Box For The West Light
std::string westCarCount = "0";
std::string westCarTimeStamp ="0";
SDL_Surface * westTextSurface[3];
SDL_Texture * westTextTexture[3];
SDL_Rect westTextRectSrc[3];
SDL_Rect westTextRectDst[3];
SDL_Rect westTextBox;
std::string westText[3];
std::string westColorState;
int wTextWidth,wTextHeight;
// End Text Box For The West Light

// Begin Text Box For The East Light
std::string eastCarCount = "0";
std::string eastCarTimeStamp ="0";
SDL_Surface * eastTextSurface[3];
SDL_Texture * eastTextTexture[3];
SDL_Rect eastTextRectSrc[3];
SDL_Rect eastTextRectDst[3];
SDL_Rect eastTextBox;
std::string eastText[3];
std::string eastColorState;
int eTextWidth,eTextHeight;
// End Text Box For The East Light

SDL_Rect textRectDst;
SDL_Rect textRectSrc;
SDL_Color color = {255,64,0};
SDL_Surface * spriteSurface;
SDL_Texture * spriteTexture;
SDL_Rect spriteRectDst[4];
SDL_Rect spriteRectSrc[4];

SDL_Rect GUI_Background_Dst;
SDL_Rect GUI_Background_Src;
SDL_Texture * GUI_Background_Tex;
SDL_Surface * GUI_Background_Sur;

const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000/ SCREEN_FPS;

std::string quitString="";
std::string msg1="";
BOOL   StatusRead; // Status
    char data[128] = { 0 }; //Buffer to receive data
    char  readData;        //data Character
    DWORD NoBytes;     // Bytes read by ReadFile()
    unsigned char loopData = 0;
    std::string readMessage="";
char code[64];
std::string accesscode = "2682209";
bool Exit1,quit = false;
bool isPromptAgain = true;
bool isEnteredSystem = false;
SDL_Event event;
UINT32 time;
UINT32 startTime = SDL_GetTicks();
UINT32 targetTime = 0;
UINT32 previousTime = 0;
UINT32 flashingTime = 0;
UINT32 elapsedTime = 0;
char bufferWrite[64]={0};
char cReset[] ="#RESET";
char cFlashing[] = "#FLASH";

bool isNorthandSouthGreen,isNorthandSouthYellow,isNorthandSouthRed = false;
bool isEastandWestGreen,isEastandWestYellow,isEastandWestRed = false;
bool isFlashingRed,isReset,isFlashing = false;

HANDLE hComm;  // Handle to the Serial port
    BOOL   Status; // Status
    DCB dcbSerialParams = { 0 };  // Initializing DCB structure
    COMMTIMEOUTS timeouts = { 0 };  //Initializing timeouts structure
    DWORD BytesWritten = 0;          // No of bytes written to the port
    DWORD dwEventMask;     // Event mask to trigger
    char  ReadData;        //temperory Character
    DWORD NoBytesRead;     // Bytes read by ReadFile()
    unsigned char loop = 0;
    char PortName[5]; //com port id

SDL_DisplayMode current;


int posBeginNorthCarCount;
int posBeginSouthCarCount;
int posBeginEastCarCount;
int posBeginWestCarCount;
int posEndCarCount;

int posBeginCommand;
int posEndCommand;
std::string carCountString;

//Function Declarations
void Animation(SDL_Rect &srcRect, int startFrame, int endFrame, int Speed);
void WriteMessage(HANDLE hComm, char message[]);
std::string ReadMessage(HANDLE hComm);
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_version version;
    SDL_GetVersion(&version);
    SDL_Log("SDL Version %d.%d.%d\n",version.major,version.minor,version.patch);
    SDL_Log("Platform: %s",SDL_GetPlatform());
    SDL_Log("Current Video Card Driver: %s",SDL_GetCurrentVideoDriver());
    SDL_Log("Display Monitor: %s",SDL_GetDisplayName(0));

     for(int i = 0; i < SDL_GetNumVideoDisplays(); ++i){

    int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);

    if(should_be_zero != 0)
      // In case of error...
      SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError());

    else
      // On success, print the current display mode.
      SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz.", i, current.w, current.h, current.refresh_rate);

  }
    while(isPromptAgain)
    {
    //Enter the com port id
    std::cout <<"\n\nEnter the Com Port:";
    std::cin >> PortName;
    std::cout << "\nPort Name Entered:" << PortName << std::endl;
    if(strcmp(PortName,"COM3")==0)
    {
        isPromptAgain = false;
    }else{std::cout << "\n\n\nPlease Enter A Valid Com Port Again... I.E.(COM1,COM2,COM3)" << std::endl;}
    }
    //Open the serial com port
    hComm = CreateFile(PortName, //friendly name
                       GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
                       0,                                 // No Sharing, ports cant be shared
                       NULL,                              // No Security
                       OPEN_EXISTING,                     // Open existing port only
                       0,                                 // Non Overlapped I/O
                       NULL);                             // Null for Comm Devices
    if (hComm == INVALID_HANDLE_VALUE)
    {
        std::cout << "\nPort" << PortName << " can't be opened\n\n";
        Exit1=true;
    }else{ std::cout << "Opening "<< PortName << " Port";}
    //Setting the Parameters for the SerialPort
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
    if (Status == FALSE)
    {
        std::cout << "\nError in getting "<< PortName << " Com state\n\n";
        Exit1 = true;
    }else{ std::cout << "\nSuccess in getting "<< PortName << " Com state";}
    dcbSerialParams.BaudRate = CBR_9600;      //BaudRate = 9600
    dcbSerialParams.ByteSize = DATABITS_8;             //ByteSize = 8 bits
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None
    Status = SetCommState(hComm, &dcbSerialParams);
    if (Status == FALSE)
    {
        std::cout <<"\nError to Setting DCB Structure\n\n";
        Exit1=true;
    }else{ std::cout << "\nSuccess in setting "<< PortName << " DCB Structure";}
    //Setting Timeouts
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (SetCommTimeouts(hComm, &timeouts) == FALSE)
    {
        std::cout <<"\nError to Setting Time outs";
        Exit1=true;
    }else{ std::cout << "\nSuccess in setting"<< PortName << " Timeouts";}
    if(!Exit1)
    {
        std::cout << "\nSuccess in Establishing Communication with "<< PortName << " Port!";
        WriteMessage(hComm,PortName);
        std::cout << "\n" << ReadMessage(hComm) << std::endl;
    }
    if(!Exit1)
    {
    while(!isEnteredSystem)
    {
    //Access Code
    if(!isEnteredSystem)
    {

    std::cout << "\nEnter Access Code:";
    std::cin >> code;
    }
    std::cout << code << std::endl;
    std::cout << "Number Of Bytes (Code):" << sizeof(code) << std::endl;
    WriteMessage(hComm,code);
    msg1 = ReadMessage(hComm);
    std::cout << "\nReceived Message:" << msg1 << std::endl;
    int posS = msg1.find("#SUCCESS!");
    std::string successCheck = msg1.substr(posS,9);
    if( successCheck == "#SUCCESS!")
    {
        std::cout << "\nEntering System...." << std::endl;
        isEnteredSystem = true;
    }else{std::cout << "\nPlease Enter Code Again..." << std::endl;}

    std::cout << "\n\n";
    }
    }



    if(Exit1)
    {
    std::cout << "\nExiting Before Main While Loop" <<std::endl;
    CloseHandle(hComm);//Closing the Serial Port
    quit=true;
    }


    window = SDL_CreateWindow("Street Smart Traffic Lighting System GUI",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_PRESENTVSYNC);
    SDL_GetCurrentDisplayMode(1, &current);
    GUI_Background_Sur = IMG_Load("Assets/GUIbackground.png");
    GUI_Background_Tex = SDL_CreateTextureFromSurface(renderer,GUI_Background_Sur);
    SDL_FreeSurface(GUI_Background_Sur);
    int w,h;
    SDL_QueryTexture(GUI_Background_Tex,nullptr,nullptr,&w,&h);
    GUI_Background_Dst.w=w;
    GUI_Background_Dst.h=h;
    SDL_Surface * SDLimage = IMG_Load("Assets/pointer.png");
    SDL_Cursor * cursor = SDL_CreateColorCursor(SDLimage,0,0);
    SDL_SetCursor(cursor);
    SDL_FreeSurface(SDLimage);
    SDLimage = IMG_Load("Assets/TextBox.png");
    SDL_Texture * TextBoxTexture = SDL_CreateTextureFromSurface(renderer,SDLimage);
    SDL_FreeSurface(SDLimage);
    font = TTF_OpenFont("Assets/Terminal.fon",16);
    textSurface = TTF_RenderText_Solid(font,"Street Smart Traffic Lighting System GUI",color);
    textTexture = SDL_CreateTextureFromSurface(renderer,textSurface);
    SDL_QueryTexture(textTexture,nullptr,nullptr,&textRectSrc.w,&textRectSrc.h);
    textRectDst={WINDOW_WIDTH/2 - (textRectSrc.w/2),0,textRectSrc.w,textRectSrc.h};


    // Defining North's TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            northText[i] ="North Light State:0";
            northTextSurface[i] = TTF_RenderText_Blended(font,northText[i].c_str(),color);
            northTextTexture[i] = SDL_CreateTextureFromSurface(renderer,northTextSurface[i]);
            SDL_QueryTexture(northTextTexture[i],nullptr,nullptr,&northTextRectSrc[i].w,&northTextRectSrc[i].h);
        }
        if(i == 1)
        {
            northText[i] ="Detect Time stamp:0";
            northTextSurface[i] = TTF_RenderText_Blended(font,northText[i].c_str(),color);
            northTextTexture[i] = SDL_CreateTextureFromSurface(renderer,northTextSurface[i]);
            SDL_QueryTexture(northTextTexture[i],nullptr,nullptr,&northTextRectSrc[i].w,&northTextRectSrc[i].h);
        }
        if(i == 2)
        {
            northText[i] ="Number Of Cars Detected:0";
            northTextSurface[i] = TTF_RenderText_Blended(font,northText[i].c_str(),color);
            northTextTexture[i] = SDL_CreateTextureFromSurface(renderer,northTextSurface[i]);
            SDL_QueryTexture(northTextTexture[i],nullptr,nullptr,&northTextRectSrc[i].w,&northTextRectSrc[i].h);
        }
        SDL_FreeSurface(northTextSurface[i]);
    }
    //End Defining North's TextBox Strings and Dimensions


    // Begin Defining South TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            southText[i] ="South Light State:"+southColorState;
            southTextSurface[i] = TTF_RenderText_Blended(font,southText[i].c_str(),color);
            southTextTexture[i] = SDL_CreateTextureFromSurface(renderer,southTextSurface[i]);
            SDL_QueryTexture(southTextTexture[i],nullptr,nullptr,&southTextRectSrc[i].w,&southTextRectSrc[i].h);
        }
        if(i == 1)
        {
            southText[i] ="Detect Time stamp:"+southCarTimeStamp;
            southTextSurface[i] = TTF_RenderText_Blended(font,southText[i].c_str(),color);
            southTextTexture[i] = SDL_CreateTextureFromSurface(renderer,southTextSurface[i]);
            SDL_QueryTexture(southTextTexture[i],nullptr,nullptr,&southTextRectSrc[i].w,&southTextRectSrc[i].h);
        }
        if(i == 2)
        {
            southText[i] ="Number Of Cars Detected:"+southCarCount;
            southTextSurface[i] = TTF_RenderText_Blended(font,southText[i].c_str(),color);
            southTextTexture[i] = SDL_CreateTextureFromSurface(renderer,southTextSurface[i]);
            SDL_QueryTexture(southTextTexture[i],nullptr,nullptr,&southTextRectSrc[i].w,&southTextRectSrc[i].h);
        }
        SDL_FreeSurface(southTextSurface[i]);
    }
    // End Defining South TextBox Strings and Dimensions

    // Begin Defining West TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            westText[i] ="West Light State:"+westColorState;
            westTextSurface[i] = TTF_RenderText_Blended(font,westText[i].c_str(),color);
            westTextTexture[i] = SDL_CreateTextureFromSurface(renderer,westTextSurface[i]);
            SDL_QueryTexture(westTextTexture[i],nullptr,nullptr,&westTextRectSrc[i].w,&westTextRectSrc[i].h);
        }
        if(i == 1)
        {
            westText[i] ="Detect Time stamp:"+westCarTimeStamp;
            westTextSurface[i] = TTF_RenderText_Blended(font,westText[i].c_str(),color);
            westTextTexture[i] = SDL_CreateTextureFromSurface(renderer,westTextSurface[i]);
            SDL_QueryTexture(westTextTexture[i],nullptr,nullptr,&westTextRectSrc[i].w,&westTextRectSrc[i].h);
        }
        if(i == 2)
        {
            westText[i] ="Number Of Cars Detected:"+westCarCount;
            westTextSurface[i] = TTF_RenderText_Blended(font,westText[i].c_str(),color);
            westTextTexture[i] = SDL_CreateTextureFromSurface(renderer,westTextSurface[i]);
            SDL_QueryTexture(westTextTexture[i],nullptr,nullptr,&westTextRectSrc[i].w,&westTextRectSrc[i].h);
        }
        SDL_FreeSurface(westTextSurface[i]);
    }
    // End Defining West TextBox Strings and Dimensions

     // Begin Defining East TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            eastText[i] ="East Light State:"+eastColorState;
            eastTextSurface[i] = TTF_RenderText_Blended(font,eastText[i].c_str(),color);
            eastTextTexture[i] = SDL_CreateTextureFromSurface(renderer,eastTextSurface[i]);
            SDL_QueryTexture(eastTextTexture[i],nullptr,nullptr,&eastTextRectSrc[i].w,&eastTextRectSrc[i].h);
        }
        if(i == 1)
        {
            eastText[i] ="Detect Time stamp:"+eastCarTimeStamp;
            eastTextSurface[i] = TTF_RenderText_Blended(font,eastText[i].c_str(),color);
            eastTextTexture[i] = SDL_CreateTextureFromSurface(renderer,eastTextSurface[i]);
            SDL_QueryTexture(eastTextTexture[i],nullptr,nullptr,&eastTextRectSrc[i].w,&eastTextRectSrc[i].h);
        }
        if(i == 2)
        {
            eastText[i] ="Number Of Cars Detected:"+eastCarCount;
            eastTextSurface[i] = TTF_RenderText_Blended(font,eastText[i].c_str(),color);
            eastTextTexture[i] = SDL_CreateTextureFromSurface(renderer,eastTextSurface[i]);
            SDL_QueryTexture(eastTextTexture[i],nullptr,nullptr,&eastTextRectSrc[i].w,&eastTextRectSrc[i].h);
        }
        SDL_FreeSurface(eastTextSurface[i]);
    }
    // End Defining East TextBox Strings and Dimensions



    spriteSurface = IMG_Load("Assets/LEDsprite.png");
    if(spriteSurface == nullptr)
    {
        std::cout << "Failed To Load Sprite" << std::endl;
    }
    spriteTexture = SDL_CreateTextureFromSurface(renderer,spriteSurface);
    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[NORTH].w,&spriteRectSrc[NORTH].h);
    spriteRectDst[NORTH]={WINDOW_WIDTH/2 - (spriteRectSrc[NORTH].w/2),WINDOW_HEIGHT/2 - (spriteRectSrc[NORTH].h/2)-128,128,128};

    // Begin Position North TextBox
    for(int i = 0; i<3; i++)
    {
    northTextBox.x = spriteRectDst[NORTH].x;
    northTextRectDst[i].x = northTextBox.x;
    northTextRectDst[i].w = northTextRectSrc[i].w;
    northTextRectDst[i].h = northTextRectSrc[i].h;
    northTextBox.h = northTextRectSrc[0].h + northTextRectSrc[1].h + northTextRectSrc[2].h;
    if(i==0)
    {
    northTextBox.y = spriteRectDst[NORTH].y-northTextBox.h;
    northTextRectDst[i].y = northTextBox.y;
    }
    if(i==1)
    {
    northTextRectDst[i].y = northTextBox.y + northTextRectDst[0].h;
    }
    if(i==2)
    {
    northTextRectDst[i].y = northTextBox.y + northTextRectDst[1].h+32;
    }
    northTextBox.w = std::max(northTextRectSrc[i].w,northTextRectSrc[0].w);
    }
    // End Position North TextBox



    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[SOUTH].w,&spriteRectSrc[SOUTH].h);
    spriteRectDst[SOUTH]={WINDOW_WIDTH/2 - (spriteRectSrc[SOUTH].w/2),WINDOW_HEIGHT/2 - (spriteRectSrc[SOUTH].h/2)+128,128,128};

    // Begin Position South TextBox
    for(int i = 0; i<3; i++)
    {
    southTextBox.x = spriteRectDst[SOUTH].x;
    southTextRectDst[i].x = southTextBox.x;
    southTextRectDst[i].w = southTextRectSrc[i].w;
    southTextRectDst[i].h = southTextRectSrc[i].h;
    southTextBox.h = southTextRectSrc[0].h + southTextRectSrc[1].h + southTextRectSrc[2].h;
    if(i==0)
    {
    southTextBox.y = spriteRectDst[SOUTH].y-southTextBox.h;
    southTextRectDst[i].y = southTextBox.y;
    }
    if(i==1)
    {
    southTextRectDst[i].y = southTextBox.y + southTextRectDst[0].h;
    }
    if(i==2)
    {
    southTextRectDst[i].y = southTextBox.y + southTextRectDst[1].h+32;
    }
    southTextBox.w = std::max(southTextRectSrc[i].w,southTextRectSrc[0].w);
    }
    // End Position South TextBox



    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[EAST].w,&spriteRectSrc[EAST].h);
    spriteRectDst[EAST]={WINDOW_WIDTH/2 + (spriteRectSrc[EAST].w/2)+128,WINDOW_HEIGHT/2 - (spriteRectSrc[EAST].h/2),128,128};


    // Begin Position East TextBox
    for(int i = 0; i<3; i++)
    {
    eastTextBox.x = spriteRectDst[EAST].x;
    eastTextRectDst[i].x = eastTextBox.x;
    eastTextRectDst[i].w = eastTextRectSrc[i].w;
    eastTextRectDst[i].h = eastTextRectSrc[i].h;
    eastTextBox.h = eastTextRectSrc[0].h + eastTextRectSrc[1].h + eastTextRectSrc[2].h;
    if(i==0)
    {
    eastTextBox.y = spriteRectDst[EAST].y-eastTextBox.h;
    eastTextRectDst[i].y = eastTextBox.y;
    }
    if(i==1)
    {
    eastTextRectDst[i].y = eastTextBox.y + eastTextRectDst[0].h;
    }
    if(i==2)
    {
    eastTextRectDst[i].y = eastTextBox.y + eastTextRectDst[1].h+32;
    }
    eastTextBox.w = std::max(eastTextRectSrc[i].w,eastTextRectSrc[0].w);
    }
    // End Position East TextBox



    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[WEST].w,&spriteRectSrc[WEST].h);
    spriteRectDst[WEST]={WINDOW_WIDTH/2 - (spriteRectSrc[WEST].w/2)-256,WINDOW_HEIGHT/2 - (spriteRectSrc[WEST].h/2),128,128};

    // Begin Position West TextBox
    for(int i = 0; i<3; i++)
    {
    westTextBox.x = spriteRectDst[WEST].x;
    westTextRectDst[i].x = westTextBox.x;
    westTextRectDst[i].w = westTextRectSrc[i].w;
    westTextRectDst[i].h = westTextRectSrc[i].h;
    westTextBox.h = westTextRectSrc[0].h + westTextRectSrc[1].h + westTextRectSrc[2].h;
    if(i==0)
    {
    westTextBox.y = spriteRectDst[WEST].y-westTextBox.h;
    westTextRectDst[i].y = westTextBox.y;
    }
    if(i==1)
    {
    westTextRectDst[i].y = westTextBox.y + westTextRectDst[0].h;
    }
    if(i==2)
    {
    westTextRectDst[i].y = westTextBox.y + westTextRectDst[1].h+32;
    }
    westTextBox.w = std::max(westTextRectSrc[i].w,westTextRectSrc[0].w);
    }
    // End Position West TextBox



    int countedFrames =0;

    while(isEnteredSystem && !quit)
    {
        time = SDL_GetTicks()-startTime;
        float avgFPS = countedFrames / (time/1000.0f);
        if(avgFPS > 2000000)
        {
            avgFPS=0;
        }
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                 std::cout << "Quitting Application" << std::endl;
                 quit=true;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    if(event.key.repeat == 0)
                    {
                       std::cout << "Pressed Escaped" << std::endl;
                       quit= true;
                    }
                    break;
                case SDLK_r:
                    if(event.key.repeat == 0)
                    {
                       std::cout << "Reset Initialized" << std::endl;
                       isReset = true;
                    }
                    break;
                case SDLK_f:
                    if(event.key.repeat == 0)
                    {
                       std::cout << "FLASHING RED INITIALIZED!" << std::endl;
                       isFlashing=true;
                    }
                    break;
                }
                break;
            }
        }
        if(isReset)
        {
            WriteMessage(hComm,cReset);
            isReset = false;
        }
        if(isFlashing)
        {
            WriteMessage(hComm,cFlashing);
            isFlashing=false;
        }
        std::string checkCommand;
    if(time > previousTime+500)
    {
     msg1 = ReadMessage(hComm);
     std::cout << "\n" << msg1 << std::endl;
     std::cout << "Size of msg1:" << msg1.size() << std::endl;
     posBeginNorthCarCount = msg1.find("~North:");
     posBeginSouthCarCount = msg1.find("~South:");
     posBeginEastCarCount = msg1.find("~East:");
     posBeginWestCarCount = msg1.find("~West:");

     posEndCarCount = msg1.find("$");

      if(posBeginNorthCarCount  != std::string::npos)
    {
    // Extract the North Car Count Up To 2 Digits Dynamically
    carCountString = msg1.substr(posBeginNorthCarCount+7,posEndCarCount-(posBeginNorthCarCount+7)-2);
    northCarCount=carCountString;
    southCarCount = northCarCount;
    }
    if(posBeginEastCarCount  != std::string::npos)
    {
    // Extract the East Car Count Up To 2 Digits Dynamically
    carCountString = msg1.substr(posBeginEastCarCount+6,posEndCarCount-(posBeginEastCarCount+6)-2);
    eastCarCount=carCountString;
    westCarCount=carCountString;
    }
    posBeginCommand = msg1.find("#");
    posEndCommand = msg1.find("!");
    while(posBeginCommand != std::string::npos && posEndCommand != std::string::npos)
    {
    checkCommand = msg1.substr(posBeginCommand+1, (posEndCommand-1)-posBeginCommand);
    std::cout << "COMMAND FOUND:" << checkCommand << std::endl;
    if(checkCommand == "NORTHGREEN")
    {
        northColorState = "GREEN";
        southColorState = "GREEN";
        isNorthandSouthGreen = true;
        isNorthandSouthRed = false;
        isNorthandSouthYellow = false;
        isFlashingRed =false;
    }
    if(checkCommand == "NORTHYELLOW")
    {
        northColorState = "YELLOW";
        southColorState = "YELLOW";
        isNorthandSouthYellow = true;
        isNorthandSouthGreen = false;
        isFlashingRed = false;
        isNorthandSouthRed = false;
    }
    if(checkCommand == "NORTHRED")
    {
        northColorState = "RED";
        southColorState = "RED";
        isNorthandSouthRed = true;
        isNorthandSouthYellow = false;
        isNorthandSouthGreen = false;
        isFlashingRed = false;
    }
    if(checkCommand == "EASTGREEN")
    {
        eastColorState = "GREEN";
        westColorState = "GREEN";
        isEastandWestGreen = true;
        isEastandWestRed = false;
        isEastandWestYellow = false;
        isFlashingRed =false;
    }
    if(checkCommand == "EASTYELLOW")
    {
        eastColorState = "YELLOW";
        westColorState = "YELLOW";
        isEastandWestGreen = false;
        isEastandWestRed = false;
        isEastandWestYellow = true;
        isFlashingRed =false;
    }

    if(checkCommand == "EASTRED")
    {
        eastColorState = "RED";
        westColorState = "RED";
        isEastandWestGreen = false;
        isEastandWestRed = true;
        isEastandWestYellow = false;
        isFlashingRed =false;
    }





    if(checkCommand == "FLASHINGRED")
    {
        std::cout << "\nLights Are Flashing Red!" << std::endl;
        northColorState = "FLASHING RED";
        southColorState = "FLASHING RED";
        eastColorState = "FLASHING RED";
        westColorState = "FLASHING RED";
        isFlashingRed = true;
        isNorthandSouthYellow = false;
        isNorthandSouthGreen = false;
        isNorthandSouthRed = false;
        isEastandWestGreen = false;
        isEastandWestYellow = false;
        isEastandWestRed = false;
    }
    if(checkCommand == "RESET")
    {
        std::cout << "\nReset Success!" << std::endl;
        isFlashingRed = false;
        northCarCount = "0";
        southCarCount = "0";
        westCarCount = "0";
        eastCarCount = "0";
        northCarTimeStamp = "0";
        southCarTimeStamp = "0";
        westCarTimeStamp = "0";
        eastCarTimeStamp = "0";
        northColorState = "N/A";
        southColorState = "N/A";
        westColorState = "N/A";
        eastColorState = "N/A";
    }
    posBeginCommand = msg1.find("#",posBeginCommand+1);
    posEndCommand = msg1.find("!",posEndCommand+1);


    }

    previousTime = time; // Resets the printout/checking time to every 500 ms

    }



    // Begin Update North TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            northText[i] ="North Light State:"+northColorState;
            northTextSurface[i] = TTF_RenderText_Blended(font,northText[i].c_str(),color);
            northTextTexture[i] = SDL_CreateTextureFromSurface(renderer,northTextSurface[i]);
            SDL_QueryTexture(northTextTexture[i],nullptr,nullptr,&northTextRectSrc[i].w,&northTextRectSrc[i].h);
        }
        if(i == 1)
        {
            northText[i] ="Detect Time stamp:"+northCarTimeStamp;
            northTextSurface[i] = TTF_RenderText_Blended(font,northText[i].c_str(),color);
            northTextTexture[i] = SDL_CreateTextureFromSurface(renderer,northTextSurface[i]);
            SDL_QueryTexture(northTextTexture[i],nullptr,nullptr,&northTextRectSrc[i].w,&northTextRectSrc[i].h);
        }
        if(i == 2)
        {
            northText[i] ="Number Of Cars Detected:"+northCarCount;
            northTextSurface[i] = TTF_RenderText_Blended(font,northText[i].c_str(),color);
            northTextTexture[i] = SDL_CreateTextureFromSurface(renderer,northTextSurface[i]);
            SDL_QueryTexture(northTextTexture[i],nullptr,nullptr,&northTextRectSrc[i].w,&northTextRectSrc[i].h);
        }
        SDL_FreeSurface(northTextSurface[i]);
    }
    for(int j = 0; j<3; j++)
    {
     northTextBox.w = std::max(northTextRectSrc[j].w,northTextRectSrc[0].w);
    }
    for(int i = 0; i<3; i++)
    {
    northTextRectDst[i].w = northTextRectSrc[i].w;
    northTextBox.x = (spriteRectDst[NORTH].x+spriteRectDst[NORTH].w) - (northTextBox.w/2) - (spriteRectDst[NORTH].w/2);
    northTextRectDst[i].x = northTextBox.x;
    northTextRectDst[i].h = northTextRectSrc[i].h;
    northTextBox.h = northTextRectSrc[0].h + northTextRectSrc[1].h + northTextRectSrc[2].h;
    if(i==0)
    {
    northTextBox.y = spriteRectDst[NORTH].y-northTextBox.h;
    northTextRectDst[i].y = northTextBox.y;
    }
    if(i==1)
    {
    northTextRectDst[i].y = northTextBox.y + northTextRectDst[i-1].h;
    }
    if(i==2)
    {
    northTextRectDst[i].y = northTextBox.y + northTextRectDst[i-2].h + northTextRectDst[i-1].h;
    }
    }
    // End Update North TextBox Strings and Dimensions






    // Begin Update South TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            southText[i] ="South Light State:"+southColorState;
            southTextSurface[i] = TTF_RenderText_Blended(font,southText[i].c_str(),color);
            southTextTexture[i] = SDL_CreateTextureFromSurface(renderer,southTextSurface[i]);
            SDL_QueryTexture(southTextTexture[i],nullptr,nullptr,&southTextRectSrc[i].w,&southTextRectSrc[i].h);
        }
        if(i == 1)
        {
            southText[i] ="Detect Time stamp:"+southCarTimeStamp;
            southTextSurface[i] = TTF_RenderText_Blended(font,southText[i].c_str(),color);
            southTextTexture[i] = SDL_CreateTextureFromSurface(renderer,southTextSurface[i]);
            SDL_QueryTexture(southTextTexture[i],nullptr,nullptr,&southTextRectSrc[i].w,&southTextRectSrc[i].h);
        }
        if(i == 2)
        {
            southText[i] ="Number Of Cars Detected:"+southCarCount;
            southTextSurface[i] = TTF_RenderText_Blended(font,southText[i].c_str(),color);
            southTextTexture[i] = SDL_CreateTextureFromSurface(renderer,southTextSurface[i]);
            SDL_QueryTexture(southTextTexture[i],nullptr,nullptr,&southTextRectSrc[i].w,&southTextRectSrc[i].h);
        }
        SDL_FreeSurface(southTextSurface[i]);
    }
    for(int j = 0; j<3; j++)
    {
     southTextBox.w = std::max(southTextRectSrc[j].w,southTextRectSrc[0].w);
    }
    for(int i = 0; i<3; i++)
    {
    southTextRectDst[i].w = southTextRectSrc[i].w;
    southTextBox.x = (spriteRectDst[SOUTH].x+spriteRectDst[SOUTH].w) - (southTextBox.w/2) - (spriteRectDst[SOUTH].w/2);
    southTextRectDst[i].x = southTextBox.x;
    southTextRectDst[i].h = southTextRectSrc[i].h;
    southTextBox.h = southTextRectSrc[0].h + southTextRectSrc[1].h + southTextRectSrc[2].h;
    if(i==0)
    {
    southTextBox.y = spriteRectDst[SOUTH].y-southTextBox.h;
    southTextRectDst[i].y = southTextBox.y;
    }
    if(i==1)
    {
    southTextRectDst[i].y = southTextBox.y + southTextRectDst[i-1].h;
    }
    if(i==2)
    {
    southTextRectDst[i].y = southTextBox.y + southTextRectDst[i-2].h + southTextRectDst[i-1].h;
    }
    }
    // End Update South TextBox Strings and Dimensions


    // Begin Update West TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            westText[i] ="West Light State:"+westColorState;
            westTextSurface[i] = TTF_RenderText_Blended(font,westText[i].c_str(),color);
            westTextTexture[i] = SDL_CreateTextureFromSurface(renderer,westTextSurface[i]);
            SDL_QueryTexture(westTextTexture[i],nullptr,nullptr,&westTextRectSrc[i].w,&westTextRectSrc[i].h);
        }
        if(i == 1)
        {
            westText[i] ="Detect Time stamp:"+westCarTimeStamp;
            westTextSurface[i] = TTF_RenderText_Blended(font,westText[i].c_str(),color);
            westTextTexture[i] = SDL_CreateTextureFromSurface(renderer,westTextSurface[i]);
            SDL_QueryTexture(westTextTexture[i],nullptr,nullptr,&westTextRectSrc[i].w,&westTextRectSrc[i].h);
        }
        if(i == 2)
        {
            westText[i] ="Number Of Cars Detected:"+westCarCount;
            westTextSurface[i] = TTF_RenderText_Blended(font,westText[i].c_str(),color);
            westTextTexture[i] = SDL_CreateTextureFromSurface(renderer,westTextSurface[i]);
            SDL_QueryTexture(westTextTexture[i],nullptr,nullptr,&westTextRectSrc[i].w,&westTextRectSrc[i].h);
        }
        SDL_FreeSurface(westTextSurface[i]);
    }
    for(int j = 0; j<3; j++)
    {
     westTextBox.w = std::max(westTextRectSrc[j].w,westTextRectSrc[0].w);
    }
    for(int i = 0; i<3; i++)
    {
    westTextBox.x = (spriteRectDst[WEST].x+spriteRectDst[WEST].w) - (westTextBox.w/2) - (spriteRectDst[WEST].w/2);
    westTextRectDst[i].x = westTextBox.x;
    westTextRectDst[i].w = westTextRectSrc[i].w;
    westTextRectDst[i].h = westTextRectSrc[i].h;
    westTextBox.h = westTextRectSrc[0].h + westTextRectSrc[1].h + westTextRectSrc[2].h;
    if(i==0)
    {
    westTextBox.y = spriteRectDst[WEST].y-westTextBox.h;
    westTextRectDst[i].y = westTextBox.y;
    }
    if(i==1)
    {
    westTextRectDst[i].y = westTextBox.y + westTextRectDst[0].h;
    }
    if(i==2)
    {
    westTextRectDst[i].y = westTextBox.y + westTextRectDst[i-2].h + westTextRectDst[i-1].h;
    }
    }
    // End Update West TextBox Strings and Dimensions

     // Begin Update East TextBox Strings and Dimensions
    for(int i = 0; i < 3; i++)
    {
        if(i == 0)
        {
            eastText[i] ="East Light State:"+eastColorState;
            eastTextSurface[i] = TTF_RenderText_Blended(font,eastText[i].c_str(),color);
            eastTextTexture[i] = SDL_CreateTextureFromSurface(renderer,eastTextSurface[i]);
            SDL_QueryTexture(eastTextTexture[i],nullptr,nullptr,&eastTextRectSrc[i].w,&eastTextRectSrc[i].h);
        }
        if(i == 1)
        {
            eastText[i] ="Detect Time stamp:"+eastCarTimeStamp;
            eastTextSurface[i] = TTF_RenderText_Blended(font,eastText[i].c_str(),color);
            eastTextTexture[i] = SDL_CreateTextureFromSurface(renderer,eastTextSurface[i]);
            SDL_QueryTexture(eastTextTexture[i],nullptr,nullptr,&eastTextRectSrc[i].w,&eastTextRectSrc[i].h);
        }
        if(i == 2)
        {
            eastText[i] ="Number Of Cars Detected:"+eastCarCount;
            eastTextSurface[i] = TTF_RenderText_Blended(font,eastText[i].c_str(),color);
            eastTextTexture[i] = SDL_CreateTextureFromSurface(renderer,eastTextSurface[i]);
            SDL_QueryTexture(eastTextTexture[i],nullptr,nullptr,&eastTextRectSrc[i].w,&eastTextRectSrc[i].h);
        }
        SDL_FreeSurface(eastTextSurface[i]);
    }

    for(int j = 0; j<3; j++)
    {
     eastTextBox.w = std::max(eastTextRectSrc[j].w,eastTextRectSrc[0].w);
    }
    for(int i = 0; i<3; i++)
    {
    eastTextBox.x = (spriteRectDst[EAST].x+spriteRectDst[EAST].w) - (eastTextBox.w/2) - (spriteRectDst[EAST].w/2);
    eastTextRectDst[i].x = eastTextBox.x;
    eastTextRectDst[i].w = eastTextRectSrc[i].w;
    eastTextRectDst[i].h = eastTextRectSrc[i].h;
    eastTextBox.h = eastTextRectSrc[0].h + eastTextRectSrc[1].h + eastTextRectSrc[2].h;
    if(i==0)
    {
    eastTextBox.y = spriteRectDst[EAST].y-eastTextBox.h;
    eastTextRectDst[i].y = eastTextBox.y;
    }
    if(i==1)
    {
    eastTextRectDst[i].y = eastTextBox.y + eastTextRectDst[0].h;
    }
    if(i==2)
    {
    eastTextRectDst[i].y = eastTextBox.y + eastTextRectDst[i-2].h + eastTextRectDst[i-1].h;
    }
    }
    // End Update East TextBox Strings and Dimensions



    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer,GUI_Background_Tex,nullptr,nullptr);
    SDL_RenderCopyEx(renderer,textTexture,nullptr,&textRectDst,0,nullptr,SDL_FLIP_NONE);
    if(!isFlashingRed && !isNorthandSouthRed && !isNorthandSouthYellow && !isNorthandSouthGreen && !isEastandWestGreen && !isEastandWestRed && !isEastandWestYellow)
    {
       for(int j = 0; j <4; j++)
        {
             Animation(spriteRectSrc[j],0,0,1000);
        }
    }
    if(isNorthandSouthGreen)
    {

        Animation(spriteRectSrc[NORTH],1,1,1000);
        Animation(spriteRectSrc[SOUTH],1,1,1000);
    }

    if(isNorthandSouthYellow)
    {

    Animation(spriteRectSrc[NORTH],2,2,1000);
    Animation(spriteRectSrc[SOUTH],2,2,1000);


    }
    if(isNorthandSouthRed)
    {
       Animation(spriteRectSrc[NORTH],3,3,1000);
       Animation(spriteRectSrc[SOUTH],3,3,1000);
    }

    if(isEastandWestGreen)
    {

        Animation(spriteRectSrc[WEST],1,1,1000);
        Animation(spriteRectSrc[EAST],1,1,1000);
    }

    if(isEastandWestYellow)
    {

    Animation(spriteRectSrc[WEST],2,2,1000);
    Animation(spriteRectSrc[EAST],2,2,1000);


    }
    if(isEastandWestRed)
    {
       Animation(spriteRectSrc[WEST],3,3,1000);
       Animation(spriteRectSrc[EAST],3,3,1000);
    }


    if(isFlashingRed)
    {
        if( (time - flashingTime) >1000)
        {
         for(int j = 0; j <4; j++)
        {
        Animation(spriteRectSrc[j],0,0,1000);
        }
        }
        if(time - flashingTime > 2000)
        {
            for(int j = 0; j <4; j++)
        {
        Animation(spriteRectSrc[j],3,3,1000);
        }
        flashingTime = time;
        }

    }
    SDL_RenderCopy(renderer,spriteTexture,&spriteRectSrc[NORTH],&spriteRectDst[NORTH]);
    SDL_RenderCopy(renderer,spriteTexture,&spriteRectSrc[SOUTH],&spriteRectDst[SOUTH]);
    SDL_RenderCopy(renderer,spriteTexture,&spriteRectSrc[EAST],&spriteRectDst[EAST]);
    SDL_RenderCopy(renderer,spriteTexture,&spriteRectSrc[WEST],&spriteRectDst[WEST]);
    SDL_RenderCopy(renderer,TextBoxTexture,nullptr,&northTextBox);
    SDL_RenderCopy(renderer,TextBoxTexture,nullptr,&southTextBox);
    SDL_RenderCopy(renderer,TextBoxTexture,nullptr,&westTextBox);
    SDL_RenderCopy(renderer,TextBoxTexture,nullptr,&eastTextBox);
    for(int i =0; i < 3; i++)
    {
    SDL_RenderCopy(renderer,northTextTexture[i],nullptr,&northTextRectDst[i]);
    SDL_RenderCopy(renderer,southTextTexture[i],nullptr,&southTextRectDst[i]);
    SDL_RenderCopy(renderer,westTextTexture[i],nullptr,&westTextRectDst[i]);
    SDL_RenderCopy(renderer,eastTextTexture[i],nullptr,&eastTextRectDst[i]);
    }
    SDL_RenderPresent(renderer);
    for(int i =0; i<3; i++)
    {
    SDL_DestroyTexture(northTextTexture[i]);
    SDL_DestroyTexture(southTextTexture[i]);
    SDL_DestroyTexture(westTextTexture[i]);
    SDL_DestroyTexture(eastTextTexture[i]);
    }
    int frameTicks = SDL_GetTicks();
    if(frameTicks < SCREEN_TICKS_PER_FRAME)
    {
        SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
    }
    }

if(!Exit1 && quit)
{
    do
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopyEx(renderer,textTexture,nullptr,&textRectDst,0,nullptr,SDL_FLIP_NONE);
        Animation(spriteRectSrc[NORTH],0,4,1000);
        Animation(spriteRectSrc[SOUTH],0,4,1000);
        for(int j = 0; j <4; j++)
        {
        SDL_RenderCopy(renderer,spriteTexture,&spriteRectSrc[j],&spriteRectDst[j]);
        }
        SDL_RenderPresent(renderer);
        char mQuit[] = "#QUIT";
        WriteMessage(hComm,mQuit);
        std::cout << "\nQuiting Application..." << std::endl;
        quitString = ReadMessage(hComm);
        std::cout << "\nMessage Received:" << quitString << std::endl;
        if(quitString == "#QUIT!")
        {
            std::cout << "\nQuiting Application..." << std::endl;
            SDL_Quit();
        }
    }while(quitString !="#QUIT!");
}
    CloseHandle(hComm);//Closing the Serial Port
    return 0;
}




void WriteMessage(HANDLE hComm, char message[])
{
    //Writing data to Serial Port
    std::cout << "\nMessage Sent:" << message << std::endl;
    Status = WriteFile(hComm,// Handle to the Serialport
                       message,            // Data to be written to the port
                       strlen(message),   // No of bytes to write into the port
                       &BytesWritten,  // No of bytes written to the port
                       NULL);
    if (Status == FALSE)
    {
        std::cout <<"\nFailed to Write";
        quit = true;
        Exit1 = true;
        MessageBox(handle3, NULL, NULL, MB_OK|MB_ICONWARNING  );
    }
    //print numbers of byte written to the serial port
    std::cout <<"\nNumber of bytes written to the serial port = " << BytesWritten;
    //Setting Receive Mask
    Status = SetCommMask(hComm, EV_RXCHAR | EV_TXEMPTY);
    if (Status == FALSE)
    {
        std::cout <<"\nError to in Setting CommMask\n\n";
        quit = true;
        Exit1 = true;
        MessageBox(handle3, NULL, NULL, MB_OK|MB_ICONWARNING  );
    }
    //Setting WaitComm() Event
    Status = WaitCommEvent(hComm, &dwEventMask,NULL); //Wait for the character to be received
    if (Status == FALSE)
    {
        std::cout <<"\nError! in Setting WaitCommEvent()\n\n";
        quit = true;
        Exit1 = true;
        MessageBox(handle3, NULL, NULL, MB_OK|MB_ICONWARNING  );
    }
}
std::string ReadMessage(HANDLE hComm)
{
    readMessage = "";
    loopData = 0;
    readData=0;
    for(int i = 0; i < 128; i++)
    {
        data[i] = {0};
    }
    //Read data and store in a buffer
    do
    {
        StatusRead = ReadFile(hComm, &readData, sizeof(readData), &NoBytes, NULL);
        data[loopData] = readData;
        ++loopData;
    }
    while (NoBytes > 0);

    if(StatusRead == FALSE)
    {
        std::cout << "\nFailed to ReadFile";
        quit = true;
        Exit1 = true;
    }
    --loopData; //Get Actual length of received data
    std::cout <<"\nNumber of bytes received = " << (unsigned int)loopData;
     for (int index = 0; index < loopData; ++index)
    {
        readMessage += data[index];
    }
     return readMessage;


}

void Animation(SDL_Rect &srcRect, int startFrame, int endFrame, int Speed)
{

    srcRect.w =srcRect.h= 32;

    if(startFrame == endFrame)
    {
     srcRect.x = 32*startFrame;
    }
    else{
     while(startFrame >=0 && startFrame < endFrame)
     {
         srcRect.x = (32 *(static_cast<int>((SDL_GetTicks() / Speed) % endFrame)));
         ++startFrame;
     }
    }
}

