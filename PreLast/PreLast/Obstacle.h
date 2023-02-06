#pragma once
#include "Header.h"

class Obstacle
{
protected:
	string name;
	bool bType;
	float fParam;
public:
	
	Obstacle() : bType(0), fParam(0) { name = "obstacle"; }
	Obstacle(float fParam) :bType(0), fParam(fParam) { name = "obstacle"; }
	Obstacle(float fParam, bool type) : fParam(fParam), bType(type) { name = "obstacle"; }


	virtual float GetfParam() const { return fParam; }
	virtual bool GetType() const { return bType; }
	virtual string GetName() const { return name; }

};

class RunningTrack : public Obstacle
{
public:
	RunningTrack() : Obstacle(0, false) { name = "running track"; }
	RunningTrack(float fParam) : Obstacle(fParam, false) { name = "running track"; }

	virtual void Info() const
	{
		cout << "Length of running track " << fParam << "m\n";
	}
};

class Wall : public Obstacle
{
public:
	Wall() : Obstacle(0, true) { name = "wall"; }
	Wall (float fParam) : Obstacle(fParam, true) { name = "wall"; }

	virtual void Info() const
	{
		cout << "Height of wall " << fParam << "m\n";
	}
};

