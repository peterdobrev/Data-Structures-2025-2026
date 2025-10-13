# 🖥️ Семинар 1: Указатели, Референции, Четене и Писане от файлове, Сложност и Локалност  🖥️ #

## Указатели и Референции ##

- подаване по стойност (с копиране):
```cpp
void f(int a);  // ако променим стойността на а във функцията
                // това няма да промени оригиналната променлива
```

- подаване по указател:

```cpp
void f(int* a); // предпочитаме, когато работим с масиви (трябва да 
				// правим проверка за nullptr)
				// ако не променяме масива, използваме const int*
```

- подаване по референция

```cpp
void f(int& a); // предпочитаме, когато използваме единичен обект, 
				// защото ни гарантира, че обектът съществува
                // не можем да пренасочваме референция към друг обект
```

- ако искаме не просто да променим не просто паметта, към която сочи указателя, а да го пренасочим:
```cpp
void f(int*& p);
    p = new int(99);    // променяме указателя
    *p = 42;            // променяме стойноста към която сочи
```


Пример:

```cpp
void resize(int*& ptr, int n) { 
	ptr = new int[n];
}

int main() {
	int* ptr = nullptr;
    int n;
    std::cin>>n;
	resize(ptr, n);     // ако не е подаден по референция, ще се направи 
                        // копие на указателя, ще се задели нова памет, към 
                        // която само копието ще сочи и в края на функцията  
                        // f, копието ще  се изтрие от  стека, но не и 
                        // паметта в heap-a, към която сочи
	delete[] ptr;
}
```

- функцията ни може да връща копие (интересуваме се само от стойността)
- * RVO предотвратява копиране със C++17

```cpp
int f(...);
``` 
- може да връща променлива
- трябва да си подсигурим, че променливата, което връщаме, ще живее след края на функцията, т.е. няма да е изтрита (да е подадана по референция във функцията или да е глобална променлива)

```cpp
int& f(...);
```

- може да връща указател
- отново трябва да подсигурим, че указателя ще съществува (подаден отвън или заделяме памет във heap-а)
- ако върнем уакзател към локална променлива, указателят става невалиден след return!!

```cpp
int* f(...);
```

## Потоци ##

- Последователност от байтове, насочени в определена посока 
- При операциите за вход, байтовете идват от източник за вход(клавиатура, файл, друга програма и т.н.)
- При операциите за изход, байтовете данни "излизат" от програмата и се "вливат" във външно устройство(козола, файл и т.н.) 
- Потоците не предоставят произволен достъп до данните си, а само последователен, т.е можем да манипулираме данните само в реда, в който те пристигат от потока.
- Потоците служат като посредник между програмите и входно-изходните устройства.
- Потокът дефинира интерфейс с операции върху него, които не зависят от входно-изходните устройства. 


Познат за нас поток за вход е `std::cin`, а поток за изход е `std::cout`

## Работа с текстови файлове ##

- файл - последователност от байтове
- можем да отворим поток за вход/изход към файл

## Четене от файл ##

Четене символ по символ:
```cpp
#include<iostream>
//Тази библиотека ни позволява да отваряме потоци за четене/писане от файл
#include<fstream>

void readFromFile(const char* fileName){
   std::ifstream file;
   file.open(fileName);

   // Задължително проверяваме дали файлът се е отворил
    if(!file.is_open()){
        return;
    }

    //Четем символ по символ
    char ch;
    while(true){
        //вариант 1
        file.get(ch);
        //вариант 2
        // ch = file.get()

        //eof() ни казва дали сме в края на файла
        if(ch == file.eof()){
            break;
        }
        std::cout << ch << std::endl;
    }

    file.close();
}

int main(){
    readFromFile("myFile.txt");
}
```
**get указател - указател на istream/ifstream, реферира следващия елемент, който ще се прочете от потока**

Функцията `get()` без аргумент ни връща цяло число, а с агрумент записва в ch поредния символ, който трябва да се прочете от потока.Функцията get() е **неформатирана** (т.е чете интервали, символи за край на ред и др.)

