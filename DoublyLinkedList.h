#pragma once
#include <iostream>
#include <algorithm>

using namespace std;

template <typename T> class Node {
public:
	T data;
	Node<T>* prev;
	Node<T>* next;
	template<typename U> friend class DoublyLinkedList;
	friend ostream& operator<<(ostream& out, const T& output);
public:
	Node() {
		this->next = NULL;
		this->prev = NULL;
	}
	~Node() {
	}
};


template <typename T> class DoublyLinkedList
{
	private: 
		Node<T>* head;
		Node<T>* tail;
		int length;
	public:
		DoublyLinkedList() {
			this->head = NULL;
			this->tail = NULL;
			this->length = 0;
		}

		DoublyLinkedList(DoublyLinkedList<T>&& toCopy) : DoublyLinkedList()
		{
			swap(this->head, toCopy.head);
			swap(this->tail, toCopy.tail);
			swap(this->length, toCopy.length);

		}

		DoublyLinkedList<T>& operator=(DoublyLinkedList<T>&& toCopy)
		{
			swap(this->head, toCopy.head);
			swap(this->tail, toCopy.tail);
			swap(this->length, toCopy.length);
			return *this;
		}

		Node<T>* getHead() const
		{
			return this->head;
		}

		Node<T>* getTail() const
		{
			return this->tail;
		}

		T getHeadData() 
		{
			return this->head->data;
		}

		T getTailData() 
		{
			return this->tail->data;
		}

		int getLength() const 
		{
			return this->length;
		}

		int calcLength() 
		{
			int cnt = 0;
			Node<T>* temp = head;
			while (temp->next != NULL) 
			{
				cnt++;
				temp = temp->next;
			}
			if (cnt > this->length)
				this->length = cnt;
			
			return this->length;
		}

		void add(T &&item)
		{	
			Node<T>* node = new Node<T>;
			node->data = move(item);
			node->next = NULL;
			node->prev = NULL;

			if (this->head == NULL)
			{
				this->head = node;
				this->tail = this->head;
				this->length = this->length + 1;
			}
			else
			{
				this->tail->next = node;
				node->prev = this->tail;
				this->tail = node;
				this->length = this->length + 1;
			}
		}
		
		void addFront(T &&item) 
		{
			Node<T>* node = new Node<T>;
			node->next = NULL;
			node->prev = NULL;
			node->data = move(item);

			if (this->head == NULL)
			{
				this->head = node;
				this->tail = this->head;
				this->length = this->length + 1;
			}
			else 
			{
				this->head->prev = node;
				node->next = this->head;
				this->head = node;
				this->length = this->length + 1;
			}
		}

		T* get(int pos)
		{
			pos--;
			if (pos >= this->length || pos < 0)
				return nullptr;

			Node<T>* temp = head;
			while (pos--)
			{
				temp = temp->next;
			}
			
			return &(temp->data);
		}

		void add(int pos, T &&item)
		{
			if (pos > this->length || pos < 0)
				return;
			
			if (pos == 0)
			{
				addFront(move(item));
				return;
			}

			if (pos == (length - 1))
			{
				add(move(item));
				return;
			}

			int cnt = 0;
			Node<T>* temp = head;
			while (cnt != (pos - 1))
			{
				cnt++;
				temp = temp->next;
			}
			Node<T>* node = new Node<T>;
			node->data = move(item);
			node->prev = temp;
			node->next = temp->next;
			temp->next->prev = node;
			temp->next = node;
			this->length = this->length + 1;
		}


		void printBackwards() 
		{
			if (head == NULL || head->next == NULL)
				return;
			
			Node<T>* temp = tail;
			while (temp != NULL)
			{
				cout << temp->data;
				if (typeid(T) != typeid(char))
					cout << " ";

				temp = temp->prev;
			}
		}

		void printAll() 
		{
			if (head == NULL)
				return;
			
			Node<T>* temp = head;
			while (temp != NULL)
			{
				cout << temp->data;
				if (typeid(T) != typeid(char))
					cout << " ";

				temp = temp->next;
			}
		}

		void remove()
		{
			if (head == NULL)
			{
				this->length = 0;
				return;
			}
			if (tail->prev == NULL)
			{
				delete tail;
				tail = NULL;
				head = NULL;
				this->length = 0;
				return;
			}
			Node<T>* temp = tail->prev;
			temp->next = NULL;
			tail->prev = NULL;
			delete tail;
			tail = temp;
			this->length = this->length - 1;
		}

		void removeAll()
		{
			while (getLength() > 0)
			{
				removeFirst();
			}
		}

		void remove(int pos)
		{
			if (pos >= this->length || pos < 0)
				return;
		
			if (pos == 0)
			{
				removeFirst();
				return;
			}
			if (pos == (length - 1))
			{
				remove();
				return;
			}

			Node<T>* temp = this->head;
			int cnt = 0;
			while (cnt != pos)
			{
				cnt++;
				temp = temp->next;
			}
			temp->prev->next = temp->next;
			temp->next->prev = temp->prev;
			temp->prev = NULL;
			temp->next = NULL;
			delete temp;
			temp = NULL;
			this->length = this->length - 1;
			return;
		}

		void removeFirst()
		{
			if (head == NULL)
				return;
			if (head->next == NULL)
			{
				delete head;
				head = NULL;
				tail = NULL;
				this->length = this->length - 1;
				return;
			}
			Node<T>* temp = head->next;
			temp->prev = NULL;
			head->next = NULL;
			delete head;
			head = temp;
			this->length = this->length - 1;
		}

		~DoublyLinkedList() 
		{
			while (this->head != NULL)
			{
				remove();
			}
		}

};

template <typename T> bool areEqual(const DoublyLinkedList<T>& a, const DoublyLinkedList<T>& b)
{
	if (a.getLength() != b.getLength())
		return false;

	Node<T>* A = a.getHead();
	Node<T>* B = b.getHead();
	while (A != NULL && B != NULL)
	{
		if (A->data != B->data)
			return false;

		A = A->next;
		B = B->next;
	}
	return true;
}

