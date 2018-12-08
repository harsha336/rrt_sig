# pragma once

# include <sig/sn_poly_editor.h>
# include <sig/sn_lines2.h>

# include <sigogl/ui_check_button.h>
# include <sigogl/ui_slider.h>
# include <sigogl/ws_viewer.h>

# include <sigkin/kn_skeleton.h>
# include <sigkin/kn_motion.h>
# include <sigkin/kn_scene.h>

// Viewer for this example:
class MyViewer : public WsViewer
{  protected :
	enum MenuEv { EvPlay, EvTimeSlider, EvFrameSlider, EvExit };
	KnScene* _kns;
	UiCheckButton* _playbut;
	UiSlider* _tislider;
	UiSlider* _tfslider;
	UiSlider* _frslider;
	// skeleton data:
	KnSkeleton* _sk;
	KnJoint* _rootj;
	KnMotion* _motion;

   public:
	MyViewer ( int x, int y, int w, int h, const char* l );
	void build_scene ( bool addfloor );
	KnScene* sk_scene() { return _kns; }
	void play ();

   protected:
	void build_ui ();
	virtual int handle_keyboard ( const GsEvent &e ) override;
	virtual int uievent ( int e ) override;
};

