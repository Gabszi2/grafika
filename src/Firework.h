
#ifndef INC_FIREWORK_H_
#define INC_FIREWORK_H_

#include "StdAfx.h"
#include "RandomMT.h"

class CFireworkRocket
{
// Construction
public:
	CFireworkRocket();
	virtual ~CFireworkRocket();
	bool Create();

// Operations
	bool Update(float dt);
	bool Render() const;

// Implementation
private:
	static const int ROCKET_FIRE = 300;
	static const int EXPLOSION_PINK = 70;
	static const int EXPLOSION_FIRE = 20;

	// Structures
	typedef struct
	{
		float	ini_life;
		float	ini_size;
		float	weight;
		float	xi;
		float	yi;
		float	zi;
		float	ini_color[3];
		
		bool	active;
		float	life;
		float	size;
		float	x;
		float	y;
		float	z;
		float	color[3];
	} PARTICLE;

	void Explode();
	void CreateRocketSpark(PARTICLE *pParticle);
	
	PARTICLE m_partSpark[ROCKET_FIRE],
			*m_partPink,
			*m_partFire,
			m_partRocket;
	bool	m_bExploding;
};

#endif

