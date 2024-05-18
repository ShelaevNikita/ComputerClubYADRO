
/*
    Файл для запуска класса ComputerClub и передачи ему
        в качестве аргумента названия входного файла
*/

#include "include/ComputerClub.h"

int main(int argc, const char *argv[]) {
    // Все аргументы командной строки, начиная с 1-го, 
    //  используются как названия входных файлов
    if (argc > 1)
        for (unsigned short i = 1; i < argc; i++)
            ComputerClub::computerClubMain(argv[i]);

    // Если аргумент только 1 (0-й), то пользователь должен
    //  ввести название входного файла из директории "examples/"
    else {
        cout << " Please, enter filename: examples/";
        string input = "\0";
        cin >> input;
        if ((input[0] != '\x0') && (input[0] != '\n'))
            ComputerClub::computerClubMain("examples/" + input);
    }
    return 0;
}