#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <cstring>

// 手写哈希表（拉链法解决冲突）
// key 固定为字符串（char*），value 为模板类型 T
// 支持拼接 key，如 "DEPT03_2026-03-15"

template <typename T>
class HashMap {
public:
    // 键值对节点
    struct Entry {
        char key[128];
        T value;
        Entry* next;        // 拉链法的下一个节点

        Entry() : next(nullptr) { key[0] = '\0'; }
        Entry(const char* k, const T& v) : value(v), next(nullptr) {
            strncpy(key, k, 127);
            key[127] = '\0';
        }
    };

private:
    static const int DEFAULT_CAPACITY = 127;  // 桶数量用质数减少冲突

    Entry** buckets;    // 桶数组
    int capacity;
    int count;

    // djb2 字符串哈希
    unsigned int hash(const char* key) const {
        unsigned int h = 5381;
        while (*key) {
            h = ((h << 5) + h) + (unsigned char)(*key);  // h * 33 + c
            key++;
        }
        return h % capacity;
    }

public:
    // ========== 构造 / 析构 ==========

    HashMap() : capacity(DEFAULT_CAPACITY), count(0) {
        buckets = new Entry*[capacity];
        for (int i = 0; i < capacity; i++) {
            buckets[i] = nullptr;
        }
    }

    HashMap(int cap) : capacity(cap > 0 ? cap : DEFAULT_CAPACITY), count(0) {
        buckets = new Entry*[capacity];
        for (int i = 0; i < capacity; i++) {
            buckets[i] = nullptr;
        }
    }

    ~HashMap() {
        clear();
        delete[] buckets;
    }

    // 禁止拷贝（简化实现，避免深拷贝复杂性）
    HashMap(const HashMap&) = delete;
    HashMap& operator=(const HashMap&) = delete;

    // ========== 插入 / 更新 ==========

    void put(const char* key, const T& value) {
        unsigned int idx = hash(key);
        Entry* cur = buckets[idx];

        while (cur) {
            if (strcmp(cur->key, key) == 0) {
                cur->value = value;  // 已存在则更新
                return;
            }
            cur = cur->next;
        }

        // 不存在则头插到桶里
        Entry* entry = new Entry(key, value);
        entry->next = buckets[idx];
        buckets[idx] = entry;
        count++;
    }

    // ========== 查找 ==========

    T* get(const char* key) const {
        unsigned int idx = hash(key);
        Entry* cur = buckets[idx];

        while (cur) {
            if (strcmp(cur->key, key) == 0) {
                return &(cur->value);
            }
            cur = cur->next;
        }
        return nullptr;
    }

    bool contains(const char* key) const {
        return get(key) != nullptr;
    }

    // ========== 删除 ==========

    bool remove(const char* key) {
        unsigned int idx = hash(key);

        Entry* prev = nullptr;
        Entry* cur = buckets[idx];

        while (cur) {
            if (strcmp(cur->key, key) == 0) {
                if (prev) {
                    prev->next = cur->next;
                } else {
                    buckets[idx] = cur->next;
                }
                delete cur;
                count--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    void clear() {
        for (int i = 0; i < capacity; i++) {
            Entry* cur = buckets[i];
            while (cur) {
                Entry* tmp = cur;
                cur = cur->next;
                delete tmp;
            }
            buckets[i] = nullptr;
        }
        count = 0;
    }

    // ========== 遍历 ==========

    void forEach(void (*func)(const char* key, T& value)) {
        for (int i = 0; i < capacity; i++) {
            Entry* cur = buckets[i];
            while (cur) {
                func(cur->key, cur->value);
                cur = cur->next;
            }
        }
    }

    void forEach(void (*func)(const char* key, const T& value)) const {
        for (int i = 0; i < capacity; i++) {
            Entry* cur = buckets[i];
            while (cur) {
                func(cur->key, cur->value);
                cur = cur->next;
            }
        }
    }

    // ========== 状态查询 ==========

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }
    int getCapacity() const { return capacity; }

    Entry** getBuckets() const { return buckets; }
};

#endif
