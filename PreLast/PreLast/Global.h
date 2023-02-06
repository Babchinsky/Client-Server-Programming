#pragma once
#include "Header.h"
#include "Obstacle.h"
#include "Member.h"

bool operator+ (const Member& memb, const Obstacle& obst)
{
	if (obst.GetType())
	{
		return (memb.GetJumpHight() >= obst.GetfParam());
	}
	else
	{
		return (memb.GetRunLength() >= obst.GetfParam());
	}
}

bool operator+ (const Obstacle& obst, const Member& memb)
{
	return (memb + obst);
}