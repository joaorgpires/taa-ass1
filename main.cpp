#include <iostream>
#include "code/dcelutil.h"

using namespace std;

Polygon P;

int main() {
  int option; cin >> option;
  int n_vertices; cin >> n_vertices;

  for(int i = 0; i < n_vertices; i++) {
    lld x, y; cin >> x >> y;
    Point p = Point(x, y);
    P.push_back(p);
  }

  dcel *partition = new dcel();
  dcel *vertical = new dcel();
  partition->init_poly(P);

  if(option == 1) {
    vertical->vertical_init(P);
  }

  int n_holes; cin >> n_holes;

  for(int n = 0; n < n_holes; n++) {
    Polygon hole;
    int n_vert; cin >> n_vert;

    for(int i = 0; i < n_vert; i++) {
      lld x, y; cin >> x >> y;
      Point p = Point(x, y);
      hole.push_back(p);
    }

    partition->init_hole(hole);
    if(option == 1) vertical->vertical_hole(hole);
  }

  partition->sweep_line();
  if(option == 0) {
    partition->print_dcel();
    return 0;
  }

  vertical->sweep_line(true);
  vector<edge_point> interest_points = vertical->give_interest_points();
  partition->get_interest_points(interest_points);
  partition->handle_vertical();
  //DEBUG-----------
  //vertical->print_dcel(true);
  partition->print_dcel();

  return 0;
}
