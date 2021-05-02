
#include "StdAfx.h"
#include "Firework.h"

#define COLORS	12
#define COLORS2	6
#define MY_RAND(a,b)	(float(a) + ((float)g_rand.RandomReal1()-0.5f)*float(b))

static GLfloat colors[COLORS][3]=
{
	{1.0f,0.10f,0.10f},{1.0f,0.50f,0.30f},{1.0f,1.0f,0.50f},{0.50f,1.0f,0.25f},
	{0.25f,1.0f,0.25f},{0.25f,1.0f,0.50f},{0.25f,1.0f,1.0f},{0.25f,0.50f,1.0f},
	{0.25f,0.25f,1.0f},{0.50f,0.25f,1.0f},{1.0f,0.25f,1.0f},{1.0f,0.25f,0.50f}
};

static GLfloat colors2[COLORS2][3]=
{
	{1.0f,0.5f,0.0f},{1.0f,0.75f,0.5f},{1.0f,1.0f,0.7f},
 	{1.0f,1.0f,0.5f},{0.8f,0.8f,0.8f},{1.0f,1.0f,0.8f}
};

static CRandomMT	g_rand;


CFireworkRocket::CFireworkRocket() :
	m_partFire(NULL),
	m_partPink(NULL)
{
	g_rand.Randomize();
}

CFireworkRocket::~CFireworkRocket()
{
	delete [] m_partFire;
}

bool CFireworkRocket::Create()
{
	float fAngle, fAngle2, fVelocity;
	int i, j, nExplosionColor;

	m_partPink = new PARTICLE[EXPLOSION_PINK];
	if (m_partPink == NULL)
		return false;
	m_partFire = new PARTICLE[EXPLOSION_PINK*EXPLOSION_FIRE];
	if (m_partFire == NULL)
		return false;
	
	m_bExploding = false;
	
	m_partRocket.active = true;
	m_partRocket.life = m_partRocket.ini_life = MY_RAND(2.5f, 3.0f);
	m_partRocket.ini_size = 0.5f;
	m_partRocket.weight = 2.0f;
	fAngle = float(M_PI)/2.0f + (float)g_rand.RandomRange(-M_PI/4.0f, M_PI/4.0f);
	m_partRocket.xi = (float)cos(fAngle)*13.0f;
	m_partRocket.yi = (float)sin(fAngle)*13.0f;
	m_partRocket.zi = (float)sin(g_rand.RandomRange(-M_PI/8.0f, M_PI/8.0f))*13.0f;
	m_partRocket.x = float(-30.0f + fAngle*60.0f/M_PI + g_rand.RandomRange(-10.0, 10.0));
	m_partRocket.y = (float)g_rand.RandomRange(-24.0, -15.0);
	m_partRocket.z = 0.0f;
	m_partRocket.color[0] = m_partRocket.ini_color[0] = 1.0f;
	m_partRocket.color[1] = m_partRocket.ini_color[1] = 0.8f;
	m_partRocket.color[2] = m_partRocket.ini_color[2] = 0.2f;


	for (i=0; i<ROCKET_FIRE; ++i)
		CreateRocketSpark(&m_partSpark[i]);
	
	nExplosionColor = g_rand.RandomInt()%COLORS;
	for (i=0; i<EXPLOSION_PINK; ++i)
	{
		memset(&m_partPink[i], 0, sizeof(PARTICLE));
		m_partPink[i].active = false;
		m_partPink[i].life = m_partPink[i].ini_life = MY_RAND(1.6f, 1.9f);
		m_partPink[i].ini_size = 0.8f;
		m_partPink[i].weight = 1.0f * m_partPink[i].ini_life;
		fAngle  = float( g_rand.RandomReal2() * 2.0 * M_PI );
		fAngle2 = float( g_rand.RandomReal2() * 2.0 * M_PI );
		fVelocity = MY_RAND(7.2f, 11.1f);
		m_partPink[i].xi = fVelocity * float(cos(fAngle2)*cos(fAngle));
		m_partPink[i].yi = fVelocity * float(cos(fAngle2)*sin(fAngle));
		m_partPink[i].zi = fVelocity * float(sin(fAngle2));
		m_partPink[i].color[0] = m_partPink[i].ini_color[0] = colors[nExplosionColor][0];
		m_partPink[i].color[1] = m_partPink[i].ini_color[1] = colors[nExplosionColor][1];
		m_partPink[i].color[2] = m_partPink[i].ini_color[2] = colors[nExplosionColor][2];

		for (j=0; j<EXPLOSION_FIRE; ++j)
		{
			memcpy(&m_partFire[i*EXPLOSION_FIRE+j], &m_partPink[i], sizeof(PARTICLE));
			m_partFire[i*EXPLOSION_FIRE+j].weight = 0.8f;
			m_partFire[i*EXPLOSION_FIRE+j].xi = 0.0f;
			m_partFire[i*EXPLOSION_FIRE+j].yi = 0.0f;
			m_partFire[i*EXPLOSION_FIRE+j].zi = 0.0f;
		}
	}
	
	return true;
}

