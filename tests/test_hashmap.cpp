#include "../src/core/HashMap.h"
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

// ====== 基础 int 值 ======
static void testBasicInt() {
    printf("\n=== 基础 int 值测试 ===\n");

    HashMap<int> map;
    check(map.isEmpty(), "新 map 为空");
    check(map.size() == 0, "新 map size=0");

    map.put("apple", 1);
    map.put("banana", 2);
    map.put("cherry", 3);

    check(map.size() == 3, "插入 3 个后 size=3");
    check(map.contains("apple"), "contains(apple)=true");
    check(!map.contains("grape"), "contains(grape)=false");

    int* val = map.get("banana");
    check(val != nullptr && *val == 2, "get(banana)=2");

    int* missing = map.get("grape");
    check(missing == nullptr, "get(grape)=nullptr");
}

// ====== 更新 ======
static void testUpdate() {
    printf("\n=== 更新测试 ===\n");

    HashMap<int> map;
    map.put("score", 80);
    check(*map.get("score") == 80, "初始值=80");

    map.put("score", 95);
    check(*map.get("score") == 95, "更新后=95");
    check(map.size() == 1, "更新不增加 size");
}

// ====== 删除 ======
static void testRemove() {
    printf("\n=== 删除测试 ===\n");

    HashMap<int> map;
    map.put("a", 1);
    map.put("b", 2);
    map.put("c", 3);

    check(map.remove("b"), "remove(b) 成功");
    check(map.size() == 2, "删后 size=2");
    check(map.get("b") == nullptr, "get(b)=nullptr");
    check(map.get("a") != nullptr, "get(a) 仍在");
    check(map.get("c") != nullptr, "get(c) 仍在");

    check(!map.remove("d"), "remove 不存在=false");
    check(map.size() == 2, "删不存在不影响 size");
}

// ====== 拼接 key（模拟排班） ======
static void testCompositeKey() {
    printf("\n=== 拼接 key 测试(排班场景) ===\n");

    HashMap<int> scheduleMap;

    scheduleMap.put("DEPT03_2026-03-15", 20);
    scheduleMap.put("DEPT03_2026-03-16", 15);
    scheduleMap.put("DEPT01_2026-03-15", 18);

    int* v = scheduleMap.get("DEPT03_2026-03-15");
    check(v != nullptr && *v == 20, "DEPT03_3月15日=20 号");

    v = scheduleMap.get("DEPT01_2026-03-15");
    check(v != nullptr && *v == 18, "DEPT01_3月15日=18 号");

    v = scheduleMap.get("DEPT03_2026-03-17");
    check(v == nullptr, "3月17日无排班");
}

// ====== 结构体值（模拟医生） ======
struct TestDoctor {
    char name[50];
    char title[30];
    int age;
};

static void testStructValue() {
    printf("\n=== 结构体值测试(医生) ===\n");

    HashMap<TestDoctor> doctorMap;

    TestDoctor d1;
    strcpy(d1.name, "Wang");
    strcpy(d1.title, "ChiefDoctor");
    d1.age = 52;
    doctorMap.put("D005", d1);

    TestDoctor d2;
    strcpy(d2.name, "Li");
    strcpy(d2.title, "AssocDoctor");
    d2.age = 45;
    doctorMap.put("D012", d2);

    TestDoctor* found = doctorMap.get("D005");
    check(found != nullptr, "找到 D005");
    check(strcmp(found->name, "Wang") == 0, "D005 名字=Wang");
    check(found->age == 52, "D005 年龄=52");

    TestDoctor* found2 = doctorMap.get("D012");
    check(found2 != nullptr && strcmp(found2->title, "AssocDoctor") == 0, "D012 职称正确");
}

// ====== 遍历 ======
static int forEachCount = 0;
static int forEachSum = 0;

static void countAndSum(const char* key, int& value) {
    (void)key;
    forEachCount++;
    forEachSum += value;
}

static void testForEach() {
    printf("\n=== 遍历测试 ===\n");

    HashMap<int> map;
    map.put("a", 10);
    map.put("b", 20);
    map.put("c", 30);

    forEachCount = 0;
    forEachSum = 0;
    map.forEach(countAndSum);

    check(forEachCount == 3, "遍历了 3 个元素");
    check(forEachSum == 60, "值总和=60");
}

// ====== 清空 ======
static void testClear() {
    printf("\n=== 清空测试 ===\n");

    HashMap<int> map;
    map.put("x", 1);
    map.put("y", 2);
    map.put("z", 3);

    map.clear();
    check(map.isEmpty(), "clear 后为空");
    check(map.size() == 0, "clear 后 size=0");
    check(map.get("x") == nullptr, "clear 后 get(x)=nullptr");

    map.put("new", 99);
    check(map.size() == 1, "清空后重新插入 size=1");
    check(*map.get("new") == 99, "清空后重新插入能取到");
}

// ====== 大量数据（验证哈希冲突处理） ======
static void testManyEntries() {
    printf("\n=== 大量数据测试 ===\n");

    HashMap<int> map;
    char key[32];

    for (int i = 0; i < 500; i++) {
        sprintf(key, "key_%d", i);
        map.put(key, i * 10);
    }
    check(map.size() == 500, "插入 500 个 size=500");

    bool allFound = true;
    for (int i = 0; i < 500; i++) {
        sprintf(key, "key_%d", i);
        int* v = map.get(key);
        if (!v || *v != i * 10) {
            allFound = false;
            break;
        }
    }
    check(allFound, "500 个元素全部正确找到");

    for (int i = 0; i < 100; i++) {
        sprintf(key, "key_%d", i);
        map.remove(key);
    }
    check(map.size() == 400, "删 100 个后 size=400");

    sprintf(key, "key_50");
    check(map.get(key) == nullptr, "已删的 key_50 找不到");

    sprintf(key, "key_200");
    check(*map.get(key) == 2000, "未删的 key_200 仍在");
}

int main() {
    printf("==============================\n");
    printf("  HashMap 单元测试\n");
    printf("==============================\n");

    testBasicInt();
    testUpdate();
    testRemove();
    testCompositeKey();
    testStructValue();
    testForEach();
    testClear();
    testManyEntries();

    printf("\n==============================\n");
    printf("  结果: %d 通过, %d 失败\n", passed, failed);
    printf("==============================\n");

    return failed > 0 ? 1 : 0;
}
