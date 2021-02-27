#include "ArchiverHeader.h"

int Packing(char *FilePath, char *ArchiverPath, int depth)
{    
    int OpenArchive=open(ArchiverPath, O_CREAT|O_WRONLY|O_APPEND, ALLOW);

    if (OpenArchive==-1)
    {
        printf("Couldn't create or open archive\n");
        return OpenArchive;
    }

    DIR *Folder=opendir(FilePath);
    if(Folder==NULL)
    {
        printf("Can not open a folder\n");
        return -1;
    }


    chdir(FilePath);
    struct stat filesizestat;
    struct dirent *dir_ptr=readdir(Folder);
    int size;
    while(dir_ptr!=NULL)
    {
        if (strcmp((*dir_ptr).d_name, ".") != 0 && strcmp((*dir_ptr).d_name, "..") != 0)
        {
            printf("%s\n", (*dir_ptr).d_name);
            lstat((*dir_ptr).d_name, &filesizestat); 
            size=filesizestat.st_size;
            PackFile((*dir_ptr).d_name, OpenArchive, depth, size);
        }
        dir_ptr=readdir(Folder);

    }
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
        printf("File %s contains %i bytes\n", FileName, size); 
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


int PackFile(char *FilePath, int OpenArchive, int depth, int size)
{
   
    int OpenFile=open(FilePath, O_RDONLY);

    if (OpenFile==-1)
    {
        printf("Couldn't open file\n");
        return OpenFile;
    }

    if (OpenArchive==-1)
    {
        printf("Couldn't create or open archive\n");
        return OpenArchive;
    }


    write(OpenArchive, FilePath, MAXFILENAME);
    write(OpenArchive, &size, sizeof(int));

    char buff;
    
    while(size)
    {
        read(OpenFile, &buff, sizeof(char));
        write(OpenArchive, &buff, sizeof(char));
        --size;
    }
    printf("%s packed\n", FilePath);
    close(OpenFile);
   //remove(FilePath);  //commented for development stage
    return 0;
}
