#include <iostream>
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

static void changeGoal(SnManipulator* manip, const GsEvent&e, void* udata)
{
	MyViewer* v = (MyViewer*)udata;
	GsMat& m = manip->mat();
	float pnt[3];
	int update_ee;
	m.getrans(pnt[0], pnt[1], pnt[2]);
	GsPnt clicked_point(pnt);
	v->updateGoal(clicked_point);
}

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	rg_ = new RRTGraph;
	robo_ = new SnGroup;
	walls_ = new SnGroup;
	rootg()->add(robo_);
	rootg()->add(walls_);
	_nbut=0;
	_animating=false;
	add_ui ();
	build_scene ();
}

void MyViewer::add_ui ()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "View", sp=new UiPanel() ) );
	{	UiPanel* p=sp;
		p->add ( _nbut=new UiCheckButton ( "Normals", EvNormals ) ); 
	}
	p->add ( new UiButton ( "Animate", EvAnimate ) );
	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}

void MyViewer::add_model ( SnPrimitive* s, GsVec p ,int object)
{
	if(object == RRTGraph::WALL)
	{
		gsout << "Adding wall:" << gsnl;
		SnManipulator* manip = new SnManipulator;
		GsMat m;
		m.translation ( p );
		manip->initial_mat ( m );

		SnGroup* g = new SnGroup;
		g->add(s);
		manip->child(g);
		walls_->add(manip);
		rg_->addWall(GsPnt2(p.x+s->prim().ra, p.y+s->prim().rb), 
			GsPnt2(p.x-s->prim().ra, p.y+s->prim().rb),
			GsPnt2(p.x-s->prim().ra, p.y-s->prim().rb), 
			GsPnt2(p.x+s->prim().ra, p.y-s->prim().rb));
	}
	else if(object == RRTGraph::ROBOT)
	{
		gsout << "Before calling init" << gsnl;
		SnModel* sphere = new SnModel;
		sphere->model()->make_sphere(GsPnt::null, 5.0f, 20.0f, true);
		robo_->init();
		SnTransform *t;
		robo_->separator(true);
		robo_->add(t = new SnTransform );
		SnMaterial *m;
		robo_->add(m = new SnMaterial );
		robo_->add(sphere);
		position_ = &(t->get());
		t->get().translation(p);
		m->material().diffuse = GsColor::random();
		rg_->init(GsPnt2(p.x,p.y),5,-100,100,position_);
	}
	else if(object == RRTGraph::GOAL)
	{
		gsout << "Adding goal:" << gsnl;
		SnManipulator* manip = new SnManipulator;
		GsMat m;
		m.translation( p );
		manip->initial_mat(m);
		SnGroup* g = new SnGroup;
		g->add(s);
		manip->child(g);
		manip->callback(changeGoal, this);
		rootg()->add(manip);
	}
}

void MyViewer::updateGoal(GsPnt a)
{
	rg_->setGoal(a);
}

