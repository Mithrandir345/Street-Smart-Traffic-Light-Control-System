#include <windows.h>
#include <iostream>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 640
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
SDL_Rect spriteRectDst;
SDL_Rect spriteRectSrc;
std::string comPortName;
std::string serialBuffer;
std::string quitString;
bool Exit1,Exit2,quit = false;
bool isEnteredSystem = false;
SDL_Event event;
UINT32 time = SDL_GetTicks();
UINT32 targetTime = 0;
UINT32 previousTime = 0;
UINT32 elapsedTime = 0;
bool isGreen,isYellow,isRed,isFlashingRed = false;
HANDLE hComm;  // Handle to the Serial port
    BOOL   Status; // Status
    DCB dcbSerialParams = { 0 };  // Initializing DCB structure
    COMMTIMEOUTS timeouts = { 0 };  //Initializing timeouts structure
    char SerialBuffer[64] = { 0 }; //Buffer to send and receive data
    DWORD BytesWritten = 0;          // No of bytes written to the port
    DWORD dwEventMask;     // Event mask to trigger
    char  ReadData;        //temperory Character
    DWORD NoBytesRead;     // Bytes read by ReadFile()
    unsigned char loop = 0;
    std::string PortName; //com port id
void Animation(SDL_Rect srcRect, int row,int startFrame,int endFrame, int Speed);
void WriteMessage(HANDLE hComm);
void WriteMessage(HANDLE hComm,std::string message);
std::string ReadMessage(HANDLE hComm, UINT32 readTime);
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
    SDL_QueryTexture(spriteTexture,nullptr,nullptr,&spriteRectSrc.w,&spriteRectSrc.h);
    spriteRectDst={WINDOW_WIDTH/2 - (spriteRectSrc.w/2),WINDOW_HEIGHT/2 - (spriteRectSrc.h/2)+128,128,128};

    //Enter the com port id
    std::cout <<"Enter the Com Port: ";
    std::getline(std::cin,PortName);
    //Open the serial com port
    hComm = CreateFile(PortName.c_str(), //friendly name
                       GENERIC_READ | GENERIC_WRITE,      // Read/Write Access
                       0,                                 // No Sharing, ports cant be shared
                       NULL,                              // No Security
                       OPEN_EXISTING,                     // Open existing port only
                       0,                                 // Non Overlapped I/O
                       NULL);                             // Null for Comm Devices
    if (hComm == INVALID_HANDLE_VALUE)
    {
        std::cout << "\n Port can't be opened\n\n";
        Exit2=true;
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
    dcbSerialParams.ByteSize = 8;             //ByteSize = 8
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
    WriteMessage(hComm);
    ReadMessage(hComm,0);

    //Access Code
    char code[64];
    std::cout << "Enter Access Code:";
    std::cin >> code;

    //Writing data to Serial Port
    Status = WriteFile(hComm,// Handle to the Serialport
                       code,            // Data to be written to the port
                       sizeof(code),   // No of bytes to write into the port
                       &BytesWritten,  // No of bytes written to the port
                       NULL);
    if (Status == FALSE)
    {
        std::cout <<"\nFail to Written";
        Exit1=true;
    }
    //print numbers of byte written to the serial port
    std::cout <<"\nNumber of bytes written to the serial port = " << BytesWritten;
    //Setting Receive Mask
    Status = SetCommMask(hComm, EV_RXCHAR);
    if (Status == FALSE)
    {
        std::cout <<"\nError to in Setting CommMask\n\n";
        Exit1=true;
    }
    //Setting WaitComm() Event
    Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
    if (Status == FALSE)
    {
        std::cout <<"\nError! in Setting WaitCommEvent()\n\n";
        Exit1=true;
    }
    loop=0;
    ReadData=0;
    NoBytesRead=0;
    //Read data and store in a buffer
    do
    {
        Status = ReadFile(hComm, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);
        SerialBuffer[loop] = ReadData;
        ++loop;
    }
    while (NoBytesRead > 0);
    --loop; //Get Actual length of received data
    std::cout <<"\nNumber of bytes received = " << int(loop);
    //print receive data on console
    std::cout << "\n\n";
    int index1 = 0;
    std::string msg1;
    for (index1 = 0; index1 < loop; ++index1)
    {
        msg1 += SerialBuffer[index1];
    }
    std::cout << msg1 << std::endl;
    if(msg1 == "SUCCESS")
    {
        isEnteredSystem = true;
    }

    std::cout << "\n\n";





    if(Exit1)
    CloseHandle(hComm);//Closing the Serial Port
    if(Exit2)
    quit=true;

    while(isEnteredSystem && !quit)
    {
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
                }
                break;
            }
        }

    //std::cout << "\nApplication Running...\n";
