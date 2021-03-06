
#include "ArchiverHeader.h"

int main(int argc, char *argv[])
{

    if (strcmp(argv[1], "-a")==0)
    {
        printf("Archiving...\n");
        PackArchive(argv[2], argv[3], 0);
        printf("Done!\n");
    }
    else if (strcmp(argv[1], "-d")==0)
    {
        printf("Unpacking...\n");
        UnpackArchive(argv[2], 0, "FolderForUnpackedFiles");
        printf("Done!\n");
    }
    else
    {
        printf("Error. Unknown arguments\n");
    }

    return 0;
}