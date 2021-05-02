
#include "StdAfx.h"
#include "GLPong.h"
#include "Firework.h"
#include "SceneManager.h"
#include "Board.h"
#include "Paddle.h"
#include "AiPaddle.h"
#include "Ball.h"

#define ROCKETS				4
#define SCREEN_FREQUENCY	60


CSceneManager	g_scene;
CBoard		g_board;
CPaddle		*g_ppaddleLeft;
CPaddle		g_paddleRight(false);
CBall		g_ball;
SDL_Surface *g_sdlSurface;
GLuint		g_texture[NUM_TEXTURES];

int main(int argc, char *argv[])
{
	const SDL_VideoInfo *videoInfo;
	char		szBuffer[5];
	SDL_Event	event;
	int			videoFlags;
	int			done = false;
	int			isActive = true;
	Uint32		nCurTicks,
				nPrevTicks,
				nLastDraw;
	float		t;

#ifdef _DEBUG
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
#else
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
#endif
	{
		fprintf(stderr, "Video initialization failed: %s\n", SDL_GetError());
		return 1;
	}

	if (!(videoInfo = SDL_GetVideoInfo()))
	{
		fprintf(stderr, "Video query failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	videoFlags  = SDL_OPENGL;
	videoFlags |= SDL_GL_DOUBLEBUFFER;
	videoFlags |= SDL_HWPALETTE;
	videoFlags |= SDL_RESIZABLE;

	printf("Full-screen mode [Y/N]?");
#ifdef _DEBUG
	strcpy(szBuffer, "no");
#else
	scanf("%5s", szBuffer);
#endif
	if (strlen(szBuffer) > 0 && tolower(szBuffer[0]) == 'y')
	{
		videoFlags |= SDL_FULLSCREEN;
		SDL_ShowCursor(SDL_DISABLE);
	}

	printf("Play against AI [Y/N]?");
#ifdef _DEBUG
	strcpy(szBuffer, "yes");
#else
	scanf("%5s", szBuffer);
#endif
	if (strlen(szBuffer) > 0 && tolower(szBuffer[0]) == 'y')
	{
		g_ppaddleLeft = new CAiPaddle(true);
		((CAiPaddle*)g_ppaddleLeft)->Create(&g_ball);
	}
	else
	{
		g_ppaddleLeft = new CPaddle(true);
	}
	g_ball.Create(&g_board, g_ppaddleLeft, &g_paddleRight);

	if (videoInfo->hw_available)
		videoFlags |= SDL_HWSURFACE;
	else
		videoFlags |= SDL_SWSURFACE;

	if (videoInfo->blit_hw)
		videoFlags |= SDL_HWACCEL;

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	if (!(g_sdlSurface = SDL_SetVideoMode(640, 480, 16, videoFlags)))
	{
		fprintf(stderr,  "Video mode set failed: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	InitGL();

	ReSizeGLScene(640, 480);

	nLastDraw = nPrevTicks = SDL_GetTicks();
	while (!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_VIDEORESIZE:
				ReSizeGLScene(event.resize.w, event.resize.h);
				break;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					SDL_Quit();
					return 0;
				case SDLK_F1:
					SDL_WM_ToggleFullScreen(g_sdlSurface);
					break;
				case SDLK_PAUSE:
					isActive = !isActive;
					break;
				default:
					g_scene.ProcessEvent(IObject::eventKeyDown, event.key.keysym.sym, 0);
				}
				break;

			case SDL_KEYUP:
				g_scene.ProcessEvent(IObject::eventKeyUp, 0, event.key.keysym.sym);
				break;

			case SDL_QUIT:
				done = true;
				break;
			}
		}

		if (isActive)
		{
			nCurTicks = SDL_GetTicks();
			t = (nCurTicks - nPrevTicks) / 1000.0f;
			nPrevTicks = nCurTicks;
			if (t > 0.3f)
				t = 0.0f;
			UpdateScene(t);

			if ((nCurTicks - nLastDraw) > 1000/SCREEN_FREQUENCY)
			{
				nLastDraw = nCurTicks;
				DrawGLScene();
				SDL_GL_SwapBuffers();
			}

			DrawFPS();

			SDL_Delay(2);
		}
		else
			SDL_Delay(100);
	}

	SDL_Quit();
	delete g_ppaddleLeft;
	return EXIT_SUCCESS;
}

void DrawFPS()
{
	static int	nFrames = 0;
	static Uint32 nStartTime = SDL_GetTicks();
	Uint32		nCurrentTime = SDL_GetTicks();

	nFrames++;
	if (nCurrentTime - nStartTime >= 5000)
	{
		GLfloat fSeconds = (nCurrentTime - nStartTime) / 1000.0f;
		GLfloat fFPS = nFrames / fSeconds;
		printf("%d frames in %g seconds = %g FPS\n", nFrames, fSeconds, fFPS);
		nStartTime = nCurrentTime;
		nFrames = 0;
	}
}

void UpdateScene(float t)
{
	g_scene.Update(t);
}

void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	gluLookAt(	0,	-120,	-100,
				0,	0,		0,
				0,	1,		0);

	g_scene.Render();
}

