#include <sig/gs_vec.h>
#include <sig/gs_input.h>
#include <sig/gs_output.h>
#include <vector>
#include <utility>

typedef GsPnt2 pnt;

struct line
{
        pnt p1, p2;
};
class Walls
{
	std::vector <line*> wall; 
    public:
    	Walls() { gsout << "Creating environment!" <<gsnl; }
	~Walls();
	void addWall(GsPnt2 c1, GsPnt2 c2, GsPnt2 c3, GsPnt c4);
	bool queryIntersection(GsPnt2 p1, GsPnt2 p2); 
};
