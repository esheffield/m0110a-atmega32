#include <Arduino.h>

#define MODEL_ID 0x0B

#define CMD_MODEL 0x16
#define CMD_INQUIRY 0x10
#define CMD_INSTANT 0x14
#define CMD_TEST 0x36

#define KEY_GRAVE 0x65
#define KEY_1 0x25
#define KEY_2 0x27
#define KEY_3 0x29
#define KEY_4 0x2B
#define KEY_5 0x2F
#define KEY_6 0x2D
#define KEY_7 0x35
#define KEY_8 0x39
#define KEY_9 0x33
#define KEY_0 0x3B
#define KEY_MINUS 0x37
#define KEY_EQUAL 0x31
#define KEY_BS 0x67
#define KEY_TAB 0x61
#define KEY_Q 0x19
#define KEY_W 0x1B
#define KEY_E 0x1D
#define KEY_R 0x1F
#define KEY_T 0x23
#define KEY_Y 0x21
#define KEY_U 0x41
#define KEY_I 0x45
#define KEY_O 0x3F
#define KEY_P 0x47
#define KEY_LBKT 0x43
#define KEY_RBKT 0x3D
#define KEY_LOCK 0x73
#define KEY_A 0x01
#define KEY_S 0x03
#define KEY_D 0x05
#define KEY_F 0x07
#define KEY_G 0x0B
#define KEY_H 0x09
#define KEY_J 0x4D
#define KEY_K 0x51
#define KEY_L 0x4B
#define KEY_SEMI 0x53
#define KEY_APOS 0x4F
#define KEY_RETURN 0x49
#define KEY_SHIFT 0x71
#define KEY_Z 0x0D
#define KEY_X 0x0F
#define KEY_C 0x11
#define KEY_V 0x13
#define KEY_B 0x17
#define KEY_N 0x5B
#define KEY_M 0x5D
#define KEY_COMMA 0x57
#define KEY_PERIOD 0x5F
#define KEY_SLASH 0x59
#define KEY_UP 0x1B
#define KEY_OPT 0x75
#define KEY_CMD 0x6F
#define KEY_SPC 0x63
#define KEY_BSLASH 0x55
#define KEY_LEFT 0x0D
#define KEY_RIGHT 0x05
#define KEY_DOWN 0x11
#define KEY_KP_CLR 0x0F
#define KEY_KP_EQUAL 0x11
#define KEY_KP_DIV 0x1B
#define KEY_KP_MUL 0x05
#define KEY_KP_7 0x33
#define KEY_KP_8 0x37
#define KEY_KP_9 0x39
#define KEY_KP_SUB 0x1D
#define KEY_KP_4 0x2D
#define KEY_KP_5 0x2F
#define KEY_KP_6 0x31
#define KEY_KP_ADD 0x0D
#define KEY_KP_1 0x27
#define KEY_KP_2 0x29
#define KEY_KP_3 0x2B
#define KEY_KP_0 0x25
#define KEY_KP_DEC 0x03
#define KEY_KP_ENTER 0x19

#define KP_MOD 0x79

#define KEY_TXN_UP 0x80
#define KEY_NULL 0x00

const unsigned int KEYPAD_MAP_BY_COL[8] = {
    0,
    0,
    0,
    0,
    0x380,
    0x3DE,
    0x01e,
    0x380,
};

const byte KEYMAP[10][8] = {
    {
        KEY_X,
        KEY_GRAVE,
        KEY_TAB,
        KEY_A,
        KEY_SPC,
        KEY_NULL,
        KEY_Z,
        KEY_NULL,
    },
    {
        KEY_C,
        KEY_1,
        KEY_Q,
        KEY_S,
        KEY_NULL,
        KEY_KP_7,
        KEY_NULL,
        KEY_NULL,
    },
    {
        KEY_V,
        KEY_2,
        KEY_W,
        KEY_D,
        KEY_P,
        KEY_KP_8,
        KEY_KP_MUL,
        KEY_NULL,
    },
    {
        KEY_B,
        KEY_3,
        KEY_E,
        KEY_F,
        KEY_LBKT,
        KEY_KP_9,
        KEY_KP_DIV,
        KEY_BSLASH,
    },
    {
        KEY_N,
        KEY_4,
        KEY_R,
        KEY_G,
        KEY_RBKT,
        KEY_KP_SUB,
        KEY_KP_EQUAL,
        KEY_LEFT,
    },
    {
        KEY_M,
        KEY_5,
        KEY_T,
        KEY_H,
        KEY_APOS,
        KEY_UP,
        KEY_KP_CLR,
        KEY_RIGHT,
    },
    {
        KEY_COMMA,
        KEY_6,
        KEY_Y,
        KEY_J,
        KEY_RETURN,
        KEY_KP_1,
        KEY_BS,
        KEY_DOWN,
    },
    {
        KEY_PERIOD,
        KEY_7,
        KEY_U,
        KEY_K,
        KEY_KP_4,
        KEY_KP_2,
        KEY_EQUAL,
        KEY_KP_0,
    },
    {
        KEY_SLASH,
        KEY_8,
        KEY_I,
        KEY_L,
        KEY_KP_5,
        KEY_KP_3,
        KEY_MINUS,
        KEY_KP_DEC,
    },
    {
        KEY_NULL,
        KEY_9,
        KEY_O,
        KEY_SEMI,
        KEY_KP_6,
        KEY_KP_ADD,
        KEY_0,
        KEY_KP_ENTER,
    },
};
