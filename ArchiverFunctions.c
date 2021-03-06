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
    struct stat type;
    struct dirent *dir_ptr=readdir(Folder);
    int size;
    while(dir_ptr!=NULL)
    {
        if (strcmp((*dir_ptr).d_name, ".") != 0 && strcmp((*dir_ptr).d_name, "..") != 0)
        {
            printf("%s\n", (*dir_ptr).d_name);
            lstat((*dir_ptr).d_name, &type); 
            if (S_ISDIR(type.st_mode))
            {
                Packing("ZipTemp.zipper", (*dir_ptr).d_name, depth+1);
                lstat("ZipTemp.zipper", &type);
                PackFile("ZipTemp.zipper", OpenArchive, depth+1, (*dir_ptr).d_name);
                remove("ZipTemp.zipper");
            }
            else
            {   
                PackFile((*dir_ptr).d_name, OpenArchive, depth, "");
            }
        }
        dir_ptr=readdir(Folder);

    }
    closedir(Folder);
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
    int temp; 
    char FileName[MAXFILENAME];
    char buff;

    while(read(ArchiveOpen, &temp, sizeof(int)))
    {
        read(ArchiveOpen, FileName, MAXFILENAME);
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
    }
    remove(ArchivePath);
    return 0;
}


int PackFile(char *FilePath, int OpenArchive, int depth, char *FolderName)
{
    int size=0;
    int OpenFile=open(FilePath, O_RDONLY);
    struct stat SizeOfFile;
    lstat(FilePath, &SizeOfFile);

    size = SizeOfFile.st_size;

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

    write(OpenArchive, &depth, sizeof(int));
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
