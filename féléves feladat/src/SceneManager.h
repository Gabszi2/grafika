
#ifndef INC_SCENE_MANAGER_H_
#define INC_SCENE_MANAGER_H_

#ifdef _WIN32
#pragma once
#endif

#include "IObject.h"
#include <vector>

class CSceneManager : public IObject
{
// Constructors
public:
	CSceneManager(void);
	virtual ~CSceneManager(void);

	// Add object to the list.
	virtual bool	AddObject(IObject *pObject);

	// Initialize the object.
	virtual bool	Initialize();

	// Asks objects to update their content.
	virtual void	Update(float fTime);

	// Asks objects to render.
	virtual void	Render() const;

	// Asks objects to process an event.
	virtual bool	ProcessEvent(IObject::EEvent nEvent, DWORD wParam, DWORD lParam);

// Implementation
private:
	std::vector<IObject*> m_pvObjects;
};

#endif

