#ifndef HAMILTON_H
#define HAMILTON_H

#include <stdint.h>

#define MAXV 1000

typedef struct {
	int n;
	int words;
	uint64_t* adj;
}
Graph;

int graph_init(Graph* g, int n);
void graph_free(Graph* g);
void graph_add_edge(Graph* g, int u, int v);
int graph_has_edge(const Graph* g, int u, int v);

int read_graph_from_file(Graph* g,
	const char* filename);

int find_hamiltonian_path(const Graph* g, int* path);
int verify_hamiltonian_path(const Graph* g,
	const int* path);

#endif