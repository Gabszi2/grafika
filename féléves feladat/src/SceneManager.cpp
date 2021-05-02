
#include "StdAfx.h"
#include "SceneManager.h"

CSceneManager::CSceneManager(void)
{
}

CSceneManager::~CSceneManager(void)
{
}

bool CSceneManager::AddObject(IObject *pObject)
{
	try
	{
		m_pvObjects.push_back(pObject);
	}
	catch (...)
	{
		return false;
	}
	return true;
}

bool CSceneManager::Initialize()
{
	std::vector<IObject*>::iterator	iter;

	for (iter=m_pvObjects.begin(); iter!=m_pvObjects.end(); ++iter)
		if (!(*iter)->Initialize())
			return false;
	return true;
}

void CSceneManager::Update(float fTime)
{
	std::vector<IObject*>::iterator	iter;

	for (iter=m_pvObjects.begin(); iter!=m_pvObjects.end(); ++iter)
		(*iter)->Update(fTime);
}

void CSceneManager::Render() const
{
	std::vector<IObject*>::const_iterator	iter;

	for (iter=m_pvObjects.begin(); iter!=m_pvObjects.end(); ++iter)
		(*iter)->Render();
}

bool CSceneManager::ProcessEvent(IObject::EEvent nEvent, DWORD wParam, DWORD lParam)
{
	std::vector<IObject*>::iterator	iter;

	for (iter=m_pvObjects.begin(); iter!=m_pvObjects.end(); ++iter)
		if ((*iter)->ProcessEvent(nEvent, wParam, lParam))
			return true;
	return false;
}

