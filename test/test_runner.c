#include <stdarg.h>
#include "global.h"
#include "characters.h"
#include "gpu_regs.h"
#include "random.h"
#include "main.h"
#include "malloc.h"
#include "math_util.h"
#include "test_runner.h"
#include "test/test.h"

#define TIMEOUT_SECONDS 30

void CB2_TestRunner(void);

EWRAM_DATA struct TestRunnerState gTestRunnerState;
EWRAM_DATA struct FunctionTestRunnerState *gFunctionTestRunnerState;

void TestRunner_Battle(const struct Test *);

static bool32 MgbaOpen_(void);
static void MgbaExit_(u8 exitCode);
static s32 MgbaPuts_(const char *s);
static s32 MgbaVPrintf_(const char *fmt, va_list va);
static void Intr_Timer2(void);

extern const struct Test __start_tests[];
extern const struct Test __stop_tests[];

static bool32 PrefixMatch(const char *pattern, const char *string)
{
    if (string == NULL)
        return TRUE;

    while (TRUE)
    {
        if (!*pattern)
            return TRUE;
        if (*pattern != *string)
            return FALSE;
        pattern++;
        string++;
    }
}

enum
{
    STATE_INIT,
    STATE_NEXT_TEST,
    STATE_RUN_TEST,
    STATE_REPORT_RESULT,
    STATE_EXIT,
};

