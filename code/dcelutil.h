#ifndef dcelutil_h
#define dcelutil_h

#include <math.h>
#include <list>
#include <vector>
#include "geoutil.h"

using namespace std;

class dcel {
public:
  dcel();

  void init_poly(Polygon p);

private:
  struct Vertex;
  struct Face;
  struct HalfEdge;

  struct Vertex {
    Point coord;
    HalfEdge *outer;
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
      return Segment(origin->coord, twin->origin->coord);
    }
  };

  Face *out_face;

  int face_index;

  Vertex *create_vertex_from(HalfEdge *edge, Point coord);
};

#endif
