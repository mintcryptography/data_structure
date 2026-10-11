# 医院门诊预约与智能导诊系统

数据结构课设项目。本仓库当前完成 **Step 1：手写数据结构 + 单元测试**。

## 当前目录结构

```
data_structure/
├── src/core/              手写数据结构（纯头文件，不依赖 STL 容器）
│   ├── LinkedList.h       通用单链表（带头尾指针，模板）
│   ├── HashMap.h          字符串键哈希表（拉链法 + djb2 哈希）
│   ├── Queue.h            链表队列（支持过号 moveToRear）
│   ├── Graph.h            加权无向图（邻接表）+ Dijkstra 最短路径
│   └── BipartiteGraph.h   加权二部图（邻接矩阵）+ 导诊匹配算法
│
├── tests/                 对应的单元测试
│   ├── test_linkedlist.cpp
│   ├── test_hashmap.cpp
│   ├── test_queue.cpp
│   ├── test_graph.cpp
│   └── test_bipartite.cpp
│
├── build.ps1              Windows 一键编译+测试脚本（MinGW g++）
├── CMakeLists.txt         跨平台 CMake 构建配置
└── README.md
```

## 编译并运行测试

### 方式一：PowerShell 一键脚本（当前环境已验证）

```powershell
# 跑全部 5 个测试
.\build.ps1

# 只跑单个
.\build.ps1 queue
.\build.ps1 graph
```

需要 PATH 里有 `g++`（MinGW/MSYS2 任选一）。产物输出到 `build/`。

### 方式二：CMake

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
ctest --test-dir build --output-on-failure
```

## 数据结构选型与用途对照

| 文件 | 结构 | 项目中的用途 |
|---|---|---|
| `LinkedList.h` | 单链表（头尾指针） | 患者的"我的预约"链表、科室下挂的医生列表、同一(科室+日期)下的排班链表 |
| `HashMap.h` | 哈希表（拉链法） | 医生/患者按 ID 查、排班按 `DEPT_日期` 或 `医生_日期_时段` 查、预约按 ID 查 |
| `Queue.h` | 链表队列 | 候诊队列：入队签到、出队叫号、过号到队尾 |
| `Graph.h` | 加权无向图 + Dijkstra | 楼层房间地图、就诊路线最短路径计算、跨楼层电梯节点 |
| `BipartiteGraph.h` | 加权二部图（邻接矩阵） | 症状-科室关联表、智能导诊的科室评分 |

## 当前测试覆盖

| 测试 | 用例数 | 状态 |
|---|---:|:---:|
| test_linkedlist | 67 | ✅ |
| test_hashmap | 36 | ✅ |
| test_queue | 36 | ✅ |
| test_graph | 34 | ✅ |
| test_bipartite | 29 | ✅ |
| **总计** | **202** | **全部通过** |

覆盖类型：正常操作、空/单元素/满等边界、业务场景（候诊过号、跨楼层导航、症状多匹配打分）。

## 下一步计划（Step 2 起）

- `src/models/`：Department / Doctor / Schedule / Patient / Appointment / Symptom 结构体
- `src/services/DataManager`：读入 `data/*.txt`，装配到上述数据结构
- `src/services/`：TriageService / BookingService / RouteService / QueueService
- `src/ui/`：Dear ImGui 面板
- `data/`：≥10 科室、≥30 医生的测试数据
