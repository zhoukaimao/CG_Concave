#include "Brep.h"
int Brep::num_solid = 0;
int Brep::num_face = 0;
int Brep::num_halfedge = 0;
int Brep::num_loop = 0;
int Brep::num_vertex = 0;
int Brep::num_edge = 0;
void Brep::mvfs(vec3d position, Solid* & o_solid, Vertex* & o_vertex){
	Solid* s = new Solid();
	Face* f = new Face();
	Loop* lp = new Loop();
	s->sface = f;
	f->fsolid = s;
	f->floop = lp;
	lp->lface = f;

	Vertex* v = new Vertex(position);

	o_solid = s;
	o_vertex = v;
}
void Brep::mev(vec3d position,Vertex* v1, Loop* lp, HalfEdge* & o_he){
	Vertex* v2 = new Vertex(position);
	HalfEdge *he1, *he2, *he;
	Edge* edge = new Edge();
	he1 = new HalfEdge();
	he2 = new HalfEdge();

	edge->he1 = he1;
	edge->he2 = he2;
	he1->edge = he2->edge = edge;
	he1->startv = v1;
	he2->startv = v2;
	he1->next = he2;
	he2->prev = he1;
	he1->hloop = he2->hloop = lp;
	if (lp->ledge == NULL){
		//if lp is empty, link he1 and he2 as a loop
		he2->next = he1;
		he1->prev = he2;
		lp->ledge = he1;
	}
	else{
		//else, insert he1 and he2 into loop's halfEdge chain
		he = lp->ledge;
		while (he->startv->nequal(v1))//find he with v1 as startv
		{
			he = he->next;
		}
		he->prev->next = he1;
		he1->prev = he->prev;
		he2->next = he;
		he->prev = he2;
	}

	o_he = he2;
}
void Brep::mef(Vertex* v1, Vertex* v2, Loop* lp1, Loop* & o_lp2,bool mark){
	Face* f = new Face();
	Loop* lp2 = new Loop();

	f->fsolid = lp1->lface->fsolid;
	f->nextf = f->fsolid->sface;
	f->fsolid->sface = f;
	f->floop = lp2;

	lp2->lface = f;

	Edge* edge = new Edge();
	HalfEdge* he1 = new HalfEdge();
	HalfEdge* he2 = new HalfEdge();
	edge->he1 = he1;
	edge->he2 = he2;
	he1->edge = he2->edge = edge;
	he1->startv = v1;
	he2->startv = v2;
	
	HalfEdge* he_v1 = lp1->ledge;
	HalfEdge* he_v2 = lp1->ledge;
	while (he_v1->startv->nequal(v1)){ he_v1 = he_v1->next; }//find halfedge with v1 as startv
	while (he_v2->startv->nequal(v2)){ he_v2 = he_v2->next; }//find halfEdge width v2 as startv

	//then insert he1 and he2 into he chains
	he1->next = he_v2;
	he1->prev = he_v1->prev;
	he2->next = he_v1;
	he2->prev = he_v2->prev;
	he_v1->prev->next = he1;
	he_v1->prev = he2;
	he_v2->prev->next = he2;
	he_v2->prev = he1;

	lp1->ledge = he1;
	lp2->ledge = he2;
	HalfEdge* he = he1->next;
	while (he->startv->nequal( he1->startv)){
		he->hloop = lp1;
		he = he->next;
	}
	he = he2->next;
	while (he->startv->nequal(he2->startv)){
		he->hloop = lp2;
		he = he->next;
	}

	o_lp2 = lp2;

}
void Brep::kemr(Edge* edge,Loop* lp){
	HalfEdge* he1 = edge->he1;
	HalfEdge* he2 = edge->he2;
	Loop* lp2 = new Loop();
	lp2->nextl = lp->lface->floop;
	lp->lface->floop = lp2;
	HalfEdge* he = he1;
	while (he->next->nequal(he2))
	{
		he = he->next;
		he->hloop = lp2;
	}
	he->next = he1->next;
	he1->next->prev = he;
	lp2->ledge = he1->next;

	he = he2;
	while (he->next->nequal(he1)){ he = he->next; }
	he->next = he2->next;
	he2->next->prev = he;
	lp->ledge = he2->next;

	delete he1, he2;
	delete edge;
}
void Brep::kfmrh(Face* f1, Face* f2){
	f2->floop->nextl = f1->floop;
	f1->floop = f2->floop;
	//delete f2 from solid
	Solid* s = f1->fsolid;
	if (f2->equal(s->sface)){//f2 is the first face of solid
		s->sface = f2->nextf;
	}else{
		Face* f = s->sface;
		while (!f->nextf->equal(f2)){ f = f->nextf; }//find prev face of f2
		f->nextf = f2->nextf;
	}
	delete f2;
}
Brep::Solid* Brep::sweep(Face* f, vec3d dir){
	vector<Loop*> bottom;
	Solid* s = new Solid();
	Face* f1 = f->clone();
	f1->mirror();
	f->fsolid = f1->fsolid = s;
	s->sface = f;
	f->nextf = f1;
	Loop* lp = f1->floop;
	//cout << ">>Begin Sweep..." << endl;
	while (lp != NULL){
		Loop* newlp;
		HalfEdge* he = lp->ledge;
		Vertex *v, *startv, *new_startv, *new_endv;
		v = startv = he->startv;
		HalfEdge* newhe;
		mev(v->position + dir, v, lp, newhe);new_startv = newhe->startv;//create first new vertex
		Vertex* new_prevv = new_startv;
		he = he->next; v = he->startv;
		while (he->startv->nequal(startv)){
			mev(v->position + dir, v, lp, newhe);
			Loop* new_lp;
			mef(new_prevv, newhe->startv, lp, newlp);
			he = he->next;
			v = he->startv;
			new_prevv = newhe->startv;
		}
		mef(new_prevv, new_startv, lp, newlp);
		lp = lp->nextl;
	}
	return s;
}
Brep::Solid* Brep::cube(double width, double height, double depth){
	vec3d pos[8] = {
		vec3d(width / 2, height / 2, depth / 2),
		vec3d(width / 2, height / 2, -depth / 2),
		vec3d(-width / 2, height / 2, -depth / 2),
		vec3d(-width / 2, height / 2, depth / 2),
		vec3d(width / 2, -height / 2, depth / 2),
		vec3d(width / 2, -height / 2, -depth / 2),
		vec3d(-width / 2, -height / 2, -depth / 2),
		vec3d(-width / 2, -height / 2, depth / 2),
	};
	Vertex *v0, *v1, *v2, *v3, *v4, *v5, *v6, *v7;
	Loop *lp0, *lp1, *lp2, *lp3, *lp4, *lp5;
	Solid* s;	
	HalfEdge* he;
	mvfs(pos[0], s, v0);
	lp0 = s->sface->floop;
	mev(pos[1], v0, lp0, he); v1 = he->startv;
	mev(pos[2], v1, lp0, he); v2 = he->startv;
	mev(pos[3], v2, lp0, he); v3 = he->startv;
	mef(v3, v0, lp0, lp1);
	mev(pos[4], v0, lp1, he); v4 = he->startv;
	mev(pos[5], v1, lp1, he); v5 = he->startv;
	mev(pos[6], v2, lp1, he); v6 = he->startv;
	mev(pos[7], v3, lp1, he); v7 = he->startv;
	mef(v4, v5, lp1, lp2);
	mef(v5, v6, lp2, lp3);
	mef(v6, v7, lp3, lp4);
	mef(v7, v4, lp4, lp5);
	return s;

}
Brep::Solid* Brep::hole(double width, double height, double inner_width, double inner_height){
	vec3d pos[8] = {
		vec3d(width / 2, height / 2, 0),
		vec3d(-width / 2, height / 2, 0),
		vec3d(-width / 2, -height / 2, 0),
		vec3d(width / 2, -height / 2, 0),
		vec3d(inner_width / 2, inner_height / 2, 0),
		vec3d(-inner_width / 2, inner_height / 2, 0),
		vec3d(-inner_width / 2, -inner_height / 2, 0),
		vec3d(inner_width / 2, -inner_height / 2, 0)
	};
	Solid* s;
	Vertex *v0, *v1, *v2, *v3, *v4, *v5, *v6, *v7;
	Loop *lp0, *lp1, *lp2;
	mvfs(pos[0], s, v0);
	lp0 = s->sface->floop;
	HalfEdge* he; Edge* edge;
	mev(pos[1], v0, lp0, he); v1 = he->startv;
	mev(pos[2], he->startv, lp0, he); v2 = he->startv;
	mev(pos[3], he->startv, lp0, he); v3 = he->startv;
	mef(v0, he->startv, lp0, lp1);
	mev(pos[4], v0, lp0, he); v4 = he->startv; edge = he->edge;
	mev(pos[5], he->startv, lp0, he); v5 = he->startv;
	mev(pos[6], he->startv, lp0, he); v6 = he->startv;
	mev(pos[7], he->startv, lp0, he); v7 = he->startv;
	mef(v4, v7, lp0, lp2);
	kemr(edge,lp0);
	return s;
}
Brep::Solid* Brep::demo1(){
	double width = 2;
	double height = 2;
	double depth = 2;
	vec3d pos[8] = {
		vec3d(width / 2, height / 2, depth / 2),
		vec3d(width / 2, height / 2, -depth / 2),
		vec3d(-width / 2, height / 2, -depth / 2),
		vec3d(-width / 2, height / 2, depth / 2),
		vec3d(width / 2, -height / 2, depth / 2),
		vec3d(width / 2, -height / 2, -depth / 2),
		vec3d(-width / 2, -height / 2, -depth / 2),
		vec3d(-width / 2, -height / 2, depth / 2),
	};
	Vertex *v0, *v1, *v2, *v3, *v4, *v5, *v6, *v7;
	Loop *lp0, *lp1, *lp2, *lp3, *lp4, *lp5;
	Solid* s;
	HalfEdge* he;
	mvfs(pos[0], s, v0);
	lp0 = s->sface->floop;
	mev(pos[1], v0, lp0, he); v1 = he->startv;
	mev(pos[2], v1, lp0, he); v2 = he->startv;
	mev(pos[3], v2, lp0, he); v3 = he->startv;
	mef(v3, v0, lp0, lp1);
	return sweep(lp1->lface, vec3d(0, -height, 0));
	//mev(pos[4], v0, lp1, he); v4 = he->startv;
	//mev(pos[5], v1, lp1, he); v5 = he->startv;
	//mev(pos[6], v2, lp1, he); v6 = he->startv;
	//mev(pos[7], v3, lp1, he); v7 = he->startv;
	//mef(v4, v5, lp1, lp2);
	//mef(v5, v6, lp2, lp3);
	//mef(v6, v7, lp3, lp4);
	//mef(v7, v4, lp4, lp5);
	//return s;
}
Brep::Solid* Brep::demo2(){
	double width = 2,
		height = 2,
		inner_width = 1,
		inner_height = 1;
	vec3d pos[8] = {
		vec3d(width / 2, height / 2, 0),
		vec3d(-width / 2, height / 2, 0),
		vec3d(-width / 2, -height / 2, 0),
		vec3d(width / 2, -height / 2, 0),
		vec3d(inner_width / 2, inner_height / 2, 0),
		vec3d(-inner_width / 2, inner_height / 2, 0),
		vec3d(-inner_width / 2, -inner_height / 2, 0),
		vec3d(inner_width / 2, -inner_height / 2, 0)
	};
	Solid* s;
	Vertex *v0, *v1, *v2, *v3, *v4, *v5, *v6, *v7;
	Loop *lp0, *lp1, *lp2;
	mvfs(pos[0], s, v0);
	lp0 = s->sface->floop;
	HalfEdge* he; Edge* edge;
	mev(pos[1], v0, lp0, he); v1 = he->startv;
	mev(pos[2], he->startv, lp0, he); v2 = he->startv;
	mev(pos[3], he->startv, lp0, he); v3 = he->startv;
	mef(v0, he->startv, lp0, lp1);
	mev(pos[4], v0, lp0, he); v4 = he->startv; edge = he->edge;
	mev(pos[5], he->startv, lp0, he); v5 = he->startv;
	mev(pos[6], he->startv, lp0, he); v6 = he->startv;
	mev(pos[7], he->startv, lp0, he); v7 = he->startv;
	mef(v4, v7, lp0, lp2);
	kemr(edge, lp0);
	//return s;
	return sweep(s->sface->nextf->nextf, vec3d(0, 0, 1));
}
Brep::Solid* Brep::demo3(){
	vec3d vertices[20] = {
		vec3d(3, 3, 0), vec3d(-3, 3, 0), vec3d(-3, -3, 0), vec3d(3, -3, 0),
		vec3d(2, 2, 0), vec3d(1, 2, 0), vec3d(1, 1, 0), vec3d(2, 1, 0),
		vec3d(-1, 2, 0), vec3d(-2, 2, 0), vec3d(-2, 1, 0), vec3d(-1, 1, 0),
		vec3d(-1, -1, 0), vec3d(-2, -1, 0), vec3d(-2, -2, 0), vec3d(-1, -2, 0),
		vec3d(2, -1, 0), vec3d(1, -1, 0), vec3d(1, -2, 0), vec3d(2, -2, 0)

	};
	Solid* s;
	Vertex *v, *v1, *v2;
	HalfEdge* he;
	Edge* edge;
	Loop* newlp;
	mvfs(vertices[0], s, v);
	Loop* lp = s->sface->floop;
	mev(vertices[1], v, lp, he);
	mev(vertices[2], he->startv, lp, he);
	mev(vertices[3], he->startv, lp, he);
	mef(he->startv, v, lp, newlp);

	mev(vertices[4], v, lp, he); v1 = he->startv; edge = he->edge;
	mev(vertices[5], he->startv, lp, he);
	mev(vertices[6], he->startv, lp, he);
	mev(vertices[7], he->startv, lp, he); v2 = he->startv;
	mef(v1, v2, lp, newlp); 
	//lp = newlp;
	kemr(edge, lp);

	mev(vertices[8], v, lp, he); v1 = he->startv; edge = he->edge;
	mev(vertices[9], he->startv, lp, he);
	mev(vertices[10], he->startv, lp, he);
	mev(vertices[11], he->startv, lp, he); v2 = he->startv;
	mef(v1, v2, lp, newlp);
	kemr(edge, lp);

	mev(vertices[12], v, lp, he); v1 = he->startv; edge = he->edge;
	mev(vertices[13], he->startv, lp, he);
	mev(vertices[14], he->startv, lp, he);
	mev(vertices[15], he->startv, lp, he); v2 = he->startv;
	mef(v1, v2, lp, newlp);
	kemr(edge, lp);

	mev(vertices[16], v, lp, he); v1 = he->startv; edge = he->edge;
	mev(vertices[17], he->startv, lp, he);
	mev(vertices[18], he->startv, lp, he);
	mev(vertices[19], he->startv, lp, he); v2 = he->startv;
	mef(v1, v2, lp, newlp);
	kemr(edge, lp);

	return sweep(lp->lface, vec3d(0, 0, 1));

}

