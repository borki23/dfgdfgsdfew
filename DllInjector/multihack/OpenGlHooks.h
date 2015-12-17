#pragma once
#pragma comment(lib,"OpenGL32.lib")
#pragma comment(lib,"GLu32.lib")
#include <Windows.h>
#include <stdio.h>
#include <gl\GLU.h>
#include <gl\gl.h>
#include "GL\freeglut.h"

typedef void (APIENTRY *glEnableP)(GLenum);
typedef bool (APIENTRY *wglSwapBuffersP)(HDC);
typedef void (APIENTRY *glBeginP)(GLenum);
typedef void (APIENTRY *glClearP)(GLbitfield);

class OpenGlHooks {
	public:
		OpenGlHooks();
		~OpenGlHooks();
		void customDraw();
	private:
		HMODULE opengl32;
		HMODULE thisModule;
		

		void glEnable_HOOK(GLenum en);
		//glEnableP glEnable_OLD;

		//wglSwapBuffersP wglSwapBuffers_OLD;
		bool APIENTRY wglSwapBuffers_HOOK(HDC hdc);

		void * retour(BYTE* src, BYTE* dst, GLint length);
		GLboolean deRetourAll();
		HANDLE process;
}; 

typedef bool (APIENTRY OpenGlHooks::*wglSwapBuffers_HOOKP)(HDC);
