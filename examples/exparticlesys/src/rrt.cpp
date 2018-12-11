#include "rrt.h"

RRTGraph::RRTGraph()
{
	walls_ = new Walls;
	alpha_ = 1;
	maxi_ = 3000;
}

void RRTGraph::init(GsPnt2 start, GsPnt2 goal, float rt, float win_w, float win_h)
{
	root_ = new Node;
	root_->parent = NULL;
	root_->p = start;
	last_node_ = root_;
	nodes_.push_back(root_);
	goal_ = goal;
	win_width_ = win_w;
	win_height_ = win_h;
	reach_thresh_ = rt;
}

Node* RRTGraph::getRandomNode()
{
	float x = rand() % 201 + (-100);
	float y = rand() % 201 + (-100);
	pnt p(x,y);
	Node *ret;
	//gsout << "Getting random node: " << p << gsnl;
	if(x >= -100 && x <= 100 && y >= -100 && y <= 100)
	{
		ret = new Node;
		ret->p = p;
		return ret;
	}
	return NULL;
}

Node* RRTGraph::nearestPoint(pnt p)
{
	float minDist = 1e9;
	Node* nearest = NULL;
	for(int i=0; i<nodes_.size(); i++)
	{
		float d = dist(p,nodes_[i]->p);
		if(d < minDist)
		{
			minDist = d;
			nearest = nodes_[i];
		}
	}
	return nearest;
}

pnt RRTGraph::findConfig(Node *c, Node *c_near)
{
	pnt to = c->p;
	pnt from = c_near->p;
	pnt inter = to - from;
	inter = inter / inter.norm();
	pnt ai(alpha_*inter.x, alpha_*inter.y);
	pnt ret = from + ai;
	return ret;
}

void RRTGraph::addConfig(Node *c_near, Node *c_new)
{
	c_new->parent = c_near;
	c_near->children.push_back(c_new);
	nodes_.push_back(c_new);
	last_node_ = c_new;
}

bool RRTGraph::reached()
{
	if(dist(last_node_->p, goal_) < reach_thresh_)
	{
		gsout << "Goal reached" << gsnl;
		return true;
	}
	return false;
}

/*void RRTGraph::run(SnLines *pl, SnLines *gl)
{
	std::thread t(&RRTGraph::performRRT, pl, gl);
	t.detach();
	
	return;
}*/
void RRTGraph::performRRT(SnLines *pl, SnLines *gl)
{
	for(int i=0; i<maxi_; i++)
	{
		Node *c = getRandomNode();
		if(c)
		{
			Node *c_near = nearestPoint(c->p);
			if(dist(c->p, c_near->p) > alpha_)
			{
				pnt new_config = findConfig(c, c_near);
				if(walls_->queryIntersection(new_config, c_near->p))
				{
					Node *c_new = new Node;
					c_new->p = new_config;
					pl->push(c_new->p.x, c_new->p.y, 0.0f);
					addConfig(c_near, c_new);
				}
			}
		}
		if(reached())
		{
			break;
		}
		//std::this_thread::sleep_for (std::chrono::seconds(1));
	}
	Node *c;
	if(reached())
		c = last_node_;
	else
		c = nearestPoint(goal_);
	while(c != NULL)
	{
		path_.push_back(c);
		gl->push(c->p.x, c->p.y, 0.0f);
		gsout << "Path of the goal: " << c->p << gsnl;
		c = c->parent;
	}
	gsout << "Start: " << root_->p << " goal: " << goal_ << gsnl;
}

std::vector<pnt> RRTGraph::getNodePoints()
{
	std::vector<pnt> ret;
	for(int i = 0;i < nodes_.size();i++)
		ret.push_back(nodes_[i]->p);
	return ret;
}

std::vector<pnt> RRTGraph::getPathPoints()
{
	std::vector<pnt> ret;
	for(int i = 0;i < path_.size();i++)
		ret.push_back(path_[i]->p);
	return ret;
}
