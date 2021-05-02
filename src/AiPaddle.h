
#ifndef INC_AI_PADDLE_H_
#define INC_AI_PADDLE_H_

#ifdef _WIN32
#pragma once
#endif

#include "Paddle.h"
#include "Ball.h"

class CAiPaddle : public CPaddle
{
public:
// Constructor
	CAiPaddle(bool bLeftPaddle);

	// Create the AI.
	bool Create(CBall *pBall);

	// Update the object.
	void Update(float fTime);

	// Process event.
	bool ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam);

// Implementation
private:
	CBall	*m_pBall;
};

#endif

