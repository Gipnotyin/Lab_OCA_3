#include <iostream>
#include <string>

using namespace std;

struct Library
{
public:
	std::string nameHuman;
	int number;
	std::string nameBook;
	int date;
	bool isEmpty;
	Library()
	{
		nameHuman = "";
		number = -1;
		nameBook = "";
		date = -1;
		isEmpty = true;
	}
	void Reset()
	{
		nameHuman = "";
		number = -1;
		nameBook = "";
		date = -1;
		isEmpty = true;
	}
	std::string PrintF()
	{
		return +"ФИО: " + nameHuman + '\n'
			+ "Номер читательского билета: " + std::to_string(number) + '\n'
			+ "Название книги: " + nameBook + '\n'
			+ "Срок возварата: " + std::to_string(date) + '\n';
	}
	friend std::istream& operator>>(std::istream& s, Library& l);
	friend std::ostream& operator<<(std::ostream& s, Library& l);
};

std::istream& operator>>(std::istream& s, Library& l)
{
	std::cout << "Введите ФИО: "; s >> l.nameHuman;
	std::cout << "Введите номер читательского билета: "; s >> l.number;
	std::cout << "Введите название книги: "; s >> l.nameBook;
	std::cout << "Введите срок возврата книги: "; s >> l.date;
	return s;
}

std::ostream& operator<<(std::ostream& s, Library& l)
{
	s << "ФИО: " << l.nameHuman << std::endl
		<< "Номер читательского билета: " << l.number << std::endl
		<< "Название книги: " << l.nameBook << std::endl
		<< "Срок возврата книги: " << l.date;
	return s;
}

template<typename T>
class list
{
public:
	list()
	{ }
	virtual bool isEmpty() = 0;
	virtual void push_back(T data) = 0;
	virtual void push_front(T data) = 0;
	virtual void insert(T data, int index) = 0;
	virtual void pop_front() = 0;
	virtual void pop_back() = 0;
	virtual void removeAt(int index) = 0;
	virtual void clear() = 0;
	virtual int GetSize() = 0;
	virtual T* SearchBy(bool(predicate)(T)) = 0;
	virtual T& operator[](const int index) = 0;
};

template<typename T>
class DoublyLinked : public list<T>
{
private:
	template<typename T>
	class Node
	{
	public:
		Node* pNext;
		Node* pLast;
		T data;
		Node(T data, Node* pNext = nullptr, Node* pLast = nullptr)
		{
			this->pNext = pNext;
			this->pLast = pLast;
			this->data = data;
		}
	};
	Node<T>* Move(int index)
	{
		Node<T>* temp = head;
		for (int i = 0; i < index; i++)
		{
			temp = temp->pNext;
		}
		return temp;
	}
	bool IsCorrectIndex(int index);
public:
	int size;
	Node<T>* head;
	Node<T>* tail;
	int GetSize() override { return size; };
	DoublyLinked();
	bool isEmpty() override;
	void push_back(T data) override;
	void push_front(T data) override;
	void insert(T data, int index) override;
	void pop_front() override;
	void pop_back() override;
	void removeAt(int index) override;
	void clear() override;
	T* SearchBy(bool(predicate)(T)) override;
	T& operator[](const int index) override;
	~DoublyLinked();
};

template<typename T>
bool DoublyLinked<T>::IsCorrectIndex(int index)
{
	if (index >= 0 && index < size)
	{
		return true;
	}
	return false;
}

template<typename T>
DoublyLinked<T>::DoublyLinked()
{
	size = 0;
	head = nullptr;
	tail = nullptr;
}

template<typename T>
bool DoublyLinked<T>::isEmpty()
{
	return size == 0;
}

template<typename T>
void DoublyLinked<T>::push_back(T data)
{
	if (isEmpty())
	{
		Node<T>* current = new Node<T>(data);
		head = current;
		tail = current;
		size++;
		return;
	}
	Node<T>* current = new Node<T>(data, nullptr, tail);
	tail->pNext = current;
	tail = current;
	size++;
}

template<typename T>
void DoublyLinked<T>::push_front(T data)
{
	Node<T>* current = new Node<T>(data, head, nullptr);
	head->pLast = current;
	head = current;
	size++;
}

template<typename T>
void DoublyLinked<T>::insert(T data, int index)
{
	if (!IsCorrectIndex(index))
	{
		return;
	}
	if (index == size - 1)
	{
		push_back(data);
		return;
	}
	if (index == 0)
	{
		push_front(data);
		return;
	}
	try
	{
		Node<T>* itemLast = Move(index - 1);
		Node<T>* item = Move(index);
		Node<T>* temp = new Node<T>(data, item, itemLast);
		itemLast->pNext = temp;
		item->pLast = temp;
		size++;
	}
	catch (bad_alloc e)
	{
		cout << e.what() << endl;
	}
}

