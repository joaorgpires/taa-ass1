# Tópicos Avançados em Algoritmos
## Practical Assignment 1

### Question 1
Develop and implement a **sweep-based** algorithm to compute the *horizontal* and the *grid partition* of an *n*-vertex orthogonal polygon *P*.

The horizontal partition is obtained by extending the horizontal edges towards the interior of *P*. The grid partition is obtained by extending also the vertical edges. The polygon can have **holes** and **collinear edges**.

The input starts with an option that is either 0 or 1 (0 for computing the horizontal partition and 1 for the grid partition). Then, it contains the number of vertices of the outer boundary cycle of *P*, followed by the coordinates of the vertices (given in CCW order), the number of holes and the description of each hole (the number of vertices and their coordinates, given in CW order).

The output is the DCEL representing the partition. The coordinates of the vertices are given in a canonical cartesian system and are integers.

### Question 2
Exploit the DCEL constructed in **Question 1** to compute the region that is *rectangularly* visible from a given **vertex** *v* of *P*. The output must be a DCEL representing either the horizontal or the grid partition of the visibility region, depending on a given parameter.

### Question 3
Study experimentally the minimum vertex guard problem for orthogonal polygons, under rectangular visibility with the restriction that each point must be visible to at least *k* guards (with *k* defined as a parameter).

### TODO
Vertical partition is not yet finished. Questions 2 and 3 are not finished yet.
