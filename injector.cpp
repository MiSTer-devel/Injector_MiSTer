
#include <stdio.h>
#include <tchar.h>
#include <Windows.h>

BOOL LoadFile(TCHAR* FileName, BYTE** base, DWORD* size)
{
	HANDLE HF = CreateFile(FileName, GENERIC_READ, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);

	if (HF == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("Error! Cannot open file %s\n"), FileName);
		exit(1);
	}

	DWORD wasread = 0;
	*size = GetFileSize(HF, NULL);
	*base = new BYTE[*size];
	if (!ReadFile(HF, *base, *size, &wasread, NULL) || wasread != *size)
	{
		_tprintf(TEXT("Error! Cannot read file %s\n"), FileName);
		exit(1);
	}

	CloseHandle(HF);
	return true;
}

BOOL SaveFile(TCHAR* FileName, void* base, DWORD size)
{
	HANDLE HF = CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	if (HF == INVALID_HANDLE_VALUE)
	{
		_tprintf(TEXT("Error! Cannot create file %s\n"), FileName);
		exit(1);
	}

	DWORD written = 0;

	if (!::WriteFile(HF, base, size, &written, NULL) || written != size)
	{
		_tprintf(TEXT("Error! Cannot write to file %s\n"), FileName);
		exit(1);
	}

	CloseHandle(HF);

	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc<4)
	{
		_tprintf(_T("ROM -> RBF injector.\n(c)2018 Sorgelig.\n\nusage: <inRBFfile> <inROMfile> <outRBFfile>\n"));
		return 1;
	}

	BYTE* rbfbase;
	DWORD rbfsz;
	LoadFile(argv[1], &rbfbase, &rbfsz);

	BYTE* rombase;
	DWORD romsz;
	LoadFile(argv[2], &rombase, &romsz);

	BYTE *outbase = (BYTE*)malloc(rbfsz + romsz + 16);
	if (!outbase)
	{
		_tprintf(TEXT("Error! Out of memory.\n"));
		exit(1);
	}

	memset(outbase, 0, 16);

	strcpy((char*)outbase, "MiSTer");
	*(DWORD*)(outbase + 12) = rbfsz;

	memcpy(outbase + 16, rbfbase, rbfsz);
	memcpy(outbase + 16 + rbfsz, rombase, romsz);

	SaveFile(argv[3], outbase, 16 + rbfsz + romsz);

	return 0;
}