int InitGL(GLvoid)
{
	GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };

	if (!LoadGLTextures())
		return false;

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	GLfloat LightPosition[]=	{ 30.0f, 50.0f, -100.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);

	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glBlendFunc(GL_ONE, GL_ONE);

	if (!g_scene.AddObject(&g_board) ||
		!g_scene.AddObject(g_ppaddleLeft) ||
		!g_scene.AddObject(&g_paddleRight) ||
		!g_scene.AddObject(&g_ball) ||
        !g_scene.Initialize())
		return false;
	return true;
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height == 0)
		height = 1;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int LoadGLTextures()
{
	int Status = false;
	int	i;

    SDL_Surface *TextureImage[NUM_TEXTURES]; 

	if ((TextureImage[0] = SDL_LoadBMP("Particle.bmp")) &&
		(TextureImage[1] = SDL_LoadBMP("Small Blur Star.bmp")))
	{
		Status = true;

		glGenTextures(NUM_TEXTURES, &g_texture[0]);

		for (i=0; i<NUM_TEXTURES; i++)
		{
			glBindTexture(GL_TEXTURE_2D, g_texture[i]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->w, TextureImage[0]->h, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->pixels);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		}
	}

	for (i=0; i<NUM_TEXTURES && TextureImage[i]; i++)
		SDL_FreeSurface(TextureImage[i]);

	return Status;
}

void DrawFirework()
{
	CFireworkRocket	rockets[ROCKETS];
	SDL_Event	event;
	bool		done=false;
	Uint32		nStartTime;
	int 		nCurTicks, nPrevTicks;
	float		dt;
	int 		i;

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glBlendFunc(GL_ONE, GL_ONE);
	glBindTexture(GL_TEXTURE_2D, g_texture[1]);

	for (i=0; i<ROCKETS; ++i)
		if (!rockets[i].Create())
			return;

	nStartTime = SDL_GetTicks();

	nPrevTicks = SDL_GetTicks();
	while(!done)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_VIDEORESIZE:
				ReSizeGLScene(event.resize.w, event.resize.h);
				break;

			case SDL_KEYDOWN:
				if (event.key.keysym.sym != SDLK_ESCAPE)
					break;
			case SDL_QUIT:
				done = true;
				break;
			}
		}

		nCurTicks = SDL_GetTicks();
		dt = (nCurTicks - nPrevTicks) / 1000.0f;
		nPrevTicks = nCurTicks;
		if (dt > 0.3f)
			dt = 0.0f;
		for (i=0; i<ROCKETS; ++i)
			rockets[i].Update(dt);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		glLoadIdentity();

		gluLookAt(	0,	-120,	-100,
					0,	0,		0,
					0,	1,		0);

		g_board.Render();
		g_paddleRight.Render();
		g_ppaddleLeft->Render();

		glLoadIdentity();
		glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glEnable(GL_TEXTURE_2D);
			glTranslatef(0.0f, 0.0f, -40.0f);
			for (i=0; i<ROCKETS; ++i)
				rockets[i].Render();
		glPopAttrib();

		glFlush();
		SDL_GL_SwapBuffers();
	}
	glPopAttrib();
}