void CFireworkRocket::CreateRocketSpark(PARTICLE *pParticle)
{
	int color = g_rand.RandomInt()%COLORS2;
	float fRnd = float(g_rand.RandomReal2())*0.1f;

	pParticle->active = true;
	pParticle->life = pParticle->ini_life = MY_RAND(1.1f, 2.0f);
	pParticle->ini_size = MY_RAND(0.2f, 0.3f);
	pParticle->weight = 0.5f;
	pParticle->x = MY_RAND(m_partRocket.x, 0.1f) - fRnd*m_partRocket.xi;
 	pParticle->y = MY_RAND(m_partRocket.y, 0.1f) - fRnd*m_partRocket.yi;
 	pParticle->z = MY_RAND(m_partRocket.z, 0.1f) - fRnd*m_partRocket.zi;
	pParticle->xi = MY_RAND(0.0f, 0.8f);
	pParticle->yi = MY_RAND(0.0f, 0.8f);
	pParticle->zi = MY_RAND(0.0f, 0.8f);
	pParticle->color[0] = pParticle->ini_color[0] = colors2[color][0];
	pParticle->color[1] = pParticle->ini_color[1] = colors2[color][1];
	pParticle->color[2] = pParticle->ini_color[2] = colors2[color][2];
}

bool CFireworkRocket::Render() const
{
	float	x, y, z;
	register int i;

	for (i=0; i<ROCKET_FIRE; ++i)
	{
		if (m_partSpark[i].active)
		{
			x = m_partSpark[i].x;
			y = m_partSpark[i].y;
			z = m_partSpark[i].z;

			glColor3fv(m_partSpark[i].color);

			glBegin(GL_TRIANGLE_STRIP);
				glTexCoord2f(1,1); glVertex3f(x+m_partSpark[i].size,y+m_partSpark[i].size,z);
				glTexCoord2f(0,1); glVertex3f(x-m_partSpark[i].size,y+m_partSpark[i].size,z);
				glTexCoord2f(1,0); glVertex3f(x+m_partSpark[i].size,y-m_partSpark[i].size,z);
				glTexCoord2f(0,0); glVertex3f(x-m_partSpark[i].size,y-m_partSpark[i].size,z);
			glEnd();
		}
	}

	if (m_bExploding)
	{
		for (i=0; i<EXPLOSION_PINK; ++i)
		{
			if (m_partPink[i].active)
			{
				glColor3fv(m_partPink[i].color);

				x = m_partPink[i].x;
				y = m_partPink[i].y;
				z = m_partPink[i].z;

				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2f(1,1); glVertex3f(x+m_partPink[i].size,y+m_partPink[i].size,z);
					glTexCoord2f(0,1); glVertex3f(x-m_partPink[i].size,y+m_partPink[i].size,z);
					glTexCoord2f(1,0); glVertex3f(x+m_partPink[i].size,y-m_partPink[i].size,z);
					glTexCoord2f(0,0); glVertex3f(x-m_partPink[i].size,y-m_partPink[i].size,z);
				glEnd();
			}
		}

		for (i=0; i<EXPLOSION_PINK*EXPLOSION_FIRE; ++i)
		{
			if (m_partFire[i].active)
			{
				glColor3fv(m_partFire[i].color);

				x = m_partFire[i].x;
				y = m_partFire[i].y;
				z = m_partFire[i].z;

				glBegin(GL_TRIANGLE_STRIP);
					glTexCoord2f(1,1); glVertex3f(x+m_partFire[i].size,y+m_partFire[i].size,z);
					glTexCoord2f(0,1); glVertex3f(x-m_partFire[i].size,y+m_partFire[i].size,z);
					glTexCoord2f(1,0); glVertex3f(x+m_partFire[i].size,y-m_partFire[i].size,z);
					glTexCoord2f(0,0); glVertex3f(x-m_partFire[i].size,y-m_partFire[i].size,z);
				glEnd();
			}
		}
	}

	return true;
}