void CB2_TestRunner(void)
{
    switch (gTestRunnerState.state)
    {
        case STATE_INIT:
            if (!MgbaOpen_())
            {
                gTestRunnerState.state = STATE_EXIT;
                gTestRunnerState.exitCode = 2;
                return;
            }

            gIntrTable[7] = Intr_Timer2;
    
            gTestRunnerState.state = STATE_NEXT_TEST;
            gTestRunnerState.exitCode = 0;
            gTestRunnerState.tests = 0;
            gTestRunnerState.passes = 0;
            gTestRunnerState.skips = 0;
            gTestRunnerState.skipFilename = NULL;
            gTestRunnerState.test = __start_tests - 1;
            break;
    
        case STATE_NEXT_TEST:
            gTestRunnerState.test++;
    
            if (gTestRunnerState.test == __stop_tests)
            {
                MgbaPrintf_("%s%d/%d PASSED\e[0m", gTestRunnerState.exitCode == 0 ? "\e[32m" : "\e[31m", gTestRunnerState.passes, gTestRunnerState.tests);
                if (gTestRunnerState.skips)
                {
                    if (gTestRunnerSkipIsFail)
                        MgbaPrintf_("\e[31m%d SKIPPED\e[0m", gTestRunnerState.skips);
                    else
                        MgbaPrintf_("%d SKIPPED", gTestRunnerState.skips);
                }
                gTestRunnerState.state = STATE_EXIT;
                return;
            }
    
            if (gTestRunnerState.test->runner != &gAssumptionsRunner && !PrefixMatch(gTestRunnerArgv, gTestRunnerState.test->name))
                return;
            
            MgbaPrintf_(":N%s", gTestRunnerState.test->name);
            gTestRunnerState.result = TEST_RESULT_PASS;
            gTestRunnerState.expectedResult = TEST_RESULT_PASS;
            if (gTestRunnerHeadless)
                gTestRunnerState.timeoutSeconds = TIMEOUT_SECONDS;
            else
                gTestRunnerState.timeoutSeconds = UINT_MAX;
            InitHeap(gHeap, HEAP_SIZE);
            EnableInterrupts(INTR_FLAG_TIMER2);
            REG_TM2CNT_L = UINT16_MAX - (274 * 60); // Approx. 1 second.
            REG_TM2CNT_H = TIMER_ENABLE | TIMER_INTR_ENABLE | TIMER_1024CLK;
    
            // NOTE: Assumes that the compiler interns __FILE__.
            if (gTestRunnerState.skipFilename == gTestRunnerState.test->filename)
            {
                gTestRunnerState.result = TEST_RESULT_SKIP;
                return;
            }
            
            // Greedily assign tests to processes based on estimated cost.
            // TODO: Make processCosts a min heap.
            if (gTestRunnerState.test->runner != &gAssumptionsRunner)
            {
                u32 i;
                u32 minCost, minCostProcess;
                minCost = gTestRunnerState.processCosts[0];
                minCostProcess = 0;
                for (i = 1; i < gTestRunnerN; i++)
                {
                    if (gTestRunnerState.processCosts[i] < minCost)
                    {
                        minCost = gTestRunnerState.processCosts[i];
                        minCostProcess = i;
                    }
                }
                
                if (minCostProcess == gTestRunnerI)
                    gTestRunnerState.state = STATE_RUN_TEST;
                else
                    gTestRunnerState.state = STATE_NEXT_TEST;
                
                // XXX: If estimateCost exits only on some processes then
                // processCosts will be inconsistent.
                if (gTestRunnerState.test->runner->estimateCost)
                    gTestRunnerState.processCosts[minCostProcess] += gTestRunnerState.test->runner->estimateCost(gTestRunnerState.test->data);
                else
                    gTestRunnerState.processCosts[minCostProcess] += 1;
            }
            break;
        
        case STATE_RUN_TEST:
            gTestRunnerState.state = STATE_REPORT_RESULT;
            
            SeedRng(0);
            
            if (gTestRunnerState.test->runner->setUp)
                gTestRunnerState.test->runner->setUp(gTestRunnerState.test->data);
            
            gTestRunnerState.test->runner->run(gTestRunnerState.test->data);
            break;
    
        case STATE_REPORT_RESULT:
            REG_TM2CNT_H = 0;
    
            gTestRunnerState.state = STATE_NEXT_TEST;
    
            if (gTestRunnerState.test->runner->tearDown)
                gTestRunnerState.test->runner->tearDown(gTestRunnerState.test->data);
    
            if (gTestRunnerState.test->runner == &gAssumptionsRunner)
            {
                if (gTestRunnerState.result != TEST_RESULT_PASS)
                    gTestRunnerState.skipFilename = gTestRunnerState.test->filename;
            }
            else if (gTestRunnerState.result == TEST_RESULT_SKIP)
            {
                gTestRunnerState.skips++;
                if (gTestRunnerSkipIsFail)
                    gTestRunnerState.exitCode = 1;
            }
            else
            {
                const char *color;
                const char *result;
    
                gTestRunnerState.tests++;
    
                if (gTestRunnerState.result == gTestRunnerState.expectedResult)
                {
                    gTestRunnerState.passes++;
                    color = "\e[32m";
                    MgbaPrintf_(":N%s", gTestRunnerState.test->name);
                }
                else if (gTestRunnerState.result != TEST_RESULT_SKIP || gTestRunnerSkipIsFail)
                {
                    gTestRunnerState.exitCode = 1;
                    color = "\e[31m";
                }
                else
                {
                    color = "";
                }
    
                if (gTestRunnerState.result == TEST_RESULT_PASS && gTestRunnerState.result != gTestRunnerState.expectedResult)
                {
                    MgbaPuts_("\e[31mPlease remove KNOWN_FAILING if this test intentionally PASSes\e[0m");
                }
    
                switch (gTestRunnerState.result)
                {
                case TEST_RESULT_FAIL:
                    if (gTestRunnerState.expectedResult == TEST_RESULT_FAIL)
                    {
                        result = "KNOWN_FAILING";
                        color = "\e[33m";
                    }
                    else
                    {
                        result = "FAIL";
                    }
                    break;
                case TEST_RESULT_TODO:
                    result = "TO_DO";
                    color = "\e[33m";
                    break;
                case TEST_RESULT_PASS: result = "PASS"; break;
                case TEST_RESULT_SKIP: result = "SKIP"; break;
                case TEST_RESULT_INVALID: result = "INVALID"; break;
                case TEST_RESULT_ERROR: result = "ERROR"; break;
                case TEST_RESULT_TIMEOUT: result = "TIMEOUT"; break;
                default: result = "UNKNOWN"; break;
                }
    
                MgbaPrintf_(":R%s%s\e[0m", color, result);
            }
    
            break;
    
        case STATE_EXIT:
            MgbaExit_(gTestRunnerState.exitCode);
            break;
    }
}

