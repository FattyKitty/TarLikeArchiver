#include "ArchiverHeader.h"

int Packing(char *FilePath, char *ArchivePath)
{
    int OpenArchive=open(ArchivePath, O_CREAT|O_WRONLY|O_APPEND, ALLOW);


    int OpenFile=open(FilePath, O_RDONLY);

    if (OpenArchive==-1)
    {
        printf("Couldn't create or open archive\n");
        return OpenArchive;
    }
    
    if (OpenFile==-1)
    {
        printf("Couldn't open file\n");
        return OpenFile;
    }

    char *FileName=basename(FilePath);
    int size;

    struct stat filesizestat;

    lstat(FilePath, &filesizestat);
    size=filesizestat.st_size;

    

    write(OpenArchive, FileName, MAXFILENAME);

    write(OpenArchive, &size, sizeof(int));

    char buff;

    while(size)
    {
        read(OpenFile, &buff, sizeof(char));
        write(OpenArchive, &buff, sizeof(char));
        --size;
    }
    close(OpenFile);
    close(OpenArchive);
    remove(FilePath);
    return 0;
}

int Unpacking(char *ArchivePath)
{
    int ArchiveOpen=open(ArchivePath, O_RDONLY);
    

    if (ArchiveOpen==-1)
    {
        printf("Cannot open archive down this path");
        return ArchiveOpen;
    }

    int size; //possible to remove 
    char FileName[MAXFILENAME];
    char buff;

    while(read(ArchiveOpen, FileName, MAXFILENAME))
    {
        read(ArchiveOpen, &size, sizeof(int));

        int OpenNewFile=open(FileName, O_CREAT|O_WRONLY, ALLOW); 
        while(size)
        {
            read(ArchiveOpen, &buff, sizeof(char));
            write(OpenNewFile, &buff, sizeof(char));
            --size;
        }
        close(OpenNewFile);
        memset(FileName, 0, MAXFILENAME);
    }
    remove(ArchivePath);
    return 0;
}
