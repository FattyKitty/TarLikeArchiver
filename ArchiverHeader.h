#define ALLOW 00666
#define MAXFILENAME 50

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int PackArchive(char *FilePath, char* ArchiverPath, int depth);
int PackFile(char *FilePath, int OpenArchive, int depth, char *FolderName);
int UnpackFile(char *FileName, int size, int ArchiveDescr);
int UnpackArchive(char *ArchivePath, int depth, char *ZipFolder);