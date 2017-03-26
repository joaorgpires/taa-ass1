#include "dcelutil.h"
#include <iostream>

using namespace std;

dcel::dcel() {
  face_index = 0;
  out_face = NULL;
}

void dcel::init_poly(Polygon p) {
  HalfEdge *first = new HalfEdge(), *cur;
  out_face = new Face();
  Face *in_face = new Face();
  in_face->outer = first;
  in_face->index = face_index++;
  out_face->index = face_index++;
  first->origin = create_vertex_from(first, p[0]);

  cur = first;
  for(int i = 1; i < (int)p.size(); i++) {
    HalfEdge *next = new HalfEdge();

    cur->next = next;
    next->prev = cur;
    cur->incident = in_face;
    next->origin = create_vertex_from(next, p[i]);
    cur = next;
  }

  cur->next = first;
  first->prev = cur;

  for(int i = 0; i < (int)p.size(); i++) {
    cout << cur->origin->coord.X << " " << cur->origin->coord.Y << endl;
    HalfEdge *next = new HalfEdge();

    next->origin = cur->next->origin;
    next->twin = cur;
    cur->twin = next;
    next->incident = out_face;

    if(i != 0) {
      next->next = cur->prev->twin;
      cur->prev->twin->prev = next;
    }

    cur = cur->next;
  }

  first->twin->next->next = first->prev->prev->twin;
  first->twin->next->next->prev = first->twin->next;
  out_face->outer = first->twin;
}

dcel::Vertex *dcel::create_vertex_from(HalfEdge *edge, Point coord) {
  Vertex *v = new Vertex();
  v->outer = edge;
  v->coord = coord;

  return v;
}
