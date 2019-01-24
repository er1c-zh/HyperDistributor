//
// Created by eric on 19-1-24.
//

#include "HyperDistributor/ThreadSafeUseLockDeque.h"

#include <mutex>

using namespace hd;

ThreadSafeUseLockDeque::ThreadSafeUseLockDeque() {
    head = nullptr;
    tail = nullptr;
};

ThreadSafeUseLockDeque::~ThreadSafeUseLockDeque() {
    mutex.lock();
    while (head != nullptr) {
        Node* tmp = head->getNextNode();
        delete(head);
        head = tmp;
    }
    mutex.unlock();
}

Node* ThreadSafeUseLockDeque::dequeue() {
    if (head == nullptr) {
        return nullptr;
    }

    mutex.lock();

    Node* r = head;

    this->head = this->head->getNextNode();

    if (this->head == nullptr) {
        this->tail = nullptr;
    } else {
        this->head->setPreNode(nullptr);
    }

    r->setPreNode(nullptr);
    r->setNextNode(nullptr);

    mutex.unlock();
    return r;
}

void ThreadSafeUseLockDeque::append(hd::Node *node) {
    mutex.lock();

    // clean node
    node->setPreNode(nullptr);
    node->setNextNode(nullptr);

    if(tail == nullptr) {
        head = node;
        tail = node;
    } else {
        node->setPreNode(tail);
        tail->setNextNode(node);
        tail = node;
    }

    mutex.unlock();
}

