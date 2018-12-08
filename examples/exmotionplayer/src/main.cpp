
# include "my_viewer.h"

# include <sig/gs_euler.h>
# include <sigogl/ws_run.h>

int main ( int argc, char** argv )
{
	MyViewer* v = new MyViewer ( -1, -1, 800, 600, "Example Move Graph App" );
	//v->cmd ( WsViewer::VCmdAxis );
	v->view_all ();
	v->show ();

	ws_run ();

	return 1;
}
