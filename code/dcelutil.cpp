#include "dcelutil.h"
#include <iostream>

using namespace std;

dcel::dcel() {
  face_index = 0;
  out_face = NULL;
}

void dcel::init_poly(Polygon p) {

}

dcel::Vertex *dcel::create_vertex_from(HalfEdge *edge, Point coord) {
  Vertex *v = new Vertex();
  v->outer = edge;
  v->coord = coord;

  return v;
}
