#ifndef dcelutil_h
#define dcelutil_h

#include <math.h>
#include <vector>
#include <set>
#include <map>
#include "geoutil.h"

#define F first
#define S second

typedef pair<lld, lld> pii;

using namespace std;

class dcel {
public:
  dcel();

  void init_poly(Polygon p);
  void print_dcel();
  void sweep_line();

private:
  struct Vertex;
  struct Face;
  struct HalfEdge;

  struct Vertex {
    Point coord;
    HalfEdge *outer;
    int index;
  };

  struct Face {
    HalfEdge *outer;
    int index;
  };

  struct HalfEdge {
    Vertex *origin;
    Face *incident;
    HalfEdge *twin;
    HalfEdge *prev;
    HalfEdge *next;

    Segment edge() {
      Point p1, p2;
      if(origin->coord.X == twin->origin->coord.X) {
        if(origin->coord.Y < twin->origin->coord.Y) {
          p2 = origin->coord;
          p1 = twin->origin->coord;
        }
        else {
          p1 = origin->coord;
          p2 = twin->origin->coord;
        }
      }
      else {
        if(origin->coord.X < twin->origin->coord.X) {
          p1 = origin->coord;
          p2 = twin->origin->coord;
        }
        else {
          p2 = origin->coord;
          p1 = twin->origin->coord;
        }
      }
      return Segment(p1, p2);
    }
  };

  Face *out_face, *in_face;

  int face_index;
  int vertex_index;
  set<int> visited_faces;
  //vector<Vertex*> vertices;
  vector<Face*> faces;
  vector<HalfEdge*> eventsH;
  vector<HalfEdge*> eventsV;
  map<pii, Vertex*> vertices;
  map<pii, HalfEdge*> linestate;

  Vertex *create_vertex_from(HalfEdge *edge, Point coord);
  void find_faces(Face *face);
  void add_event(HalfEdge *edge);
  static bool comp(const HalfEdge *a, const HalfEdge *b);
  void splitHalfEdgeL(HalfEdge *split, HalfEdge *event, lld x, lld y);
  void splitHalfEdgeR(HalfEdge *split, HalfEdge *event, lld x, lld y);
  void create_face_from(HalfEdge *e);
};

#endif
