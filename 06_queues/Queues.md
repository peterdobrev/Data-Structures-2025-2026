# Опашка
  
### Определение
  Линейна струкутра от данни с последователен достъп. FIFO (First-In, First-Out)
  
### Абстракна структура
```cpp
template <typename T>
class QueueInterface {
public:
    virtual ~QueueInterface() = default;

    // Inserts element at the end of the queue
    virtual void push(const T& element) = 0;
    // Returns the element at the back of the queue - the most recently pushed element
    virtual T back() = 0;

    // Returns the element at the front of the queue
    virtual T front() = 0;

    // Removes the first element
    virtual void pop();

    virtual bool isEmpty() const = 0;
    virtual size_t size() const = 0;
};
```

### Реализация:
  - Свързан списък
  - Динамичен Масив

### Сложност ан операциите
| Operation | Linked List | Vector (Dynamic Array) |
|-----------|-----------------------------|----------------------------------------|
| `push` (enqueue) | O(1) | Amortized O(1), worst-case O(n) |
| `pop` (dequeue) | O(1) | O(n) |
| `front` | O(1) | O(1) |
| `back` | O(1) | O(1) |
| `isEmpty` | O(1) | O(1) |
| `size` | O(1) | O(1) |


### Задачи

#### 1. Мин/Макс Опашка (Const time)
Реализирайте структура от данни опашка с допълнителна операция `int getMin() / int getMax`, която връща минималният / максималният елемент в структурата.
Всички операции в опашката трябва да бъдат реализирани с константна времева сложност.

#### 2. Реализирайте Стек използайки `std:stack` [Leetcode](https://leetcode.com/problems/implement-stack-using-queues/description/?envType=problem-list-v2&envId=queue) 

#### 3. Напишете програма която да извежда по подадено K, К-то поредно число на Хмаинг  $ H = \{ x | x = 2^{i}\cdot 3^{j}\cdot 5^{k} \}$. Тоест при редицата $ 1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15 ... $ и $к = 10$ да извежда 12

