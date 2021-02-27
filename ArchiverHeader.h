#define ALLOW 00666
#define MAXFILENAME 50
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <libgen.h>
#include <fcntl.h>
#include <dirent.h>

int Packing(char *FilePath, char* ArchiverPath);
int Unpacking(char *ArchivePath);
