#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define RED   "\033[0;31m"
#define GREEN     "\033[0;32m"
#define RESET   "\033[0m"
#define BLUE    "\033[0;34m"
#define BOLD    "\033[1m"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf(RED "Missing arguments! Use -help for usage instructions.\n" RESET);
        return -1;
    }

    for (int i = 1; i < argc; i++) {
        // Dash-insensitive
        char *arg = argv[i];
        while (*arg == '-') arg++;

        // Case-insensitive
        for (char *p = arg; *p; p++) *p = tolower(*p);

        if (!strcmp(arg, "help") || !strcmp(arg, "h"))
            printf(
                GREEN "🌸 Welcome to Flower Compiler!\n\n" RESET
    
                BLUE "Usage:\n" RESET
                "\tflower\t[options] <filepath>\n\n"
    
                BLUE "Options:\n" RESET
                "\t-help,    -h\tShow this help message\n"
                "\t-version, -b\tShow the current version of FloC\n"
                "\t<filepath>\tSpecify the source code file to compile\n\n"
    
                BLUE "Example:\n" RESET
                "\tflower -help\n"
                "\tflower main.flo\n\n"
    
                BLUE "Tips:\n" RESET
                " - You can use any number of dashes before a flag, e.g., ---help\n"
                " - Flags are " BOLD "case-insensitive" RESET ": -HELP works too!\n\n"
    
                GREEN "Happy Compiling with Flower! 🌼\n" RESET
            );

        else if (!strcmp(arg, "version") || !strcmp(arg, "v"))
            printf(
                "Version: 0.0.1\n"
            );

        else
            printf(RED "Unrecognized flag argument! Use -help for more information.\n" RESET);
    }

    // printf("%s", argv[1]);

    return 0;
}