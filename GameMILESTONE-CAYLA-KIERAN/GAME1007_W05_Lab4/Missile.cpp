#include "Missile.h"

Missile::Missile(int x, int y) :m_src( {0, 439, 9, 26}), m_FrameCtr(0), m_FrameMax(5),
m_spriteIdx(0), m_spriteMax(4)
{
	m_dst = {x,y,m_src.w*2,m_src.h*2};
}

void Missile::update()
{
	if (m_FrameCtr++ == m_FrameMax)
	{

		m_FrameCtr = 0;
		m_spriteIdx++;
		if (++m_spriteIdx == m_spriteMax)
		{
			m_spriteIdx = 0;

		}
		m_src.x = 0 + m_src.w * m_spriteIdx;
	}
 


	m_dst.y -= MOVESPEED;
}
