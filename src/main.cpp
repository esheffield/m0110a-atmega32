#include <Arduino.h>
#include <wiring_extras.h>
#include "m0110a.h"

#define CMD_MASK 0x08
#define OPT_MASK 0x04
#define LCK_MASK 0x02
#define SHF_MASK 0x01

const byte ROWS = 10;
const byte COLS = 8;
const byte MODS = 4;

const byte CLK_PIN = PIN_PD3;
const byte DATA_PIN = PIN_PD2;

const byte ROW_PINS[] = {PIN_PB0, PIN_PB1, PIN_PB2, PIN_PB3, PIN_PB4, PIN_PB5, PIN_PB6, PIN_PB7, PIN_PA0, PIN_PA1};
const byte COL_PINS[] = {PIN_PC0, PIN_PC1, PIN_PC2, PIN_PC3, PIN_PC4, PIN_PC5, PIN_PC6, PIN_PC7};
const byte COL_PORT = PORTC;

const byte CMD_PIN = PIN_PA5;
const byte OPT_PIN = PIN_PA4;
const byte LCK_PIN = PIN_PA3;
const byte SHF_PIN = PIN_PA2;

const byte MOD_PINS[] = {CMD_PIN, OPT_PIN, LCK_PIN, SHF_PIN};

byte key_states[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
byte opt_state = 0;
byte cmd_state = 0;
byte lck_state = 0;
byte shf_state = 0;

// put function declarations here:

byte readMods();
byte readCols();
void initRows();

bool isKeypad(byte active_row, byte col);
bool isKeypadShift(byte active_row, byte col);
void printKeyCode(byte key_code);

void setup()
{
    for (int i = 0; i < ROWS; i++)
    {
        pinMode(ROW_PINS[i], INPUT);
    };

    for (int i = 0; i < COLS; i++)
    {
        pinMode(COL_PINS[i], INPUT);
    };

    for (int i = 0; i < MODS; i++)
    {
        pinMode(MOD_PINS[i], INPUT_PULLUP);
    };

    // pinMode(CLK_PIN, OUTPUT);
    // pinMode(DATA_PIN, INPUT);

    Serial.begin(9600);
}

void loop()
{
    byte mods = readMods();
    if (mods != 0x0F)
    {
        Serial.printf("Mods:  %02X\n\r", mods);
    }

    initRows();
    Serial.printf("--------------------\n\r");

    char key_code = KEY_NULL;
    for (int row = 0; row < ROWS; row++)
    {
        pinMode(ROW_PINS[row], OUTPUT);
        digitalWrite(ROW_PINS[row], LOW);
        byte col_values = readCols();
        pinMode(ROW_PINS[row], INPUT);

        Serial.printf("ROW: %d\tCOLS: %02X\n\r", row, col_values);

        byte key_col_states = key_states[row];
        for (int col = 0; col < COLS; col++)
        {
            byte mask = 1 << col;
            key_code = KEYMAP[row][col];
            byte last_key_state = key_col_states & mask;
            byte cur_key_state = col_values & mask;

            if (last_key_state && !cur_key_state) // key transition from pressed to released
            {
                // Set bit 7 to indicate key release
                key_code |= KEY_TXN_UP;
            }
            else if (cur_key_state == last_key_state) // key state unchanged
            {
                key_code = KEY_NULL;
            }

            if (key_code)
            {
                if (isKeypadShift(row, col))
                {
                    printKeyCode(KEY_SHIFT);
                    printKeyCode(KP_MOD);
                }
                else if (isKeypad(row, col))
                {
                    printKeyCode(KP_MOD);
                }
            }
            printKeyCode(key_code);
        }

        key_states[row] = col_values;
    }

    delay(2000);
}

byte readMods()
{
    byte cmd = digitalRead(CMD_PIN) << 3;
    byte opt = digitalRead(OPT_PIN) << 2;
    byte lck = digitalRead(LCK_PIN) << 1;
    byte shf = digitalRead(SHF_PIN);

    return cmd | opt | lck | shf;
}

byte readCols()
{
    byte cols = 0;
    for (int i = COLS - 1; i >= 0; i--)
    {
        cols |= digitalRead(COL_PINS[i]) << i;
    }

    return ~cols;
}

void initRows()
{
    for (int i = 0; i < ROWS; i++)
    {
        digitalWrite(ROW_PINS[i], HIGH);
    }
}

// Check if the key is a keypad or arrow key needing a "keypad" prefix (0x79)
bool isKeypad(byte active_row, byte col)
{
    byte kp_row = KEYPAD_MAP_BY_COL[col];
    return active_row && kp_row;
}

// Check if the key is one of the keypad keys that needs a "shift" prefix (0x71)
bool isKeypadShift(byte row, byte col)
{
    return (col == 5 && (row == 9)) || (col == 6 && (row == 2 || row == 3));
}

// Debug method to print out a key code with transition flag
void printKeyCode(byte key_code)
{
    if (key_code)
    {
        const char *txn = (key_code & KEY_TXN_UP) ? "Released" : "Pressed";
        Serial.printf("Key code: %02X %s\n\r", key_code, txn);
    }
}

/*
int main()
{

    bi_decl(bi_program_description("M0110A controller replacement"));

    stdio_init_all();

    // comms pins are managed by pio

    gpio_init_mask(MATRIX_INIT_MASK);
    gpio_set_dir_masked(MATRIX_INIT_MASK, MATRIX_DIR_MASK);

    // Set pull-downs on the column inputs
    for (uint col = 0; col < COLS; col++)
    {
        gpio_pull_down(COL_PINS[col]);
    }

    // Set pull-ups on the modifier keys
    for (uint mod = 0; mod < MODS; mod++)
    {
        gpio_pull_up(MOD_PINS[mod]);
    }

    // Just here to give USB connection a chance to finish connecting for debugging printfs
    sleep_ms(2000);
    printf("Begin scanning...\n");

    char row = 0;
    while (1)
    {
        uint active_row = 1 << (8 + row);

        // Set only the active row to output, others input so will effectively float
        gpio_set_dir_masked(ROW_INIT_MASK, active_row);
        sleep_ms(5);
        gpio_put_masked(ROW_INIT_MASK, active_row);
        sleep_ms(5);

        volatile uint raw_cols = gpio_get_all();

        char key_code = KEY_NULL;

        uint mod_values = (~raw_cols) & MOD_INIT_MASK;

        // Code is getting printed from check_modifier for now, in future will add the return to the buffer
        key_code = check_modifier((OPT_PIN_MASK & mod_values), KEY_OPT, &opt_state);
        key_code = check_modifier((CMD_PIN_MASK & mod_values), KEY_CMD, &cmd_state);
        key_code = check_modifier((LCK_PIN_MASK & mod_values), KEY_LOCK, &lck_state);
        key_code = check_modifier((SHF_PIN_MASK & mod_values), KEY_SHIFT, &shf_state);

        key_code = KEY_NULL;
        char col_values = raw_cols & COL_INIT_MASK;

        char key_col_states = key_states[row];
        for (char col = 0; col < COLS; col++)
        {
            char mask = 1 << col;
            key_code = KEYMAP[row][col];
            char last_key_state = key_col_states & mask;
            char cur_key_state = col_values & mask;

            if (last_key_state && !cur_key_state) // key transition from pressed to released
            {
                // Set bit 7 to indicate key release
                key_code |= KEY_TXN_UP;
            }
            else if (cur_key_state == last_key_state) // key state unchanged
            {
                key_code = KEY_NULL;
            }

            if (key_code)
            {
                if (isKeypadShift(active_row, col))
                {
                    printKeyCode(KEY_SHIFT);
                    printKeyCode(KP_MOD);
                }
                else if (isKeypad(active_row, col))
                {
                    printKeyCode(KP_MOD);
                }
            }
            printKeyCode(key_code);
        }

        key_states[row] = col_values;
        row = (row + 1) % 10;
    }
}
*/

/*
// Checks if a given modifier key (Option, Command, Shift, Shift-Lock)
// has transitioned and returns the key_code with transition flag if so
char check_modifier(uint masked_value, char mod_key_code, char *state)
{
    char key_code = mod_key_code;
    if (!masked_value && *state)
    {
        key_code |= KEY_TXN_UP;
        *state = 0;
    }
    else if (masked_value && !(*state))
    {
        *state = 1;
    }
    else
    {
        key_code = KEY_NULL;
    }

    printKeyCode(key_code);

    return key_code;
}
*/