void MyViewer::build_scene()
{
	// SnGroup to hold the gear with sharp teeth
	gsout << "MyViewer::build_scene" << gsnl;
	SnPrimitive *p;
	GsVec c1(100,0,0);
	p = new SnPrimitive(GsPrimitive::Box, 2, 50, 5);
	p->prim().material.diffuse = GsColor::blue;
	add_model(p, GsVec(100,0,0),RRTGraph::WALL);

        p = new SnPrimitive(GsPrimitive::Box, 50, 2, 5);
        p->prim().material.diffuse = GsColor::blue;
        add_model(p, GsVec(0,100,0),RRTGraph::WALL);


        p = new SnPrimitive(GsPrimitive::Box, 2, 50, 5);
        p->prim().material.diffuse = GsColor::blue;
        add_model(p, GsVec(-100,0,0),RRTGraph::WALL);

        p = new SnPrimitive(GsPrimitive::Box, 50, 2, 5);
        p->prim().material.diffuse = GsColor::blue;
        add_model(p, GsVec(0,-100,0),RRTGraph::WALL);

	p = new SnPrimitive(GsPrimitive::Box, 2, 2, 2);
	p->prim().material.diffuse = GsColor::green;
	add_model(p, GsVec(35.0f, 35.0f, 0.0f), RRTGraph::GOAL);

	p = new SnPrimitive(GsPrimitive::Sphere, 5);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(10,10,0),RRTGraph::ROBOT);

	gsout << "Added all the walls!" << gsnl;

	path_lines_ = new SnLines;
	goal_lines_ = new SnLines;
	rootg()->add(path_lines_);
	rootg()->add(goal_lines_);

	gsout << "Sent lines" << gsnl;
	//run_animation();
        /*p = new SnPrimitive(GsPrimitive::Box, 1, 2, 2);
        p->prim().material.diffuse = GsColor::blue;
        add_model(p, GsVec(4,0,0));

        p = new SnPrimitive(GsPrimitive::Box, 1, 2, 2);
        p->prim().material.diffuse = GsColor::blue;
        add_model(p, GsVec(5,0,0));i*/

	std::thread t(&RRTGraph::performRRT, rg_, path_lines_, goal_lines_);
	t.detach();
	std::thread t1(&RRTGraph::eulerIntegration, rg_);
	t1.detach();
	gsout << "++++++++HARSHA==============" << gsnl;

	
	//rg_->eulerIntegration(GsVec(-10.0f, -10.0f, 0.0f));
	//gsout << "9999999999999END999999999999" << gsnl;

	//rg_->run(path_lines_, goal_lines_);
}


// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{


	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
	double t = 0, lt = 0, t0 = gs_time();

	do // run for a while:
	{
		while (t - lt<frdt) t = gs_time() - t0; // wait until it is time for next frame
		lt = t;

		// Rotate the gear on z-axis 
		GsVec tr;
		//position_->setrans(rg_->getPosition());


		// This is to create the pendulum action of the belt.
		//_t3->get().translation(-2.45f+_curdisp, -1.75f, 0);
		//_curdisp += dir*0.05f; // delta displacement along with direction

		// Change the direction of the rotation/translation every 20th delta time
		//std::thread t(&RRTGraph::performRRT, rg_, path_lines_, goal_lines_);
        	//t.detach();
        	//gsout << "Starting the thread" << gsnl;
                //std::thread t1(&RRTGraph::eulerIntegration, rg_, GsVec(99.0f, 99.0f, 0.0f));
                //t1.detach();
                //gsout << "++++++++HARSHA==============" << gsnl;

		render(); // notify it needs redraw
		ws_check(); // redraw now
	} while (true);// m.e24 > 0 );
	_animating = false;
}

void MyViewer::show_normals ( bool b )
{
	// Note that primitives are only converted to meshes in GsModel
	// at the first draw call.


	GsArray<GsVec> fn;
	SnGroup* r = (SnGroup*)root();
	for ( int k=0; k<r->size(); k++ )
	{	SnManipulator* manip = r->get<SnManipulator>(k);
		SnShape* s = manip->child<SnGroup>()->get<SnShape>(0);
		SnLines* l = manip->child<SnGroup>()->get<SnLines>(1);
		if ( !b ) { l->visible(false); continue; }
		l->visible ( true );
		if ( !l->empty() ) continue; // build only once
		l->init();
		if ( s->instance_name()==SnPrimitive::class_name )
		{	GsModel& m = *((SnModel*)s)->model();
			m.get_normals_per_face ( fn );
			const GsVec* n = fn.pt();
			float f = 0.33f;
			for ( int i=0; i<m.F.size(); i++ )
			{	const GsVec& a=m.V[m.F[i].a]; l->push ( a, a+(*n++)*f );
				const GsVec& b=m.V[m.F[i].b]; l->push ( b, b+(*n++)*f );
				const GsVec& c=m.V[m.F[i].c]; l->push ( c, c+(*n++)*f );
			}
		}  
	}
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
		case 'n' : { bool b=!_nbut->value(); _nbut->value(b); show_normals(b); return 1; }
		default: gsout<<"Key pressed: "<<e.key<<gsnl;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvNormals: show_normals(_nbut->value()); return 1;
		case EvAnimate: run_animation(); return 1;
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
