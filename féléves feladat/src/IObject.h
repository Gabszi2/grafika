
#ifndef INC_INTERFACE_OBJECT_H_
#define INC_INTERFACE_OBJECT_H_

#ifdef _WIN32
#pragma once
#endif

class IObject
{
public:
	enum EEvent
	{
		eventMouseMove,
		eventMouseClick,
		eventChar,
		eventKeyDown,
		eventKeyUp
	};

// Constructors
	virtual ~IObject() {};

	virtual bool Initialize() = 0;

// Operations
	virtual void Update(float fTime) = 0;

	virtual void Render() const = 0;

	virtual bool ProcessEvent(EEvent nEvent, unsigned long wParam, unsigned long lParam) = 0;
};

#endif

