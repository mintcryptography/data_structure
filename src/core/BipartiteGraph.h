#ifndef BIPARTITE_GRAPH_H
#define BIPARTITE_GRAPH_H

#include <cstring>

// 加权二部图（邻接矩阵实现）
// 左侧：症状集合（最多 100 个）
// 右侧：科室集合（最多 20 个）
// 边权：症状与科室的关联度 0.0 ~ 1.0

class BipartiteGraph {
public:
    static const int MAX_SYMPTOMS = 100;
    static const int MAX_DEPARTMENTS = 20;

private:
    char symptomNames[MAX_SYMPTOMS][50];
    int symptomCount;

    char deptIds[MAX_DEPARTMENTS][16];
    int deptCount;

    // 邻接矩阵：weight[症状下标][科室下标]
    float weight[MAX_SYMPTOMS][MAX_DEPARTMENTS];

public:
    BipartiteGraph() : symptomCount(0), deptCount(0) {
        for (int i = 0; i < MAX_SYMPTOMS; i++)
            for (int j = 0; j < MAX_DEPARTMENTS; j++)
                weight[i][j] = 0.0f;
    }

    // ========== 构建 ==========

    // 添加症状，返回下标；重复则返回已有下标；满返回 -1
    int addSymptom(const char* name) {
        for (int i = 0; i < symptomCount; i++) {
            if (strcmp(symptomNames[i], name) == 0) return i;
        }
        if (symptomCount >= MAX_SYMPTOMS) return -1;

        strncpy(symptomNames[symptomCount], name, 49);
        symptomNames[symptomCount][49] = '\0';
        return symptomCount++;
    }

    // 注册科室（二部图右侧顶点集）
    int addDepartment(const char* deptId) {
        for (int i = 0; i < deptCount; i++) {
            if (strcmp(deptIds[i], deptId) == 0) return i;
        }
        if (deptCount >= MAX_DEPARTMENTS) return -1;

        strncpy(deptIds[deptCount], deptId, 15);
        deptIds[deptCount][15] = '\0';
        return deptCount++;
    }

    // 设置边权（症状名 → 科室ID → 权重）
    bool setWeight(const char* symptomName, const char* deptId, float w) {
        int si = findSymptom(symptomName);
        int di = findDepartment(deptId);
        if (si < 0 || di < 0) return false;

        weight[si][di] = w;
        return true;
    }

    bool setWeightByIndex(int symptomIdx, int deptIdx, float w) {
        if (symptomIdx < 0 || symptomIdx >= symptomCount) return false;
        if (deptIdx < 0 || deptIdx >= deptCount) return false;

        weight[symptomIdx][deptIdx] = w;
        return true;
    }

    // ========== 查询 ==========

    float getWeight(const char* symptomName, const char* deptId) const {
        int si = findSymptom(symptomName);
        int di = findDepartment(deptId);
        if (si < 0 || di < 0) return 0.0f;
        return weight[si][di];
    }

    float getWeightByIndex(int symptomIdx, int deptIdx) const {
        if (symptomIdx < 0 || symptomIdx >= symptomCount) return 0.0f;
        if (deptIdx < 0 || deptIdx >= deptCount) return 0.0f;
        return weight[symptomIdx][deptIdx];
    }

    int findSymptom(const char* name) const {
        for (int i = 0; i < symptomCount; i++) {
            if (strcmp(symptomNames[i], name) == 0) return i;
        }
        return -1;
    }

    int findDepartment(const char* deptId) const {
        for (int i = 0; i < deptCount; i++) {
            if (strcmp(deptIds[i], deptId) == 0) return i;
        }
        return -1;
    }

    // ========== 导诊核心算法 ==========

    // 输入：匹配到的症状下标数组
    // 输出：各科室得分（归一化到 0~1）
    int calculateScores(const int* matchedSymptoms, int matchCount,
                        float* scores) const {
        for (int d = 0; d < deptCount; d++) {
            scores[d] = 0.0f;
            for (int s = 0; s < matchCount; s++) {
                int si = matchedSymptoms[s];
                if (si >= 0 && si < symptomCount) {
                    scores[d] += weight[si][d];
                }
            }
            if (matchCount > 0) {
                scores[d] /= matchCount;  // 归一化
            }
        }
        return deptCount;
    }

    // 从患者文本中提取症状（子串匹配）
    int extractSymptoms(const char* text, int* matchedIndices, int maxMatch) const {
        int count = 0;
        for (int i = 0; i < symptomCount && count < maxMatch; i++) {
            if (strstr(text, symptomNames[i]) != nullptr) {
                matchedIndices[count++] = i;
            }
        }
        return count;
    }

    // ========== 状态查询 ==========

    int getSymptomCount() const { return symptomCount; }
    int getDeptCount() const { return deptCount; }

    const char* getSymptomName(int idx) const {
        if (idx < 0 || idx >= symptomCount) return "";
        return symptomNames[idx];
    }

    const char* getDeptId(int idx) const {
        if (idx < 0 || idx >= deptCount) return "";
        return deptIds[idx];
    }
};

#endif