template<typename T>
void DoublyLinked<T>::pop_front()
{
	Node<T>* item = head;
	Node<T>* itemNext = item->pNext;
	head = itemNext;
	delete item;
	size--;
}

template<typename T>
void DoublyLinked<T>::pop_back()
{
	Node<T>* item = tail;
	Node<T>* itemLast = item->pLast;
	tail = itemLast;
	delete item;
	size--;
}

template<typename T>
void DoublyLinked<T>::removeAt(int index)
{
	if (size == 0) return;
	if (!IsCorrectIndex(index)) return;
	if (index == 0)
	{
		pop_front();
		return;
	}
	if (index == size - 1)
	{
		pop_back();
		return;
	}
	Node<T>* temp = Move(index);
	Node<T>* tempNext = temp->pNext;
	Node<T>* tempLast = temp->pLast;
	if ((size > 1) && (tempLast != nullptr))
		tempLast->pNext = tempNext;
	if ((size > 1) && (tempNext != nullptr))
		tempNext->pLast = tempLast;
	delete temp;
	size--;
}

template<typename T>
void DoublyLinked<T>::clear()
{
	int count = size;
	for (int i = 0; i < count; i++)
	{
		removeAt(i);
	}
}

template<typename T>
T* DoublyLinked<T>::SearchBy(bool(predicate)(T))
{
	if (size == 1)
	{
		if (predicate(head->data))
		{
			return &head->data;
		}
	}
	Node<T>* temp = head;
	while (temp->pNext != nullptr)
	{
		if (predicate(temp->data))
		{
			return &temp->data;
		}
		temp = temp->pNext;
	}
	return (T*)nullptr;
}

template<typename T>
T& DoublyLinked<T>::operator[](const int index)
{
	if (index < 0 || index >= size)
	{
		throw "Выход за границы списка!!!";
	}
	else
	{
		int count = 0;
		Node<T>* current = this->head;
		while (current != nullptr)
		{
			if (count == index)
			{
				return current->data;
			}
			current = current->pNext;
			count++;
		}
	}
}

template<typename T>
DoublyLinked<T>::~DoublyLinked()
{
	clear();
}

template<typename T>
class SinglyList : public list<T>
{
private:
	int size;
	template<typename T>
	class Node
	{
	public:
		Node* pNext;
		T data;
		Node(T data, Node* pNext = nullptr)
		{
			this->pNext = pNext;
			this->data = data;
		}
	};
public:
	Node<T>* head;
	SinglyList();
	bool isEmpty() override;
    void push_front(T data) override;//вставка в начало
	void push_back(T data) override;//вставка в конец
    void pop_front() override;//удаление первого элемента
    void insert(T value, int index) override;//вставка в любое место
    void clear() override;//очистка всего листа
    void removeAt(int index) override;//очистка одного элемента
    void pop_back() override;//удаление последнего элемента
	int GetSize() override { return size; }
	T* SearchBy(bool(predicate)(T));
	T& operator[](const int index);
	~SinglyList();
};

template<typename T>
SinglyList<T>::SinglyList()
{
	size = 0;
	head = nullptr;
}

template<typename T>
bool SinglyList<T>::isEmpty()
{
	return head == nullptr;
}

template<typename T>
void SinglyList<T>::push_front(T data)
{
    head = new Node<T>(data, head);
    size++;
}

template<typename T>
void SinglyList<T>::push_back(T data)
{
	if (isEmpty())
	{
		head = new Node<T>(data);
		size++;
		return;
	}
	Node<T>* current = this->head;
	while (current->pNext != nullptr)
	{
		current = current->pNext; 
	}
	current->pNext = new Node<T>(data);
	size++;
}

template<typename T>
void SinglyList<T>::pop_front()
{
    Node<T> *temp = head;
    head = head->pNext;
    delete temp;
    size--;
}

template<typename T>
void SinglyList<T>::insert(T value, int index)
{
    if (index == 0)
    {
        push_front(value);
        return;
    }
    Node<T>* previos = this->head;
    for (int i = 0; i < index - 1; i++)
    {
        previos = previos->pNext;
    }
    Node<T>* newNode = new Node<T>(value, previos->pNext);
    previos->pNext = newNode;
    size++;
}

template<typename T>
void SinglyList<T>::clear()
{
    while (size)
    {
        pop_front();
    }
}

template<typename T>
void SinglyList<T>::removeAt(int index)
{
    if (index == 0)
    {
        pop_front();
        return;
    }
    Node<T>* previos = this->head;
    for (int i = 0; i < index - 1; i++)
    {
        previos = previos->pNext;
    }
    Node<T>* toDelete = previos->pNext;
    previos->pNext = toDelete->pNext;
    delete toDelete;
    size--;
}

