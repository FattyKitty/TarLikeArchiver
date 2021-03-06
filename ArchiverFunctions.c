#include "ArchiverHeader.h"

int PackArchive(char *FilePath, char *ArchiverPath, int depth)
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
    while(dir_ptr!=NULL)
    {
        if (strcmp((*dir_ptr).d_name, ".") != 0 && strcmp((*dir_ptr).d_name, "..") != 0)
        {
            lstat((*dir_ptr).d_name, &type); //getting info about content of directory
            if (S_ISDIR(type.st_mode)) //checking for directory
            {
                PackArchive((*dir_ptr).d_name, "Temp.zipper", depth+1);             //
                PackFile("Temp.zipper", OpenArchive, depth+1, (*dir_ptr).d_name);   // packing folder + it's content
                remove("Temp.zipper");                                              //
            }
            else
            {   
                PackFile((*dir_ptr).d_name, OpenArchive, depth, ""); //packing file
            }
        }
        dir_ptr=readdir(Folder);

    }
    chdir(".."); //getting to the upper level
    closedir(Folder);
    close(OpenArchive);
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

    if (strcmp("", FolderName)==0)
      write(OpenArchive, FilePath, MAXFILENAME);
    else
        write(OpenArchive, FolderName, MAXFILENAME);


    write(OpenArchive, &depth, sizeof(int));    //infomation about how deep file was held
    write(OpenArchive, &size, sizeof(int));     //writing information about the file

    char buff;
    
    while(size)
    {
        read(OpenFile, &buff, sizeof(char));
        write(OpenArchive, &buff, sizeof(char));
        --size;
    }
    close(OpenFile);
    return 0;
}

int UnpackFile(char *FileName, int size, int ArchiveDescr)
{
   int OpenNewFile=open(FileName, O_CREAT|O_WRONLY, ALLOW);     //creating file after unpacking info about it
   char buff;
        while(size)
        {
            read(ArchiveDescr, &buff, sizeof(char));
            write(OpenNewFile, &buff, sizeof(char));
            --size;
        }
        close(OpenNewFile);
    return 0;
}

int UnpackArchive(char *ArchivePath, int depth, char *ZipFolder)
{
    int ArchiveOpen=open(ArchivePath, O_RDONLY);

    if (ArchiveOpen==-1)
    {
        printf("Cannot open archive down this path");
        return ArchiveOpen;
    }

    DIR *Folder=opendir(ZipFolder);    

    if (Folder == NULL)
    {
        mkdir(ZipFolder, S_IRWXO|S_IRWXU|S_IRWXG);      //making folder in case it was not found or created before
    }

    closedir(Folder);

    chdir(ZipFolder);

    int size;
    int depthfinder;
    char FileName[MAXFILENAME];

    while(read(ArchiveOpen, FileName, MAXFILENAME))                 //unpacking cycle
    {
        read(ArchiveOpen, &depthfinder, sizeof(int));
        read(ArchiveOpen, &size, sizeof(int)); 
        if (depthfinder==depth+1)
        {
            UnpackFile("Unzippertemp.zipper", size, ArchiveOpen);
            UnpackArchive("Unzippertemp.zipper", depth+1, FileName);
            remove("Unzippertemp.zipper");
        }
        else if (depth==depthfinder)
            UnpackFile(FileName, size, ArchiveOpen);                    //unpacking files, which were laying in the root of a folder
        else
        {
            printf("Error. Depth not matching!. Check archiving algorithm for possible mistakes or mistake has been made in specifiying start parameter.\n"); 
            return -1;
        }    
    }
    close(ArchiveOpen);
    chdir("..");  //getting to the upper level
    remove(ArchivePath);
    return 0;
}