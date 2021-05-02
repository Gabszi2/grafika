
#ifndef __GLPONG_H__
#define __GLPONG_H__

#define NUM_TEXTURES		2


void DrawFPS();
void DrawGLScene();
int InitGL(GLvoid);
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
void UpdateScene(float t);
int LoadGLTextures();
void DrawFirework();

#endif

