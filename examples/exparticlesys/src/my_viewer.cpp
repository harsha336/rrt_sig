
# include "my_viewer.h"

# include <sig/sn_model.h>
# include <sig/sn_material.h>
# include <sig/sn_transform.h>

# include <sigogl/ws_run.h>

# define CHKVAR(s) if(_vars->search(s)<0) { fltk::alert("Missing parameter [%s]!",s); exit(0); }

MyViewer::MyViewer ( int x, int y, int w, int h, const char *l ) : WsViewer ( x, y, w, h, l )
{
	_root = WsViewer::rootg();

	_points = new SnPoints;
	_spheres = new SnGroup;
	path_lines_ = new SnLines;
	goal_lines_ = new SnLines;

	view ( false, true, true ); // lines, world, spheres

	_root->add(_points);
	_root->add(_spheres);
	_root->add(path_lines_);
	_root->add(goal_lines_);
  
	_sphereradius = 1.0f;

	rg_ = new RRTGraph;
	walls_ = new SnGroup;
	_root->add(walls_);
}

MyViewer::~MyViewer ()
{
}

enum Events {	EvInit,
				EvViewSpheres, EvViewVelVecs,
				EvRun, EvWind, EvBounce,
				EvSCollision, EvVCollision, EvTScale, EvCRestitut,
				EvView, EvExit
			};

void MyViewer::build_ui ()
{
	UiPanel *p, *sp, *paramsp;
	UiManager* uim = WsWindow::uim();
	bool detachparms = true;

	// Top Menu Bar:
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( new UiButton ( "Init", EvInit ) );
	if (!detachparms) p->add ( new UiButton ( "Parameters", sp=new UiPanel() ) ); paramsp=sp;
	p->add ( new UiButton ( "View", sp=new UiPanel() ) );
	{	UiPanel* p=sp;
		p->add ( _vvel = new UiCheckButton ( "Velocities", EvView, false ) ); p->top()->separate();
		p->add ( _vworld = new UiCheckButton ( "World", EvView, true ) ); 
		p->add ( _vsphere = new UiCheckButton ( "Spheres", EvView, true ) ); 
	}
	p->add ( new UiButton ( "Exit", EvExit ) );

	if (!detachparms) p = paramsp; // build parameters panel now
	else p = uim->add_panel ( "Parameters", UiPanel::Vertical, UiPanel::Left, 0, 30 );

	p->add ( _run = new UiCheckButton ( "Run", EvRun ) );
	p->add ( _wind = new UiCheckButton ( "Wind", EvWind ) ); 
	p->add ( _scol = new UiCheckButton ( "Spring-Based Collision", EvSCollision ) );
	p->add ( _vcol = new UiCheckButton ( "Vel. Reflection Collision", EvVCollision ) );
	p->add ( _bounce = new UiCheckButton ( "Bounce", EvBounce ) ); _bounce->value(true);

	p->add ( new UiElement("Wind Force:") ); p->top()->separate();
	p->add ( _windslider = new UiSlider("", EvWind) );
	_windslider->range(-5.0f,5.0f);

	p->add ( new UiElement("Time Scale:") ); 
	p->add ( _tscaleslider = new UiSlider("", EvTScale) );
	_tscaleslider->range(0.01f,4.0f); _tscaleslider->value(1.0f);

	p->add ( new UiElement("Coeff. of Restitution:") ); 
	p->add ( _crestslider = new UiSlider("", EvCRestitut) );
}

void MyViewer::view ( bool vel, bool world, bool spheres )
{
	_spheres->visible ( spheres );
	_points->visible ( !spheres );
	path_lines_->visible(true);
	goal_lines_->visible(true);
	walls_->visible(true);
}

