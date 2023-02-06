#include "Header.h"
#include "Obstacle.h"
#include "Member.h"
#include "Global.h"
#include <vector>

void main()
{
	vector<Obstacle> obst;
	vector<Member> memb;
	memb.push_back(Cat(25, 1.5));
	memb.push_back(Human(50, 0.5));
	memb.push_back(Robot(100, 2));

	obst.push_back(Wall(1));
	obst.push_back(RunningTrack(50));

	for (int i = 0; i < memb.size(); i++)
	{
		for (int j = 0; j < obst.size() && i < memb.size();)
		{
			if ((memb[i] + obst[j]))
			{
				cout  << memb[i].GetName() << "(" << memb[i].GetRunLength() << "m, " << memb[i].GetJumpHight() << "m)";
				cout << " get throw " << obst[j].GetName() << "(" << obst[j].GetfParam() << "m)\n";
				j++;
			}
			else
			{
				cout << memb[i].GetName() << "(" << memb[i].GetRunLength() << "m, " << memb[i].GetJumpHight() << "m)";
				cout << " doesn't get throw " << obst[j].GetName() << "(" << obst[j].GetfParam() << "m)\n";
				cout << endl;
				i++;
				j = 0;
			}
		}
		cout << i + 1 << " member win!\n\n";
	}
}

/*RESULT*/

/*
cat(25m, 1.5m) get throw wall(1m)
cat(25m, 1.5m) doesn't get throw running track(50m)

human(50m, 0.5m) doesn't get throw wall(1m)

robot(100m, 2m) get throw wall(1m)
robot(100m, 2m) get throw running track(50m)
3 member win!
*/