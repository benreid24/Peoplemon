#include "World/People/PathFinder.hpp"
#include "Game/Game.hpp"
#include "Globals.hpp"
using namespace std;
using namespace sf;

PathFinder::PathFinder(Game* g, sf::Vector2i s, sf::Vector2i d, int sD)
{
	start = s;
	dest = d;
	startDir = sD;
	game = g;
}

int PathFinder::getDir(Vector2i cur, Vector2i next)
{
	if (cur.x<next.x)
		return 1;
	if (cur.x>next.x)
		return 3;
	if (cur.y>next.y)
		return 0;
	return 2;
}

vector<int> PathFinder::getPath()
{
	if (start==dest)
		return vector<int>();

	vector<Vector3i> nodes;
	nodes.reserve(abs(start.x-dest.x)*abs(start.y-dest.y));

	nodes.push_back(Vector3i(dest.x,dest.y,0));
	for (unsigned int i = 0; i<nodes.size(); ++i)
	{
        Vector3i next[4] = { Vector3i(nodes[i].x+1,nodes[i].y,nodes[i].z+1),
                             Vector3i(nodes[i].x-1,nodes[i].y,nodes[i].z+1),
                             Vector3i(nodes[i].x,nodes[i].y+1,nodes[i].z+1),
							 Vector3i(nodes[i].x,nodes[i].y-1,nodes[i].z+1) };

		for (int j = 0; j<4; ++j)
		{
            if (next[j].x==start.x && next[j].y==start.y)
				goto found;

			if (!game->world.spaceFree(Vector2i(next[j].x,next[j].y)))
				continue;
			for (unsigned int k = 0; k<nodes.size(); ++k)
			{
				if (nodes[k].x==next[j].x && nodes[k].y==next[j].y)
				{
					if (nodes[k].z<=next[j].z)
						goto noAdd;
					nodes[k].z = next[j].z;
					goto noAdd;
				}
			}
			nodes.push_back(next[j]);
			noAdd:;
		}
	}
	found: //path from end to start is now mapped in nodes vector by distance to the destination

	vector<Vector2i> path;
	path.push_back(start);
	Vector2i curPos = start;

    while (true)
	{
		Vector3i adjNodes[4];
		int adjCount = 0;
        for (unsigned int i = 0; i<nodes.size(); ++i)
		{
			if ((abs(nodes[i].x-curPos.x)==1 && nodes[i].y==curPos.y) || (abs(nodes[i].y-curPos.y)==1 && nodes[i].x==curPos.x))
			{
				adjNodes[adjCount] = nodes[i];
				adjCount++;
				if (adjCount==4)
					break;
			}
		}

        int lowI = -1, lowC = 10000;
        for (int i = 0; i<adjCount; ++i)
		{
			if (adjNodes[i].z<lowC)
			{
				lowC = adjNodes[i].z;
				lowI = i;
			}
		}

		path.push_back(Vector2i(adjNodes[lowI].x,adjNodes[lowI].y));
		curPos = Vector2i(adjNodes[lowI].x,adjNodes[lowI].y);

		if (curPos==dest)
			break;
	}

	vector<int> ret;
	int cDir = startDir;

	for (unsigned int i = 0; i<path.size()-1; ++i)
	{
		int d = getDir(path[i],path[i+1]);
		if (cDir!=d)
			ret.push_back(d);
		ret.push_back(d);
		cDir = d;
	}

	return ret;
}
