#include "../src/core/LinkedList.h"
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

// ====== 基础 int 测试 ======
static void testBasicInt() {
    printf("\n=== 基础 int 测试 ===\n");

    LinkedList<int> list;
    check(list.isEmpty(), "新链表为空");
    check(list.size() == 0, "新链表 size=0");

    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    check(list.size() == 3, "尾插 3 个后 size=3");
    check(*list.front() == 10, "头元素=10");
    check(*list.back() == 30, "尾元素=30");

    check(*list.getAt(0) == 10, "getAt(0)=10");
    check(*list.getAt(1) == 20, "getAt(1)=20");
    check(*list.getAt(2) == 30, "getAt(2)=30");
    check(list.getAt(3) == nullptr, "getAt(3) 越界=nullptr");
    check(list.getAt(-1) == nullptr, "getAt(-1) 越界=nullptr");
}

// ====== 头插 ======
static void testInsertFront() {
    printf("\n=== 头插测试 ===\n");

    LinkedList<int> list;
    list.insertFront(30);
    list.insertFront(20);
    list.insertFront(10);
    check(list.size() == 3, "头插 3 个 size=3");
    check(*list.getAt(0) == 10, "[0]=10");
    check(*list.getAt(1) == 20, "[1]=20");
    check(*list.getAt(2) == 30, "[2]=30");
}

// ====== 位置插入 ======
static void testInsertAt() {
    printf("\n=== 位置插入测试 ===\n");

    LinkedList<int> list;
    list.insertBack(10);
    list.insertBack(30);

    list.insertAt(1, 20);  // [10, 20, 30]
    check(list.size() == 3, "insertAt 后 size=3");
    check(*list.getAt(0) == 10, "[0]=10");
    check(*list.getAt(1) == 20, "[1]=20");
    check(*list.getAt(2) == 30, "[2]=30");

    list.insertAt(0, 5);
    check(*list.getAt(0) == 5, "insertAt(0)=头插");

    list.insertAt(100, 99);
    check(*list.back() == 99, "insertAt(超界)=尾插");
}

// ====== 删除 ======
static void testRemove() {
    printf("\n=== 删除测试 ===\n");

    LinkedList<int> list;
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);
    list.insertBack(40);

    check(list.removeFront(), "removeFront 成功");
    check(list.size() == 3, "删头后 size=3");
    check(*list.front() == 20, "新头=20");

    check(list.removeBack(), "removeBack 成功");
    check(list.size() == 2, "删尾后 size=2");
    check(*list.back() == 30, "新尾=30");

    list.insertBack(40);  // [20, 30, 40]
    check(list.removeAt(1), "removeAt(1) 成功");
    check(list.size() == 2, "按位置删后 size=2");
    check(*list.getAt(0) == 20, "[0]=20");
    check(*list.getAt(1) == 40, "[1]=40");

    check(!list.removeAt(5), "removeAt(5) 越界=false");
    check(!list.removeAt(-1), "removeAt(-1) 越界=false");
}

// ====== 条件删除 ======
static bool isEven(const int& x) { return x % 2 == 0; }
static bool isNegative(const int& x) { return x < 0; }

static void testRemoveIf() {
    printf("\n=== 条件删除测试 ===\n");

    LinkedList<int> list;
    list.insertBack(1);
    list.insertBack(2);
    list.insertBack(3);
    list.insertBack(4);

    check(list.removeIf(isEven), "删除第一个偶数(2) 成功");
    check(list.size() == 3, "删后 size=3");
    check(*list.getAt(0) == 1, "[0]=1");
    check(*list.getAt(1) == 3, "[1]=3");
    check(*list.getAt(2) == 4, "[2]=4");

    check(!list.removeIf(isNegative), "没有负数，删除失败");
}

// ====== 查找 ======
static bool isThirty(const int& x) { return x == 30; }
static bool isHundred(const int& x) { return x == 100; }

static void testFind() {
    printf("\n=== 查找测试 ===\n");

    LinkedList<int> list;
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    int* p = list.find(isThirty);
    check(p != nullptr && *p == 30, "find(30) 成功");

    int* q = list.find(isHundred);
    check(q == nullptr, "find(100)=nullptr");
}

// ====== 遍历 ======
static int forEachSum = 0;
static void addToSum(int& x) { forEachSum += x; }

