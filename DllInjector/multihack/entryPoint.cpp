
#include "OpenGlHooks.h"

OpenGlHooks *glHook;

DWORD WINAPI dllStart(LPVOID lpParam)
{
	Beep(100,50);
	MessageBox(NULL, "DLL SUCCESSFULLY ATTACHED", "SUCCESS", MB_OK);
	//FARPROC GetProcAddress(opengl32, );

	glHook = new OpenGlHooks();
	
	//fprintf(test, "Address of opengl32dll: 0x%x", opengl32);
	//fprintf(test, "glEnable: 0x%x", enableHook);

	return S_OK;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD  fdwReason, LPVOID lpvReserved) {
	HANDLE thread;
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		thread = CreateThread(NULL,0, dllStart, 0, 0, NULL);
		//WaitForSingleObject(thread, INFINITE);
	}
	
	return TRUE;
}
/*
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")

#include <windows.h>
#include <string>
#include <tlhelp32.h>
#include <iostream>
#include <gl\gl.h>
#include <gl\glu.h>

typedef void (APIENTRY *glBegin_t)(GLenum);
typedef void (APIENTRY *glEnd_t)(void);
typedef void (APIENTRY *glClear_t)(GLbitfield);
typedef void (APIENTRY *glVertex3fv_t)(const GLfloat *v);
typedef void (APIENTRY *glVertex3f_t)(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY *glEnable_t)(GLenum);
typedef bool (APIENTRY *wglSwapBuffers_t)(HDC);

wglSwapBuffers_t pglSwapBuffers = NULL;
glBegin_t pglBegin = NULL;
glEnd_t pglEnd = NULL;
glClear_t pglClear = NULL;
glVertex3fv_t pglVertex3fv = NULL;
glVertex3f_t pglVertex3f = NULL;
glEnable_t pglEnable = NULL;

bool wallhack = false;

void DrawBox(GLfloat x, GLfloat y, GLfloat z, GLfloat width, GLfloat height)
{
	glBegin(GL_QUADS);
	glVertex3f(x, y, z);
	glVertex3f(x + width, y, z);
	glVertex3f(x + width, y + height, z);
	glVertex3f(x, y + height, z);
	glEnd();
}


//=================================================================================
// ###################### Hooked OpenGL Functions #################################
//=================================================================================

void APIENTRY Hooked_glBegin(GLenum mode)
{
	if (GetAsyncKeyState(VK_F1) & 1) wallhack = !wallhack;

	if (wallhack)
	{
		if (mode == GL_TRIANGLES || mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
			glDepthRange(0, 0.5);
		else
			glDepthRange(0.5, 1);
	}

	if (pglBegin)
		(*pglBegin)(mode);
}

void APIENTRY Hooked_glEnd(void)
{
	(*pglEnd)();
}

void APIENTRY Hooked_glVertex3fv(GLfloat *v)
{
	(*pglVertex3fv)(v);
}


void APIENTRY Hooked_glVertex3f(GLfloat x, GLfloat y, GLfloat z)
{
	(*pglVertex3f)(x, y, z);
}

void APIENTRY Hooked_glClear(GLbitfield mask)
{
	(*pglClear)(mask);
}

void APIENTRY Hooked_glEnable(GLenum cap)
{
	/*if (GetAsyncKeyState(VK_F2))
	DrawBox(200,100,0,50,30);*/
/*
	(*pglEnable)(cap);
}

bool APIENTRY Hooked_wglSwapBuffers(HDC hdc)
{
	/*if (GetAsyncKeyState(VK_F2))
	DrawBox(200,100,0,50,30);*/
/*
	return (*pglSwapBuffers)( hdc);
}

void *DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len + 5);
	DWORD dwback;
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);
	memcpy(jmp, src, len);
	jmp += len;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + len - jmp) - 5;
	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;
	VirtualProtect(src, len, dwback, &dwback);
	return (jmp - len);
}
void HookGL()
{
	HMODULE hOpenGL = GetModuleHandle("opengl32.dll");
	pglVertex3fv = (glVertex3fv_t)DetourFunc((LPBYTE)GetProcAddress(hOpenGL, "glVertex3fv"), (LPBYTE)&Hooked_glVertex3fv, 6);
	pglVertex3f = (glVertex3f_t)DetourFunc((LPBYTE)GetProcAddress(hOpenGL, "glVertex3f"), (LPBYTE)&Hooked_glVertex3f, 6);
	pglBegin = (glBegin_t)DetourFunc((LPBYTE)GetProcAddress(hOpenGL, "glBegin"), (LPBYTE)&Hooked_glBegin, 6);
	pglEnd = (glEnd_t)DetourFunc((LPBYTE)GetProcAddress(hOpenGL, "glEnd"), (LPBYTE)&Hooked_glEnd, 6);
	pglClear = (glClear_t)DetourFunc((LPBYTE)GetProcAddress(hOpenGL, "glClear"), (LPBYTE)&Hooked_glClear, 7);
	pglEnable = (glEnable_t)DetourFunc((LPBYTE)GetProcAddress(hOpenGL, "glEnable"), (LPBYTE)&Hooked_glEnable, 6);
}


DWORD WINAPI dwMainThread(LPVOID)
{
	HookGL();

	return TRUE;
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, dwMainThread, 0, 0, 0);

	return TRUE;
}

*/