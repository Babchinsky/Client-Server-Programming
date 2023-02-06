#pragma once
#include "Header.h"

class Member
{
protected:
	string name;
	float fRunLength;
	float fJumpHight;

public:

	Member() : fRunLength(0), fJumpHight(0) { name = "member"; }
	Member(float RunLength) : Member() { fRunLength = RunLength; name = "member"; }
	Member(float RunLength, float HightJump) : Member(RunLength) { fJumpHight = HightJump; name = "member"; }

	virtual void Run() const
	{
		cout << name << " can run " << fRunLength << "m\n";
	}

	virtual void Jump() const
	{
		cout << name << " jumped to a height of " << fJumpHight << "m\n";
	}

	virtual float GetRunLength() const { return fRunLength; }
	virtual float GetJumpHight() const { return fJumpHight; }
	virtual string GetName() const { return name; }

};

class Human : public Member
{
public:
	Human() : Member() { name = "human"; }
	Human(float RunLength) : Member(RunLength) { name = "human"; }
	Human(float RunLength, float HightJump) : Member(RunLength, HightJump) { name = "human"; }

	virtual void Run() const
	{
		cout << "Human can run " << fRunLength << "m\n";
	}

	virtual void Jump() const
	{
		cout << "Human jumped to a height of " << fJumpHight << "m\n";
	}
};


class Cat : public Member
{
public:
	Cat() : Member() { name = "cat"; }
	Cat(float RunLength) : Member(RunLength) { name = "cat"; }
	Cat(float RunLength, float HightJump) : Member(RunLength, HightJump) { name = "cat"; }

	virtual void Run() const
	{
		cout << "Cat can run " << fRunLength << "m\n";

	}

	virtual void Jump() const
	{
		cout << "Cat jumped to a height of " << fJumpHight << "m\n";
	}
};

class Robot : public Member
{
public:
	Robot() : Member() { name = "robot"; }
	Robot(float RunLength) : Member(RunLength) { name = "robot"; }
	Robot(float RunLength, float HightJump) : Member(RunLength, HightJump) { name = "robot"; }

	virtual void Run() const
	{
		cout << "Robot can run " << fRunLength << "m\n";

	}

	virtual void Jump() const
	{
		cout << "Robot jumped to a height of " << fJumpHight << "m\n";
	}
};