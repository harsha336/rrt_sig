
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

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
	else if(object == RRTGraph::ROBOT)
	{
		rg_->init(GsPnt2(p.x,p.y), GsPnt2(p.x+35, p.y+35),1.5,w(),h());
	}
}

void MyViewer::build_scene()
{
	// SnGroup to hold the gear with sharp teeth
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

	p = new SnPrimitive(GsPrimitive::Sphere, 5);
	p->prim().material.diffuse = GsColor::red;
	add_model(p, GsVec(10,10,0),RRTGraph::ROBOT);

        /*p = new SnPrimitive(GsPrimitive::Box, 1, 2, 2);
        p->prim().material.diffuse = GsColor::blue;
        add_model(p, GsVec(4,0,0));

        p = new SnPrimitive(GsPrimitive::Box, 1, 2, 2);
        p->prim().material.diffuse = GsColor::blue;
        add_model(p, GsVec(5,0,0));i*/

	rg_->performRRT();
}

// Method to create a gear with trapezium teeth
/*void MyViewer::make_sharp_teeth_gear(SnGroup *g1,GsPnt c, double tw, double radius, double tn, GsColor color) {
	int count_faces = 0;
	for (int i = 1; i <= 48; i++) {

	
	SnModel *_model;
	_model = new SnModel;

	GsModel* m = _model->model();

	double pi = 3.14;
	
	// Add the main circular body of the gear
	m->V.size(6);
	m->V[0].set(c.x, c.y, c.z);
	m->V[1].set((double)c.x+radius*cos((i-1)*(pi / 12)), (double)c.y+radius*sin((i-1)*(pi / 12)), (double)c.z);
	m->V[2].set((double)c.x+radius*cos(i*(pi / 12)), (double)c.y+radius*sin(i*(pi / 12)), (double)c.z);
	m->V[3].set((double)c.x, (double)c.y, (double)c.z+tn);
	m->V[4].set(c.x+radius*cos(i*(pi / 12)), c.y+radius*sin(i*(pi / 12)), c.z+tn);
	m->V[5].set(c.x+radius*cos((i - 1)*(pi / 12)), c.y+radius*sin((i - 1)*(pi / 12)), c.z+tn);

	m->F.push().set(0, 1, 2);
	m->F.push().set(0, 2, 1);
	m->F.push().set(3, 4, 5);
	m->F.push().set(3, 5, 4);
	count_faces += 4;
	GsMaterial mtl;
	mtl.diffuse = color;
	m->set_one_material(mtl);

	// Add the sharp teeth of the gear.
	if (i % 2) {
		SnModel *teeth3;
		teeth3 = new SnModel;
		GsModel* m1 = teeth3->model();
		m1->V.size(6);
		m1->V[0].set((double)c.x+radius*cos((i-1)*(pi / 12)), (double)c.y+(radius)*sin((i-1)*(pi / 12)), (double)c.z);
		m1->V[1].set(c.x+(radius+tw)*cos(i*(pi / 12)), (double)c.y+(radius+tw)*sin(i*(pi / 12)), (double)c.z);
		m1->V[2].set((double)c.x+radius*cos(i*(pi / 12)), (double)c.y+radius*sin(i*(pi / 12)), (double)c.z);
		m1->V[3].set(c.x+(radius + tw)*cos(i*(pi / 12)), c.y+(radius + tw)*sin(i*(pi / 12)), c.z+tn);
		m1->V[4].set(c.x+radius*cos((i - 1)*(pi / 12)), c.y+radius*sin((i - 1)*(pi / 12)), c.z+tn);
		m1->V[5].set(c.x+radius*cos(i*(pi / 12)), c.y+radius*sin(i*(pi / 12)), c.z+tn);
		m1->F.push().set(0, 1, 2);
		m1->F.push().set(0, 2, 1);
		m1->F.push().set(3,4,5);
		m1->F.push().set(3, 5, 4);
		m1->F.push().set(0, 1, 3);
		m1->F.push().set(0, 3, 1);
		m1->F.push().set(0, 3, 4);
		m1->F.push().set(0, 4, 3);
		count_faces += 8;
		GsMaterial mtl1;
		mtl1.diffuse = color;
		m1->set_one_material(mtl1);
		g1->add(teeth3);
	}
	else {

		SnModel *teeth4;
		teeth4 = new SnModel;
		GsModel* m2 = teeth4->model();
		m2->V.size(6);
		m2->V[0].set((double)c.x + (radius + tw)*cos((i - 1)*(pi / 12)), (double)c.y + (radius + tw)*sin((i - 1)*(pi / 12)), (double)c.z);
		m2->V[1].set((double)c.x + radius*cos(i*(pi / 12)), (double)c.y + radius*sin(i*(pi / 12)), (double)c.z);
		m2->V[2].set((double)c.x + radius*cos((i - 1)*(pi / 12)), (double)c.y + radius*sin((i - 1)*(pi / 12)), (double)c.z);
		m2->V[3].set(c.x + radius*cos((i - 1)*(pi / 12)), c.y + radius*sin((i - 1)*(pi / 12)), c.z + tn);
		m2->V[4].set(c.x + radius*cos(i*(pi / 12)), c.y + radius*sin(i*(pi / 12)), c.z + tn);
		m2->V[5].set(c.x + (radius + tw)*cos((i - 1)*(pi / 12)), c.y + (radius + tw)*sin((i - 1)*(pi / 12)), c.z + tn);
		m2->F.push().set(0, 1, 2);
		m2->F.push().set(0, 2, 1);
		m2->F.push().set(3, 4, 5);
		m2->F.push().set(3, 5, 4);
		m2->F.push().set(1, 4, 5);
		m2->F.push().set(0, 5, 4);
		m2->F.push().set(0, 1, 5);
		m2->F.push().set(0, 5, 1);
		count_faces += 8;
		GsMaterial mtl2;
		mtl2.diffuse = color;
		m2->set_one_material(mtl2);
		g1->add(teeth4);
	}
	g1->add(_model);
	}
	gsout << "No of faces:" << count_faces << gsnl;
}*/


