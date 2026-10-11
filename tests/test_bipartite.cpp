#include "../src/core/BipartiteGraph.h"
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
    return fabs(a - b) < 0.001f;
}

// ====== 构建测试 ======
static void testBuild() {
    printf("\n=== 构建测试 ===\n");

    BipartiteGraph bg;
    check(bg.getSymptomCount() == 0, "初始症状数=0");
    check(bg.getDeptCount() == 0, "初始科室数=0");

    int s1 = bg.addSymptom("Headache");
    int s2 = bg.addSymptom("Dizziness");
    bg.addSymptom("Cough");
    bg.addSymptom("Insomnia");
    check(s1 == 0, "Headache 下标=0");
    check(s2 == 1, "Dizziness 下标=1");
    check(bg.getSymptomCount() == 4, "4 个症状");

    int dup = bg.addSymptom("Headache");
    check(dup == 0, "重复添加返回已有下标");
    check(bg.getSymptomCount() == 4, "重复添加不增加数量");

    int d1 = bg.addDepartment("DEPT01");
    int d2 = bg.addDepartment("DEPT02");
    int d3 = bg.addDepartment("DEPT03");
    check(d1 == 0 && d2 == 1 && d3 == 2, "科室下标正确");
    check(bg.getDeptCount() == 3, "3 个科室");
}

// ====== 权重 ======
static void testWeight() {
    printf("\n=== 权重测试 ===\n");

    BipartiteGraph bg;
    bg.addSymptom("Headache");    // 0
    bg.addSymptom("Dizziness");   // 1
    bg.addSymptom("Cough");       // 2
    bg.addSymptom("Insomnia");    // 3

    bg.addDepartment("DEPT01"); // Neurology    0
    bg.addDepartment("DEPT02"); // Pulmonology  1
    bg.addDepartment("DEPT03"); // Psychology   2

    bg.setWeight("Headache", "DEPT01", 0.9f);
    bg.setWeight("Headache", "DEPT03", 0.2f);
    bg.setWeight("Dizziness", "DEPT01", 0.85f);
    bg.setWeight("Cough", "DEPT02", 0.9f);
    bg.setWeight("Insomnia", "DEPT01", 0.3f);
    bg.setWeight("Insomnia", "DEPT03", 0.85f);

    check(floatEq(bg.getWeight("Headache", "DEPT01"), 0.9f), "Headache->Neurology=0.9");
    check(floatEq(bg.getWeight("Cough", "DEPT02"), 0.9f), "Cough->Pulmonology=0.9");
    check(floatEq(bg.getWeight("Headache", "DEPT02"), 0.0f), "Headache->Pulmonology=0");

    check(floatEq(bg.getWeight("Diarrhea", "DEPT01"), 0.0f), "不存在的症状返回 0");
}

// ====== 导诊算法 ======
static void testDiagnose() {
    printf("\n=== 导诊算法测试 ===\n");

    BipartiteGraph bg;
    bg.addSymptom("Headache");    // 0
    bg.addSymptom("Dizziness");   // 1
    bg.addSymptom("Cough");       // 2
    bg.addSymptom("Insomnia");    // 3

    bg.addDepartment("DEPT01");
    bg.addDepartment("DEPT02");
    bg.addDepartment("DEPT03");

    bg.setWeight("Headache", "DEPT01", 0.9f);
    bg.setWeight("Headache", "DEPT03", 0.2f);
    bg.setWeight("Dizziness", "DEPT01", 0.85f);
    bg.setWeight("Cough", "DEPT02", 0.9f);
    bg.setWeight("Insomnia", "DEPT01", 0.3f);
    bg.setWeight("Insomnia", "DEPT03", 0.85f);

    // 场景 1：Headache + Insomnia
    int matched[10];
    int matchCount = bg.extractSymptoms("I have Headache and Insomnia", matched, 10);
    check(matchCount == 2, "匹配到 2 个症状");

    float scores[20];
    int deptCount = bg.calculateScores(matched, matchCount, scores);
    check(deptCount == 3, "3 个科室参与评分");

    // Neurology: (0.9 + 0.3) / 2 = 0.6
    // Pulmonology: 0
    // Psychology: (0.2 + 0.85) / 2 = 0.525
    check(floatEq(scores[0], 0.6f), "Neurology 得分=0.6");
    check(floatEq(scores[1], 0.0f), "Pulmonology 得分=0");
    check(floatEq(scores[2], 0.525f), "Psychology 得分=0.525");

    int bestDept = 0;
    for (int i = 1; i < deptCount; i++) {
        if (scores[i] > scores[bestDept]) bestDept = i;
    }
    check(strcmp(bg.getDeptId(bestDept), "DEPT01") == 0, "推荐第一的是 Neurology");

    // 场景 2：Cough
    matchCount = bg.extractSymptoms("Cough", matched, 10);
    check(matchCount == 1, "匹配到 1 个症状");

    bg.calculateScores(matched, matchCount, scores);
    check(floatEq(scores[1], 0.9f), "Cough->Pulmonology=0.9");
    check(scores[1] > scores[0] && scores[1] > scores[2], "Pulmonology 得分最高");
}

// ====== 无匹配 ======
static void testNoMatch() {
    printf("\n=== 无匹配测试 ===\n");

    BipartiteGraph bg;
    bg.addSymptom("Headache");
    bg.addDepartment("DEPT01");
    bg.setWeight("Headache", "DEPT01", 0.9f);

    int matched[10];
    int matchCount = bg.extractSymptoms("I feel fine today", matched, 10);
    check(matchCount == 0, "没有匹配症状");

    float scores[20];
    bg.calculateScores(matched, matchCount, scores);
    check(floatEq(scores[0], 0.0f), "无匹配时得分=0");
}

// ====== 查找 ======
static void testFind() {
    printf("\n=== 查找测试 ===\n");

    BipartiteGraph bg;
    bg.addSymptom("Headache");
    bg.addSymptom("Insomnia");
    bg.addDepartment("DEPT01");

    check(bg.findSymptom("Headache") == 0, "findSymptom(Headache)=0");
    check(bg.findSymptom("Insomnia") == 1, "findSymptom(Insomnia)=1");
    check(bg.findSymptom("Diarrhea") == -1, "findSymptom(Diarrhea)=-1");

    check(bg.findDepartment("DEPT01") == 0, "findDepartment 正确");
    check(bg.findDepartment("DEPT99") == -1, "findDepartment 不存在=-1");
}

int main() {
    printf("==============================\n");
    printf("  BipartiteGraph 单元测试\n");
    printf("==============================\n");

    testBuild();
    testWeight();
    testDiagnose();
    testNoMatch();
    testFind();

    printf("\n==============================\n");
    printf("  结果: %d 通过, %d 失败\n", passed, failed);
    printf("==============================\n");

    return failed > 0 ? 1 : 0;
}
