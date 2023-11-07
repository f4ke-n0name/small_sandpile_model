#ifndef STACK_HEADER
#define STACK_HEADER
#include <stdint.h>

struct Node {
public:
    std::pair<uint16_t, uint16_t> data;
    Node* next;

    Node(uint16_t x, uint16_t y) {
        this->data = std::make_pair(x, y);
        this->next = nullptr;
    };
};

class Stack {
    Node* head;

public:
    Stack() {
        head = nullptr;
    }

    void push(uint16_t x, uint16_t y) {
        Node* temp = new Node(x, y);
        temp->data = std::make_pair(x, y);
        temp->next = head;
        head = temp;
    }

    bool empty() {
        return head == nullptr;
    }

    std::pair<uint16_t, uint16_t> pop() {
        Node* temp;
        temp = head;
        std::pair<uint16_t, uint16_t> value = temp->data;
        head = head->next;
        free(temp);
        return value;
    }
};

#endif