void MyViewer::build_scene ( ParticleSystem& psys, int nfaces )
{
	const GsArray<Particle>& p = psys.particles();
	rg_->init(GsPnt2(p[0].x.x,p[0].x.y), GsPnt2(-80,5),5,-100,100);
	int i, psize=p.size();
	_psys = &psys;
	_nfaces = nfaces;

	_crestslider->value ( _psys->coeff_restitution() );

	SnModel* sphere = new SnModel;
	sphere->model()->make_sphere ( GsPnt::null, _sphereradius, nfaces, true );

	SnGroup* g;
	SnTransform* t;
	SnMaterial* m;

	_spheres->init();
	_positions.size(0); // fast access to later update positions
	for ( i=0; i<psize; i++ )
	{	_spheres->add ( g=new SnGroup );
		g->separator(true); // transformations inside group do not affect outside group
		g->add ( t=new SnTransform );
		g->add ( m=new SnMaterial );
		g->add ( sphere );
		_positions.push() = &(t->get());
		t->get().translation ( p[i].x );
		m->material().diffuse = GsColor::random();
    }

	_points->init();
	_points->point_size ( 3.0f );
	_points->color ( GsColor::blue );
	for ( i=0; i<psize; i++ )
	{	_points->push ( p[i].x );
	}

	SnPrimitive *pri;
	GsVec c1(100,0,0);
	pri = new SnPrimitive(GsPrimitive::Box, 2, 50, 5);
        pri->prim().material.diffuse = GsColor::blue;
        add_model(pri, GsVec(100,0,0),RRTGraph::WALL);

        pri = new SnPrimitive(GsPrimitive::Box, 50, 2, 5);
        pri->prim().material.diffuse = GsColor::blue;
        add_model(pri, GsVec(0,100,0),RRTGraph::WALL);


        pri = new SnPrimitive(GsPrimitive::Box, 2, 50, 5);
        pri->prim().material.diffuse = GsColor::blue;
        add_model(pri, GsVec(-100,0,0),RRTGraph::WALL);

        pri = new SnPrimitive(GsPrimitive::Box, 50, 2, 5);
        pri->prim().material.diffuse = GsColor::blue;
        add_model(pri, GsVec(0,-100,0),RRTGraph::WALL);
}

void MyViewer::add_model ( SnPrimitive* s, GsVec p ,int object)
{
        SnManipulator* manip = new SnManipulator;
        GsMat m;
        m.translation ( p );
        manip->initial_mat ( m );

        SnGroup* g = new SnGroup;
        g->add(s);
        manip->child(g);
        walls_->add(manip);
        if(object == RRTGraph::WALL)
        {
                rg_->addWall(GsPnt2(p.x+s->prim().ra, p.y+s->prim().rb),
                        GsPnt2(p.x-s->prim().ra, p.y+s->prim().rb),
                        GsPnt2(p.x-s->prim().ra, p.y-s->prim().rb),
                        GsPnt2(p.x+s->prim().ra, p.y-s->prim().rb));
        }
}

void MyViewer::buildPath()
{
	std::thread t(&RRTGraph::performRRT, rg_, path_lines_, goal_lines_);
	t.detach();
}

void MyViewer::update_scene ()
{
	gsout << "MyViewer::update_scene: Entered" << gsnl;
	const GsArray<Particle>& p = _psys->particles();
	int i, psize=p.size();

	if ( _spheres->visible() )
	{	for ( i=0; i<psize; i++ )
		{	_positions[i]->setrans ( p[i].x );
		}
	}
	else
    {	for ( i=0; i<psize; i++ )
		{	_points->P[i] = p[i].x;
			_points->touch();
		}
	}

	buildPath();
}

void MyViewer::run ()
{
	if ( !_run->value() ) return; // turning sim off
	gsout << "Coming here!" <<gsnl;

	double tfac = 1.0; //ui_timeslider->value();
	double t0, tprior, tnow;

	t0 = tprior = gs_time()*tfac;
	while ( _run->value() )
	{
		// draw current state:
		update_scene ();
		ws_check();

		// check time scale:
		if ( tfac!=_tscaleslider->value() )
		{	t0 /= tfac;
			tprior /= tfac;
			tfac=_tscaleslider->value();
			t0 *= tfac;
			tprior *= tfac;
		}

		// compute next step:
		tnow = gs_time () * tfac;
		_psys->euler_step ( float(tnow-tprior) );
		tprior = tnow;

		// display some info:
		message().setf ( "run: %5.2f s", float(tnow-t0) );
	}
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvInit:
		{	_psys->init ( _psys->last_cfg_file() );
			build_scene ( *_psys, _nfaces );
		} break;

		case EvWind:
		{	float windmag = _windslider->value();
			if ( !_wind->value() ) windmag=0;
			_psys->external_force ( GsVec( windmag, 0, 0 ) );
		} break;

		case EvBounce:
		{	_psys->bounce ( _bounce->value() );
		} break;

		case EvSCollision:
		{	_psys->spring_collision_prevention ( _scol->value() );
		} break;

		case EvVCollision:
		{	_psys->velocity_collision_prevention ( _vcol->value() );
		} break;

		case EvCRestitut:
		{	_psys->coeff_restitution ( _crestslider->value() );
		} break;

		case EvView:
		{	view ( _vvel->value(), _vworld->value(), _vsphere->value() );
		} break;

		case EvRun: run(); break;

		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}

int MyViewer::handle_scene_event ( const GsEvent &e )
{
	if ( e.button1 )
	{	// nothing here for now
	}

	return WsViewer::handle_scene_event ( e );
}
