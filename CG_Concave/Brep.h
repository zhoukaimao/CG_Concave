#pragma once
#include <vector>
#include <iostream>
#include <sstream>
#include "BRepEntity.h"
using namespace std;
class Brep :
	public BRepEntity
{
public:
	Brep();
	~Brep();
	static int num_solid;
	static int num_face;
	static int num_loop;
	static int num_halfedge;
	static int num_vertex;
	static int num_edge;

	struct Solid;
	struct Face;
	struct Loop;
	struct HalfEdge;
	struct Edge;
	struct Vertex;
	struct vec3d;
	struct Solid{
		Solid* prevs;
		Solid* nexts;
		Face* sface;
		int ID;

		Solid() : prevs(NULL), nexts(NULL), sface(NULL) { ID = num_solid++; }
	};
	struct Face{
		Face* prevf;
		Face* nextf;
		Solid* fsolid;
		Loop* floop;
		int ID;

		Face() :prevf(NULL), nextf(NULL), fsolid(NULL), floop(NULL){ ID = num_face++; }
		bool equal(const Face* rh){ return rh->ID == ID; }
		Face* clone(){
			Face* f = new Face();
			f->fsolid = fsolid;
			Loop* prev_lp = f->floop = floop->clone();
			prev_lp->lface = f;
			Loop* lp = floop->nextl;
			while (lp != NULL){
				prev_lp->nextl = lp->clone();
				prev_lp->nextl->lface = f;
				lp = lp->nextl;
				prev_lp = prev_lp->nextl;
			}
			return f;
		}
		void mirror(){
			Loop* lp = floop;
			while (lp != NULL){
				lp->mirror();
				lp = lp->nextl;
			}
		}
	};
	struct Loop{
		Loop* prevl;
		Loop* nextl;
		Face* lface;
		HalfEdge* ledge;
		int ID;

		Loop() :prevl(NULL), nextl(NULL), lface(NULL), ledge(NULL){ ID = num_loop++; }
		Loop* clone(){
			Loop* lp = new Loop();
			HalfEdge* prev_he = lp->ledge = ledge->clone();
			lp->ledge->hloop = lp;
			HalfEdge* he = ledge->next;
			while (he!=ledge){
				prev_he->next = he->clone();
				prev_he->next->hloop = lp;
				prev_he->next->prev = prev_he;
				he = he->next;
				prev_he = prev_he->next;
			}
			prev_he->next = lp->ledge;
			lp->ledge->prev = prev_he;
			return lp;
		}
		void mirror(){
			HalfEdge *he, *prevHe, *nextHe;
			he = ledge;

			Vertex* startv = ledge->startv;
			while (he->next->nequal(ledge)){
				he->startv = he->next->startv;
				he = he->next;
			}
			he->startv = startv;

			he = ledge->next;
			while (he->nequal(ledge)){
				prevHe = he->prev;
				nextHe = he->next;
				he->prev = nextHe;
				he->next = prevHe;
				he = nextHe;
			}
			prevHe = he->prev;
			nextHe = he->next;
			he->prev = nextHe;
			he->next = prevHe;
		}
	};
	struct HalfEdge{
		HalfEdge* prev;//useless
		HalfEdge* next;
		Loop* hloop;
		Vertex* startv;
		Edge* edge;
		int ID;

		HalfEdge():/*prev(NULL),*/next(NULL),hloop(NULL),startv(NULL),edge(NULL){ ID = num_halfedge++;}
		//bool operator != (const HalfEdge& rh){ return rh.ID != ID; }
		bool nequal(const HalfEdge* rh){ return rh->ID != ID; }
		HalfEdge* clone(){
			HalfEdge* he = new HalfEdge();
			he->startv = startv;
			he->edge = edge;
			return he;
		}
	};	
	struct vec3d{
		double data[3];
		//double x, y, z;
		vec3d(){ data[0] = data[1] = data[2] = 0; }
		vec3d(double _x, double _y, double _z) { data[0] = _x; data[1] = _y; data[2] = _z; }
		vec3d operator + (const vec3d& rh){ return vec3d(rh.data[0] + data[0], rh.data[1] + data[1], rh.data[2] + data[2]); }
	};
	struct Vertex{
		Vertex* prevv;
		Vertex* nextv;
		HalfEdge* vhe;
		vec3d position;
		int ID;

		Vertex(vec3d pos) : prevv(NULL),nextv(NULL),vhe(NULL),position(pos){ ID = num_vertex++; }
		//bool operator != (const Vertex& rh){ return ID != rh.ID; }
		bool nequal(const Vertex* rh){ return rh->ID != ID; }
	};
	struct Edge{
		Edge* prev;
		Edge* next;
		HalfEdge* he1;
		HalfEdge* he2;
		int ID;

		Edge() :prev(NULL), next(NULL), he1(NULL), he2(NULL){ ID = num_edge++; }
	};


	void mvfs(vec3d position, Solid* & o_solid, Vertex* & o_vertex);
	void mev(vec3d position,Vertex* v1, Loop* lp,HalfEdge* & o_he);
	void mef(Vertex* v1, Vertex* v2, Loop* lp1, Loop* & o_lp2,bool mark=true);
	void kemr(Edge* edge,Loop* lp);
	void kfmrh(Face* f1,Face* f2);
	//void semv();

	Solid* sweep(Face* f,vec3d dir);

	Solid* cube(double width, double height, double depth);
	Solid* hole(double width, double height, double inner_width, double inner_height);
	Solid* demo1();
	Solid* demo2();
	Solid* demo3();
	Solid* demo4();
};

