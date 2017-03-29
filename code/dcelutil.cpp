#include "dcelutil.h"
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

//Idea for holes: guardar os indices das outer faces dos holes, depois fica facil imprimir (tenho uma halfedge a representar aquilo e depois posso usar as twins)

dcel::dcel() {
  face_index = 0;
  vertex_index = 0;
  out_face = NULL;
  in_face = NULL;
}

void dcel::init_poly(Polygon p) {
  HalfEdge *first = new HalfEdge(), *cur;
  out_face = new Face();
  in_face = new Face();
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
    add_event(cur);
    cur = next;
  }

  cur->next = first;
  first->prev = cur;
  cur->incident = in_face;
  add_event(cur);

  for(int i = 0; i < (int)p.size(); i++) {
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
  v->index = vertex_index++;

  pii pv = pii(coord.X, coord.Y);
  vertices[pv] = v;

  return v;
}

void dcel::find_faces(Face *face) {
  if (visited_faces.find(face->index) != visited_faces.end()) {
    return;
  }

  visited_faces.insert(face->index);
  faces.push_back(face);

  HalfEdge *first = face->outer;
  HalfEdge *cur = first->next;

  while(cur != first) {
    find_faces(cur->twin->incident);
    cur = cur->next;
  }
}

void dcel::print_dcel() {
  find_faces(out_face);

  cout << "------VERTICES------" << endl;
  const char sep = ' ';
  int w1 = 14;
  int w2 = 3;
  cout << "Vertex Index: X: Y: Outer edge:" << endl;
  for(map<pii, Vertex*>::iterator it = vertices.begin(); it != vertices.end(); ++it) {
    cout << left << setw(w1) << setfill(sep) << it->S->index;
    cout << left << setw(w2) << setfill(sep) << it->S->coord.X;
    cout << left << setw(w2) << setfill(sep) << it->S->coord.Y;
    cout << "v" << it->S->index << ",v" << it->S->outer->twin->origin->index;
    cout << endl;
  }

  cout << endl << "------FACES------" << endl;
  w1 = 12;
  cout << "Face Index: Outer edge:" << endl;
  for(int i = 0; i < face_index; i++) {
    cout << left << setw(w1) << setfill(sep) << faces[i]->index;
    cout << "v" << faces[i]->outer->origin->index << ",v" << faces[i]->outer->twin->origin->index;
    cout << endl;
  }

  cout << endl << "------HALF EDGES------" << endl;
  w1 = 11;
  w2 = 8;
  int w3 = 9;
  int w4 = 15;
  cout << "Half Edge: Origin: Twin:    Incident Face: Next:    Previous:" << endl;
  for(int i = 0; i < face_index; i++) {
    HalfEdge *cur = faces[i]->outer;
    char s[20];
    sprintf(s, "v%d,v%d", cur->origin->index, cur->twin->origin->index);
    cout << left << setw(w1) << setfill(sep) << s;
    sprintf(s, "v%d", cur->origin->index);
    cout << left << setw(w2) << setfill(sep) << s;
    sprintf(s, "v%d,v%d", cur->twin->origin->index, cur->origin->index);
    cout << left << setw(w3) << setfill(sep) << s;
    cout << left << setw(w4) << setfill(sep) << cur->incident->index;
    sprintf(s, "v%d,v%d", cur->next->origin->index, cur->next->twin->origin->index);
    cout << left << setw(w3) << setfill(sep) << s;
    sprintf(s, "v%d,v%d", cur->prev->origin->index, cur->prev->twin->origin->index);
    cout << s << endl;

    HalfEdge *first = cur;
    cur = cur->next;

    while(cur != first) {
      sprintf(s, "v%d,v%d", cur->origin->index, cur->twin->origin->index);
      cout << left << setw(w1) << setfill(sep) << s;
      sprintf(s, "v%d", cur->origin->index);
      cout << left << setw(w2) << setfill(sep) << s;
      sprintf(s, "v%d,v%d", cur->twin->origin->index, cur->origin->index);
      cout << left << setw(w3) << setfill(sep) << s;
      cout << left << setw(w4) << setfill(sep) << cur->incident->index;
      sprintf(s, "v%d,v%d", cur->next->origin->index, cur->next->twin->origin->index);
      cout << left << setw(w3) << setfill(sep) << s;
      sprintf(s, "v%d,v%d", cur->prev->origin->index, cur->prev->twin->origin->index);
      cout << s << endl;
      cur = cur->next;
    }
  }
}

void dcel::add_event(HalfEdge *edge) {
  if(edge->origin->coord.Y == edge->next->origin->coord.Y) eventsH.push_back(edge);
  else eventsV.push_back(edge);

  return;
}

bool dcel::comp(const HalfEdge *a, const HalfEdge *b) {
  if(a->origin->coord.Y > b->origin->coord.Y) return true;
  int mn1 = min(a->origin->coord.X, a->twin->origin->coord.X), mn2 = min(b->origin->coord.X, b->twin->origin->coord.X);
  if(a->origin->coord.Y == b->origin->coord.Y && mn1 < mn2) return true;

  return false;
}

void dcel::sweep_line() {
  sort(eventsH.begin(), eventsH.end(), comp); //tested, ordering is ok

  HalfEdge *cur = eventsH[0];
  HalfEdge *next = cur->next;
  HalfEdge *prev = cur->prev;

  Segment sn = next->edge(), sp = prev->edge();
  linestate[pii(sn.p2.Y, sn.p2.X)] = next;
  linestate[pii(sp.p2.Y, sp.p2.X)] = prev;

  for(int i = 1; i < (int)eventsH.size(); i++) {
    cur = eventsH[i];
    next = cur->next;
    prev = cur->prev;
    map<pii, HalfEdge*>::iterator it = linestate.lower_bound(pii(cur->origin->coord.Y, -INF));
    linestate.erase(linestate.begin(), it);

    if(linestate.empty()) continue;

    lld xmin = min(cur->origin->coord.X, cur->twin->origin->coord.X), xmax = max(cur->origin->coord.X, cur->twin->origin->coord.X);

    //Usar a tactica que tens no caderno para ver se a halfedge a esquerda ou a direita e dentro ou fora

    sn = next->edge(); sp = prev->edge();
    linestate[pii(sn.p2.Y, sn.p2.X)] = next;
    linestate[pii(sp.p2.Y, sp.p2.X)] = prev;
  }
}
