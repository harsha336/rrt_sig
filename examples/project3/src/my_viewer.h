# pragma once
#include <thread>

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>
#include <sig/sn_primitive.h>
#include <sig/sn_material.h>

# include <sigogl/ui_button.h>
# include <sigogl/ws_viewer.h>

#include "rrt.h"

using namespace std;
// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvNormals, EvAnimate, EvExit };
	UiCheckButton* _nbut;
	bool _animating;
	RRTGraph *rg_;
	GsMat *position_;
   public :
	   SnTransform *_t1,*_t2,*_t3,*_t4,*_t5;
	   SnGroup *walls_, *robo_;
	   SnLines *path_lines_, *goal_lines_;
	MyViewer ( int x, int y, int w, int h, const char* l );
	void add_ui ();
	void add_model ( SnPrimitive* s, GsVec p, int object);
	void build_scene ();
	void make_traz_teeth_gear(SnGroup *g1, GsPnt c, double tw, double radius, double tn, GsColor color);
	void make_sharp_teeth_gear(SnGroup *g1, GsPnt c, double tw, double radius, double tn, GsColor color);
	void make_square_gear(SnGroup *g1);
	void show_normals ( bool b );
	void run_animation ();
	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
	void updateGoal(GsPnt a);
	//void gear1( class GsModel* m, const GsPnt& c, float radius);
};

