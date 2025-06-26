#include "keyboard.h"

// ==== Дебаунс и опрос ====
#define DEBOUNCE_MS 20

DigitalOut col[] = {
    DigitalOut(PC_4), // D3 - COL0
    DigitalOut(PB_13), // D4 - COL1
    DigitalOut(PB_14), // D5 - COL2
    DigitalOut(PB_15)  // D6 - COL3
};

DigitalIn row[] = {
    DigitalIn(PB_1, PullUp),  // D7 - ROW0
    DigitalIn(PB_2, PullUp),  // D8 - ROW1
    DigitalIn(PB_12, PullUp),  // D9 - ROW2
    DigitalIn(PA_11, PullUp)   // D10 - ROW3
};

// ==== Раскладка клавиатуры (4x4) ====
char keys[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

char get_key() {
    for (int c = 0; c < 4; c++) {
        // Устанавливаем все столбцы в высокий уровень
        for (int i = 0; i < 4; i++) col[i] = 1;
        col[c] = 0; // Активируем текущий столбец

        for (int r = 0; r < 4; r++) {
            if (!row[r]) { // Если строка активна (низкий уровень)
                return keys[c][r];
            }
        }
    }
    return '\0'; // Нет нажатия
}

// ==== Последний нажатый символ ====
char last_key = '\0';

// ==== Пароль ====
const char correct_password[] = "1234";
char input_password[4];
int pass_index = 0;


int handle_password_input(char next){
    input_password[pass_index++] = next;
    if (pass_index >= strlen(correct_password)){
        if(strcmp(input_password, correct_password) == 0){
            printf("Correct password.\n");
            toggle_relay();
            pass_index = 0;
            return 0;
        }  
        pass_index = 0;
        printf("Incorrect password.\n");
        return 1;
    }
    input_password[pass_index] = '\0';
    printf("password: %s\n", input_password);

    return pass_index;
}

void process_keyboard(){
    while(true){      
        char key = get_key();
        if (key != '\0' && key != last_key) {
            
            handle_password_input(key);
            last_key = key;
        }

        if (key == '\0') {
            last_key = '\0'; // Сброс после отпускания
        }

        ThisThread::sleep_for(DEBOUNCE_MS);
        
    }
}