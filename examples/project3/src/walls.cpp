#include "walls.h"

void Walls::addWall(GsPnt2 c1, GsPnt2 c2, GsPnt2 c3, GsPnt c4)
{
	struct line *l;
	l = new line;
	l->p1 = c1;l->p2 = c2;
	wall.push_back(l);
	l = new line;
	l->p1 = c1;l->p2 = c2;
	wall.push_back(l);
	l = new line;
        l->p1 = c1;l->p2 = c2;
        wall.push_back(l);
	l = new line;
        l->p1 = c1;l->p2 = c2;
        wall.push_back(l);
}

bool Walls::queryIntersection(GsPnt2 p1, GsPnt2 p2)
{
	float a1, b1, c1, a2, b2, c2, x, y, det;
	for( int i=0; i<wall.size(); i++ )
	{
		a1 = wall[i]->p2.y - wall[i]->p1.y;
		b1 = wall[i]->p2.x - wall[i]->p1.x;
		c1 = a1*wall[i]->p1.x + b1*wall[i]->p1.y;

		a2 = p2.y - p1.y;
		b2 = p2.x - p1.x;
		c2 = a2*p1.x + a2*p1.y;

		det = a1*b2 - a2*b1;
		if(det == 0)
		{
			gsout<<"Walls::queryIntersection: Error in calculating determinant!"<<gsnl;
		}

		x = (b2*c1 - b1*c2)/det;
		y = (a1*c2 - a2*c1)/det;

		if((std::min(wall[i]->p1.x, wall[i]->p2.x) <= x &&
		    std::max(wall[i]->p1.x, wall[i]->p2.x) >= x) &&
		   (std::min(wall[i]->p1.y, wall[i]->p2.y) <= y &&
		    std::max(wall[i]->p1.y, wall[i]->p2.y) >= y))
		    	return true;
		else
			return false;

	}
}
