#ifndef GUARD_OAK_SPEECH_H
#define GUARD_OAK_SPEECH_H

struct OakSpeechNidoranFStruct
{
    u8 spriteCount:4;
    u8 battlePosition:4;
    u8 frameCount;
    u8 enable;
    bool8 enable2;
    u32 sizePerSprite;
    u8 *dataBuffer;
    u8 **bufferPtrs;
    struct SpriteTemplate *templates;
    struct SpriteFrameImage *frameImages;
};

extern struct OakSpeechNidoranFStruct *gOakSpeechNidoranResources;

void StartNewGameScene(void);
void CreatePikaOrGrassPlatformSpriteAndLinkToCurrentTask(u8 taskId, u8 state);

#endif //GUARD_OAK_SPEECH_H
