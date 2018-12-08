
# include "move_graph.h"

MoveGraph::MoveGraph ()
{
}

static KnMotion* load_motion ( KnSkeleton* sk, const char* filename )
{
	KnMotion* m = new KnMotion;
	if ( !m->load ( filename ) ) gsout.fatal("Error loading [%s]!",filename);
	m->connect ( sk );
	m->ref ();
	return m;
}

void MoveGraph::init ( KnSkeleton* sk )
{
	nodes.push()->motion = load_motion ( sk, "../data/idle_stand.bvh" ); // 0
	nodes.push()->motion = load_motion ( sk, "../data/stand2right_step.bvh" ); // 1
	nodes.push()->motion = load_motion ( sk, "../data/right_step.bvh" ); // 2
	nodes.push()->motion = load_motion ( sk, "../data/left_step.bvh" ); // 3
	nodes.push()->motion = load_motion ( sk, "../data/right_step2stand.bvh" ); // 4
	nodes.push()->motion = load_motion ( sk, "../data/turn180.bvh" ); // 5

	Node* n;
	Transition* t;

	// motion node 0:
	n=nodes[0]; n->nextnode=0;
	n->anytimetransitions=true;
	t=&n->trans.push(); t->key='w'; t->nextnode=1;

	// motion node 1:
	n=nodes[1]; n->nextnode=2;
	n->anytimetransitions=false;

	// motion node 2:
	n=nodes[2]; n->nextnode=3;
	n->anytimetransitions=false;
	t=&n->trans.push(); t->key='t'; t->nextnode=5;

	// motion node 3:
	n=nodes[3]; n->nextnode=2;
	n->anytimetransitions=false;
	t=&n->trans.push(); t->key='s'; t->nextnode=4;

	// motion node 4:
	n=nodes[4]; n->nextnode=0;
	n->anytimetransitions=false;

	// motion node 5:
	n=nodes[5]; n->nextnode=3;
	n->anytimetransitions=false;
}
