
#include "StdAfx.h"
#include "Ball.h"
#include "GLPong.h"

extern GLuint		g_texture[NUM_TEXTURES];

#define BALL_SPEED			110.0f
#define BALL_SPEED_INCREASE	5.0f
#define BALL_RADIUS			2.0f
#define BALL_MAX_ANGLE		M_PI/3.0f
#define BALL_MIN_ANGLE		M_PI/7.0f
#define PART_SIZE			1.7f

// Constructor
CBall::CBall()
{
}

CBall::~CBall()
{
}

// Create the ball.
bool CBall::Create(CBoard *pBoard, CPaddle *pLeftPaddle, CPaddle *pRightPaddle)
{
	int i;

	m_pBoard = pBoard;
	m_pLeftPaddle = pLeftPaddle;
	m_pRightPaddle = pRightPaddle;

	m_vBallPosition.y = 0.0f;
	NewBall(m_rand.RandomInt()%2 == 0);

	for (i=0; i<sizeof(m_particles)/sizeof(m_particles[0]); ++i)	
	{
		m_particles[i].life = 1.0f;
		m_particles[i].fade = (float) m_rand.RandomRange(3.0f, 28.0f);	
		m_particles[i].x = m_vBallSpeed.x;
		m_particles[i].y = m_vBallSpeed.y;
		m_particles[i].z = -BALL_RADIUS;
	}
	return true;
}

//Initialize the object.

bool CBall::Initialize()
{
	GLUquadricObj	*pQuadObj;

	m_rand.Randomize();

	m_nList = glGenLists(1);
	glNewList(m_nList, GL_COMPILE);
		glColor3f(0.0f, 1.0f, 0.0f);
		pQuadObj = gluNewQuadric();
		gluQuadricNormals(pQuadObj, GLU_SMOOTH);
		gluSphere(pQuadObj, BALL_RADIUS, 7, 5);
		gluDeleteQuadric(pQuadObj);
	glEndList();
	return true;
}

// Update the object.

void CBall::Update(float fTime)
{
	CVector2D	vNewBallPos(m_vBallPosition + m_vBallSpeed*fTime);
	double		dAngle,
				dSpeed;
	register int i;

	if (vNewBallPos.y+BALL_RADIUS > CBoard::GetTop())
	{
		m_vBallSpeed.y = -m_vBallSpeed.y;
		vNewBallPos.y = 2.0f*(CBoard::GetTop()-BALL_RADIUS) - vNewBallPos.y;
	}
	else if (vNewBallPos.y-BALL_RADIUS < CBoard::GetBottom())
	{
		m_vBallSpeed.y = -m_vBallSpeed.y;
		vNewBallPos.y = 2.0f*(CBoard::GetBottom()+BALL_RADIUS) - vNewBallPos.y;
	}

	if (vNewBallPos.x+BALL_RADIUS > CBoard::GetLeft()-CPaddle::GetWidth())
	{
		float	a = m_vBallSpeed.y / m_vBallSpeed.x,
				b = m_vBallPosition.y - a*m_vBallPosition.x,
				y;

		if (m_vBallPosition.x+BALL_RADIUS <= CBoard::GetLeft()-CPaddle::GetWidth() &&
			(y = a*(CBoard::GetLeft()-CPaddle::GetWidth()-BALL_RADIUS) + b)-BALL_RADIUS <= m_pLeftPaddle->GetPosition()+CPaddle::GetHeight()*0.5f &&
			y+BALL_RADIUS >= m_pLeftPaddle->GetPosition()-CPaddle::GetHeight()*0.5f)
		{
			m_pLeftPaddle->Illuminate();
			vNewBallPos.x = 2.0f*(CBoard::GetLeft()-CPaddle::GetWidth()-BALL_RADIUS) - vNewBallPos.x;
			dAngle = (m_pLeftPaddle->GetPosition() - vNewBallPos.y) * M_PI/4.0f / (CPaddle::GetHeight()/2.0f) + M_PI;

			dSpeed = m_vBallSpeed.Norm() + BALL_SPEED_INCREASE;
			m_vBallSpeed.x = float(cos(dAngle)*dSpeed);
			m_vBallSpeed.y = float(sin(dAngle)*dSpeed);
		}

		else if (vNewBallPos.x+BALL_RADIUS > CBoard::GetLeft())
		{
			m_pBoard->Score(true);
			NewBall(true);
			vNewBallPos = m_vBallPosition;
		}
	}
	else if (vNewBallPos.x-BALL_RADIUS < CBoard::GetRight()+CPaddle::GetWidth())
	{
		float	a = m_vBallSpeed.y / m_vBallSpeed.x,
				b = m_vBallPosition.y - a*m_vBallPosition.x,
				y;

		if (m_vBallPosition.x-BALL_RADIUS >= CBoard::GetRight()+CPaddle::GetWidth() &&
			(y = a*(CBoard::GetRight()+CPaddle::GetWidth()+BALL_RADIUS) + b)-BALL_RADIUS <= m_pRightPaddle->GetPosition()+CPaddle::GetHeight()*0.5f &&
			y+BALL_RADIUS >= m_pRightPaddle->GetPosition()-CPaddle::GetHeight()*0.5f)
		{
			m_pRightPaddle->Illuminate();
			vNewBallPos.x = 2.0f*(CBoard::GetRight()+CPaddle::GetWidth()+BALL_RADIUS) - vNewBallPos.x;
			dAngle = (vNewBallPos.y - m_pRightPaddle->GetPosition()) * M_PI/4.0f / (CPaddle::GetHeight()/2.0f);

			dSpeed = m_vBallSpeed.Norm() + BALL_SPEED_INCREASE;
			m_vBallSpeed.x = float(cos(dAngle)*dSpeed);
			m_vBallSpeed.y = float(sin(dAngle)*dSpeed);
		}

		else if (vNewBallPos.x-BALL_RADIUS < CBoard::GetRight())
		{
			m_pBoard->Score(false);
			NewBall(false);
			vNewBallPos = m_vBallPosition;
		}
	}

	m_vBallPosition = vNewBallPos;

	for (i=0; i<sizeof(m_particles)/sizeof(m_particles[0]); ++i)
	{
		m_particles[i].life -= m_particles[i].fade*fTime;

		if (m_particles[i].life < 0.0f)
		{
			m_particles[i].life = 1.0f;
			m_particles[i].fade = (float) m_rand.RandomRange(3.0f, 28.0f);
			m_particles[i].x = m_vBallPosition.x + (float)m_rand.RandomRange(-BALL_RADIUS*0.5f, BALL_RADIUS*0.5f);
			m_particles[i].y = m_vBallPosition.y + (float)m_rand.RandomRange(-BALL_RADIUS*0.5f, BALL_RADIUS*0.5f);
			m_particles[i].z = -BALL_RADIUS + (float)m_rand.RandomRange(-BALL_RADIUS*0.5f, BALL_RADIUS*0.5f);
		}
	}
}

