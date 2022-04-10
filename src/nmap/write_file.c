#include	"nmap.h"

void write_file(char* fileName, char* buff, char* mode) {
    FILE *outFile = fopen(fileName, mode);
    if (outFile == NULL) {
        err_sys("Can't open file");
    }

    fprintf(outFile, "%s", buff);
    fclose(outFile);
}
