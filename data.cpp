#include "data.h"

int convertStringTimeToMinutes(const string &time) {
    size_t colonPos = time.find(':');
    string hoursStr = time.substr(0, colonPos);
    string minutesStr = time.substr(colonPos + 1);

    int hours = stoi(hoursStr);
    int minutes = stoi(minutesStr);

    return hours * 60 + minutes;
}

string toUpperCase(const string &str) {
    string upperStr = str;
    for (char &c : upperStr) {
        if (c >= 'a' && c <= 'z') {
            c = c - ('a' - 'A');
        }
    }
    return upperStr;
}

// UserPreferences implementation
void UserPreferences::addPreferredAirline(const string &airline) {
    if (airline != "none") {
        string upperAirline = toUpperCase(airline);
        if (!contains(upperAirline)) {
            Node *newNode = new Node(upperAirline);
            newNode->next = head;
            head = newNode;
        }
        hasAirlinePreference = true;
    }
}

bool UserPreferences::isPreferredAirline(const string &airline) const {
    return !hasAirlinePreference || contains(toUpperCase(airline));
}

bool UserPreferences::contains(const string &airline) const {
    Node *current = head;
    while (current) {
        if (current->airline == airline) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// LayoverQueue implementation
LayoverQueue::Node::Node(Layover d) : data(d), next(nullptr) {}

LayoverQueue::LayoverQueue() : front(nullptr), rear(nullptr) {}

LayoverQueue::~LayoverQueue() {
    while (front != nullptr) {
        Node *temp = front;
        front = front->next;
        delete temp;
    }
}

void LayoverQueue::enqueue(Layover layover) {
    Node *newNode = new Node(layover);
    if (rear == nullptr) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
}

Layover LayoverQueue::dequeue() {
    if (front == nullptr) {
        throw runtime_error("Queue is empty");
    }
    Node *temp = front;
    Layover data = front->data;
    front = front->next;
    if (front == nullptr) {
        rear = nullptr;
    }
    delete temp;
    return data;
}

bool LayoverQueue::isEmpty() const {
    return front == nullptr;
}

// Queue implementation
queue::queue(int capacity) : capacity(capacity), front(0), rear(-1), size(0) {
    data = new int[capacity];
}

queue::~queue() {
    delete[] data;
}

bool queue::enqueue(int value) {
    if (isFull()) {
        cerr << "Queue is full. Cannot enqueue." << std::endl;
        return false;
    }
    rear = (rear + 1) % capacity;
    data[rear] = value;
    size++;
    return true;
}

int queue::dequeue() {
    if (isEmpty()) {
        cerr << "Queue is empty. Cannot dequeue." << endl;
        return -1;
    }
    int value = data[front];
    front = (front + 1) % capacity;
    size--;
    return value;
}

int queue::peek() const {
    if (isEmpty()) {
        cerr << "Queue is empty. Nothing to peek." << endl;
        return -1;
    }
    return data[front];
}

bool queue::isEmpty() const {
    return size == 0;
}

bool queue::isFull() const {
    return size == capacity;
}

// PriorityQueue implementation
PriorityQueue::PriorityQueue(int capacity) : capacity(capacity), size(0) {
    data = new pair<int, int>[capacity];
}

PriorityQueue::~PriorityQueue() {
    delete[] data;
}

void PriorityQueue::push(pair<int, int> element) {
    if (size == capacity) {
        cerr << "Priority queue is full. Cannot push." << std::endl;
        return;
    }
    data[size] = element;
    heapifyUp(size);
    size++;
}

pair<int, int> PriorityQueue::pop() {
    if (empty()) {
        cerr << "Priority queue is empty. Cannot pop." << std::endl;
        return {-1, -1};
    }
    pair<int, int> value = data[0];
    data[0] = data[size - 1];
    size--;
    heapifyDown(0);
    return value;
}

bool PriorityQueue::empty() const {
    return size == 0;
}

void PriorityQueue::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (data[index].second >= data[parent].second) {
            break;
        }
        swap(data[index], data[parent]);
        index = parent;
    }
}

void PriorityQueue::heapifyDown(int index) {
    while (2 * index + 1 < size) {
        int leftChild = 2 * index + 1;
        int rightChild = 2 * index + 2;
        int smallest = index;

        if (leftChild < size && data[leftChild].second < data[smallest].second) {
            smallest = leftChild;
        }
        if (rightChild < size && data[rightChild].second < data[smallest].second) {
            smallest = rightChild;
        }
        if (smallest == index) {
            break;
        }
        swap(data[index], data[smallest]);
        index = smallest;
    }
}
