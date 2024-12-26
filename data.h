#ifndef SFML_PROJECT_DATA_H
#define SFML_PROJECT_DATA_H

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int convertStringTimeToMinutes(const string &time);

string toUpperCase(const string &str);

class UserPreferences {
public:
    void addPreferredAirline(const string &airline);

    bool isPreferredAirline(const string &airline) const;

private:
    struct Node {
        string airline;
        Node *next;
        Node(const string &airline) : airline(airline), next(nullptr) {}
    };

    Node *head = nullptr;
    bool hasAirlinePreference = false;

    bool contains(const string &airline) const;
};

class Layover {
public:
    string airline;
    string departureTime;
    string arrivalTime;
    int cost;

    Layover(string a, string dep, string arr, int c)
            : airline(a), departureTime(dep), arrivalTime(arr), cost(c) {}
};

class LayoverQueue {
private:
    struct Node {
        Layover data;
        Node *next;

        Node(Layover d);
    };

    Node *front;
    Node *rear;

public:
    LayoverQueue();
    ~LayoverQueue();

    void enqueue(Layover layover);

    Layover dequeue();

    bool isEmpty() const;
};

class queue {
private:
    int *data;
    int front;
    int rear;
    int capacity;
    int size;

public:
    queue(int capacity);

    ~queue();

    bool enqueue(int value);

    int dequeue();

    int peek() const;

    bool isEmpty() const;

    bool isFull() const;
};

class PriorityQueue {
private:
    pair<int, int> *data;
    int capacity;
    int size;

    void heapifyUp(int index);

    void heapifyDown(int index);

public:
    PriorityQueue(int capacity);

    ~PriorityQueue();

    void push(pair<int, int> element);

    pair<int, int> pop();

    bool empty() const;
};

#endif //SFML_PROJECT_DATA_H