// Below is an example of how to control the main loop of an animation:
void MyViewer::run_animation()
{
	if (_animating) return; // avoid recursive calls
	_animating = true;

	int ind = gs_random(0, rootg()->size() - 1); // pick one child
	SnManipulator* manip = rootg()->get<SnManipulator>(ind); // access one of the manipulators
	GsMat m = manip->mat();

	float _curang1 = 0;
	float _curang2 = 0;
	float _curang3 = 0;
	float _curang4 = 0;
	float _curdisp = 0;
	int dir = 1;
	int ud_flag = 0;

	double frdt = 1.0 / 30.0; // delta time to reach given number of frames per second
	double v = 4; // target velocity is 1 unit per second
	double t = 0, lt = 0, t0 = gs_time();
	do // run for a while:
	{
		while (t - lt<frdt) t = gs_time() - t0; // wait until it is time for next frame
		lt = t;

		// Rotate the gear on z-axis 
		GsVec tr;
		_t1->get().getrans(tr);
		_t1->get().rotz(GS_TODEG(_curang1));
		_t1->get().setrans(tr);
		_curang1 += dir*0.0005f; // delta angle along with direction

		_t2->get().getrans(tr);
		_t2->get().rotz(GS_TODEG(_curang2));
		_t2->get().setrans(tr);
		_curang2 += dir*0.0005f; // delta angle along with direction

		_t4->get().getrans(tr);
		_t4->get().rotz(GS_TODEG(_curang3));
		_t4->get().setrans(tr);
		_curang3 -= dir*0.0005f; // delta angle along with direction

		_t5->get().getrans(tr);
		_t5->get().rotz(GS_TODEG(_curang3));
		_t5->get().setrans(tr);
		_curang4 -= dir*0.0005f;

		// This is to create the pendulum action of the belt.
		_t3->get().translation(-2.45f+_curdisp, -1.75f, 0);
		_curdisp += dir*0.05f; // delta displacement along with direction

		// Change the direction of the rotation/translation every 20th delta time
		ud_flag += 1;
		if (!(ud_flag % 20)) {
			dir = -(dir);
		}

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
