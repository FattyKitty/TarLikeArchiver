#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>
#include <fcntl.h>

const int MAXFILENAME=50;

int Packing(char *FilePath, char* ArchiverPath);
int Unpacking(char *ArchivePath);

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

int Packing(char *FilePath, char *ArchivePath)
{
    int OpenArchive=open(ArchivePath, O_CREAT|O_WRONLY, 00666);
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
        printf("%c", buff);
        write(OpenArchive, &buff, sizeof(char));
        --size;
    }
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

    int size;
    char FileName[MAXFILENAME];
    char buff;

    read(ArchiveOpen, FileName, MAXFILENAME);

    read(ArchiveOpen, &size, sizeof(int));

    int OpenNewFile=open(FileName, O_CREAT|O_WRONLY, 00666); 
    while(size)
    {
        read(ArchiveOpen, &buff, sizeof(char));
        write(OpenNewFile, &buff, sizeof(char));
        --size;
    }
    remove(ArchivePath);
    return 0;
}