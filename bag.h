#include <atomic>
#include <optional>

template<class T>
struct Node {
    T data;
    Node* next;
};

template<class T>
class Bag {
private:
    std::atomic<Node*> head;
    std::atomic<Node*> toDelete;
public:
    Bag() : head{nullptr}, toDelete{nullptr} {}
    ~Bag();
    void push(T data);
    std::optional<T> pop();
    bool empty() {return head == nullptr};
    auto claim();
};

template<class T>
Bag<T>::~Bag() {
    while (!empty()) {
        pop();//Dump everything into toDelete
    }

    Node* current = toDelete;
    while (current != nullptr) {
        Node* next = current->next;
        delete current;
        current = next;
    }
}

template<class T>
void Bag<T>::push(T data) {
    Node* newNode = new Node{data, head};
    while (!head.compare_exchange_weak(newNode->next, newNode));
}

template<class T>
std::optional<T> Bag<T>::pop() {
    if(empty()) return std::nullopt;

    Node* oldHead = head;
    while (!head.compare_exchange_weak(oldHead, oldHead->next));

    oldHead->next = toDelete;
    while(!toDelete.compare_exchange_weak(oldHead->next, oldHead));

    return oldHead->data;
}

template<class T>
auto Bag<T>::claim() {
    auto oldHead = head;
    while (!head.compare_exchange_weak(oldHead, nullptr));
    return oldHead;
}