void Test_ExpectedResult(enum TestResult result)
{
    gTestRunnerState.expectedResult = result;
}

static void FunctionTest_SetUp(void *data)
{
    (void)data;
    gFunctionTestRunnerState = AllocZeroed(sizeof(*gFunctionTestRunnerState));
    SeedRng(0);
}

static void FunctionTest_Run(void *data)
{
    void (*function)(void) = data;
    do
    {
        if (gFunctionTestRunnerState->parameters)
            MgbaPrintf_(":N%s %d/%d", gTestRunnerState.test->name, gFunctionTestRunnerState->runParameter + 1, gFunctionTestRunnerState->parameters);
        gFunctionTestRunnerState->parameters = 0;
        function();
    } while (++gFunctionTestRunnerState->runParameter < gFunctionTestRunnerState->parameters);
}

static void FunctionTest_TearDown(void *data)
{
    (void)data;
    FREE_AND_SET_NULL(gFunctionTestRunnerState);
}

const struct TestRunner gFunctionTestRunner =
{
    .setUp = FunctionTest_SetUp,
    .run = FunctionTest_Run,
    .tearDown = FunctionTest_TearDown,
};

static void Assumptions_Run(void *data)
{
    void (*function)(void) = data;
    function();
}

const struct TestRunner gAssumptionsRunner =
{
    .run = Assumptions_Run,
};

#define IRQ_LR (*(vu32 *)0x3007F9C)

/* Returns to AgbMainLoop.
 * Similar to a longjmp except that we only restore sp (and cpsr via
 * overwriting the value of lr_irq on the stack).
 *
 * WARNING: This could potentially be flaky because other global state
 * will not be cleaned up, we may decide to Exit on a timeout instead. */
static NAKED void JumpToAgbMainLoop(void)
{
    asm(".arm\n\
         .word 0xe3104778\n\
         ldr r0, =gAgbMainLoop_sp\n\
         ldr sp, [r0]\n\
         ldr r0, =AgbMainLoop\n\
         bx r0\n\
         .pool");
}

void ReinitCallbacks(void)
{
    gMain.callback1 = NULL;
    SetMainCallback2(CB2_TestRunner);
    gMain.vblankCallback = NULL;
    gMain.hblankCallback = NULL;
}

static void Intr_Timer2(void)
{
    if (--gTestRunnerState.timeoutSeconds == 0)
    {
        if (gTestRunnerState.test->runner->checkProgress && gTestRunnerState.test->runner->checkProgress(gTestRunnerState.test->data))
        {
            gTestRunnerState.timeoutSeconds = TIMEOUT_SECONDS;
        }
        else
        {
            if (gTestRunnerState.state == STATE_RUN_TEST)
                gTestRunnerState.state = STATE_REPORT_RESULT;
            gTestRunnerState.result = TEST_RESULT_TIMEOUT;
            ReinitCallbacks();
            IRQ_LR = ((uintptr_t)JumpToAgbMainLoop & ~1) + 4;
        }
    }
}

void Test_ExitWithResult(enum TestResult result, const char *fmt, ...)
{
    gTestRunnerState.result = result;
    ReinitCallbacks();
    
    if (gTestRunnerState.state == STATE_REPORT_RESULT && gTestRunnerState.result != gTestRunnerState.expectedResult)
    {
        if (!gTestRunnerState.test->runner->handleExitWithResult || !gTestRunnerState.test->runner->handleExitWithResult(gTestRunnerState.test->data, result))
        {
            va_list va;
            va_start(va, fmt);
            MgbaVPrintf_(fmt, va);
            va_end(va);
        }
    }
    JumpToAgbMainLoop();
}

#define REG_DEBUG_ENABLE (*(vu16 *)0x4FFF780)
#define REG_DEBUG_FLAGS  (*(vu16 *)0x4FFF700)
#define REG_DEBUG_STRING ((char *)0x4FFF600)

