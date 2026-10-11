#include "../src/core/Queue.h"
#include <cstdio>
#include <cstring>

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

// ====== 基础入队出队 ======
static void testBasic() {
    printf("\n=== 基础入队出队 ===\n");

    Queue<int> q;
    check(q.isEmpty(), "新队列为空");
    check(q.size() == 0, "size=0");
    check(q.peek() == nullptr, "peek=nullptr");

    q.enqueue(10);
    q.enqueue(20);
    q.enqueue(30);
    check(q.size() == 3, "入队 3 个 size=3");
    check(*q.peek() == 10, "peek=10 (先入先出)");

    int val;
    check(q.dequeue(val) && val == 10, "出队=10");
    check(q.dequeue(val) && val == 20, "出队=20");
    check(q.dequeue(val) && val == 30, "出队=30");
    check(q.isEmpty(), "全部出队后为空");

    check(!q.dequeue(val), "空队列出队=false");
}

// ====== 过号 ======
static void testMoveToRear() {
    printf("\n=== 过号测试 ===\n");

    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    // 1 号过号 -> [2, 3, 1]
    check(q.moveToRear(), "过号成功");
    check(*q.peek() == 2, "过号后队头=2");

    int val;
    q.dequeue(val); check(val == 2, "出队=2");
    q.dequeue(val); check(val == 3, "出队=3");
    q.dequeue(val); check(val == 1, "出队=1 (过号的)");
    check(q.isEmpty(), "全部出队后为空");
}

// ====== 单元素过号 ======
static void testMoveToRearSingle() {
    printf("\n=== 单元素过号 ===\n");

    Queue<int> q;
    q.enqueue(42);

    check(!q.moveToRear(), "单元素过号=false");
    check(*q.peek() == 42, "peek 仍是 42");
}

// ====== 查位置 ======
static int searchTarget = 0;
static bool isTarget(const int& x) { return x == searchTarget; }

static void testGetPosition() {
    printf("\n=== 查位置测试 ===\n");

    Queue<int> q;
    q.enqueue(101);
    q.enqueue(102);
    q.enqueue(103);
    q.enqueue(104);

    searchTarget = 101;
    check(q.getPosition(isTarget) == 0, "患者 101 在位置 0");

    searchTarget = 103;
    check(q.getPosition(isTarget) == 2, "患者 103 在位置 2");

    searchTarget = 999;
    check(q.getPosition(isTarget) == -1, "患者 999 不在队列中");
}

// ====== 遍历 ======
static int forEachCount = 0;
static void countItem(const int& x) { (void)x; forEachCount++; }

static void testForEach() {
    printf("\n=== 遍历测试 ===\n");

    Queue<int> q;
    q.enqueue(1);
    q.enqueue(2);
    q.enqueue(3);

    forEachCount = 0;
    q.forEach(countItem);
    check(forEachCount == 3, "遍历了 3 个元素");
}

// ====== 候诊场景 ======
struct WaitInfo {
    char patientId[16];
    char patientName[50];
    int queueNumber;
};

static bool isPatient102(const WaitInfo& w) {
    return strcmp(w.patientId, "P102") == 0;
}

static void testWaitingScene() {
    printf("\n=== 候诊场景模拟 ===\n");

    Queue<WaitInfo> waitQueue;

    WaitInfo w1; strcpy(w1.patientId, "P101"); strcpy(w1.patientName, "ZhangSan"); w1.queueNumber = 1;
    WaitInfo w2; strcpy(w2.patientId, "P102"); strcpy(w2.patientName, "LiSi"); w2.queueNumber = 2;
    WaitInfo w3; strcpy(w3.patientId, "P103"); strcpy(w3.patientName, "WangWu"); w3.queueNumber = 3;

    waitQueue.enqueue(w1);
    waitQueue.enqueue(w2);
    waitQueue.enqueue(w3);

    WaitInfo called;
    check(waitQueue.dequeue(called), "叫号成功");
    check(strcmp(called.patientName, "ZhangSan") == 0, "叫到 ZhangSan");

    check(waitQueue.getPosition(isPatient102) == 0, "LiSi 在队头(位置 0)");

    check(waitQueue.moveToRear(), "LiSi 过号成功");
    check(strcmp(waitQueue.peek()->patientName, "WangWu") == 0, "过号后队头=WangWu");

    check(waitQueue.size() == 2, "队列剩 2 人");
}

// ====== 边界 ======
static void testEdgeCases() {
    printf("\n=== 边界测试 ===\n");

    Queue<int> q;

    int val;
    check(!q.dequeue(val), "空队列出队=false");
    check(!q.moveToRear(), "空队列过号=false");
    check(q.peek() == nullptr, "空队列 peek=nullptr");

    q.enqueue(1);
    q.dequeue(val);
    q.enqueue(2);
    q.dequeue(val);
    check(val == 2, "交替入队出队正确");
    check(q.isEmpty(), "交替后为空");

    q.enqueue(1);
    q.enqueue(2);
    q.clear();
    check(q.isEmpty(), "clear 后为空");
    check(q.size() == 0, "clear 后 size=0");

    q.enqueue(99);
    check(*q.peek() == 99, "clear 后重新入队正常");
}

int main() {
    printf("==============================\n");
    printf("  Queue 单元测试\n");
    printf("==============================\n");

    testBasic();
    testMoveToRear();
    testMoveToRearSingle();
    testGetPosition();
    testForEach();
    testWaitingScene();
    testEdgeCases();

    printf("\n==============================\n");
    printf("  结果: %d 通过, %d 失败\n", passed, failed);
    printf("==============================\n");

    return failed > 0 ? 1 : 0;
}