static void testForEach() {
    printf("\n=== 遍历测试 ===\n");

    LinkedList<int> list;
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    forEachSum = 0;
    list.forEach(addToSum);
    check(forEachSum == 60, "forEach 求和=60");
}

// ====== 手动迭代 ======
static void testIterator() {
    printf("\n=== 手动迭代测试 ===\n");

    LinkedList<int> list;
    list.insertBack(1);
    list.insertBack(2);
    list.insertBack(3);

    int sum = 0;
    LinkedList<int>::Node* cur = list.getHead();
    while (cur) {
        sum += cur->data;
        cur = cur->next;
    }
    check(sum == 6, "手动遍历求和=6");
}

// ====== 清空 ======
static void testClear() {
    printf("\n=== 清空测试 ===\n");

    LinkedList<int> list;
    list.insertBack(1);
    list.insertBack(2);
    list.insertBack(3);

    list.clear();
    check(list.isEmpty(), "clear 后为空");
    check(list.size() == 0, "clear 后 size=0");
    check(list.front() == nullptr, "clear 后 front=nullptr");

    list.insertBack(99);
    check(list.size() == 1, "clear 后重新插入 size=1");
    check(*list.front() == 99, "clear 后重新插入 front=99");
}

// ====== 拷贝 ======
static void testCopy() {
    printf("\n=== 拷贝测试 ===\n");

    LinkedList<int> list;
    list.insertBack(10);
    list.insertBack(20);
    list.insertBack(30);

    LinkedList<int> copy(list);
    check(copy.size() == 3, "拷贝构造 size=3");
    check(*copy.getAt(0) == 10, "拷贝[0]=10");
    check(*copy.getAt(1) == 20, "拷贝[1]=20");
    check(*copy.getAt(2) == 30, "拷贝[2]=30");

    list.removeFront();
    check(copy.size() == 3, "修改原表后拷贝不变");
    check(*copy.getAt(0) == 10, "修改原表后拷贝[0]不变");
}

// ====== 边界 ======
static void testEdgeCases() {
    printf("\n=== 边界测试 ===\n");

    LinkedList<int> list;

    check(!list.removeFront(), "空链表 removeFront=false");
    check(!list.removeBack(), "空链表 removeBack=false");
    check(!list.removeAt(0), "空链表 removeAt(0)=false");
    check(list.front() == nullptr, "空链表 front=nullptr");
    check(list.back() == nullptr, "空链表 back=nullptr");

    list.insertBack(42);
    check(*list.front() == 42, "单元素 front=42");
    check(*list.back() == 42, "单元素 back=42");
    check(list.removeFront(), "单元素 removeFront 成功");
    check(list.isEmpty(), "删完后为空");

    list.insertBack(42);
    check(list.removeBack(), "单元素 removeBack 成功");
    check(list.isEmpty(), "删完后为空");
}

// ====== 结构体类型 ======
struct SimplePatient {
    char name[50];
    int age;
};

static bool isOld(const SimplePatient& p) { return p.age > 60; }

static void testStructType() {
    printf("\n=== 结构体类型测试 ===\n");

    LinkedList<SimplePatient> list;

    SimplePatient p1; strcpy(p1.name, "ZhangSan"); p1.age = 30;
    SimplePatient p2; strcpy(p2.name, "LiSi"); p2.age = 65;
    SimplePatient p3; strcpy(p3.name, "WangWu"); p3.age = 45;

    list.insertBack(p1);
    list.insertBack(p2);
    list.insertBack(p3);

    check(list.size() == 3, "插入 3 个患者");
    check(strcmp(list.getAt(0)->name, "ZhangSan") == 0, "[0]=ZhangSan");

    SimplePatient* old = list.find(isOld);
    check(old != nullptr && strcmp(old->name, "LiSi") == 0, "find age>60=LiSi");
}

int main() {
    printf("==============================\n");
    printf("  LinkedList 单元测试\n");
    printf("==============================\n");

    testBasicInt();
    testInsertFront();
    testInsertAt();
    testRemove();
    testRemoveIf();
    testFind();
    testForEach();
    testIterator();
    testClear();
    testCopy();
    testEdgeCases();
    testStructType();

    printf("\n==============================\n");
    printf("  结果: %d 通过, %d 失败\n", passed, failed);
    printf("==============================\n");

    return failed > 0 ? 1 : 0;
}
