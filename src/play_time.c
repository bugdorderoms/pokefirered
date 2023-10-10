#include "play_time.h"

static u8 sPlayTimeCounterState;

enum
{
    PLAYTIME_STOPPED,
    PLAYTIME_RUNNING,
    PLAYTIME_MAXED_OUT,
};

void PlayTimeCounter_Reset(void)
{
    sPlayTimeCounterState = PLAYTIME_STOPPED;
    gSaveBlock2Ptr->playTimeHours = 0;
    gSaveBlock2Ptr->playTimeMinutes = 0;
    gSaveBlock2Ptr->playTimeSeconds = 0;
    gSaveBlock2Ptr->playTimeVBlanks = 0;
}

static void PlayTimeCounter_SetToMax(void)
{
    sPlayTimeCounterState = PLAYTIME_MAXED_OUT;
    gSaveBlock2Ptr->playTimeHours = 999;
    gSaveBlock2Ptr->playTimeMinutes = 59;
    gSaveBlock2Ptr->playTimeSeconds = 59;
    gSaveBlock2Ptr->playTimeVBlanks = 59;
}

void PlayTimeCounter_Start(void)
{
    sPlayTimeCounterState = PLAYTIME_RUNNING;
	
    if (gSaveBlock2Ptr->playTimeHours > 999)
        PlayTimeCounter_SetToMax();
}

void PlayTimeCounter_Update(void)
{
    if (sPlayTimeCounterState == PLAYTIME_RUNNING)
    {
        if (++gSaveBlock2Ptr->playTimeVBlanks > 59)
        {
            gSaveBlock2Ptr->playTimeVBlanks = 0;
			
            if (++gSaveBlock2Ptr->playTimeSeconds > 59)
            {
                gSaveBlock2Ptr->playTimeSeconds = 0;
				
                if (++gSaveBlock2Ptr->playTimeMinutes > 59)
                {
                    gSaveBlock2Ptr->playTimeMinutes = 0;
					
                    if (++gSaveBlock2Ptr->playTimeHours > 999)
                        PlayTimeCounter_SetToMax();
                }
            }
        }
    }
}
