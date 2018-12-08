
# include "my_viewer.h"

# include <sigogl/ui_dialogs.h>
# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	// 1. Load skeleton definition:
	_sk = new KnSkeleton();
	if ( !_sk->load ( "../data/dudetube.s" ) ) gsout.fatal("Error loading skeleton!");
	KnJoint* rootj = _sk->joint("Hips");
	if ( !rootj ) gsout.fatal("Could not find root joint!");

	// 2. Load a motion:
	_motion = new KnMotion;
	GsString filename = "../data/dudewalkaround.bvh";
	if ( !_motion->load ( filename ) ) gsout.fatal("Error loading [%s]!",filename);
	_motion->connect ( _sk );

	// 3. Build UI after motion is loaded:
	build_ui ();

	// 4. Now build scene:
	bool addfloor = true;
	build_scene ( addfloor );
}

void MyViewer::build_ui ()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add(_tislider=new UiSlider ("ti=",EvTimeSlider));
	p->add(_tfslider=new UiSlider ( "tf=", EvTimeSlider ) );
	p->add ( _playbut=new UiCheckButton ( "Play", EvPlay ) );

	_tislider->range ( 0, _motion->duration() );
	_tfslider->range ( 0, _motion->duration() );
	_tfslider->value ( _motion->duration() );

	if ( true ) // set this to false when the slider below is no longer needed
	{	p->add ( _frslider=new UiSlider ( "frame:", EvFrameSlider ) ); 
		p->top()->separate();
		_frslider->minw(400);
		_frslider->range ( 0, float(_motion->frames()-1) );
		_frslider->increment ( 1.0f );
		_frslider->format ( 3, 0 );
		_frslider->all_events(1);
	}

	p->add ( new UiButton ( "Exit", EvExit ) );
	p->top()->separate();
}

void MyViewer::play ()
{
	float ti = _tislider->value();
	float tf = _tfslider->value();
	float duration = tf-ti;

	if ( ti>tf ) { ui_message("Wrong Parameters!"); _playbut->value ( false ); return; }

	double mt0 = gs_time(); // start time

	while ( _playbut->value() )
	{	// Compute local time, which is from 0 to the duration of the motion:
		float localt = ti+float(gs_time()-mt0);

		// When local time goes beyond the end we stop:
		if ( localt>tf ) break; // past end of current motion

		// Now apply motion at the current local time, update scene, and draw:
		_motion->apply ( localt );
		_kns->update();
		message().setf ( "duration=%4.2f   local t=%4.2f   interpolated frames:%d;%d",
						 duration, localt, _motion->last_applied_frame(), _motion->last_applied_frame()+1 );
		render();
		ws_check(); // this updates window and interface (otherwise application will be unresponsive)
	}
	_playbut->value ( false );
}

void MyViewer::build_scene ( bool addfloor )
{
	if ( addfloor )
	{	// Let's add a ground floor:
		GsPrimitive floor;
		floor.box(400.0f,1.0f,400.0f); floor.center.y=-1.0f;
		floor.material.diffuse=GsColor::darkgreen;
		SnModel* ground = new SnModel;
		ground->model()->make_primitive ( floor );
		rootg()->add ( ground );
	}

	// Create a skeleton scene node:
	_kns = new KnScene;
	_kns->connect ( _sk );
	_kns->set_visibility ( 0, 0, 1, 0, 0 );
	rootg()->add ( _kns );
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	//gsout<<"Key pressed: "<<e.key<<gsnl;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvPlay: if ( _playbut->value() ) play(); break;
		case EvTimeSlider: break;
		case EvFrameSlider:
			{	int f = (int)_frslider->value();
				_motion->apply_frame ( f );
				_kns->update();
				message().setf ( "keyframe=%d keytime=%4.2f", f, _motion->keytime(f) );
				render();
			} break;

		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
