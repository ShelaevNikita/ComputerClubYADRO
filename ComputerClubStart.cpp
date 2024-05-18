
#include "include/ComputerClub.h"

int main(int argc, const char *argv[]) {
    if (argc > 1)
        for (unsigned short i = 1; i < argc; i++)
            ComputerClub::computerClubMain(argv[i]);
    else {
        cout << " Please, enter filename: examples/";
        string input = "\0";
        cin >> input;
        if ((input[0] != '\x0') && (input[0] != '\n'))
            ComputerClub::computerClubMain("examples/" + input);
    }
    return 0;
}