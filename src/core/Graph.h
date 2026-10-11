#ifndef GRAPH_H
#define GRAPH_H

#include <cstring>

// 手写加权无向图（邻接表实现）+ Dijkstra 最短路径
// 节点用 int 编号（如 100=大厅, 201=内科1, 601=神经内科）
// 支持跨楼层边（如电梯连接不同楼层）

class Graph {
public:
    // 邻接表中的边节点
    struct EdgeNode {
        int dest;           // 目标节点下标（不是 id）
        float weight;       // 边权
        EdgeNode* next;

        EdgeNode() : dest(0), weight(0), next(nullptr) {}
        EdgeNode(int d, float w) : dest(d), weight(w), next(nullptr) {}
    };

    // 节点信息
    struct VertexInfo {
        int id;             // 节点编号
        char name[64];
        int floor;
        bool active;

        VertexInfo() : id(-1), floor(0), active(false) {
            name[0] = '\0';
        }
    };

private:
    static const int MAX_VERTICES = 256;
    static const int MAX_ID = 10000;   // 节点编号 < 10000

    VertexInfo vertices[MAX_VERTICES];
    EdgeNode* adjList[MAX_VERTICES];
    int vertexCount;

    // 节点编号 → 数组下标（因为编号可能不连续，如 100, 101, 201）
    int idToIndex[MAX_ID];
    int indexToId[MAX_VERTICES];

    int findIndex(int id) const {
        if (id < 0 || id >= MAX_ID) return -1;
        int idx = idToIndex[id];
        if (idx >= 0 && idx < vertexCount && vertices[idx].active) {
            return idx;
        }
        return -1;
    }

public:
    // ========== 构造 / 析构 ==========

    Graph() : vertexCount(0) {
        for (int i = 0; i < MAX_VERTICES; i++) {
            adjList[i] = nullptr;
        }
        for (int i = 0; i < MAX_ID; i++) {
            idToIndex[i] = -1;
        }
    }

    ~Graph() {
        for (int i = 0; i < vertexCount; i++) {
            EdgeNode* cur = adjList[i];
            while (cur) {
                EdgeNode* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
        }
    }

    Graph(const Graph&) = delete;
    Graph& operator=(const Graph&) = delete;

    // ========== 添加节点 ==========

    bool addVertex(int id, const char* name, int floor) {
        if (vertexCount >= MAX_VERTICES) return false;
        if (id < 0 || id >= MAX_ID) return false;
        if (idToIndex[id] != -1) return false;  // 已存在

        int idx = vertexCount;
        vertices[idx].id = id;
        strncpy(vertices[idx].name, name, 63);
        vertices[idx].name[63] = '\0';
        vertices[idx].floor = floor;
        vertices[idx].active = true;

        idToIndex[id] = idx;
        indexToId[idx] = id;
        vertexCount++;
        return true;
    }

    // ========== 添加边 ==========

    // 无向边，自动添加双向
    bool addEdge(int fromId, int toId, float weight) {
        int fromIdx = findIndex(fromId);
        int toIdx = findIndex(toId);
        if (fromIdx < 0 || toIdx < 0) return false;

        EdgeNode* edge1 = new EdgeNode(toIdx, weight);
        edge1->next = adjList[fromIdx];
        adjList[fromIdx] = edge1;

        EdgeNode* edge2 = new EdgeNode(fromIdx, weight);
        edge2->next = adjList[toIdx];
        adjList[toIdx] = edge2;

        return true;
    }

    // ========== Dijkstra 最短路径 ==========

    // 求 startId → endId 的最短路径
    // path[]：输出路径节点编号序列（调用者提供数组）
    // pathLen：输出路径长度
    // 返回最短距离，不可达返回 -1
    float dijkstra(int startId, int endId, int* path, int& pathLen) const {
        int startIdx = findIndex(startId);
        int endIdx = findIndex(endId);
        if (startIdx < 0 || endIdx < 0) {
            pathLen = 0;
            return -1;
        }

        const float INF = 1e9f;
        float dist[MAX_VERTICES];
        int prev[MAX_VERTICES];
        bool visited[MAX_VERTICES];

        for (int i = 0; i < vertexCount; i++) {
            dist[i] = INF;
            prev[i] = -1;
            visited[i] = false;
        }

        dist[startIdx] = 0;

        for (int iter = 0; iter < vertexCount; iter++) {
            // 找未访问中距离最小的节点
            int u = -1;
            float minDist = INF;
            for (int i = 0; i < vertexCount; i++) {
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    u = i;
                }
            }

            if (u == -1) break;          // 剩余不可达
            if (u == endIdx) break;      // 已找到终点
            visited[u] = true;

            EdgeNode* edge = adjList[u];
            while (edge) {
                int v = edge->dest;
                float newDist = dist[u] + edge->weight;
                if (newDist < dist[v]) {
                    dist[v] = newDist;
                    prev[v] = u;
                }
                edge = edge->next;
            }
        }

        if (dist[endIdx] >= INF) {
            pathLen = 0;
            return -1;
        }

        // 回溯并翻转
        int reversePath[MAX_VERTICES];
        int reverseLen = 0;
        int cur = endIdx;
        while (cur != -1) {
            reversePath[reverseLen++] = indexToId[cur];
            cur = prev[cur];
        }

        pathLen = reverseLen;
        for (int i = 0; i < reverseLen; i++) {
            path[i] = reversePath[reverseLen - 1 - i];
        }

        return dist[endIdx];
    }

    // ========== 查询 ==========

    int getVertexCount() const { return vertexCount; }

    const VertexInfo* getVertexInfo(int id) const {
        int idx = findIndex(id);
        if (idx < 0) return nullptr;
        return &vertices[idx];
    }

    const EdgeNode* getAdjList(int id) const {
        int idx = findIndex(id);
        if (idx < 0) return nullptr;
        return adjList[idx];
    }

    // 边节点中存的 dest 是数组下标，这里转成实际的节点编号
    int getEdgeDestId(const EdgeNode* edge) const {
        if (!edge || edge->dest < 0 || edge->dest >= vertexCount) return -1;
        return indexToId[edge->dest];
    }
};

#endif
