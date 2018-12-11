#include <stdlib.h>
#include <thread>
#include <chrono>

#include <sig/gs_vec.h>
#include <sig/gs_input.h>
#include <sig/sn_lines.h>
#include <sig/gs_output.h>

#include "walls.h"

struct Node
{
	std::vector <Node*> children;
	Node *parent;
	pnt p;
};

class RRTGraph
{
    protected:
    	Walls *walls_;
	int alpha_, maxi_;
	pnt goal_;
	float reach_thresh_;
	float win_height_, win_width_;
	Node *root_, *last_node_;
	std::vector <Node*> nodes_;
	std::vector <Node*> path_;
    public:
     	enum obj_type{ ROBOT, WALL };
	RRTGraph();
	void init(GsPnt2 start, GsPnt2 goal, float rt, float win_w, float win_h);
	Node* getRandomNode();
	Node* nearestPoint(pnt p);
	pnt findConfig(Node *c, Node *c_near);
	void addConfig(Node *cN, Node *c_new);
	bool reached();
	void run(SnLines *pl, SnLines *gl);
	void performRRT(SnLines* pl, SnLines* gl);
	void addWall(pnt c1, pnt c2, pnt c3, pnt c4)
	{
		walls_->addWall(c1, c2, c3, c4);
	}
	std::vector<pnt> getNodePoints();
	std::vector<pnt> getPathPoints();
};
