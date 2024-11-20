#include <iostream>
#include <stdexcept>
#include <string>

class BaseException : public std::exception {
protected:
    std::string message;

public:
    BaseException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class MemoryException : public BaseException {
public:
    MemoryException(const std::string& msg = "Недостатньо пам'яті.") : BaseException(msg) {}
};

class FileException : public BaseException {
public:
    FileException(const std::string& msg = "Помилка роботи з файлом.") : BaseException(msg) {}
};

class MathException : public BaseException {
public:
    MathException(const std::string& msg = "Математична помилка.") : BaseException(msg) {}
};

class DivisionByZeroException : public MathException {
public:
    DivisionByZeroException(const std::string& msg = "Ділення на нуль.") : MathException(msg) {}
};

class InvalidFileFormatException : public FileException {
public:
    InvalidFileFormatException(const std::string& msg = "Недійсний формат файлу.") : FileException(msg) {}
};

class Node {
public:
    int data;
    Node* prev;
    Node* next;

    Node(int value) : data(value), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    Node* head;
    Node* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}

    ~DoublyLinkedList() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void add(int value) {
        try {
            Node* newNode = new Node(value);
            if (!tail) {
                head = tail = newNode;
            }
            else {
                tail->next = newNode;
                newNode->prev = tail;
                tail = newNode;
            }
        }
        catch (const std::bad_alloc&) {
            throw MemoryException("Недостатньо пам’яті для додавання елемента.");
        }
    }

    void removeLast() {
        if (!tail) {
            throw BaseException("Неможливо видалити елемент із пустого списку.");
        }
        Node* temp = tail;
        if (tail->prev) {
            tail = tail->prev;
            tail->next = nullptr;
        }
        else {
            head = tail = nullptr;
        }
        delete temp;
    }

    void print() const {
        if (!head) {
            throw BaseException("Список порожній.");
        }
        Node* current = head;
        while (current) {
            std::cout << current->data << " ";
            current = current->next;
        }
        std::cout << std::endl;
    }
};

void divide(double a, double b) {
    if (b == 0) {
        throw DivisionByZeroException();
    }
    std::cout << "Результат: " << a / b << std::endl;
}

int main() {
    try {
        std::cout << "Демонстрація двозв’язного списку:" << std::endl;
        DoublyLinkedList list;
        list.add(10);
        list.add(20);
        list.print();
        list.removeLast();
        list.print();
        list.removeLast();
        list.removeLast(); 

    }
    catch (const BaseException& ex) {
        std::cerr << "Помилка у списку: " << ex.what() << std::endl;
    }

    try {
        std::cout << "\nДемонстрація ділення:" << std::endl;
        divide(10, 0); 
    }
    catch (const BaseException& ex) {
        std::cerr << "Помилка у математиці: " << ex.what() << std::endl;
    }

    try {
        std::cout << "\nДемонстрація файлів:" << std::endl;
        throw InvalidFileFormatException("Файл не підтримується.");
    }
    catch (const BaseException& ex) {
        std::cerr << "Помилка у файлах: " << ex.what() << std::endl;
    }

    return 0;
}
