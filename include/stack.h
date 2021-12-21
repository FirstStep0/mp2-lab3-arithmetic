#pragma once
// объ€вление и реализаци€ шаблонного стека
// стек поддерживает операции: 
// - вставка элемента, 
// - извлечение элемента, 
// - просмотр верхнего элемента (без удалени€)
// - проверка на пустоту, 
// - получение количества элементов в стеке
// - очистка стека
// при вставке в полный стек должна перевыдел€тьс€ пам€ть

template<class T>
class my_stack {
	template<class T>
	struct Node {
		Node<T>* next;
		T value;
		Node() = default;
	};
	Node<T>* top;
	size_t count;
public:
	my_stack();                //конструктор по умолчанию
	~my_stack();			   //деструктор
	bool empty() noexcept;     //проверка на пустоту
	size_t size() noexcept;    //получение количества элементов в стеке
	void push(const T& value); //вставка элемента
	void pop();                //извлечение элемента
	T& back();                 //просмотр верхнего элемента (без удалени€)
	void clear();              //очистка стека
private:
	Node<T>* attach_node(Node<T>* node);
	Node<T>* detach_node(Node<T>* node);
};

template<class T>
my_stack<T>::my_stack() {
	top = new Node<T>;
	count = 0;
}

template<class T>
my_stack<T>::~my_stack() {
	clear();
	delete top;
};

template<class T>
bool my_stack<T>::empty() noexcept {
	return !count;
}

template<class T>
void my_stack<T>::push(const T& value) {
	top = attach_node(top);
	top->value = value;
	count++;
}

template<class T>
size_t my_stack<T>::size() noexcept {
	return count;
}

template <class T>
void my_stack<T>::pop() {
	if (!this->empty()) {
		top = detach_node(top);
		count--;
	}
}

template <class T>
T& my_stack<T>::back() {
	if (!this->empty()) {
		return top->value;
	}
	throw std::string("stack is empty");
}

template<class T>
void my_stack<T>::clear() {
	while (count) {
		top = detach_node(top);
		count--;
	}
}

template<class T>
my_stack<T>::Node<T>* my_stack<T>::attach_node(Node<T>* node) {
	Node<T>* att_node = new Node<T>;
	att_node->next = node;
	return att_node;
}

template<class T>
my_stack<T>::Node<T>* my_stack<T>::detach_node(Node<T>* node) {
	Node<T>* next = node->next;
	delete node;
	return next;
};

const size_t MAX_VECTOR_SIZE = 1000000;

template<class T>
class my_vector {
	size_t _size, _real_size;
	double k = 2;
	T* data;
public:
	my_vector(size_t size = 0) {
		_size = 0;
		_real_size = 0;
		data = nullptr;
		resize(size);
	}
	~my_vector() {
		delete[] data;
	}
	void resize(size_t new_size) {
		if (new_size > MAX_VECTOR_SIZE) {
			throw std::string("new_size > MAX_VECTOR_SIZE");
		}
		if ((_real_size / (k * k)) < new_size && new_size <= _real_size) {
			_size = new_size;
		}
		else {
			_real_size = (MAX_VECTOR_SIZE < next_size(new_size) ? MAX_VECTOR_SIZE : next_size(new_size));
			T* temp = new T[_real_size];
			for (int i = 0; i < (_size < new_size ? _size : new_size); i++) {
				temp[i] = data[i];
			}
			delete[] data;
			data = temp;
			_size = new_size;
		}
	}
	void push_back(const T& value) {
		resize(size() + 1);
		back() = value;
	}
	void pop_back() {
		resize(size() - 1);
	}
	bool empty() {
		return !size();
	}
	T& back() {
		return data[size() - 1];
	}
	size_t size() {
		return _size;
	}
	size_t real_size() {
		return _real_size;
	}
	T& operator[](size_t index) {
		return data[index];
	}
private:
	size_t next_size(size_t size) {
		size_t ans = (size_t)(pow(k, (size_t)(log(size) / log(k)) + 1)); //блишайша€ степень числа k, что больше size
		return ans;
	}
};
