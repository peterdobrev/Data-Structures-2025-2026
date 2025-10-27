## Задачи

```cpp
struct Node
{
    Node(int val) : val(val), next(nullptr), prev(nullptr) {}
    int val;
    Node* next;
    Node* prev;
};

void print(Node<T>* head) {
    Node<int>* curr = head;
    while (curr) {
        std::cout << curr->data << " ";
        curr = curr->next;
    }
   std::cout << endl;
}

// Task 1
Node* sum(Node*& lhs, const Node* rhs);

// Task 2
void removeIf(Node*& from, const Node* what);

// Task 3
Node* merge(Node* first, Node* second);

int main() {
    // ---- List 1: 1 -> 4 -> 7 -> 2 ----
    Node<int>* n1  = new Node<int>(1);
    Node<int>* n2  = new Node<int>(4);
    Node<int>* n3  = new Node<int>(7);
    Node<int>* n4  = new Node<int>(2);

    n1->next = n2;
    n2->prev = n1;
    n2->next = n3;
    n3->prev = n2;
    n3->next = n4;
    n4->prev = n3;

    Node<int>* list1 = n1;

    // ---- List 2: 5 -> 2 -> 3 -> 5 -> 1 ----
    Node<int>* m1 = new Node<int>(5);
    Node<int>* m2 = new Node<int>(2);
    Node<int>* m3 = new Node<int>(3);
    Node<int>* m4 = new Node<int>(5);
    Node<int>* m5 = new Node<int>(1);

    m1->next = m2;
    m2->prev = m1;
    m2->next = m3;
    m3->prev = m2;
    m3->next = m4;
    m4->prev = m3;
    m4->next = m5;
    m5->prev = m4;

    Node<int>* list2 = m1;

    return 0;
}

```
### 

### 1. sum()
Да се реализира следна функция `Node* sum(Node*& lhs, const Node* rhs);` – lhs и rhs са двусвързани списъци репрезентиращи числа. По подадени два двусвързани списъка, sum трябва да върне трети репрезентиращ сумата на тези числа.
Пример:
```
lhs:   1 <-> 2 <-> 3
rhs:   2 <-> 3 <-> 4
ans:   3 <-> 5 <-> 7
```

### 2. removeIf()
Да се реализира следна функция `void removeIf(Node*& from, const Node* what);` – премахва от списъка с начало from
всички елементи, които се срещат в списъка с начало what. След изпълнението from трябва
да сочи началото на резултатния списък

### 3. removeIf()
Да се реализира следна функция `Node* merge(Node* first, Node* second);` – слива сортираните във възходящ ред списъ-
ци с начало first и second в резултатен, сортиран по същия начин списък. Подадените
списъци могат да имат повторения, но в резултата не трябва да има повтарящи се стойнос-
ти. Елементите да се преизползват (не трябва да се създават нови елементи). Функцията да
връща указател към началото на резултатния списък.
