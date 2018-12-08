
# include "my_viewer.h"

# include <sigogl/ui_button.h>
# include <sigogl/ui_radio_button.h>
# include <sig/sn_primitive.h>
# include <sig/sn_transform.h>
# include <sig/sn_manipulator.h>

# include <sigogl/ws_run.h>

MyViewer::MyViewer ( int x, int y, int w, int h, const char* l ) : WsViewer(x,y,w,h,l)
{
	_blendbut=_followbut=0;
	_animating=false;
	_lastkey=0;
	build_ui ();
}

void MyViewer::build_ui ()
{
	UiPanel *p, *sp;
	UiManager* uim = WsWindow::uim();
	p = uim->add_panel ( "", UiPanel::HorizLeft );
	p->add ( _blendbut=new UiCheckButton ( "Blend", EvBlend ) );
	p->add ( _followbut=new UiCheckButton ( "Follow", EvFollow ) );
	p->add ( new UiButton ( "Exit", EvExit ) ); p->top()->separate();
}

void MyViewer::build_scene ( KnSkeleton* sk, bool addfloor )
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
	_kns->connect ( sk );
	_kns->set_visibility ( 0, 0, 1, 0, 0 );
	rootg()->add ( _kns );
}

int MyViewer::handle_keyboard ( const GsEvent &e )
{
	int ret = WsViewer::handle_keyboard ( e ); // 1st let system check events
	if ( ret ) return ret;

	_lastkey = e.key;
	//gsout<<"Key pressed: "<<e.key<<gsnl;

	switch ( e.key )
	{	case GsEvent::KeyEsc : gs_exit(); return 1;
	}

	return 0;
}

int MyViewer::uievent ( int e )
{
	switch ( e )
	{	case EvBlend: break;
		case EvFollow: break;
		case EvExit: gs_exit();
	}
	return WsViewer::uievent(e);
}