msg1="";
     if(time > previousTime+1000)
    {
    ReadData=0;
    NoBytesRead=0;
    loop=0;
    SerialBuffer[64]={0};

    //Read data and store in a buffer
    do
    {
        Status = ReadFile(hComm, &ReadData, sizeof(ReadData), &NoBytesRead, NULL);
        SerialBuffer[loop] = ReadData;
        ++loop;
    }
    while (NoBytesRead > 0);
    --loop; //Get Actual length of received data
     for (int index1 = 0; index1 < loop; ++index1)
    {
        msg1 += SerialBuffer[index1];
    }

    std::cout <<"\nNumber of bytes received = " << int(loop);
    //print receive data on console
    std::cout << "\n\n";
    std::cout << msg1 << std::endl;
     previousTime = time;
    }




    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer,textTexture,nullptr,&textRectDst,0,nullptr,SDL_FLIP_NONE);
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
    if(msg1 == "NORTHFLASHINGRED")
    {
        isFlashingRed = true;
        isYellow = false;
        isGreen = false;
        isRed = false;
    }

    if(isGreen)
    {
    Animation(spriteRectSrc,0,1,1,1000);
    }

    if(isYellow)
    Animation(spriteRectSrc,0,2,2,1000);

    if(isRed)
    Animation(spriteRectSrc,0,3,3,1000);


    if(isFlashingRed)
    {
    Animation(spriteRectSrc,0,0,0,1000);
    Animation(spriteRectSrc,0,3,3,1000);
    }


    SDL_RenderPresent(renderer);
    }


    do
    {
        WriteMessage(hComm,"QUIT");
        quitString = ReadMessage(hComm,0);
        if(quitString == "QUIT")
        {
            SDL_Quit();
        }
    SDL_RenderClear(renderer);
    SDL_RenderCopyEx(renderer,textTexture,nullptr,&textRectDst,0,nullptr,SDL_FLIP_NONE);
    Animation(spriteRectSrc,0,0,4,1000);
    SDL_RenderPresent(renderer);
    }while(quitString !="QUIT");
    return 0;
}



void WriteMessage(HANDLE hComm)
{
    std::cout <<"\n\nEnter your message: ";
    std::cin >> SerialBuffer;
    //Writing data to Serial Port
    Status = WriteFile(hComm,// Handle to the Serialport
                       SerialBuffer,            // Data to be written to the port
                       sizeof(SerialBuffer),   // No of bytes to write into the port
                       &BytesWritten,  // No of bytes written to the port
                       NULL);
    if (Status == FALSE)
    {
        std::cout <<"\nFail to Written";
        quit = true;
    }
    //print numbers of byte written to the serial port
    std::cout <<"\nNumber of bytes written to the serial port = " << BytesWritten;
    //Setting Receive Mask
    Status = SetCommMask(hComm, EV_RXCHAR);
    if (Status == FALSE)
    {
        std::cout <<"\nError to in Setting CommMask\n\n";
        quit = true;
    }
    //Setting WaitComm() Event
    Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
    if (Status == FALSE)
    {
        std::cout <<"\nError! in Setting WaitCommEvent()\n\n";
        quit = true;
    }
}
void WriteMessage(HANDLE hComm,std::string message)
{
    //Writing data to Serial Port
    Status = WriteFile(hComm,// Handle to the Serialport
                       message.c_str(),            // Data to be written to the port
                       sizeof(message.c_str()),   // No of bytes to write into the port
                       &BytesWritten,  // No of bytes written to the port
                       NULL);
    if (Status == FALSE)
    {
        std::cout <<"\nFail to Written";
        quit = true;
    }
    //print numbers of byte written to the serial port
    std::cout <<"\nNumber of bytes written to the serial port = " << BytesWritten;
    //Setting Receive Mask
    Status = SetCommMask(hComm, EV_RXCHAR);
    if (Status == FALSE)
    {
        std::cout <<"\nError to in Setting CommMask\n\n";
        quit = true;
    }
    //Setting WaitComm() Event
    Status = WaitCommEvent(hComm, &dwEventMask, NULL); //Wait for the character to be received
    if (Status == FALSE)
    {
        std::cout <<"\nError! in Setting WaitCommEvent()\n\n";
        quit = true;
    }
}
std::string ReadMessage(HANDLE hComm, UINT32 readTime)
{
    BOOL   Status; // Status
    char data[128] = { 0 }; //Buffer to receive data
    char  readData;        //temperory Character
    DWORD NoBytes;     // Bytes read by ReadFile()
    unsigned char loopData = 0;
    std::string message="";
    if(true)
    {
    //Read data and store in a buffer
    do
    {
        Status = ReadFile(hComm, &readData, sizeof(readData), &NoBytes, NULL);
        data[loopData] = readData;
        ++loopData;
    }
    while (NoBytes > 0);
    if(Status == FALSE)
    {
        std::cout << "\nFailed to ReadFile";
        quit = true;
    }
    --loopData; //Get Actual length of received data
    std::cout <<"\nNumber of bytes received = " << (unsigned int)loopData;
     for (int index1 = 0; index1 < loopData; ++index1)
    {
        message += data[index1];
    }

    //print receive data on console
    std::cout << "\n\n";
    std::cout << std::endl <<  message << std::endl;
     previousTime = time;
     return message;
    }

}

void Animation(SDL_Rect srcRect, int row, int startFrame,int endFrame, int Speed)
 {
     srcRect.h =32;
     srcRect.w=32;
    if(startFrame == endFrame)
    {
     srcRect.x = 32*startFrame;
     SDL_RenderCopy(renderer,spriteTexture,&srcRect,&spriteRectDst);
    }
    else{
     while(startFrame >=0 && startFrame < endFrame)
     {
         srcRect.x = (32 *(static_cast<int>((SDL_GetTicks() / Speed) % endFrame)));
         ++startFrame;
         //std::cout << "Frame:" << startFrame << std::endl;
         SDL_RenderCopy(renderer,spriteTexture,&srcRect,&spriteRectDst);
     }
    }
 }
