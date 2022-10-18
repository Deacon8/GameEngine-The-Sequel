#include <stdio.h>
#include "memory.h"

int GetFileSize(char* path)
{
	FILE* file = fopen(path, "r");
	int size = 0;
	if (file != NULL) 
	{
		fseek(file, 0, SEEK_END);
		size = ftell(file);
		fclose(file);
	}
	return size;
}