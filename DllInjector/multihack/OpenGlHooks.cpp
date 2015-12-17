#include "OpenGlHooks.h"

glEnableP glEnable_OLD;
glBeginP glBegin_OLD = NULL;
glClearP glClear_OLD = NULL;
FILE *logFile;


void APIENTRY glClearHOOK(GLbitfield mask) {
	if (mask == GL_DEPTH_BUFFER_BIT)
	{
		mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	}

	(glClear_OLD)(mask);
}


void APIENTRY glBeginHook(GLenum mode) {
	//if (GetAsyncKeyState(VK_F1) & 1) detach();
	/*if (mode == GL_QUADS)
	{
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0, 1.0, -1.0);
		

		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
	}
	
	if ((mode == GL_TRIANGLE_STRIP) || mode == GL_TRIANGLE_FAN)
	{
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0, 1.0, -1.0);


		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glDisable(GL_DEPTH_TEST);
	}*/
	/*if (mode == GL_POLYGON) //If mode is a polygon, then lets draw an outline over it...
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Draws a line over all the Polygon functions..
		glLineWidth(3.0); // Sets the width of the line, the lower the #, the less wide it is...
		glColor3f(255, 255, 255); //Sets the color of the RGB... 255,255,255 is black by default
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // If there isnt anything there, just fill it like normal..
	}*/

	if (mode == GL_TRIANGLE_STRIP || mode == GL_TRIANGLE_FAN)
	{
		glPushMatrix();
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0.0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT), 0.0, 1.0, -1.0);


		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();


		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(3.0);
		glColor3f(255, 255, 255);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	

	(glBegin_OLD)(mode);
}

OpenGlHooks::OpenGlHooks() {
	opengl32 = GetModuleHandle("OPENGL32");
	// thisModule = GetModuleHandle("multihack.dll");
	//fopen();
	process = GetCurrentProcess();

	glBegin_OLD = (glBeginP)retour((BYTE*)GetProcAddress(opengl32, "glBegin"), (BYTE*)&glBeginHook, 6);
	glClear_OLD = (glClearP)retour((BYTE*)GetProcAddress(opengl32, "glClear"), (BYTE*)&glClearHOOK, 7);

	//fclose(logFile);
}

void* OpenGlHooks::retour(BYTE* src, BYTE* dst, GLint length) {


//	unsigned char jmp[11] = { 0xE9, 0x0, 0x0, 0x0, 0x0, 0x0 };
	BYTE* jmp = (BYTE*)malloc(length + 5);

	//fprintf(logFile, "original:0x%x\n", src);


//	fprintf(logFile, "new beign:0x%x \n", dst);

	DWORD dwback;
	VirtualProtect(src, length, PAGE_READWRITE, &dwback);

	memcpy(jmp, src, length);

	//fprintf(logFile, "JMP:0x%x 0x%x 0x%x 0x%x 0x%x - at 0x%x \n", jmp[0], jmp[1], jmp[2], jmp[3], jmp[4], jmp);

	jmp += length;
	jmp[0] = 0xE9;
	*(DWORD*)(jmp + 1) = (DWORD)(src + length - jmp) - 5;

	//fprintf(logFile, "newJMP:0x%x 0x%x 0x%x 0x%x 0x%x\n", jmp[0], jmp[1], jmp[2], jmp[3], jmp[4]);

	src[0] = 0xE9;
	*(DWORD*)(src + 1) = (DWORD)(dst - src) - 5;

	//fprintf(logFile, "rel:0x%x 0x%x 0x%x 0x%x 0x%x\n", ((BYTE*)src)[0], ((BYTE*)src)[1], ((BYTE*)src)[2], ((BYTE*)src)[3], ((BYTE*)src)[4]);

	dwback;
	VirtualProtect(src, length, dwback, &dwback);

	return (jmp - length);


	//fprintf(logFile, "target:0x%x -0x%x \n", wglSwapBuffers_OLD, jmp);
	/*fprintf(logFile, "Addressof new fct:0x%x\n", (DWORD)addr);
	DWORD calced = (DWORD)pointer - (DWORD)addr - 5;
	fprintf(logFile, "Relative jump length:%d\n", calced);
	ReadProcessMemory(GetCurrentProcess(), pointer, buffer, 6, &bytesRead);
	fprintf(logFile, "Read %d bytes at address: 0x%x are  %2x %2x %2x %2x %2x\n", bytesRead, wglSwapBuffers_OLD, buffer[0], buffer[1], buffer[2], buffer[3], buffer[4]);*/

	//memcpy(&jmp[1], &calced, 4);

	//WriteProcessMemory(GetCurrentProcess(), (void*)pointer, jmp, 5, &bytesRead);

	//fprintf(logFile, "bytes %d - Error %d", bytesRead, GetLastError());
	//delete buffer;

}

OpenGlHooks::~OpenGlHooks() {
	fclose(logFile);
}

/*glEnable_OLD = (glEnableP)GetProcAddress(GetModuleHandle("OPENGL32"), "glEnable");

//fprintf(logFile, "Addressof old fct:0x%x\n", addr);
fprintf(logFile, "Addressof new fct:0x%x\n", (DWORD)pointer);

fprintf(logFile, "Acacled :%d\n", calced);*/

//fprintf(logFile, "Read %d bytes at address: 0x%x are  %2x %2x %2x %2x\n", bytesRead, wglSwapBuffers_OLD, buffer[1], buffer[2], buffer[3], buffer[4]);
//wglSwapBuffers_HOOKP temp = &OpenGlHooks::wglSwapBuffers_HOOK;


//HookGeneralFunction("test", "test2", &wglSwapBuffers_HOOKd, buffer);
//void* temp = OpenGlHooks::wglSwapBuffers_HOOK;
//wglSwapBuffers_HOOKP temp = &OpenGlHooks::wglSwapBuffers_HOOK;
//LPBYTE test = reinterpret_cast<PBYTE>(temp);
//fprintf(logFile, "Addressof new fct:0x%x", (DWORD) pointer );
