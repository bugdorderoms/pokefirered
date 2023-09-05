#include "global.h"
#include "link.h"
#include "ereader_helpers.h"

enum
{
    EREADER_XFR_STATE_INIT = 0,
    EREADER_XFR_STATE_HANDSHAKE,
    EREADER_XFR_STATE_START,
    EREADER_XFR_STATE_TRANSFER,
    EREADER_XFR_STATE_TRANSFER_DONE,
    EREADER_XFR_STATE_CHECKSUM,
    EREADER_XFR_STATE_DONE
};

#define EREADER_XFER_SHIFT 0
#define EREADER_XFER_EXE   1
#define EREADER_XFER_CHK   2
#define EREADER_XFER_MASK  3

#define EREADER_CANCEL_TIMEOUT 1
#define EREADER_CANCEL_KEY     2
#define EREADER_CANCEL_MASK    0xC
#define EREADER_CANCEL_SHIFT   2

#define EREADER_CHECKSUM_OK    1
#define EREADER_CHECKSUM_ERR   2
#define EREADER_CHECKSUM_MASK  0x30
#define EREADER_CHECKSUM_SHIFT 4

struct SendRecvMgr
{
    u8 master_slave;       // 0: clock slave; 1: clock master
    u8 state;              // EREADER_XFR_STATE_*
    u8 xferState;          // EREADER_XFER_*
    u8 checksumResult;     // EREADER_CHECKSUM_*
    u8 cancellationReason; // EREADER_CANCEL_*
    u32 * dataptr;         // Payload source or destination
    int cursor;            // Index of the next word
    int size;              // Last word index
    u32 checksum;          // Validation checksum
};

static bool16 DetermineSendRecvState(u8);
static void SetUpTransferManager(size_t, const void *, void *);
static void StartTm3(void);
static void EnableSio(void);
static void GetKeyInput(void);

static struct SendRecvMgr sSendRecvMgr;
static u16 sJoyNewOrRepeated;
static u16 sJoyNew;
static u16 sSendRecvStatus;
static u16 sCounter1;
static u32 sCounter2;
static u16 sSavedIme;
static u16 sSavedIe;
static u16 sSavedTm3Cnt;
static u16 sSavedSioCnt;
static u16 sSavedRCnt;

static void EReaderHelper_SaveRegsState(void)
{
    sSavedIme = REG_IME;
    sSavedIe = REG_IE;
    sSavedTm3Cnt = REG_TM3CNT_H;
    sSavedSioCnt = REG_SIOCNT;
    sSavedRCnt = REG_RCNT;
}

static void EReaderHelper_RestoreRegsState(void)
{
    REG_IME = sSavedIme;
    REG_IE = sSavedIe;
    REG_TM3CNT_H = sSavedTm3Cnt;
    REG_SIOCNT = sSavedSioCnt;
    REG_RCNT = sSavedRCnt;
}

static u16 EReaderHandleTransfer(u8 mode, size_t size, const void * data, void * recvBuffer)
{
    switch (sSendRecvMgr.state)
    {
    case EREADER_XFR_STATE_INIT:
        OpenSerialMulti();
        sSendRecvMgr.xferState = EREADER_XFER_EXE;
        sSendRecvMgr.state = EREADER_XFR_STATE_HANDSHAKE;
        break;
    case EREADER_XFR_STATE_HANDSHAKE:
        if (DetermineSendRecvState(mode))
            EnableSio();
        if (gShouldAdvanceLinkState == 2)
        {
            sSendRecvMgr.cancellationReason = EREADER_CANCEL_KEY;
            sSendRecvMgr.state = EREADER_XFR_STATE_DONE;
        }
        // Progression is handled by the serial callback
        break;
    case EREADER_XFR_STATE_START:
        OpenSerial32();
        SetUpTransferManager(size, data, recvBuffer);
        sSendRecvMgr.state = EREADER_XFR_STATE_TRANSFER;
        // fallthrough
    case EREADER_XFR_STATE_TRANSFER:
        if (gShouldAdvanceLinkState == 2)
        {
            sSendRecvMgr.cancellationReason = EREADER_CANCEL_KEY;
            sSendRecvMgr.state = EREADER_XFR_STATE_DONE;
        }
        else
        {
            sCounter1++;
            sCounter2++;
            if (sSendRecvMgr.master_slave == 0 && sCounter2 > 60)
            {
                sSendRecvMgr.cancellationReason = EREADER_CANCEL_TIMEOUT;
                sSendRecvMgr.state = EREADER_XFR_STATE_DONE;
            }
            if (sSendRecvMgr.xferState != EREADER_XFER_CHK)
            {
                if (sSendRecvMgr.master_slave != 0 && sCounter1 > 2)
                {
                    EnableSio();
                    sSendRecvMgr.xferState = EREADER_XFER_CHK;
                }
                else
                {
                    EnableSio();
                    sSendRecvMgr.xferState = EREADER_XFER_CHK;
                }
            }
        }
        // Progression is handled by the serial callback
        break;
    case EREADER_XFR_STATE_TRANSFER_DONE:
        OpenSerialMulti();
        sSendRecvMgr.state = EREADER_XFR_STATE_CHECKSUM;
        break;
    case EREADER_XFR_STATE_CHECKSUM:
        if (sSendRecvMgr.master_slave == 1 && sCounter1 > 2)
            EnableSio();
        if (++sCounter1 > 60)
        {
            sSendRecvMgr.cancellationReason = EREADER_CANCEL_TIMEOUT;
            sSendRecvMgr.state = EREADER_XFR_STATE_DONE;
        }
        break;
        // Progression is handled by the serial callback
    case EREADER_XFR_STATE_DONE:
        if (sSendRecvMgr.xferState != 0)
        {
            CloseSerial();
            sSendRecvMgr.xferState = 0;
        }
        break;
    }
    return
        (sSendRecvMgr.xferState << EREADER_XFER_SHIFT)
      | (sSendRecvMgr.cancellationReason << EREADER_CANCEL_SHIFT)
      | (sSendRecvMgr.checksumResult << EREADER_CHECKSUM_SHIFT);
}