Brep::Solid* Brep::demo4(){
	vec3d vertices[14] = {
		vec3d(4, 4, 0), vec3d(-4, 4, 0), vec3d(-4, -4, 0), vec3d(4, -4, 0),
		vec3d(0, 3, 0), vec3d(-0.5, 1, 0), vec3d(-3, 1, 0), vec3d(-1, -1, 0), vec3d(-2, -3, 0), vec3d(0, -2, 0), vec3d(2, -3, 0), vec3d(1, -1, 0), vec3d(3, 1, 0), vec3d(0.5, 1, 0)
	};
	Solid* s;
	Vertex* v;
	Loop* lp;
	Loop* newlp;
	HalfEdge* he;
	Edge* edge;
	Vertex *v1, *v2;
	mvfs(vertices[0], s, v);
	lp = s->sface->floop;

	mev(vertices[1], v, lp, he);
	mev(vertices[2], he->startv, lp, he);
	mev(vertices[3], he->startv, lp, he);
	mef(he->startv, v, lp, newlp);

	mev(vertices[4], v, lp, he); v1 = he->startv; edge = he->edge;
	mev(vertices[5], he->startv, lp, he);
	mev(vertices[6], he->startv, lp, he);
	mev(vertices[7], he->startv, lp, he);
	mev(vertices[8], he->startv, lp, he);
	mev(vertices[9], he->startv, lp, he);
	mev(vertices[10], he->startv, lp, he);
	mev(vertices[11], he->startv, lp, he);
	mev(vertices[12], he->startv, lp, he);
	mev(vertices[13], he->startv, lp, he); v2 = he->startv;
	mef(v1, v2, lp, newlp);
	kemr(edge, lp);

	return sweep(lp->lface,vec3d(0,0,1));
}
Brep::Brep()
{
}


Brep::~Brep()
{
}
