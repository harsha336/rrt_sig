#include "my_viewer.h"
#include <sigogl/ws_run.h>

int main( int argc, char** argv)
{
	gsout<<"Nothing to do yet!"<<gsnl;
	MyViewer* v = new MyViewer ( -1, -1, 640, 480, "My SIG Application" );
	v->view_all ();
	v->show ();
	ws_run();
	return 1;
}