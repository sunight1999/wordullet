#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <time.h>

#define INIT_NUM 100
#define INIT_SIZE 1024

#define KEY_A 0x41
#define	KEY_a 0x61
#define KEY_z 0x7A

HHOOK hMouseHook;
HHOOK hKeyHook;

// for wordullets
char **wordullets = NULL;
int idx = 0;
int eidx = 0;

// for double click check
int dblclktim = 0;
clock_t clktim;
int is_dblck = 0;

void TypeKey(char c, int flag)
{
	INPUT input;
	memset(&input, 0, sizeof(INPUT));

	input.type = INPUT_KEYBOARD;

	/* Press Key */
	// Press shift key for capital character
	int is_cap = KEY_A <= c && c < KEY_a;
	if (is_cap)
	{
		input.ki.wVk = VK_SHIFT;
		SendInput(1, &input, sizeof(INPUT));
	}
	else if (KEY_a <= c && c <= KEY_z)
	{
		c -= 0x20;
	}

	input.ki.wVk = c;
	SendInput(1, &input, sizeof(INPUT));

	/* Depress Key */
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	
	// Depress character
	SendInput(1, &input, sizeof(INPUT));

	// Depress shift
	if (is_cap)
	{
		input.ki.wVk = VK_SHIFT;
		SendInput(1, &input, sizeof(INPUT));
	}
}

char Convert2VK(char c)
{
	switch (c)
	{
	case '\'':
		return 22;
	case '/':
		return 191;
	default:
		return c;
	}
}

void Type(char *str)
{
	int len = strlen(str);

	while (*str)
	{
		TypeKey(Convert2VK(*str++), 0);
	}
}

void LoadWordullet()
{
	/* Collect wordullets from text file */
	FILE *f = fopen("errlist.txt", "r");
	if (!f)
	{
		printf("%s\n", strerror(errno));
		return;
	}

	int size = INIT_NUM;
	wordullets = (char **)malloc(sizeof(char *) * size);
	memset(wordullets, 0, sizeof(char *) * INIT_NUM);

	for (int i = 0; i < size; ++i)
	{
		wordullets[i] = (char *)malloc(INIT_SIZE);
		memset(wordullets[i], 0, INIT_SIZE);
	}
	
	char word[INIT_SIZE] = {0, };
	while (fgets(word, INIT_SIZE, f))
	{
		if (idx == size)
		{
			size += INIT_NUM;
			wordullets = (char **)realloc(wordullets, size);

			for (int i = size - INIT_NUM; i < size; ++i)
			{
				wordullets[i] = (char *)malloc(INIT_SIZE);
				memset(wordullets[i], 0, INIT_SIZE);
			}
		}

		strcpy(wordullets[idx], word);
		wordullets[idx][strlen(wordullets[idx])-1] = 0;
		++idx;
	}

	for (int i = 0; i < idx; ++i)
	{
		printf("%s\n", wordullets[i]);
	}
}

LRESULT CALLBACK mouseProc (int nCode, WPARAM wParam, LPARAM lParam)
{
    MOUSEHOOKSTRUCT * pMouseStruct = (MOUSEHOOKSTRUCT *)lParam;
    if (pMouseStruct != NULL){
		if(wParam == WM_LBUTTONDOWN)
        { 
			if (is_dblck){
				Type(wordullets[eidx++]);
				is_dblck = 0;
			}
			
			if (clock() - clktim <= dblclktim)
				is_dblck = 1;

			clktim = clock();
        }
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

LRESULT CALLBACK keyProc (int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT * pKeyStruct = (KBDLLHOOKSTRUCT *)lParam;
    if (pKeyStruct != NULL){
		printf("%d", pKeyStruct->vkCode);
    }
    return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

DWORD WINAPI ShootStart(LPVOID lpParam)
{
	//hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, NULL, NULL);
	hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, NULL, NULL);

	MSG message;
    while (GetMessage(&message,NULL,0,0)) {
        TranslateMessage( &message );
        DispatchMessage( &message );
    }

	UnhookWindowsHookEx(hMouseHook);

	return 0;
}

int main()
{
	HANDLE hThread;
    DWORD dwThread;

	// Initialize time
	clktim = clock();

	LoadWordullet();
	
	/* Prepare double click checking */
	dblclktim = GetDoubleClickTime() ;

	hThread = CreateThread(NULL, NULL, ShootStart, NULL, NULL, &dwThread);
	idx = 10;
	while (eidx < idx)
		Sleep(100);

	CloseHandle(hThread);

	return WaitForSingleObject(hThread,INFINITE);
}