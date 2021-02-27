#include "ArchiverHeader.h"

int main(int argc, char *argv[])
{
    int FileOpen=open(argv[2], O_RDONLY);

    if (strcmp(argv[1], "-a")==0)
    {
        printf("Archiving\n");
        Packing(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "-d")==0)
    {
        printf("Unpacking\n");
        Unpacking(argv[2]);
    }
    else
    {
        printf("Error\n");
    }

    return 0;
}
