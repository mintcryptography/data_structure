#include "../src/core/Graph.h"
#include <cstdio>
#include <cstring>
#include <cmath>

static int passed = 0;
static int failed = 0;

static void check(bool condition, const char* testName) {
    if (condition) {
        printf("  [PASS] %s\n", testName);
        passed++;
    } else {
        printf("  [FAIL] %s\n", testName);
        failed++;
    }
}

static bool floatEq(float a, float b) {
    return fabs(a - b) < 0.01f;
}

// ====== 基础构建 ======
static void testBuild() {
    printf("\n=== 基础构建测试 ===\n");

    Graph g;
    check(g.getVertexCount() == 0, "空图 0 个节点");

    check(g.addVertex(100, "Hall", 1), "添加大厅");
    check(g.addVertex(101, "Registry", 1), "添加挂号处");
    check(g.addVertex(102, "Pharmacy", 1), "添加药房");
    check(g.getVertexCount() == 3, "3 个节点");

    check(!g.addVertex(100, "dup", 1), "重复添加失败");

    const Graph::VertexInfo* v = g.getVertexInfo(100);
    check(v != nullptr, "能查到大厅");
    check(strcmp(v->name, "Hall") == 0, "名字=Hall");
    check(v->floor == 1, "楼层=1");

    check(g.getVertexInfo(999) == nullptr, "查不存在的=nullptr");
}

// ====== 简单路径 ======
static void testSimplePath() {
    printf("\n=== 简单路径测试 ===\n");

    // Hall --3-- Registry --2-- Pharmacy
    Graph g;
    g.addVertex(100, "Hall", 1);
    g.addVertex(101, "Registry", 1);
    g.addVertex(102, "Pharmacy", 1);

    g.addEdge(100, 101, 3.0f);
    g.addEdge(101, 102, 2.0f);

    int path[256];
    int pathLen;

    float dist = g.dijkstra(100, 102, path, pathLen);
    check(floatEq(dist, 5.0f), "Hall->Pharmacy 距离=5");
    check(pathLen == 3, "路径经过 3 个节点");
    check(path[0] == 100, "起点=100");
    check(path[1] == 101, "中间=101");
    check(path[2] == 102, "终点=102");
}

// ====== 多条路径选最短 ======
static void testShortestPath() {
    printf("\n=== 多条路径选最短 ===\n");

    //       2
    //  A -------> B
    //  |          |
    //  |1         |1
    //  v          v
    //  C -------> D
    //       1
    // A->B->D = 3, A->C->D = 2，应选后者

    Graph g;
    g.addVertex(1, "A", 1);
    g.addVertex(2, "B", 1);
    g.addVertex(3, "C", 1);
    g.addVertex(4, "D", 1);

    g.addEdge(1, 2, 2.0f);
    g.addEdge(1, 3, 1.0f);
    g.addEdge(2, 4, 1.0f);
    g.addEdge(3, 4, 1.0f);

    int path[256];
    int pathLen;
    float dist = g.dijkstra(1, 4, path, pathLen);

    check(floatEq(dist, 2.0f), "最短距离=2");
    check(pathLen == 3, "路径 3 个节点");
    check(path[0] == 1 && path[1] == 3 && path[2] == 4, "路径=A->C->D");
}

// ====== 跨楼层（模拟医院） ======
static void testMultiFloor() {
    printf("\n=== 跨楼层测试(模拟医院) ===\n");

    Graph g;

    // 1F
    g.addVertex(100, "Hall", 1);
    g.addVertex(101, "Registry", 1);
    g.addVertex(150, "Elev1F", 1);

    // 2F
    g.addVertex(250, "Elev2F", 2);
    g.addVertex(201, "Internal1", 2);
    g.addVertex(202, "Internal2", 2);

    // 6F
    g.addVertex(650, "Elev6F", 6);
    g.addVertex(601, "Neurology", 6);

    g.addEdge(100, 101, 3.0f);
    g.addEdge(100, 150, 5.0f);

    g.addEdge(150, 250, 2.0f);   // 1F -> 2F
    g.addEdge(250, 650, 8.0f);   // 2F -> 6F

    g.addEdge(250, 201, 2.0f);
    g.addEdge(201, 202, 3.0f);

    g.addEdge(650, 601, 4.0f);

    int path[256];
    int pathLen;

    // Hall -> Neurology: 100 -> 150 -> 250 -> 650 -> 601
    float dist = g.dijkstra(100, 601, path, pathLen);
    check(floatEq(dist, 19.0f), "Hall->Neurology=19");
    check(pathLen == 5, "经过 5 个节点");
    check(path[0] == 100, "起点=100");
    check(path[1] == 150, "经过 Elev1F");
    check(path[4] == 601, "终点=Neurology");

    // Hall -> Internal2
    dist = g.dijkstra(100, 202, path, pathLen);
    check(floatEq(dist, 12.0f), "Hall->Internal2=12");

    // Internal1 -> Neurology
    dist = g.dijkstra(201, 601, path, pathLen);
    check(floatEq(dist, 14.0f), "Internal1->Neurology=14");
}

// ====== 不可达 ======
static void testUnreachable() {
    printf("\n=== 不可达测试 ===\n");

    Graph g;
    g.addVertex(1, "A", 1);
    g.addVertex(2, "B", 1);
    g.addVertex(3, "Island", 1);

    g.addEdge(1, 2, 1.0f);

    int path[256];
    int pathLen;
    float dist = g.dijkstra(1, 3, path, pathLen);
    check(dist < 0, "孤岛不可达=-1");
    check(pathLen == 0, "不可达路径长度=0");

    dist = g.dijkstra(999, 1, path, pathLen);
    check(dist < 0, "起点不存在=-1");
}

// ====== 起点=终点 ======
static void testSameStartEnd() {
    printf("\n=== 起点=终点 ===\n");

    Graph g;
    g.addVertex(1, "A", 1);
    g.addVertex(2, "B", 1);
    g.addEdge(1, 2, 5.0f);

    int path[256];
    int pathLen;
    float dist = g.dijkstra(1, 1, path, pathLen);
    check(floatEq(dist, 0.0f), "起点=终点距离=0");
    check(pathLen == 1, "路径只有起点自己");
    check(path[0] == 1, "路径=[1]");
}

// ====== 邻居查询 ======
static void testAdjList() {
    printf("\n=== 邻居查询测试 ===\n");

    Graph g;
    g.addVertex(100, "Hall", 1);
    g.addVertex(101, "Registry", 1);
    g.addVertex(102, "Pharmacy", 1);

    g.addEdge(100, 101, 3.0f);
    g.addEdge(100, 102, 5.0f);

    const Graph::EdgeNode* edge = g.getAdjList(100);
    int neighborCount = 0;
    while (edge) {
        neighborCount++;
        int destId = g.getEdgeDestId(edge);
        check(destId == 101 || destId == 102, "邻居编号正确");
        edge = edge->next;
    }
    check(neighborCount == 2, "Hall 有 2 个邻居");
}

int main() {
    printf("==============================\n");
    printf("  Graph + Dijkstra 单元测试\n");
    printf("==============================\n");

    testBuild();
    testSimplePath();
    testShortestPath();
    testMultiFloor();
    testUnreachable();
    testSameStartEnd();
    testAdjList();

    printf("\n==============================\n");
    printf("  结果: %d 通过, %d 失败\n", passed, failed);
    printf("==============================\n");

    return failed > 0 ? 1 : 0;
}