bool CFireworkRocket::Update(float dt)
{
	static float	t = 0.0f;
	float			fLife,
					fAlpha;
	bool			bEndExplode = true;
	register int	i;

	t += dt;

	if (m_partRocket.active)
	{
		m_partRocket.x += m_partRocket.xi*dt;
		m_partRocket.y += m_partRocket.yi*dt;
		m_partRocket.z += m_partRocket.zi*dt;

		if (m_partRocket.life > 0.0f)
		{
			fLife = m_partRocket.life / m_partRocket.ini_life;

			m_partRocket.xi += 0.03f*sin(t*m_partRocket.ini_life*4.0f);
			m_partRocket.yi += 0.03f*cos(t*m_partRocket.ini_life*2.0f) - m_partRocket.weight*dt;
		}

		m_partRocket.life -= dt;
		if (m_partRocket.life < -0.3f)
		{
			m_partRocket.active = false;
			Explode();
		}
	}
	
	for (i=0; i<ROCKET_FIRE; ++i)
	{
		if (!m_partSpark[i].active)
			continue;

		fLife = m_partSpark[i].life / m_partSpark[i].ini_life;

		m_partSpark[i].x += m_partSpark[i].xi*fLife*fLife*dt;
		m_partSpark[i].y += m_partSpark[i].yi*fLife*fLife*dt;
		m_partSpark[i].z += m_partSpark[i].zi*fLife*fLife*dt;

		m_partSpark[i].yi -= float(m_partSpark[i].weight*dt);
		m_partSpark[i].size = m_partSpark[i].ini_size * fLife*fLife;
		if (fLife < 0.2f)
		{
			fAlpha = fLife*5.0f;
			m_partSpark[i].color[0] = m_partSpark[i].ini_color[0] * fAlpha;
			m_partSpark[i].color[1] = m_partSpark[i].ini_color[2] * fAlpha;
			m_partSpark[i].color[2] = m_partSpark[i].ini_color[3] * fAlpha;
		}

		m_partSpark[i].life -= dt;
		if (m_partSpark[i].life < 0.0f)
			if (m_partRocket.life < float(i)/ROCKET_FIRE)
				m_partSpark[i].active = false;
			else
				CreateRocketSpark(&m_partSpark[i]);
    }
    
    if (m_bExploding)
    {
	    for (i=0; i<EXPLOSION_PINK; ++i)
	   	{
			if (m_partPink[i].active)
			{
				fLife = m_partPink[i].life / m_partPink[i].ini_life;

				m_partPink[i].x += m_partPink[i].xi*fLife*fLife*dt;
				m_partPink[i].y += m_partPink[i].yi*fLife*fLife*dt;
				m_partPink[i].z += m_partPink[i].zi*fLife*fLife*dt;

				m_partPink[i].yi -= m_partPink[i].weight*dt;
				m_partPink[i].size = m_partPink[i].ini_size * fLife*fLife;
				if (fLife < 0.2f)
				{
					fAlpha = fLife*5.0f;
					m_partPink[i].color[0] = m_partPink[i].ini_color[0] * fAlpha;
					m_partPink[i].color[1] = m_partPink[i].ini_color[2] * fAlpha;
					m_partPink[i].color[2] = m_partPink[i].ini_color[3] * fAlpha;
				}

				m_partPink[i].life -= (float) dt;
				if (m_partPink[i].life < 0.0f)
					m_partPink[i].active = false;

				int j = int(fLife*(EXPLOSION_FIRE-2)+1 + cos(t*10.0f));
				if (!m_partFire[i*EXPLOSION_FIRE+j].active)
				{
					m_partFire[i*EXPLOSION_FIRE+j].active = true;
					m_partFire[i*EXPLOSION_FIRE+j].life = 0.7f * m_partPink[i].life;
					m_partFire[i*EXPLOSION_FIRE+j].x = m_partPink[i].x;
					m_partFire[i*EXPLOSION_FIRE+j].y = m_partPink[i].y;
					m_partFire[i*EXPLOSION_FIRE+j].z = m_partPink[i].z;
				}
			}
		}

	    for (i=0; i<EXPLOSION_PINK*EXPLOSION_FIRE; ++i)
	   	{
			if (m_partFire[i].active)
			{
				bEndExplode = false;
	
				fLife = m_partFire[i].life / m_partFire[i].ini_life;

				m_partFire[i].x += m_partFire[i].xi*dt;
				m_partFire[i].y += m_partFire[i].yi*dt;
				m_partFire[i].z += m_partFire[i].zi*dt;

				m_partFire[i].yi -= m_partFire[i].weight*dt;
				m_partFire[i].size = m_partFire[i].ini_size * fLife*fLife;
				if (fLife < 0.2f)
				{
					fAlpha = fLife*5.0f;
					m_partFire[i].color[0] = m_partFire[i].ini_color[0] * fAlpha;
					m_partFire[i].color[1] = m_partFire[i].ini_color[2] * fAlpha;
					m_partFire[i].color[2] = m_partFire[i].ini_color[3] * fAlpha;
				}

				m_partFire[i].life -= dt;
				if (m_partFire[i].life < 0.0f)
					m_partFire[i].active = false;
			}
	    }

	    if (bEndExplode)
	        Create();
    }
	return true;
}

void CFireworkRocket::Explode()
{
	register int i;
	
	for (i=0; i<EXPLOSION_PINK; ++i)
	{
		m_partPink[i].active = true;
		m_partPink[i].x = m_partRocket.x;
		m_partPink[i].y = m_partRocket.y;
		m_partPink[i].z = m_partRocket.z;
	}

	m_bExploding = true;
}