static bool32 MgbaOpen_(void)
{
    REG_DEBUG_ENABLE = 0xC0DE;
    return REG_DEBUG_ENABLE == 0x1DEA;
}

static void MgbaExit_(u8 exitCode)
{
    register u32 _exitCode asm("r0") = exitCode;
    asm("swi 0x3" :: "r" (_exitCode));
}

static s32 MgbaPuts_(const char *s)
{
    return MgbaPrintf_("%s", s);
}

s32 MgbaPrintf_(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    return MgbaVPrintf_(fmt, va);
}

static s32 MgbaPutchar_(s32 i, s32 c)
{
    REG_DEBUG_STRING[i++] = c;
    
    if (i == 255)
    {
        REG_DEBUG_STRING[i] = '\0';
        REG_DEBUG_FLAGS = MGBA_LOG_INFO | 0x100;
        i = 0;
    }
    return i;
}

static const u8 sRSEtoASCIITable[256] = {
    [CHAR_SPACE] = ' ',
    0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d,
    0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94, 0x95,
    0x96, 0x97, 0x98, 0x99, 0x9a, 0x9b, 0x9c, 0x9d,
    0x9e, 0x9f, 0xa0, 0xe0, 0xe1, 0xe2, 0xe3, 0xe4,
    0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec,
    0xed, 0xee, 0xef, 0xf0, 0x7b, 0xf1, 0x7c, 0x7d,
    0x7e, 0x7f, 0x80, 0x81, 0x82, 0x83, 0x07, 0x08,
    0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x84,
    0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
    0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
    0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
    0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
    0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
    0xd9, 0xda, 0xdb, 0xdc, 0xa6, 0xdd, 0xa7, 0xa8,
    0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xf2, 0xf3,
    0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb,
    0xfc, 0xfd, 0xfe, 0xff, 0x01, 0x02, 0x03, 0x04,
    0x05, 0x06, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0xaf,
    [CHAR_0] = '0',
    [CHAR_1] = '1',
    [CHAR_2] = '2',
    [CHAR_3] = '3',
    [CHAR_4] = '4',
    [CHAR_5] = '5',
    [CHAR_6] = '6',
    [CHAR_7] = '7',
    [CHAR_8] = '8',
    [CHAR_9] = '9',
    [CHAR_EXCL_MARK] = '!',
    0xdf, 0xa1, 0xb0, 0xa5, 0xde, 0x24, 0x2a,
    0xa2, 0xa3, 0x22, 0x23, 0x20, 0xa4, 0x20,
    [CHAR_SLASH] = '/',
    [CHAR_A] = 'A',
    [CHAR_B] = 'B',
    [CHAR_C] = 'C',
    [CHAR_D] = 'D',
    [CHAR_E] = 'E',
    [CHAR_F] = 'F',
    [CHAR_G] = 'G',
    [CHAR_H] = 'H',
    [CHAR_I] = 'I',
    [CHAR_J] = 'J',
    [CHAR_K] = 'K',
    [CHAR_L] = 'L',
    [CHAR_M] = 'M',
    [CHAR_N] = 'N',
    [CHAR_O] = 'O',
    [CHAR_P] = 'P',
    [CHAR_Q] = 'Q',
    [CHAR_R] = 'R',
    [CHAR_S] = 'S',
    [CHAR_T] = 'T',
    [CHAR_U] = 'U',
    [CHAR_V] = 'V',
    [CHAR_W] = 'W',
    [CHAR_X] = 'X',
    [CHAR_Y] = 'Y',
    [CHAR_Z] = 'Z',
    [CHAR_a] = 'a',
    [CHAR_b] = 'b',
    [CHAR_c] = 'c',
    [CHAR_d] = 'd',
    [CHAR_e] = 'e',
    [CHAR_f] = 'f',
    [CHAR_g] = 'g',
    [CHAR_h] = 'h',
    [CHAR_i] = 'i',
    [CHAR_j] = 'j',
    [CHAR_k] = 'k',
    [CHAR_l] = 'l',
    [CHAR_m] = 'm',
    [CHAR_n] = 'n',
    [CHAR_o] = 'o',
    [CHAR_p] = 'p',
    [CHAR_q] = 'q',
    [CHAR_r] = 'r',
    [CHAR_s] = 's',
    [CHAR_t] = 't',
    [CHAR_u] = 'u',
    [CHAR_v] = 'v',
    [CHAR_w] = 'w',
    [CHAR_x] = 'x',
    [CHAR_y] = 'y',
    [CHAR_z] = 'z',
    0x20, 0x20, 0x2b, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f,
    [CHAR_DYNAMIC_PLACEHOLDER] = ' ',
    [CHAR_KEYPAD_ICON] = ' ',
    [CHAR_EXTRA_EMOJI] = ' ',
    [CHAR_PROMPT_SCROLL] = ' ',
    [CHAR_PROMPT_CLEAR] = ' ',
    [EXT_CTRL_CODE_BEGIN] = ' ',
    [PLACEHOLDER_BEGIN] = ' ',
    [CHAR_NEWLINE] = ' ',
    [EOS] = 0
};

