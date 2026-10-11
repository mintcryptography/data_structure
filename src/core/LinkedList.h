#ifndef LINKED_LIST_H
#define LINKED_LIST_H

// 手写单链表（带头尾指针）
// 模板参数 T：存储的元素类型
// 时间复杂度：头插/尾插 O(1)，按位置/条件查找/删除 O(n)

template <typename T>
class LinkedList {
public:
    // 节点定义（公开，方便外部手动遍历）
    struct Node {
        T data;
        Node* next;

        Node() : next(nullptr) {}
        Node(const T& val) : data(val), next(nullptr) {}
    };

private:
    Node* head;     // 头指针
    Node* tail;     // 尾指针（尾插 O(1) 用）
    int count;      // 元素个数

public:
    // ========== 构造 / 析构 ==========

    LinkedList() : head(nullptr), tail(nullptr), count(0) {}

    ~LinkedList() {
        clear();
    }

    // 深拷贝构造
    LinkedList(const LinkedList& other) : head(nullptr), tail(nullptr), count(0) {
        Node* cur = other.head;
        while (cur) {
            insertBack(cur->data);
            cur = cur->next;
        }
    }

    // 赋值运算符
    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            clear();
            Node* cur = other.head;
            while (cur) {
                insertBack(cur->data);
                cur = cur->next;
            }
        }
        return *this;
    }

    // ========== 插入操作 ==========

    // 头插 O(1)
    void insertFront(const T& val) {
        Node* node = new Node(val);
        node->next = head;
        head = node;
        if (!tail) tail = node;
        count++;
    }

    // 尾插 O(1)
    void insertBack(const T& val) {
        Node* node = new Node(val);
        if (!tail) {
            head = tail = node;
        } else {
            tail->next = node;
            tail = node;
        }
        count++;
    }

    // 在指定位置插入（0-based），pos 超出范围则插到末尾
    void insertAt(int pos, const T& val) {
        if (pos <= 0) {
            insertFront(val);
            return;
        }
        if (pos >= count) {
            insertBack(val);
            return;
        }

        Node* prev = head;
        for (int i = 0; i < pos - 1; i++) {
            prev = prev->next;
        }

        Node* node = new Node(val);
        node->next = prev->next;
        prev->next = node;
        count++;
    }

    // ========== 删除操作 ==========

    // 删除头部 O(1)
    bool removeFront() {
        if (!head) return false;

        Node* tmp = head;
        head = head->next;
        if (!head) tail = nullptr;
        delete tmp;
        count--;
        return true;
    }

    // 删除尾部 O(n)（单链表无法 O(1) 删尾）
    bool removeBack() {
        if (!head) return false;

        if (head == tail) {
            delete head;
            head = tail = nullptr;
            count--;
            return true;
        }

        Node* cur = head;
        while (cur->next != tail) {
            cur = cur->next;
        }
        delete tail;
        tail = cur;
        tail->next = nullptr;
        count--;
        return true;
    }

    // 删除第一个满足条件的元素
    bool removeIf(bool (*predicate)(const T&)) {
        if (!head) return false;

        if (predicate(head->data)) {
            return removeFront();
        }

        Node* prev = head;
        Node* cur = head->next;
        while (cur) {
            if (predicate(cur->data)) {
                prev->next = cur->next;
                if (cur == tail) tail = prev;
                delete cur;
                count--;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // 按位置删除
    bool removeAt(int pos) {
        if (pos < 0 || pos >= count) return false;
        if (pos == 0) return removeFront();

        Node* prev = head;
        for (int i = 0; i < pos - 1; i++) {
            prev = prev->next;
        }

        Node* target = prev->next;
        prev->next = target->next;
        if (target == tail) tail = prev;
        delete target;
        count--;
        return true;
    }

    // 清空
    void clear() {
        while (head) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        tail = nullptr;
        count = 0;
    }

    // ========== 查找操作 ==========

    // 按条件查找第一个匹配元素，返回指针（可修改原数据）
    T* find(bool (*predicate)(const T&)) const {
        Node* cur = head;
        while (cur) {
            if (predicate(cur->data)) {
                return &(cur->data);
            }
            cur = cur->next;
        }
        return nullptr;
    }

    // 按位置获取，越界返回 nullptr
    T* getAt(int pos) const {
        if (pos < 0 || pos >= count) return nullptr;

        Node* cur = head;
        for (int i = 0; i < pos; i++) {
            cur = cur->next;
        }
        return &(cur->data);
    }

    T* front() const {
        return head ? &(head->data) : nullptr;
    }

    T* back() const {
        return tail ? &(tail->data) : nullptr;
    }

    // ========== 遍历操作 ==========

    void forEach(void (*func)(T&)) {
        Node* cur = head;
        while (cur) {
            func(cur->data);
            cur = cur->next;
        }
    }

    void forEach(void (*func)(const T&)) const {
        Node* cur = head;
        while (cur) {
            func(cur->data);
            cur = cur->next;
        }
    }

    // ========== 状态查询 ==========

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }

    Node* getHead() const { return head; }
    Node* getTail() const { return tail; }
};

#endif