Линк към документация на [get()](https://en.cppreference.com/w/cpp/io/basic_istream/get)

Затваряме файлов поток за четене с функцията `close()`

Четене дума по дума:
```cpp
#include<iostream>
#include<fstream>

void readFromFile(const char* fileName){
   std::ifstream file;
   file.open(fileName);

   // Задължително проверяваме дали файлът се е отворил
    if(!file.is_open()){
        return;
    }

    //Четем файла дума по дума
    // внимаваме за  \0, трябва ни max word len + 1
    char tempWord[100];
    while(!file.eof()){
        file >> tempWord;
        std::cout << tempWord << std::endl;
    }

    file.close();
}

int main(){
    readFromFile("myFile.txt");
}
```

Операторът ">>" е **форматиран**, тоест чете с разделител "интервал", както при `std::cout`.

Четене ред по ред:
```cpp
#include<iostream>
#include<fstream>

void readFromFile(const char* fileName){
   std::ifstream file;
   file.open(fileName);

   // Задължително проверяваме дали файлът се е отворил
    if(!file.is_open()){
        return;
    }

    //Четем файла ред по ред
    // внимаваме за  \0, трябва ни max line len + 1
    char tempLine[1024];
    while(!file.eof()){
        file.getline(tempLine, 1024);
        std::cout << tempLine << std::endl;
    }

    file.close();
}

int main(){
    readFromFile("myFile2.txt");
}
```

## Писане във файл ##

**put указател - указател на ostream/ofstream, реферира мястото в паметта, където ще се запише следващият елемент**

Линк към документацията на [put()](https://en.cppreference.com/w/cpp/io/basic_ostream/put)

```cpp
#include <iostream>
#include <fstream> 
int main () {
    // tempInfo.txt се създава, ако не е съществувал до сега
    std::ofstream tempFile("tempInfo.txt"); 

    if (!tempFile.is_open()){
        std::cout << "Problem while opening the file" << std::endl;
        return 1;
    }

    // Пишем текст
    tempFile << "New info\n"; 
    // Пишем числа
    tempFile << 5 + 7 << std::endl; 

    tempFile.close(); 
}
```

## Подаване на поток като аргумент на функция ##

```cpp
void(std::ifstream& file){
    //Do something
} 
```


# Сложност по време и памет. Локалност

__Time complexity__: брой елементарни операции нужни за изпълнението на алгоритъм
__Space complexity__: количество памет нужно за изпълнението на даден алгоритъм. Не броим входните данни


![alt text](res/01_complexity.png)



## Примери:

Какъв е броят на операциите в тези алгоритми?

```cpp
int getFirstElement(const int* arr) {
    return arr[0];
}

int f(int a,  int b, int c) {
    int x = a+b;
    int y = b*c;
    return x + y;
}
```


Каква е сложността по време и памет?
```cpp

int* f(int n) {
    int* arr = new int[n];
     for (int i = 0; i < n; i++) {
        arr[i] = i + 1;
    }
    return arr;
}
```

Какъв резултат ще получим от тези две функции, каква е сложността по време и памет?

```cpp
int f(int** matrix, int rows, int cols) {
    int sum = 0;
    for (int i = 0; i < rows; ++i)
        for (int j = 0; j < cols; ++j)
            sum += matrix[i][j];
    return sum;
}

int g(int** matrix, int rows, int cols) {
    int sum = 0;
    for (int j = 0; j < cols; ++j) 
        for (int i = 0; i < rows; ++i)
            sum += matrix[i][j];
    return sum;
}

```


Още примери:

```cpp
void printHalf(int* arr, int len) {
    for (int i = 0; i < len; i += 2) {
        cout << arr[i] << " ";
    }
}
```

```cpp
void printPairs(int* arr, int len) {
    for (int i = 0; i < len; ++i)
        for (int j = i + 1; j < len; ++j)
            cout << "(" << arr[i] << "," << arr[j] << ") ";
        cout << endl;
}
```


## Рекурсивни алгоритми


пример 1:
```cpp
#include <iostream>
using namespace std;

void printNumbers(int n) {
    if (n == 0) return;
    cout << n << " ";
    printNumbers(n - 1);
}

int main() {
    printNumbers(5);
}
```

пример 2:
```cpp
#include <iostream>
using namespace std;

int sum(int n) {
    if (n == 0) return;
    return n + sum(n - 1);
}

int main() {
    cout << sum(5) << endl; // 15 or 
}
```

__Формула:__ `Sum(n) = n * (n + 1) / 2`