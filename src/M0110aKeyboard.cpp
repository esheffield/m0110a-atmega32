#include <Arduino.h>
#include <M0110aKeyboard.hpp>

void M0110aKeyboard::begin(byte *row_pins, byte *col_pins, byte *mod_pins)
{
    this->row_pins = row_pins;
    this->col_pins = col_pins;
    this->mod_pins = mod_pins;

    // Initialize ROW_COUNT to INPUTs (HI-Z)
    // ROW_COUNT are active LOV, but inactive lines wil be HI-Z, not HIGH
    for (int i = 0; i < ROW_COUNT; i++)
    {
        pinMode(row_pins[i], INPUT);
    };

    // The columns are already pulled up externally on the Mac Plus keyboard
    // so no need for internal pullups here
    for (int i = 0; i < COL_COUNT; i++)
    {
        pinMode(col_pins[i], INPUT);
    };

    // The modifier keys on the other hand do NOT have external pullups
    for (int i = 0; i < MOD_COUNT; i++)
    {
        pinMode(mod_pins[i], INPUT_PULLUP);
    };
}

uint16_t M0110aKeyboard::getScanCode()
{
    uint16_t key_code = KEY_NULL;

    byte mod_values = readMods();

    // Code is getting printed from check_modifier for now, in future will add the return to the buffer
    key_code = check_modifier((OPT_MASK & mod_values), KEY_OPT, &opt_state);
    if (key_code)
        return key_code;

    key_code = check_modifier((CMD_MASK & mod_values), KEY_CMD, &cmd_state);
    if (key_code)
        return key_code;

    key_code = check_modifier((LCK_MASK & mod_values), KEY_LOCK, &lck_state);
    if (key_code)
        return key_code;

    key_code = check_modifier((SHF_MASK & mod_values), KEY_SHIFT, &shf_state);
    if (key_code)
        return key_code;

    for (int row = 0; row < ROW_COUNT; row++)
    {
        pinMode(row_pins[row], OUTPUT);
        digitalWrite(row_pins[row], LOW);
        byte col_values = readCols();
        pinMode(row_pins[row], INPUT);

        for (int col = 0; col < COL_COUNT; col++)
        {
            byte mask = 1 << col;
            key_code = KEYMAP[row][col];
            byte last_key_state = key_states[row] & mask;
            byte cur_key_state = col_values & mask;

            // Update current state
            key_states[row] = (key_states[row] & ~mask) | cur_key_state;

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
                    return NUMPAD2 | key_code;
                }
                else if (isKeypad(row, col))
                {
                    return NUMPAD | key_code;
                }

                return key_code;
            }
        }
    }

    return KEY_NULL;
}

byte M0110aKeyboard::readMods()
{
    byte mods = 0;

    mods |= digitalRead(mod_pins[CMD_PIN_IDX]) << 3;
    mods |= digitalRead(mod_pins[OPT_PIN_IDX]) << 2;
    mods |= digitalRead(mod_pins[LCK_PIN_IDX]) << 1;
    mods |= digitalRead(mod_pins[SHF_PIN_IDX]);

    return ~mods & 0x0F;
}

byte M0110aKeyboard::readCols()
{
    byte cols = 0;
    for (int i = COL_COUNT - 1; i >= 0; i--)
    {
        cols |= digitalRead(col_pins[i]) << i;
    }

    return ~cols;
}

// Check if the key is a keypad or arrow key needing a "keypad" prefix (0x79)
bool M0110aKeyboard::isKeypad(byte active_row, byte col)
{
    byte kp_row = KEYPAD_MAP_BY_COL[col];
    return active_row && kp_row;
}

// Check if the key is one of the keypad keys that needs a "shift" prefix (0x71)
bool M0110aKeyboard::isKeypadShift(byte row, byte col)
{
    return (col == 5 && (row == 9)) || (col == 6 && (row == 2 || row == 3));
}

// Checks if a given modifier key (Option, Command, Shift, Shift-Lock)
// has transitioned and returns the key_code with transition flag if so
byte M0110aKeyboard::check_modifier(byte masked_value, byte mod_key_code, byte *state)
{
    byte key_code = mod_key_code;
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

    return key_code;
}
