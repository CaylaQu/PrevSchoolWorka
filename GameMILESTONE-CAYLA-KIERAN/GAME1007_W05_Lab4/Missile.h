#pragma once
#ifndef _MISSLIE_H_
#define _MISSLIE_H_
#include <SDL.h>
#define MOVESPEED 1 

class Missile
{
private:
	unsigned short m_FrameCtr, // Counts number of frames
		m_FrameMax,  // number of frames each sprite is diplayed
		m_spriteIdx,
		m_spriteMax;
public:
	SDL_Rect m_src, m_dst; 
	Missile(int = 0, int = 0 );
    void update();
};

#endif 