int EReader_Send(size_t size, const void * src)
{
    int result;
    EReaderHelper_SaveRegsState();

    while (1)
    {
        GetKeyInput();
        if (TEST_BUTTON(sJoyNew, B_BUTTON))
            gShouldAdvanceLinkState = 2;

        sSendRecvStatus = EReaderHandleTransfer(1, size, src, NULL);
        if ((sSendRecvStatus & 0x13) == 0x10) // checksum OK and xfer off
        {
            result = 0;
            break;
        }
        else if (sSendRecvStatus & 8) // cancelled by player
        {
            result = 1;
            break;
        }
        else if (sSendRecvStatus & 4) // timed out
        {
            result = 2;
            break;
        }
        else
        {
            gShouldAdvanceLinkState = 0;
            VBlankIntrWait();
        }
    }

    CpuFill32(0, &sSendRecvMgr, sizeof(sSendRecvMgr));
    EReaderHelper_RestoreRegsState();
    return result;
}

int EReader_Recv(void * dest)
{
    int result;
    EReaderHelper_SaveRegsState();

    while (1)
    {
        GetKeyInput();
        if (TEST_BUTTON(sJoyNew, B_BUTTON))
            gShouldAdvanceLinkState = 2;

        sSendRecvStatus = EReaderHandleTransfer(0, 0, NULL, dest);
        if ((sSendRecvStatus & 0x13) == 0x10) // checksum OK and xfer off
        {
            result = 0;
            break;
        }
        else if (sSendRecvStatus & 8) // cancelled by player
        {
            result = 1;
            break;
        }
        else if (sSendRecvStatus & 4) // timed out
        {
            result = 2;
            break;
        }
        else
        {
            gShouldAdvanceLinkState = 0;
            VBlankIntrWait();
        }
    }

    CpuFill32(0, &sSendRecvMgr, sizeof(sSendRecvMgr));
    EReaderHelper_RestoreRegsState();
    return result;
}

static void CloseSerial(void)
{
    REG_IME = 0;
    REG_IE &= ~(INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
    REG_IME = 1;
    REG_SIOCNT = 0;
    REG_TM3CNT_H = 0;
    REG_IF = INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL;
}

static void OpenSerialMulti(void)
{
    REG_IME = 0;
    REG_IE &= ~(INTR_FLAG_TIMER3 | INTR_FLAG_SERIAL);
    REG_IME = 1;
    REG_RCNT = 0;
    REG_SIOCNT = SIO_MULTI_MODE;
    REG_SIOCNT |= SIO_INTR_ENABLE | SIO_115200_BPS;
    REG_IME = 0;
    REG_IE |= INTR_FLAG_SERIAL;
    REG_IME = 1;
    if (sSendRecvMgr.state == 0)
        CpuFill32(0, &sSendRecvMgr, sizeof(sSendRecvMgr));
}

static void OpenSerial32(void)
{
    REG_RCNT = 0;
    REG_SIOCNT = SIO_INTR_ENABLE | SIO_32BIT_MODE;
    REG_SIOCNT |= SIO_MULTI_SD;
    gShouldAdvanceLinkState = 0;
    sCounter1 = 0;
    sCounter2 = 0;
}

static bool16 DetermineSendRecvState(u8 mode)
{
    bool16 resp;
    if ((*(vu32 *)REG_ADDR_SIOCNT & (SIO_MULTI_SI | SIO_MULTI_SD)) == SIO_MULTI_SD && mode)
        resp = sSendRecvMgr.master_slave = TRUE;
    else
        resp = sSendRecvMgr.master_slave = FALSE;
    return resp;
}

static void SetUpTransferManager(size_t size, const void * data, void * recvBuffer)
{
    if (sSendRecvMgr.master_slave)
    {
        REG_SIOCNT |= SIO_38400_BPS;
        sSendRecvMgr.dataptr = (void *)data;
        REG_SIODATA32 = size;
        sSendRecvMgr.size = size / 4 + 1;
        StartTm3();
    }
    else
    {
        REG_SIOCNT |= SIO_9600_BPS;
        sSendRecvMgr.dataptr = recvBuffer;
    }
}

static void StartTm3(void)
{
    REG_TM3CNT_L = -601;
    REG_TM3CNT_H = TIMER_INTR_ENABLE;
    REG_IME = 0;
    REG_IE |= INTR_FLAG_TIMER3;
    REG_IME = 1;
}

static void EnableSio(void)
{
    REG_SIOCNT |= SIO_ENABLE;
}

static void GetKeyInput(void)
{
    u16 rawKeys = REG_KEYINPUT ^ 0x3FF;
    sJoyNew = rawKeys & ~sJoyNewOrRepeated;
    sJoyNewOrRepeated = rawKeys;
}
