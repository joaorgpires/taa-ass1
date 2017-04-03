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
  int w2 = 10;
  cout << "Vertex Index: X:        Y:        Outer edge:" << endl;
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
  for(int i = 0; i < (int)faces.size(); i++) {
    cout << left << setw(w1) << setfill(sep) << faces[i]->index;
    cout << "v" << faces[i]->outer->origin->index << ",v" << faces[i]->outer->twin->origin->index;
    cout << endl;
  }

  cout << endl << "------HALF EDGES------" << endl;
  w1 = 13;
  w2 = 12;
  int w3 = 13;
  int w4 = 15;
  cout << "Half Edge:   Origin:     Twin:        Incident Face: Next:        Previous:" << endl;
  for(int i = 0; i < (int)faces.size(); i++) {
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

void dcel::create_face_from(HalfEdge *e) {
  Face *f = new Face();
  f->outer = e;
  f->index = face_index++;

  e->incident = f;

  HalfEdge *cur = e->next;

  while(cur != e) {
    cur->incident = f;
    cur = cur->next;
  }
}

void dcel::splitHalfEdgeL(HalfEdge *split, HalfEdge *event, lld x, lld y) {
  HalfEdge *prev = split->prev;
  HalfEdge *next = split->next;
  HalfEdge *evnext = event->next;

  HalfEdge *e1 = new HalfEdge();
  HalfEdge *e2 = new HalfEdge();
  HalfEdge *e3 = new HalfEdge();
  HalfEdge *e4 = new HalfEdge();
  HalfEdge *e5 = new HalfEdge();
  HalfEdge *e6 = new HalfEdge();

  e1->origin = split->origin;
  split->origin->outer = e1;
  //e1->incident = split->incident;
  //split->incident->outer = e1;
  e1->twin = e3;
  e1->prev = prev;
  prev->next = e1;
  e1->next = e5;
  //eventsV.push_back(e1);

  Vertex *newv = create_vertex_from(e2, Point(x, y));
  e2->origin = newv;
  e2->twin = e4;
  e2->prev = e6;
  e2->next = next;
  next->prev = e2;
  event->next = e6;
  e6->next = e2;
  //eventsV.push_back(e2);

  e3->origin = newv;
  e3->incident = split->twin->incident;
  split->twin->incident->outer = e3;
  e3->twin = e1;
  e3->prev = e4;
  e3->next = split->twin->next;
  split->twin->next->prev = e3;

  e4->origin = split->twin->origin;
  e4->incident = split->twin->incident;
  e4->twin = e2;
  e4->prev = split->twin->prev;
  e4->next = e3;
  split->twin->prev->next = e4;

  e5->origin = newv;
  //e5->incident = split->incident;
  e5->twin = e6;
  e5->prev = e1;
  e5->next = evnext;
  evnext->prev = e5;

  e6->origin = event->twin->origin;
  e6->twin = e5;
  e6->prev = event;

  create_face_from(e1);
  create_face_from(e2);

  if(e1->edge().p2.Y > e2->edge().p2.Y) linestate[pii(split->edge().p2.X, split->edge().p2.Y)] = e2;
  else linestate[pii(split->edge().p2.X, split->edge().p2.Y)] = e1;

  /*DEBUG----------
  cout << endl << "------HALF EDGES------" << endl;
  const char sep = ' ';
  int w1 = 13;
  int w2 = 12;
  int w3 = 13;
  int w4 = 15;
  cout << "Half Edge:   Origin:     Twin:        Incident Face: Next:        Previous:" << endl;
  char s[20];
  sprintf(s, "v%d,v%d", e1->origin->index, e1->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e1->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e1->twin->origin->index, e1->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e1->incident->index;
  sprintf(s, "v%d,v%d", e1->next->origin->index, e1->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e1->prev->origin->index, e1->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e2->origin->index, e2->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e2->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e2->twin->origin->index, e2->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e2->incident->index;
  sprintf(s, "v%d,v%d", e2->next->origin->index, e2->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e2->prev->origin->index, e2->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e3->origin->index, e3->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e3->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e3->twin->origin->index, e3->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e3->incident->index;
  sprintf(s, "v%d,v%d", e3->next->origin->index, e3->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e3->prev->origin->index, e3->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e4->origin->index, e4->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e4->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e4->twin->origin->index, e4->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e4->incident->index;
  sprintf(s, "v%d,v%d", e4->next->origin->index, e4->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e4->prev->origin->index, e4->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e5->origin->index, e5->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e5->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e5->twin->origin->index, e5->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e5->incident->index;
  sprintf(s, "v%d,v%d", e5->next->origin->index, e5->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e5->prev->origin->index, e5->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e6->origin->index, e6->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e6->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e6->twin->origin->index, e6->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e6->incident->index;
  sprintf(s, "v%d,v%d", e6->next->origin->index, e6->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e6->prev->origin->index, e6->prev->twin->origin->index);
  cout << s << endl;*/
}

void dcel::splitHalfEdgeR(HalfEdge *split, HalfEdge *event, lld x, lld y) {
  HalfEdge *prev = split->prev;
  HalfEdge *next = split->next;
  HalfEdge *evprev = event->prev;

  HalfEdge *e1 = new HalfEdge();
  HalfEdge *e2 = new HalfEdge();
  HalfEdge *e3 = new HalfEdge();
  HalfEdge *e4 = new HalfEdge();
  HalfEdge *e5 = new HalfEdge();
  HalfEdge *e6 = new HalfEdge();

  Vertex *newv = create_vertex_from(e1, Point(x, y));
  e1->origin = newv;
  //e1->incident = split->incident;
  //split->incident->outer = e1;
  e1->twin = e3;
  e1->prev = e5;
  e1->next = next;
  next->prev = e1;

  e2->origin = split->origin;
  split->origin->outer = e2;
  e2->twin = e4;
  e2->prev = prev;
  prev->next = e2;
  e2->next = e6;
  e6->next = event;
  event->prev = e6;

  e3->origin = split->twin->origin;
  split->twin->origin->outer = e3;
  e3->incident = split->twin->incident;
  split->twin->incident->outer = e3;
  e3->twin = e1;
  e3->prev = split->twin->prev;
  split->twin->prev->next = e3;
  e3->next = e4;

  e4->origin = newv;
  e4->incident = split->twin->incident;
  e4->twin = e2;
  e4->prev = e3;
  e4->next = split->twin->next;
  split->twin->next->prev = e4;

  e5->origin = event->origin;
  //e5->incident = split->incident;
  e5->twin = e6;
  e5->prev = evprev;
  evprev->next = e5;
  e5->next = e1;

  e6->origin = newv;
  e6->twin = e5;
  e6->prev = e2;

  create_face_from(e1);
  create_face_from(e2);
  if(e1->edge().p2.Y > e2->edge().p2.Y) linestate[pii(split->edge().p2.X, split->edge().p2.Y)] = e2;
  else linestate[pii(split->edge().p2.X, split->edge().p2.Y)] = e1;

  /*DEBUG-----------
  cout << endl << "------HALF EDGES------" << endl;
  const char sep = ' ';
  int w1 = 13;
  int w2 = 12;
  int w3 = 13;
  int w4 = 15;
  cout << "Half Edge:   Origin:     Twin:        Incident Face: Next:        Previous:" << endl;
  char s[20];
  sprintf(s, "v%d,v%d", e1->origin->index, e1->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e1->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e1->twin->origin->index, e1->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e1->incident->index;
  sprintf(s, "v%d,v%d", e1->next->origin->index, e1->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e1->prev->origin->index, e1->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e2->origin->index, e2->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e2->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e2->twin->origin->index, e2->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e2->incident->index;
  sprintf(s, "v%d,v%d", e2->next->origin->index, e2->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e2->prev->origin->index, e2->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e3->origin->index, e3->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e3->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e3->twin->origin->index, e3->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e3->incident->index;
  sprintf(s, "v%d,v%d", e3->next->origin->index, e3->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e3->prev->origin->index, e3->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e4->origin->index, e4->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e4->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e4->twin->origin->index, e4->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e4->incident->index;
  sprintf(s, "v%d,v%d", e4->next->origin->index, e4->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e4->prev->origin->index, e4->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e5->origin->index, e5->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e5->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e5->twin->origin->index, e5->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e5->incident->index;
  sprintf(s, "v%d,v%d", e5->next->origin->index, e5->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e5->prev->origin->index, e5->prev->twin->origin->index);
  cout << s << endl;

  sprintf(s, "v%d,v%d", e6->origin->index, e6->twin->origin->index);
  cout << left << setw(w1) << setfill(sep) << s;
  sprintf(s, "v%d", e6->origin->index);
  cout << left << setw(w2) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e6->twin->origin->index, e6->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  cout << left << setw(w4) << setfill(sep) << e6->incident->index;
  sprintf(s, "v%d,v%d", e6->next->origin->index, e6->next->twin->origin->index);
  cout << left << setw(w3) << setfill(sep) << s;
  sprintf(s, "v%d,v%d", e6->prev->origin->index, e6->prev->twin->origin->index);
  cout << s << endl;*/
}

void dcel::middle(HalfEdge *event, HalfEdge *connect) {
  HalfEdge *evprev = event->prev;
  HalfEdge *conext = connect->next;

  HalfEdge *e1 = new HalfEdge();
  HalfEdge *e2 = new HalfEdge();

  e1->origin = event->origin;
  e2->origin = connect->twin->origin;

  e1->twin = e2;
  e2->twin = e1;

  e1->prev = evprev;
  evprev->next = e1;

  e2->prev = connect;
  connect->next = e2;

  e1->next = conext;
  conext->prev = e1;

  e2->next = event;
  event->prev = e2;

  create_face_from(e1);
  create_face_from(e2);
}

void dcel::sweep_line() {
  sort(eventsH.begin(), eventsH.end(), comp); //tested, ordering is ok

  HalfEdge *cur = eventsH[0];
  HalfEdge *next = cur->next;
  HalfEdge *prev = cur->prev;

  Segment sn = next->edge(), sp = prev->edge();
  linestate[pii(sn.p2.X, sn.p2.Y)] = next;
  linestate[pii(sp.p2.X, sp.p2.Y)] = prev;

  for(int i = 1; i < (int)eventsH.size(); i++) {
    cur = eventsH[i];
    next = cur->next;
    prev = cur->prev;
    Segment sc = cur->edge();

    //DEBUG----------
    cout << "Event: " << sc.p1.X << " " << sc.p1.Y << " " << sc.p2.X << " " << sc.p2.Y << endl;

    sn = next->edge(); sp = prev->edge();

    //If next going down, add it to map
    if(sc.p2.Y > sn.p2.Y) linestate[pii(sn.p2.X, sn.p2.Y)] = next;
    //If prev going down, add it to map
    if(sc.p2.Y > sp.p2.Y) linestate[pii(sp.p2.X, sp.p2.Y)] = prev;

    //Event from right to left
    if(cur->origin->coord.X == sc.p2.X) {
      //Left side
      map<pii, HalfEdge*>::iterator it = linestate.lower_bound(pii(sc.p1.X, sc.p1.Y));
      if(it != linestate.begin()) {
        it--;
        //Up side
        if(sc.p2.Y == sn.p2.Y) {
          //Is test edge inside polygon
          if(it->S->origin->coord.Y > sc.p2.Y) {
            //If vertice does not exist yet
            if(vertices.find(pii(it->S->origin->coord.X, sc.p2.Y)) == vertices.end()) {
              HalfEdge *split = it->S;
              linestate.erase(it);
              splitHalfEdgeL(split, eventsH[i], split->origin->coord.X, sc.p2.Y);
            }
          }
          else if(it->S->origin->coord.Y == sc.p2.Y){
            //Pass
          }
        }
        //Down side raises no problems
      }

      //Right side
      it = linestate.lower_bound(pii(sc.p2.X, sc.p2.Y));

      map<pii, HalfEdge*>::iterator itlast = linestate.end();
      itlast--;

      if(it != itlast && it != linestate.end()) {
        it++;
        //Up side
        if(sc.p2.Y == sp.p2.Y) {
          //Is test edge inside polygon
          if(it->S->origin->coord.Y < sc.p2.Y) {
            //If vertice does not exist yet
            if(vertices.find(pii(it->S->origin->coord.X, sc.p2.Y)) == vertices.end()) {
              HalfEdge *split = it->S;
              linestate.erase(it);
              splitHalfEdgeR(split, eventsH[i], split->origin->coord.X, sc.p2.Y);
            }
          }
          //If it is equal, connect points
          else if(it->S->origin->coord.Y == sc.p2.Y) {
            middle(cur, it->S->prev);
          }
        }
        //Down side raises no problems
      }
    }
    //Event from left to right
    else {
      //Left side
      map<pii, HalfEdge*>::iterator it = linestate.lower_bound(pii(sp.p2.X, sp.p2.Y));
      if(it != linestate.begin()) {
        it--;
        //Up side raises no problems
        //Down side
        if(sc.p2.Y == sp.p1.Y) {
          //Is test edge inside polygon
          if(it->S->origin->coord.Y > sc.p2.Y) {
            //If vertice does not exist yet
            if(vertices.find(pii(it->S->origin->coord.X, sc.p2.Y)) == vertices.end()) {
              HalfEdge *split = it->S;
              linestate.erase(it);
              splitHalfEdgeR(split, eventsH[i], split->origin->coord.X, sc.p2.Y);
            }
          }
          //If it is equal, connect points
          else if(it->S->origin->coord.Y == sc.p2.Y) {
            middle(cur, it->S->prev);
          }
        }
      }
      //Right side
      it = linestate.lower_bound(pii(sn.p2.X, sn.p2.Y));

      map<pii, HalfEdge*>::iterator itlast = linestate.end();
      itlast--;

      if(it != itlast && it != linestate.end()) {
        it++;
        //Up side raises no problems
        //Down side
        if(sc.p2.Y == sn.p1.Y) {
          //Is test edge inside polygon
          if(it->S->origin->coord.Y < sc.p2.Y) {
            //If vertice does not exist yet
            if(vertices.find(pii(it->S->origin->coord.X, sc.p2.Y)) == vertices.end()) {
              HalfEdge *split = it->S;
              linestate.erase(it);
              splitHalfEdgeL(split, eventsH[i], split->origin->coord.X, sc.p2.Y);
            }
          }
          else if(it->S->origin->coord.Y == sc.p2.Y) {
            //Pass
          }
        }
      }
    }
    if(sn.p2.Y == cur->origin->coord.Y) linestate.erase(pii(sn.p2.X, sn.p2.Y));
    if(sp.p2.Y == cur->origin->coord.Y) linestate.erase(pii(sp.p2.X, sp.p2.Y));
  }
}
