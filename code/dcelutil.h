#ifndef dcelutil_h
#define dcelutil_h

#include <math.h>
#include <vector>
#include <set>
#include "geoutil.h"

using namespace std;

class dcel {
public:
  dcel();

  void init_poly(Polygon p);
  void print_dcel();

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
      return Segment(origin->coord, twin->origin->coord);
    }
  };

  Face *out_face, *in_face;

  int face_index;
  int vertex_index;
  set<int> visited_faces;
  vector<Vertex*> vertices;
  vector<Face*> faces;

  Vertex *create_vertex_from(HalfEdge *edge, Point coord);
  void find_faces(Face *face);
};

#endif
