
#ifndef INC_PADDLE_H_
#define INC_PADDLE_H_

#ifdef _WIN32
#pragma once
#endif

#include "IObject.h"

class CPaddle : public IObject
{
public:
// Constructor
	CPaddle(bool bLeftPaddle);

	// Initialize the object.
	bool Initialize();

	// Update the object.
	void Update(float fTime);

	// Render the object.
	void Render() const;

	// Process event.
	bool ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam);

	static const float GetWidth() {
		return 6.0f;
	}

	static const float GetHeight() {
		return 20.0f;
	}

	inline float GetPosition() const {
		return m_fY;
	}

	// Move paddle upward.
	void MoveUp();

	// Move paddle downward.
	void MoveDown();

	// Stop moving paddle.
	void Stop();

	// Illuminate paddle.
	void Illuminate();

protected:
	bool	m_bLeftPaddle;
	float	m_fSpeed,
			m_fY,
			m_fIlluminate;
	GLuint	m_nGLPaddle;
};

#endif