// Render the object.

void CBall::Render() const
{
	float 			fModelView[16];
	CVector3D		vCenter,
					vRight,
					vUp,
					vVertex;
	float			ps;
	register int	i;

	glPushMatrix();
	glTranslatef(m_vBallPosition.x, m_vBallPosition.y, -BALL_RADIUS);
	glCallList(m_nList);
	glPopMatrix();

	glGetFloatv(GL_MODELVIEW_MATRIX, fModelView);

	vRight.x = fModelView[0];
	vRight.y = fModelView[4];
	vRight.z = fModelView[8];

	vUp.x = fModelView[1];
	vUp.y = fModelView[5];
	vUp.z = fModelView[9];

	glColor3f(0.0f, 1.0f, 0.0f);
	glBindTexture(GL_TEXTURE_2D, g_texture[0]);
	glPushAttrib(GL_ENABLE_BIT);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		glBegin(GL_QUADS);
			for (i=0; i<sizeof(m_particles)/sizeof(m_particles[0]); ++i)
			{
				ps = m_particles[i].life*PART_SIZE;
				vCenter.x = m_particles[i].x;
				vCenter.y = m_particles[i].y;
				vCenter.z = m_particles[i].z;

				glTexCoord2d(0,1); glVertex3fv(vCenter - (vRight + vUp) * ps);
				glTexCoord2d(1,1); glVertex3fv(vCenter + (vRight - vUp) * ps);
				glTexCoord2d(1,0); glVertex3fv(vCenter + (vRight + vUp) * ps);
				glTexCoord2d(0,0); glVertex3fv(vCenter - (vRight - vUp) * ps);
			}
		glEnd();
	glPopAttrib();
}

// Process event.

bool CBall::ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam)
{
	return false;
}

// Create a new ball aimed toward left or right player.
void CBall::NewBall(bool bGoToLeft)
{
	float fAngle;

	do
		fAngle = (float)m_rand.RandomRange(-BALL_MAX_ANGLE, +BALL_MAX_ANGLE);
	while (fabs(fAngle) < BALL_MIN_ANGLE);

	if (bGoToLeft)
	{
		fAngle += float(M_PI);
		m_vBallPosition.x = CBoard::GetLeft()-CPaddle::GetWidth();
	}
	else
		m_vBallPosition.x = CBoard::GetRight()+CPaddle::GetWidth();
	m_vBallSpeed.x = (float)cos(fAngle)*BALL_SPEED;
	m_vBallSpeed.y = (float)sin(fAngle)*BALL_SPEED;
}

// Detects if the ball collides with a point A.

inline bool CBall::HitPoint(CVector2D &vOldPos, CVector2D &vNewPos, CVector2D &vSpeed, CVector2D &ptA)
{
	float	a = vSpeed.y / vSpeed.x,
			b = vOldPos.y - a*vOldPos.x,
			t0 = -a*a*(ptA.x*ptA.x - BALL_RADIUS*BALL_RADIUS) - 2.0f*a*(b-ptA.y)*ptA.x - b*b + 2.0f*b*ptA.y - ptA.y*ptA.y + BALL_RADIUS*BALL_RADIUS;
	if (t0 < 0)
		return false;
	float	t1 = sqrt(t0),
			t2 = -a*(b-ptA.y) + ptA.x,
			t3 = a*a + 1,
			x1 = (+t1 + t2)/t3,
			x2 = (-t1 + t2)/t3,
			x;
	if (fabs(vOldPos.x - x1) < fabs(vOldPos.x - x2))
		x = x1;
	else
		x = x2;
	if (!(vOldPos.x <= x && x <= vNewPos.x ||
		  vNewPos.x <= x && x <= vOldPos.x))
		return false;

	CVector2D vect(x - ptA.x, a*x+b - ptA.y);

	vSpeed = vect * ((vSpeed.Norm()+BALL_SPEED_INCREASE) / vect.Norm());

	vNewPos = vOldPos; 

	return true;
}

