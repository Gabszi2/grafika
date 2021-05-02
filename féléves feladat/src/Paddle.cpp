
#include "StdAfx.h"
#include "Paddle.h"
#include "Board.h"

#define PADDLE_BEVEL			4.0f
#define	PADDLE_SPEED			150.0f
#define	PADDLE_ILLUMINATE		0.5f
#define	PADDLE_ILLUMINATE_FADE	0.3f

// Constructor
CPaddle::CPaddle(bool bLeftPaddle) :
	m_fSpeed(0.0f),
	m_fY(0.0f),
	m_fIlluminate(0.0f)
{
	m_bLeftPaddle = bLeftPaddle;
}

bool CPaddle::Initialize()
{
	m_nGLPaddle = glGenLists(1);
	glNewList(m_nGLPaddle, GL_COMPILE);
	if (m_bLeftPaddle)
	{
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(CBoard::GetLeft(), GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetLeft(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetLeft()-GetWidth(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetLeft()-GetWidth(), GetHeight()/2.0f, -PADDLE_BEVEL);

		glNormal3f(-1.0f, 0.0f, 0.0f);
		glVertex3f(CBoard::GetLeft()-GetWidth(), GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetLeft()-GetWidth(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetLeft()-GetWidth(), -GetHeight()/2.0f, 0);
		glVertex3f(CBoard::GetLeft()-GetWidth(), GetHeight()/2.0f, 0);

		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(CBoard::GetLeft(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetLeft(), -GetHeight()/2.0f, 0);
		glVertex3f(CBoard::GetLeft()-GetWidth(), -GetHeight()/2.0f, 0);
		glVertex3f(CBoard::GetLeft()-GetWidth(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glEnd();
	}
	else
	{
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, -1.0f);
		glVertex3f(CBoard::GetRight()+GetWidth(), GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetRight()+GetWidth(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetRight(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetRight(), GetHeight()/2.0f, -PADDLE_BEVEL);

		glNormal3f(1.0f, 0.0f, 0.0f);
		glVertex3f(CBoard::GetRight()+GetWidth(), GetHeight()/2.0f, 0);
		glVertex3f(CBoard::GetRight()+GetWidth(), -GetHeight()/2.0f, 0);
		glVertex3f(CBoard::GetRight()+GetWidth(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetRight()+GetWidth(), GetHeight()/2.0f, -PADDLE_BEVEL);

		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(CBoard::GetRight()+GetWidth(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glVertex3f(CBoard::GetRight()+GetWidth(), -GetHeight()/2.0f, 0);
		glVertex3f(CBoard::GetRight(), -GetHeight()/2.0f, 0);
		glVertex3f(CBoard::GetRight(), -GetHeight()/2.0f, -PADDLE_BEVEL);
		glEnd();
	}
	glEndList();
	return true;
}

void CPaddle::Update(float fTime)
{
	m_fY += m_fSpeed * fTime;
	if (m_fY > CBoard::GetTop() - GetHeight()/2.0f)
	{
		m_fY = CBoard::GetTop() - GetHeight()/2.0f;;
		m_fSpeed = 0.0f;
	}
	if (m_fY < CBoard::GetBottom() + GetHeight()/2.0f)
	{
		m_fY = CBoard::GetBottom() + GetHeight()/2.0f;
		m_fSpeed = 0.0f;
	}

	if (m_fIlluminate > 0.0f)
		m_fIlluminate -= PADDLE_ILLUMINATE_FADE*fTime;
	else
		m_fIlluminate = 0.0f;
}

void CPaddle::Render() const
{
	glPushMatrix();
	glTranslatef(0, m_fY, 0);
	glColor3f(m_fIlluminate, 1.0f, m_fIlluminate);
	glCallList(m_nGLPaddle);
	glPopMatrix();
}

bool CPaddle::ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam)
{
	switch (nEvent)
	{
	case eventKeyDown:
		switch (wParam)
		{
		case SDLK_LSHIFT:
			if (m_bLeftPaddle)
			{
				MoveUp();
				return true;
			}
			break;
		case SDLK_LCTRL:
			if (m_bLeftPaddle)
			{
				MoveDown();
				return true;
			}
			break;
		case SDLK_UP:
			if (!m_bLeftPaddle)
			{
				MoveUp();
				return true;
			}
			break;
		case SDLK_DOWN:
			if (!m_bLeftPaddle)
			{
				MoveDown();
				return true;
			}
			break;
		}
		break;

	case eventKeyUp:
		switch (lParam)
		{
		case SDLK_LSHIFT:
		case SDLK_LCTRL:
			if (m_bLeftPaddle)
			{
				Stop();
            	return true;
			}
			break;
		case SDLK_UP:
		case SDLK_DOWN:
			if (!m_bLeftPaddle)
			{
				Stop();
				return true;
			}
			break;
		}
		break;
	}
	
	return false;
}

void CPaddle::MoveUp()
{
	m_fSpeed = PADDLE_SPEED;
}

void CPaddle::MoveDown()
{
	m_fSpeed = -PADDLE_SPEED;
}

void CPaddle::Stop()
{
	m_fSpeed = 0.0f;
}

void CPaddle::Illuminate()
{
	m_fIlluminate = PADDLE_ILLUMINATE;
}

