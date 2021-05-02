
#ifndef INC_BALL_H_
#define INC_BALL_H_

#ifdef _WIN32
#pragma once
#endif

#include "IObject.h"
#include "Board.h"
#include "Paddle.h"
#include "Vector.h"
#include "RandomMT.h"

// Structures
struct PARTICLE						
{

	float	life;					
	float	fade;					
	float	x;						
	float	y;						
	float	z;						
	float	color[4];				
};

class CBall : public IObject
{
// Constructor
public:
	CBall();
	virtual ~CBall();

	// Create the ball.
	bool Create(CBoard *pBoard, CPaddle *pLeftPaddle, CPaddle *pRightPaddle);

	// Initialize the object.
	bool Initialize();

	// Update the object.
	void Update(float fTime);

	// Render the object.
	void Render() const;

	// Process event.
	bool ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam);

	// Current ball's location.
	inline CVector2D GetPosition()
	{
		return m_vBallPosition;
	}

	// Current ball's velocity.
	inline CVector2D GetSpeed()
	{
		return m_vBallSpeed;
	}

// Implementation
private:
	// Create a new ball aimed toward left or right player.
	void NewBall(bool bGoToLeft);

	// Detects if the ball collide with a point A.
	inline bool HitPoint(CVector2D &vOldPos, CVector2D &vNewPos, CVector2D &vSpeed, CVector2D &ptA);

	CVector2D	m_vBallPosition,
				m_vBallSpeed;
	PARTICLE	m_particles[50];
	CBoard		*m_pBoard;
	CPaddle		*m_pLeftPaddle,
				*m_pRightPaddle;
	GLuint		m_nList;
	CRandomMT	m_rand;
};

#endif

