#include <stdio.h>

#include <windows.h>
#include <mmsystem.h>

#pragma comment(lib , "winmm.lib")

int open_bgm()
{
    mciSendString("play music/bgm.mp3 repeat" , NULL , 0 , NULL);
    //system("pause");
    return 0;
}

int stop_bgm()
{
    mciSendString("close music/bgm.mp3" , NULL , 0 , NULL);
    //system("pause");
    return 0;
}

int eat_sound()
{
    if(0 == PlaySound(TEXT("music/eat.wav") , NULL , SND_FILENAME | SND_ASYNC))
    {
        return 0;
    }
    //system("pause");
    return 0;
}

int score_sound()
{
    if(0 == PlaySound(TEXT("music/score.wav") , NULL , SND_FILENAME | SND_ASYNC))
    {
        return 0;
    }
    //system("pause");
    return 0;
}

int restart_sound()
{
    if(0 == PlaySound(TEXT("music/restart.wav") , NULL , SND_FILENAME | SND_ASYNC))
    {
        return 0;
    }
    //system("pause");
    return 0;
}

