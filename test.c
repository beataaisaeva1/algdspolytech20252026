#include "labD.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_TRUE(cond) do { \
    if (!(cond)) { \
        printf("  ASSERT_TRUE failed: %s (line %d)\n", #cond, __LINE__); \
        return 0; \
    } \
} while(0)

#define ASSERT_EQ_INT(a,b) do { \
    int _aa=(a), _bb=(b); \
    if (_aa != _bb) { \
        printf("  ASSERT_EQ_INT failed: %s=%d, %s=%d (line %d)\n", #a, _aa, #b, _bb, __LINE__); \
        return 0; \
    } \
} while(0)

static int write_text_file(const char* name,
    const char* text) {
    FILE* f = fopen(name, "w");
    if (!f) return 0;
    fputs(text, f);
    fclose(f);
    return 1;
}

static int make_graph(Graph * g, int n) {
    return graph_init(g, n);
}

static int TestFindPath_SingleVertex_no1(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 1));

    int path[1];
    ASSERT_TRUE(find_hamiltonian_path(&g, path));
    ASSERT_EQ_INT(path[0], 1);

    graph_free(&g);
    return 1;
}

static int TestFindPath_TwoVertices_Edge12_no2(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 2));
    graph_add_edge(&g, 1, 2);

    int path[2];
    ASSERT_TRUE(find_hamiltonian_path(&g, path));
    ASSERT_TRUE(verify_hamiltonian_path(&g, path));

    graph_free(&g);
    return 1;
}

static int TestFindPath_TwoVertices_NoEdges_Fail_no3(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 2));

    int path[2];
    ASSERT_EQ_INT(find_hamiltonian_path(&g, path), 0);

    graph_free(&g);
    return 1;
}

static int TestFindPath_Chain3_Success_no4(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 3));
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 1, 3);

    int path[3];
    ASSERT_TRUE(find_hamiltonian_path(&g, path));
    ASSERT_TRUE(verify_hamiltonian_path(&g, path));

    graph_free(&g);
    return 1;
}

static int TestFindPath_Tournament3_Cycle_no5(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 3));
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 3, 1);

    int path[3];
    ASSERT_TRUE(find_hamiltonian_path(&g, path));
    ASSERT_TRUE(verify_hamiltonian_path(&g, path));

    graph_free(&g);
    return 1;
}

static int TestVerifyPath_DuplicateVertex_Fail_no6(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 3));
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);
    graph_add_edge(&g, 1, 3);

    int bad[3] = {
      1,
      2,
      2
    };
    ASSERT_EQ_INT(verify_hamiltonian_path(&g, bad), 0);

    graph_free(&g);
    return 1;
}

static int TestVerifyPath_MissingEdgeBetweenNeighbors_Fail_no7(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 3));
    graph_add_edge(&g, 1, 2);
    graph_add_edge(&g, 2, 3);

    int bad[3] = {
      2,
      1,
      3
    };
    ASSERT_EQ_INT(verify_hamiltonian_path(&g, bad), 0);

    graph_free(&g);
    return 1;
}

static int TestGraphAddEdge_SelfLoopIgnored_no8(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 2));
    graph_add_edge(&g, 1, 1);
    graph_add_edge(&g, 2, 2);
    ASSERT_EQ_INT(graph_has_edge(&g, 1, 1), 0);
    ASSERT_EQ_INT(graph_has_edge(&g, 2, 2), 0);

    graph_free(&g);
    return 1;
}

static int TestReadGraphFromFile_EmptyAdjLine_no9(void) {
    ASSERT_TRUE(write_text_file("input.txt",
        "3\n"
        "\n"
        "3\n"
        "\n"
    ));

    Graph g = {
      0
    };
    ASSERT_TRUE(read_graph_from_file(&g, "input.txt"));

    ASSERT_EQ_INT(g.n, 3);
    ASSERT_EQ_INT(graph_has_edge(&g, 2, 3), 1);
    ASSERT_EQ_INT(graph_has_edge(&g, 1, 2), 0);

    graph_free(&g);
    return 1;
}

static int TestReadGraphFromFile_WithJunkAndSpaces_no10(void) {
    ASSERT_TRUE(write_text_file("input.txt",
        "3\n"
        "   2   3   abc !! \n"
        "  3   \n"
        "   \n"
    ));

    Graph g = {
      0
    };
    ASSERT_TRUE(read_graph_from_file(&g, "input.txt"));
    ASSERT_EQ_INT(graph_has_edge(&g, 1, 2), 1);
    ASSERT_EQ_INT(graph_has_edge(&g, 1, 3), 1);
    ASSERT_EQ_INT(graph_has_edge(&g, 2, 3), 1);

    graph_free(&g);
    return 1;
}

static int TestFindPath_LargeChain200_no11(void) {
    Graph g;
    ASSERT_TRUE(make_graph(&g, 200));
    for (int i = 1; i < 200; i++) {
        graph_add_edge(&g, i, i + 1);
    }

    int* path = (int*)malloc(sizeof(int) * g.n);
    ASSERT_TRUE(path != NULL);

    ASSERT_TRUE(find_hamiltonian_path(&g, path));
    ASSERT_TRUE(verify_hamiltonian_path(&g, path));

    free(path);
    graph_free(&g);
    return 1;
}

typedef int(*TestFn)(void);

int main(void) {
    struct {
        const char* name;
        TestFn fn;
    }
    tests[] = {
      {
        "TestFindPath_SingleVertex_no1",
        TestFindPath_SingleVertex_no1
      },
      {
        "TestFindPath_TwoVertices_Edge12_no2",
        TestFindPath_TwoVertices_Edge12_no2
      },
      {
        "TestFindPath_TwoVertices_NoEdges_Fail_no3",
        TestFindPath_TwoVertices_NoEdges_Fail_no3
      },
      {
        "TestFindPath_Chain3_Success_no4",
        TestFindPath_Chain3_Success_no4
      },
      {
        "TestFindPath_Tournament3_Cycle_no5",
        TestFindPath_Tournament3_Cycle_no5
      },
      {
        "TestVerifyPath_DuplicateVertex_Fail_no6",
        TestVerifyPath_DuplicateVertex_Fail_no6
      },
      {
        "TestVerifyPath_MissingEdgeBetweenNeighbors_Fail_no7",
        TestVerifyPath_MissingEdgeBetweenNeighbors_Fail_no7
      },
      {
        "TestGraphAddEdge_SelfLoopIgnored_no8",
        TestGraphAddEdge_SelfLoopIgnored_no8
      },
      {
        "TestReadGraphFromFile_EmptyAdjLine_no9",
        TestReadGraphFromFile_EmptyAdjLine_no9
      },
      {
        "TestReadGraphFromFile_WithJunkAndSpaces_no10",
        TestReadGraphFromFile_WithJunkAndSpaces_no10
      },
      {
        "TestFindPath_LargeChain200_no11",
        TestFindPath_LargeChain200_no11
      },
    };

    int total = (int)(sizeof(tests) / sizeof(tests[0]));
    int passed = 0;

    printf("Running %d tests...\n", total);
    for (int i = 0; i < total; i++) {
        printf("- %s\n", tests[i].name);
        int ok = tests[i].fn();
        if (ok) {
            passed++;
        }
        else {
            printf("  => FAILED: %s\n", tests[i].name);
        }
    }

    printf("Passed %d/%d tests.\n", passed, total);
    return (passed == total) ? 0 : 1;
}