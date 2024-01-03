#include <Arduino.h>

// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
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
                if (is_keypad_shift(active_row, col))
                {
                    print_key_code(KEY_SHIFT);
                    print_key_code(KP_MOD);
                }
                else if (is_keypad(active_row, col))
                {
                    print_key_code(KP_MOD);
                }
            }
            print_key_code(key_code);
        }

        key_states[row] = col_values;
        row = (row + 1) % 10;
    }
}
*/

/*
// Check if the key is a keypad or arrow key needing a "keypad" prefix (0x79)
bool is_keypad(uint active_row, char col)
{
    uint kp_row = KEYPAD_MAP_BY_COL[col];
    return active_row && kp_row;
}

// Check if the key is one of the keypad keys that needs a "shift" prefix (0x71)
bool is_keypad_shift(uint active_row, char col)
{
    return (col == 5 && (0x020000 & active_row))
        || (col == 6 && (0x000C00 & active_row));
}

// Debug method to print out a key code with transition flag
void print_key_code(char key_code)
{
    if (key_code)
    {
        char *txn = (key_code & KEY_TXN_UP) ? "Released" : "Pressed";
        printf("Key code: %02X %s\n", key_code, txn);
    }
}

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

    print_key_code(key_code);

    return key_code;
}
*/