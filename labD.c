#include "labD.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>


void parse_line(Graph* g, int u,
    const char* s) {
    while (*s) {
        while (*s && !isdigit((unsigned char)*s)) s++;
        if (!*s) break;

        int v = strtol(s, (char**)&s, 10);
        if (v >= 1 && v <= g->n)
            graph_add_edge(g, u, v);
    }
}

void insert_vertex(const Graph* g, int* path, int* len, int v) {
    int pos = 0;
    while (pos < *len && !graph_has_edge(g, v, path[pos]))
        pos++;

    for (int i = *len; i > pos; --i)
        path[i] = path[i - 1];

    path[pos] = v;
    (*len)++;
}

int graph_init(Graph* g, int n) {
    if (!g || n < 1 || n > MAXV) return 0;
    g->n = n;
    g->words = (n + 63) / 64;
    g->adj = calloc((size_t)n * g->words, sizeof(uint64_t));
    return g->adj != NULL;
}

void graph_free(Graph* g) {
    if (!g) return;
    free(g->adj);
    g->adj = NULL;
}

void graph_add_edge(Graph* g, int u, int v) {
    if (!g || u == v) return;
    g->adj[(u - 1) * g->words + (v - 1) / 64] |=
        1ULL << ((v - 1) % 64);
}

int graph_has_edge(const Graph* g, int u, int v) {
    if (!g || u == v) return 0;
    return (g->adj[(u - 1) * g->words + (v - 1) / 64] >>
        ((v - 1) % 64)) & 1ULL;
}

int read_graph_from_file(Graph* g,
    const char* filename) {
    FILE* f = fopen(filename, "r");
    if (!f) return 0;

    int n;
    if (fscanf_s(f, "%d\n", &n) != 1) {
        fclose(f);
        return 0;
    }

    if (!graph_init(g, n)) {
        fclose(f);
        return 0;
    }

    char buf[8192];
    for (int i = 1; i <= n; i++) {
        if (!fgets(buf, sizeof(buf), f))
            buf[0] = '\0';
        parse_line(g, i, buf);
    }

    fclose(f);
    return 1;
}

int find_hamiltonian_path(const Graph* g, int* path) {
    int len = 0;
    for (int v = 1; v <= g->n; v++)
        insert_vertex(g, path, &len, v);

    return verify_hamiltonian_path(g, path);
}

int verify_hamiltonian_path(const Graph* g,
    const int* path) {
    int used[MAXV + 1] = {
      0
    };

    for (int i = 0; i < g->n; i++) {
        int v = path[i];
        if (v < 1 || v > g->n || used[v])
            return 0;
        used[v] = 1;
    }

    for (int i = 0; i < g->n - 1; i++) 
        if (!graph_has_edge(g, path[i], path[i + 1]))
            return 0;

    return 1;
}