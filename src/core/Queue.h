#ifndef QUEUE_H
#define QUEUE_H

// 手写链表队列
// 支持：入队、出队、过号（队头移到队尾）、查位置
// 单链表 front/rear 双指针，入队出队过号均为 O(1)

template <typename T>
class Queue {
public:
    struct Node {
        T data;
        Node* next;

        Node() : next(nullptr) {}
        Node(const T& val) : data(val), next(nullptr) {}
    };

private:
    Node* front_;   // 队头（出队端）
    Node* rear_;    // 队尾（入队端）
    int count;

public:
    // ========== 构造 / 析构 ==========

    Queue() : front_(nullptr), rear_(nullptr), count(0) {}

    ~Queue() {
        clear();
    }

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    // ========== 入队 ==========

    // 从队尾入队 O(1)
    void enqueue(const T& val) {
        Node* node = new Node(val);
        if (!rear_) {
            front_ = rear_ = node;
        } else {
            rear_->next = node;
            rear_ = node;
        }
        count++;
    }

    // ========== 出队 ==========

    // 从队头出队 O(1)
    bool dequeue(T& outVal) {
        if (!front_) return false;

        Node* tmp = front_;
        outVal = tmp->data;
        front_ = front_->next;
        if (!front_) rear_ = nullptr;
        delete tmp;
        count--;
        return true;
    }

    // ========== 过号 ==========

    // 把队头移到队尾 O(1)
    // 场景：叫号时患者不在，过号到队尾重新排
    bool moveToRear() {
        if (!front_ || front_ == rear_) return false;  // 空或只有 1 个元素

        Node* oldFront = front_;
        front_ = front_->next;

        oldFront->next = nullptr;
        rear_->next = oldFront;
        rear_ = oldFront;
        return true;
    }

    // ========== 查看 ==========

    T* peek() const {
        return front_ ? &(front_->data) : nullptr;
    }

    // 查某个元素在队列中的位置（0-based），未找到返回 -1
    int getPosition(bool (*predicate)(const T&)) const {
        Node* cur = front_;
        int pos = 0;
        while (cur) {
            if (predicate(cur->data)) return pos;
            cur = cur->next;
            pos++;
        }
        return -1;
    }

    // ========== 遍历 ==========

    void forEach(void (*func)(const T&)) const {
        Node* cur = front_;
        while (cur) {
            func(cur->data);
            cur = cur->next;
        }
    }

    void forEach(void (*func)(T&)) {
        Node* cur = front_;
        while (cur) {
            func(cur->data);
            cur = cur->next;
        }
    }

    // ========== 状态 ==========

    int size() const { return count; }
    bool isEmpty() const { return count == 0; }

    void clear() {
        while (front_) {
            Node* tmp = front_;
            front_ = front_->next;
            delete tmp;
        }
        rear_ = nullptr;
        count = 0;
    }

    Node* getFront() const { return front_; }
};

#endif
