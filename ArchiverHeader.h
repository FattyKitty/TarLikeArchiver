#define ALLOW 00666 //code of restriction to open and write to folder
#define MAXFILENAME 50

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int PackArchive(char *FilePath, char* ArchiverPath, int depth); //packing folder
int PackFile(char *FilePath, int OpenArchive, int depth, char *FolderName); //packing one sigle file
int UnpackFile(char *FileName, int size, int ArchiveDescr); //unpacking one single file
int UnpackArchive(char *ArchivePath, int depth, char *ZipFolder); //unpacking one or multiple folders