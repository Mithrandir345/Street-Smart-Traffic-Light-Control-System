#include <windows.h>
#include <iostream>
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
SDL_Rect textRectDst;
SDL_Rect textRectSrc;
SDL_Color color = {255,64,0};
SDL_Surface * spriteSurface;
SDL_Texture * spriteTexture;
SDL_Rect spriteRectDst[4];
SDL_Rect spriteRectSrc[4];
std::string comPortName;
std::string serialBuffer;
std::string quitString="";
std::string msg1="";
BOOL   StatusRead; // Status
    char data[128] = { 0 }; //Buffer to receive data
    char  readData;        //temperory Character
    DWORD NoBytes;     // Bytes read by ReadFile()
    unsigned char loopData = 0;
    std::string readMessage="";
char code[64];
std::string accesscode = "2682209";
bool Exit1,quit = false;
bool isPromptAgain = true;
bool isEnteredSystem = false;
SDL_Event event;
UINT32 time = SDL_GetTicks();
UINT32 targetTime = 0;
UINT32 previousTime = 0;
UINT32 flashingTime = 0;
UINT32 elapsedTime = 0;
char bufferWrite[64]={0};
char port[]="COM3";
bool isGreen,isYellow,isRed,isFlashingRed,isReset,isFlashing = false;
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
void Animation(SDL_Rect &srcRect, int startFrame, int endFrame, int Speed);
void WriteMessage(HANDLE hComm, char message[]);
std::string ReadMessage(HANDLE hComm);
int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    window = SDL_CreateWindow("Street Smart Traffic Lighting System GUI",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_PRESENTVSYNC);
    font = TTF_OpenFont("Assets/Lotr.ttf",16);
    textSurface = TTF_RenderText_Solid(font,"Street Smart Traffic Lighting System GUI",color);
    textTexture = SDL_CreateTextureFromSurface(renderer,textSurface);
    SDL_QueryTexture(textTexture,nullptr,nullptr,&textRectSrc.w,&textRectSrc.h);
    textRectDst={WINDOW_WIDTH/2 - (textRectSrc.w/2),WINDOW_HEIGHT/2 - (textRectSrc.h/2),textRectSrc.w,textRectSrc.h};

    spriteSurface = IMG_Load("Assets/LightingSprite.png");
    if(spriteSurface == nullptr)
    {
        std::cout << "Failed To Load Sprite" << std::endl;
    }
    spriteTexture = SDL_CreateTextureFromSurface(renderer,spriteSurface);
    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[NORTH].w,&spriteRectSrc[NORTH].h);
    spriteRectDst[NORTH]={WINDOW_WIDTH/2 - (spriteRectSrc[NORTH].w/2),WINDOW_HEIGHT/2 - (spriteRectSrc[NORTH].h/2)-128,128,128};

    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[SOUTH].w,&spriteRectSrc[SOUTH].h);
    spriteRectDst[SOUTH]={WINDOW_WIDTH/2 - (spriteRectSrc[SOUTH].w/2),WINDOW_HEIGHT/2 - (spriteRectSrc[SOUTH].h/2)+128,128,128};

    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[EAST].w,&spriteRectSrc[EAST].h);
    spriteRectDst[EAST]={WINDOW_WIDTH/2 - (spriteRectSrc[EAST].w/2)+128,WINDOW_HEIGHT/2 - (spriteRectSrc[EAST].h/2)+128,128,128};

    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc[WEST].w,&spriteRectSrc[WEST].h);
    spriteRectDst[WEST]={WINDOW_WIDTH/2 - (spriteRectSrc[WEST].w/2)-128,WINDOW_HEIGHT/2 - (spriteRectSrc[WEST].h/2)+128,128,128};

    while(isPromptAgain)
    {
    //Enter the com port id
    std::cout <<"\n\nEnter the Com Port:";
    std::cin >> PortName;
    std::cout << "\nPort Name Entered:" << PortName << std::endl;
    if(strcmp(PortName,port)==0)
    {
        isPromptAgain = false;
        std::cout << "\n\n" << PortName << " Established Communication" << std::endl;

    }else{std::cout << "\n\n\nPlease Enter A Correct Com Port Again..." << std::endl;}
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
        std::cout << "\nPort can't be opened\n\n";
        Exit1=true;
    }
    //Setting the Parameters for the SerialPort
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    Status = GetCommState(hComm, &dcbSerialParams); //retreives  the current settings
    if (Status == FALSE)
    {
        std::cout << "\nError to Get the Com state\n\n";
        Exit1 = true;
    }
    dcbSerialParams.BaudRate = CBR_9600;      //BaudRate = 9600
    dcbSerialParams.ByteSize = DATABITS_8;             //ByteSize = 8
    dcbSerialParams.StopBits = ONESTOPBIT;    //StopBits = 1
    dcbSerialParams.Parity = NOPARITY;      //Parity = None
    Status = SetCommState(hComm, &dcbSerialParams);
    if (Status == FALSE)
    {
        std::cout <<"\nError to Setting DCB Structure\n\n";
        Exit1=true;
    }
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
    }
    if(!Exit1)
    {
        char cCom3[] = "COM3";
        WriteMessage(hComm,cCom3);
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
    if(msg1 == "SUCCESS")
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

    while(isEnteredSystem && !quit)
    {
        msg1="";
        time = SDL_GetTicks();
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
                       //std::cout << "Reset Initialized" << std::endl;
                       isReset = true;
                    }
                    break;
                case SDLK_f:
                    if(event.key.repeat == 0)
                    {
                       //std::cout << "FLASHING RED INITIALIZED!" << std::endl;
                       isFlashing=true;
                    }
                    break;
                }
                break;
            }
        }
        if(isReset)
        {
            std::cout << "Reset Initialized" << std::endl;
            char cReset[] = "RESET";
            WriteMessage(hComm,cReset);
            isReset = false;
        }
        if(isFlashing)
        {
            std::cout << "FLASHING RED INITIALIZED!" << std::endl;
            char cFlashing[] = "FLASH";
            WriteMessage(hComm,cFlashing);
            isFlashing=false;
        }
    if(time > previousTime+1000)
    {
     msg1 = ReadMessage(hComm);
     std::cout << "\n" << msg1 << std::endl;
     previousTime = time;
    }
    if(msg1 == "FLASHINGRED")
    {
        std::cout << "\nLights Are Flashing Red!" << std::endl;
        isFlashingRed = true;
        isYellow = false;
        isGreen = false;
        isRed = false;
    }
    if(msg1 == "RESET")
    {
        std::cout << "\nReset Success!" << std::endl;
        isFlashingRed = false;
    }
    if(msg1 == "NORTHGREEN")
    {
        isGreen = true;
        isRed = false;
        isYellow = false;
        isFlashingRed =false;
    }
    if(msg1 == "NORTHYELLOW")
    {
        isYellow = true;
        isGreen = false;
        isFlashingRed = false;
        isRed = false;
    }
    if(msg1 == "NORTHRED")
    {
        isRed = true;
        isYellow = false;
        isGreen = false;
        isFlashingRed = false;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer,textTexture,nullptr,&textRectDst,0,nullptr,SDL_FLIP_NONE);
    if(!isFlashingRed && !isRed && !isYellow && !isGreen)
    {
       for(int j = 0; j <4; j++)
        {
             Animation(spriteRectSrc[j],0,0,1000);
        }
    }
    if(isGreen)
    {

        Animation(spriteRectSrc[NORTH],1,1,1000);
        Animation(spriteRectSrc[SOUTH],1,1,1000);
    }

    if(isYellow)
    {

    Animation(spriteRectSrc[NORTH],2,2,1000);
    Animation(spriteRectSrc[SOUTH],2,2,1000);


    }

    if(isRed)
    {
       Animation(spriteRectSrc[NORTH],3,3,1000);
       Animation(spriteRectSrc[SOUTH],3,3,1000);
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

    SDL_RenderPresent(renderer);
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
        char mQuit[] = "QUIT";
        WriteMessage(hComm,mQuit);
        std::cout << "\nQuiting Application..." << std::endl;
        quitString = ReadMessage(hComm);
        std::cout << "\nMessage Received:" << quitString << std::endl;
        if(quitString == "QUIT")
        {
            std::cout << "\nQuiting Application..." << std::endl;
            SDL_Quit();
        }
    }while(quitString !="QUIT");
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

     srcRect.h =32;
     srcRect.w=32;

    if(startFrame == endFrame)
    {
     srcRect.x = 32*startFrame;
    }
    else{
     while(startFrame >=0 && startFrame < endFrame)
     {
         srcRect.x = (32 *(static_cast<int>((SDL_GetTicks() / Speed) % endFrame)));
         ++startFrame;
         //std::cout << "Frame:" << startFrame << std::endl;
     }
    }
 }

