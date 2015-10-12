#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <stdlib.h>

int main(int argc,char *argv[])
{
	int CopyFile(char find1[MAX_PATH],char target[MAX_PATH],TCHAR name1[MAX_PATH]);  //复制文件的函数
	int CopyDir(char path[MAX_PATH],char target[MAX_PATH]);                          //复制目录的函数

	WIN32_FIND_DATA filestruct;

	char path_search[MAX_PATH],buffer[512];                                        //寻找路径

	DWORD WLength,RLength;
	HANDLE hFile_r,hFile_w;
	HANDLE hFind;
	char path[MAX_PATH],path_target[MAX_PATH];
	strcpy(path,argv[1]);
	strcpy(path_search,path);
	strcpy(path_target,argv[2]);
	strcat(path_search,"\\*.*");
	hFind = FindFirstFile(path_search,&filestruct);

	//文件内部是否含有目录文件
	if(filestruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
	{
		CreateDirectory(path_target,NULL);
		CopyDir(path,path_target);    //复制目录文件
	}

	//直接复制文件
	else
	{
		hFile_r=CreateFile(argv[1], GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		hFile_w=CreateFile(argv[2], GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		ReadFile(hFile_r,buffer,512,&RLength,NULL);
		while(RLength>0)
		{
			WriteFile(hFile_w,buffer,RLength,&WLength,NULL);
			ReadFile(hFile_r,buffer,512,&RLength,NULL);
		}
	}
	printf("Copy succeed!\n");
	return 0;
}


//直接复制文件
int CopyFile(char path_find[MAX_PATH],char path_target[MAX_PATH],TCHAR filename[MAX_PATH])
{
	HANDLE hFile_r,hFile_w;
	char buffer[512],path_temp[MAX_PATH];
	DWORD WLength,RLength;
	strcpy(path_temp,path_target);
	strcat(path_temp,"\\");
	strcat(path_temp,filename);
	hFile_r=CreateFile(path_find, GENERIC_READ, NULL, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	hFile_w=CreateFile(path_temp, GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(hFile_r,buffer,512,&RLength,NULL);
	while(RLength>0)
	{
		WriteFile(hFile_w,buffer,RLength,&WLength,NULL);
		ReadFile(hFile_r,buffer,512,&RLength,NULL);
	}
	return 1;
}

//递归调用完成复制
int CopyDir(char path[MAX_PATH],char path_target[MAX_PATH])
{
	WIN32_FIND_DATA filestruct;
	char path_search[MAX_PATH],path_find[MAX_PATH],path_temp[MAX_PATH],path_cre[MAX_PATH];
	HANDLE hFind;
	strcpy(path_search,path);
	strcat(path_search,"\\*.*");
	hFind = FindFirstFile(path_search,&filestruct);
	if((hFind ==  INVALID_HANDLE_VALUE))
		return 0;

	do{
		if(filestruct.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if(filestruct.cFileName[0]!='.')
			{
				strcpy(path_temp,path);
				strcat(path_temp,"\\");
				strcat(path_temp,filestruct.cFileName);
				strcpy(path_cre,path_target);
				strcat(path_cre,"\\");
				strcat(path_cre,filestruct.cFileName);
				CreateDirectory(path_cre,NULL);
				CopyDir(path_temp,path_cre);
			}
		}
		else
		{
			strcpy(path_find,path);
			strcat(path_find, "\\");
			strcat(path_find,filestruct.cFileName);
			CopyFile(path_find,path_target,filestruct.cFileName);
			strcpy(path_cre,path_target);
			strcat(path_cre,"\\");
			strcat(path_cre,filestruct.cFileName);
		}   }
	while( FindNextFile(hFind,&filestruct) != 0);

	return 1;
}