// Bare-bones, only supports plain %s, %S, and %d.
static s32 MgbaVPrintf_(const char *fmt, va_list va)
{
    s32 i = 0;
    s32 c, d;
    const char *s;
    while (*fmt)
    {
        switch ((c = *fmt++))
        {
        case '%':
            switch (*fmt++)
            {
            case '%':
                i = MgbaPutchar_(i, '%');
                break;
            case 'd':
                d = va_arg(va, int);
                if (d == 0)
                {
                    i = MgbaPutchar_(i, '0');
                }
                else
                {
                    char buffer[10];
                    s32 n = 0;
                    u32 u = abs(d);
                    if (d < 0)
                        i = MgbaPutchar_(i, '-');
                    while (u > 0)
                    {
                        buffer[n++] = '0' + (u % 10);
                        u /= 10;
                    }
                    while (n > 0)
                        i = MgbaPutchar_(i, buffer[--n]);
                }
                break;
            case 'q':
                d = va_arg(va, int);
                {
                    char buffer[10];
                    s32 n = 0;
                    u32 u = abs(d) >> 12;
                    if (u == 0)
                    {
                        i = MgbaPutchar_(i, '0');
                    }
                    else
                    {
                        if (d < 0)
                            i = MgbaPutchar_(i, '-');
                        while (u > 0)
                        {
                            buffer[n++] = '0' + (u % 10);
                            u /= 10;
                        }
                        while (n > 0)
                            i = MgbaPutchar_(i, buffer[--n]);
                    }

                    n = 0;
                    i = MgbaPutchar_(i, '.');
                    u = d & 0xFFF;
                    while (TRUE)
                    {
                        u *= 10;
                        i = MgbaPutchar_(i, '0' + (u >> 12));
                        u &= 0xFFF;
                        if (u == 0)
                            break;
                        if (++n == 2)
                        {
                            u *= 10;
                            i = MgbaPutchar_(i, '0' + ((u + UQ_4_12_ROUND) >> 12));
                            break;
                        }
                    }
                }
                break;
            case 's':
                s = va_arg(va, const char *);
                while ((c = *s++) != '\0')
                    i = MgbaPutchar_(i, c);
                break;
            case 'S':
                s = va_arg(va, const u8 *);
                while ((c = *s++) != EOS)
                {
                    if ((c = sRSEtoASCIITable[c]) != '\0')
                        i = MgbaPutchar_(i, c);
                    else
                        i = MgbaPutchar_(i, '?');
                }
                break;
            }
            break;
        case '\n':
            i = 254;
            i = MgbaPutchar_(i, '\0');
            break;
        default:
            i = MgbaPutchar_(i, c);
            break;
        }
    }
    if (i != 0)
    {
        REG_DEBUG_FLAGS = MGBA_LOG_INFO | 0x100;
    }
    return i;
}