template<typename T>
void SinglyList<T>::pop_back()
{
    removeAt(size - 1);
}

template<typename T>
T* SinglyList<T>::SearchBy(bool(predicate)(T))
{
	if (size == 1)
	{
		if (predicate(head->data))
		{
			return &head->data;
		}
	}
	Node<T>* temp = head;
	while (temp->pNext != nullptr)
	{
		if (predicate(temp->data))
		{
			return &temp->data;
		}
		temp = temp->pNext;
	}
	return (T*)nullptr;
}

template<typename T>
T& SinglyList<T>::operator[](const int index)
{
	int count = 0;
	Node<T>* current = this->head;
	while (current!=nullptr)
	{
		if (count == index)
		{
			return current->data;
		}
		current = current->pNext;
		count++;
	}
}

template<typename T>
SinglyList<T>::~SinglyList()
{
    clear();
}

list<Library>* SinglyOrDoublyList(int IsList)
{
	bool end = true;
	do
	{
		switch (IsList)
		{
		case 1:
			return new SinglyList<Library>;
			break;
		case 2:
			return new DoublyLinked<Library>;
			break;
		default:
			cout << "Такой команды нет, повторите ввод!\n";
			break;
		}
	} while (end);
}

Library* SearchByPole(list<Library>* library)
{
	bool end = true;
	do
	{
		char choice;
		cout << "По какому полю хотите произвести поиск?\n"
			<< "1 - ФИО\n"
			<< "2 - номер читательского билета\n"
			<< "3 - название книги\n"
			<< "4 - по дням возврата\n"
			<< "Введите команду: "; cin >> choice;
		switch (choice)
		{
		case '1':
			return library->SearchBy([](Library lib) { return lib.nameHuman == "GRG"; });
			break;
		case '2':
			return library->SearchBy([](Library lib) { return lib.number == 9; });
			break;
		case '3':
			return library->SearchBy([](Library lib) { return lib.nameBook == "War"; });
			break;
		case '4':
			return library->SearchBy([](Library lib) { return lib.date == 66; });
			break;
		default:
			cout << "Нет такой команды, введите снова\n";
			break;
		}
	} while (end);
}

int main()
{
    setlocale(LC_ALL, "rus");
	Library lib;
	list<Library>* library;
	int IsList, index;
    bool end = true, end2 = true, end3 = true;
	char choice = '1';
    do
    {
		end2 = true;
		cout << "Какой список вы хотите использовать?(1/2) ";
		cin >> IsList;
		library = SinglyOrDoublyList(IsList);
		do
		{
			cout << "Выберите команду:\n"
				<< "1 - Добавить элемент в конец списка\n"
				<< "2 - Добавить элемент в начало списка\n"
				<< "3 - Удалить конечный элемент списка\n"
				<< "4 - Удалить начальный элемент списка\n"
				<< "5 - Поиск элемента по заданному значению поля структуры\n"
				<< "6 - Добавить элемент после найденного\n"
				<< "7 - Удаление найденного элемента\n"
				<< "9 - Выход из списка\n"
				<< "Введите команду: "; cin >> choice;
			switch (choice)
			{
			case '1':
				system("cls");
				cout << "Введите элемент, который хотите вставить в конец списка:\n";
				cin >> lib;
				library->push_back(lib);
				break;
			case '2':
				system("cls");
				cout << "Введите элемент, который хотите вставить в начало списка:\n";
				cin >> lib;
				library->push_back(lib);
				break;
			case '3':
				system("cls");
				library->pop_back();
				cout << "Элемент в конце удалён\n";
				break;
			case '4':
				system("cls");
				library->pop_front();
				cout << "Элемент в начале списка удалён\n";
				break;
			case '5':
				system("cls");
				cout << *SearchByPole(library) << endl;
				break;
			case '6':
				system("cls");
				cout << "Введите индекс, на место которого вы хотите вставить элемент: ";
				cin >> index;
				cout << "Введите элемент\n";
				cin >> lib;
				library->insert(lib, index);
				break;
			case '7':
				system("cls");
				cout << "Какой вы хотите удалить элемент?\n";
				cin >> index;
				library->removeAt(index);
				break;
			case '9':
				system("cls");
				cout << "Вы вышли из списка\n";
				delete library;
				end2 = false;
				break;
			default:
				cout << "Такой команды нет, повторите попытку!\n";
				break;
			}
		} while (end2);
		cout << "Хотите продолжить ввод?(y/n) "; cin >> choice;
		(choice == 'y') ? end = true : end = false;
    } while (end);
    return 0;
}