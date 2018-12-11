#include "rrt.h"

RRTGraph::RRTGraph()
{
	walls_ = new Walls;
	alpha_ = 1;
	maxi_ = 3000;
}

void RRTGraph::init(GsPnt2 start, float rt, float win_w, float win_h, GsMat *pos)
{
	gsout << "RRTGraph::init: Got the matrix: " << gsnl;
	position_ = pos;
	root_ = new Node;
	path_computed_ = false;
	root_->parent = NULL;
	root_->p = start;
	last_node_ = root_;
	nodes_.push_back(root_);
	win_width_ = win_w;
	win_height_ = win_h;
	reach_thresh_ = rt;
	new_goal_ = true;
	robo_= new Robot;
	robo_->pos = GsVec(start.x, start.y, 0);
	robo_->m = 2.0f;
	robo_->vel = GsVec2(0.0f,0.0f);
	robo_->max_force = 2.0f;
	robo_->max_speed = 0.1f;
	robo_->steer_dir = GsVec(0.0f, 0.0f, 0.0f);
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

void RRTGraph::performRRT(SnLines *pl, SnLines *gl)
{
	while(!new_goal_);
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
	if(c != NULL)
	{
		new_goal_ = false;
		path_computed_ = true;
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

void RRTGraph::eulerIntegration()
{
    gsout << "RRTGraph::eulerIntegration: " << robo_->pos << " to " <<
    	goal_ << gsnl;
    while(!path_computed_);
    for(int i=0;i<path_.size();i++)
    {
    	GsVec cur_g(path_[i]->p.x,path_[i]->p.y,0.0f);
	gsout << "Traversing intermidiate path: " << cur_g << gsnl;
    	while(::dist(robo_->pos,cur_g) > 0.5)
    	{
    		gsout << "++++++++++++++++++++++++++++++++++++++++" << gsnl;
		gsout << "RRTGraph::eulerIntegration: Distance to goal: " << cur_g <<
			" is " << ::dist(robo_->pos,cur_g) << gsnl;
		steering(cur_g);
		GsVec steering_force = truncate(robo_->steer_dir, robo_->max_force);
		gsout << "Steering force: " << steering_force << gsnl;
		GsVec a = steering_force/robo_->m;
		robo_->vel = truncate(robo_->vel + a, robo_->max_speed);
		gsout << "Velocity: " << robo_->vel << gsnl;
		robo_->pos = robo_->pos + robo_->vel;
		gsout << "Robot position: " << robo_->pos << gsnl;
		position_->setrans(robo_->pos);
		gsout << "++++++++++++++++++++++++++++++++++++++++++" << gsnl;
	}
    }
    path_.clear();
    path_computed_ = false;
}

GsVec RRTGraph::truncate(GsVec v, float t)
{
	gsout << "RRTGraph::truncate: v: " << v << "clip: " << t << gsnl;
	GsVec ret;
	ret.x = v.x>t?t:v.x;
	ret.y = v.y>t?t:v.y;
	ret.z = v.z>t?t:v.z;
}

void RRTGraph::steering( GsVec g)
{
	gsout << "--------------------------" << gsnl;
	GsVec target_offset = g - robo_->pos;
	float distance = magnitude(target_offset);
	gsout << "Magnitude: " << distance << gsnl;
	float ramped_speed = robo_->max_speed * (distance/slowing_distance);
	float clipped_speed = std::min(ramped_speed, robo_->max_speed);
	gsout << "Clipped speed: " << clipped_speed << gsnl;
	GsVec desired_vel = (clipped_speed/distance)*target_offset;
	gsout << "Desired velocity: " << desired_vel << gsnl;
	robo_->steer_dir = desired_vel - robo_->vel;
	gsout << "RRTGraph::steering: " << robo_->steer_dir << gsnl;
	gsout << "----------------------------" << gsnl;
}
