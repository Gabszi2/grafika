
#ifndef INC_BOARD_H_
#define INC_BOARD_H_

#ifdef _WIN32
#pragma once
#endif

#include "IObject.h"

class CBoard : public IObject
{
public:
// Constructor
	CBoard();

	// Initialize the object.
	bool Initialize();

	// Update the object.
	void Update(float fTime);

	// Render the object.
	void Render() const;

	// Process event.
	bool ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam);

	static const float GetTop() {
		return 48.0f;
	}

	static const float GetBottom() {
		return -48.0f;
	}

	static const float GetLeft() {
		return 64.0f;
	}

	static const float GetRight() {
		return -64.0f;
	}

	static const float GetWidth() {
		return GetLeft() - GetRight();
	}
	static const float GetHeight() {
		return GetTop() - GetBottom();
	}

	// Add points to a player's score.
	void Score(bool bLeftPlayer);

// Implémentation
private:
	static void DrawDigitNumber(int nNumber);

	int			m_nLeftScore,
				m_nRightScore;
	float		m_fIlluminateLeftBorder,
				m_fIlluminateRightBorder;
};

#endif

