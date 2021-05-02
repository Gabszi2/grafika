

#include "StdAfx.h"
#include "AiPaddle.h"
#include "Board.h"

// Constructor
CAiPaddle::CAiPaddle(bool bLeftPaddle) : CPaddle(bLeftPaddle),
	m_pBall(NULL)
{
}

// Create the AI.
bool CAiPaddle::Create(CBall *pBall)
{
	m_pBall = pBall;
	return true;
}
	



// Update the object.

void CAiPaddle::Update(float fTime)
{
	static float	fTotalTime = 0.0f;
	CVector2D		vBallPos;
	float			fBallDist,
					fRandom;

	
	fTotalTime += fTime;

	
	vBallPos = m_pBall->GetPosition();

	
	if (m_bLeftPaddle)
		fBallDist = CBoard::GetLeft() - vBallPos.x;
	else
		fBallDist = vBallPos.x - CBoard::GetRight();

	
	fRandom = 0.5f*CPaddle::GetHeight()*cos(fTotalTime*6.0f);

	
	if (fBallDist > 0.75f*CBoard::GetWidth())
		Stop();
	else if (m_fY < vBallPos.y-0.2f*CPaddle::GetHeight()+fRandom)
		MoveUp();
	else if (m_fY > vBallPos.y+0.2f*CPaddle::GetHeight()+fRandom)
		MoveDown();
	else
		Stop();

	
	CPaddle::Update(fTime);
}

// Process event.

bool CAiPaddle::ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam)
{
	
	return false;
}

