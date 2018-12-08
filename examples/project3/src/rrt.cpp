#include "rrt.h"

RRTGraph::RRTGraph()
{
	walls_ = new Walls;
	alpha_ = 3;
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
	pnt p(drand48()*win_width_, drand48()*win_height_);
	Node *ret;
	if(p.x >= 0 && p.x <=win_width_ && p.y >= 0 && p.y <= win_height_)
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
			gsout<<"RRTGraph::nearestPoint: to [" << p << "] is [" << 
				nearest->p << "]" << gsnl;
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
	gsout<<"RRTGraph::findConfig: Returning : "<<ret<<gsnl;
	return ret;
}

void RRTGraph::addConfig(Node *c_near, Node *c_new)
{
	c_new->parent = c_near;
	c_near->children.push_back(c_new);
	nodes_.push_back(c_new);
	last_node_ = c_new;
	gsout<<"RRTGraph::addConfig : new [" << c_new->p << "] to [" << c_near->p 
			<< gsnl;
}

bool RRTGraph::reached()
{
	if(dist(last_node_->p, goal_) < reach_thresh_)
		return true;
	return false;
}

void RRTGraph::performRRT()
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
					addConfig(c_near, c_new);
				}
			}
		}
		if(reached())
		{
			break;
		}
	}
	Node *c;
	if(reached())
		c = last_node_;
	else
		c = nearestPoint(goal_);
	while(c != NULL)
	{
		path_.push_back(c);
		c = c->parent;
	}
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
