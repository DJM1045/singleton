#include <iostream>
using namespace std;

//class Solution {
//public:
//	class Sum
//	{
//	public:
//		Sum()
//		{
//			_sum += _i;
//			_i++;
//		}
//	};
//
//	int Sum_Solution(int n) {
//		//Sum array[n];
//		_sum = 0;
//		_i = 1;
//		Sum* p = new Sum[n];
//		delete[] p;
//		return _sum;
//	}
//
//	static size_t _sum;
//	static size_t _i;
//};
//
//size_t Solution::_sum = 0;
//size_t Solution::_i = 1;
//
//int main()
//{
//	cout<<Solution().Sum_Solution(5) << endl;
//	cout << Solution().Sum_Solution(5) << endl;
//
//	return 0;
//}
//
//int main()
//{
//	int* p = (int*)operator new(4);
//
//	return 0;
//}

struct ListNode
{
	ListNode* _next;
	ListNode* _prev;
	int _data;

	//void* operator new(size_t n)
	//{
	//	void* p = nullptr;
	//	p = allocator<ListNode>().allocate(1);
	//	cout << "memory pool allocate" << endl;
	//	return p;
	//}

	//void operator delete(void* p)
	//{
	//	allocator<ListNode>().deallocate((ListNode*)p, 1);
	//	cout << "memory pool deallocate" << endl;
	//}
};

class Test
{
public:
	Test()
		: _data(0)
	{
		cout << "Test():" << this << endl;
	}
	~Test()
	{
		cout << "~Test():" << this << endl;
	}
private:
	int _data;
};

//int main()
//{
//	Test* p1 = new Test;
//	delete p1;
//
//	Test* p2 = (Test*)operator new(sizeof(Test));
//	new(p2)Test(); // 对已有空间显示调用构造函数的方式
//
//	p2->~Test();
//	operator delete(p2);
//
//	return 0;
//}

// 对象只能在堆上
class HeapOnly
{
public:
	static HeapOnly* GetHeapObj()
	{
		return new HeapOnly;
	}

private:
	HeapOnly(const HeapOnly&);
	// ...
	HeapOnly()
	{}
};

//class StackOnly
//{
//public:
//	StackOnly() {}
//private:
//	void* operator new(size_t size);
//	void operator delete(void* p);
//};
//
//class StackOnly
//{
//public:
//	static StackOnly GetStackObj()
//	{
//		return StackOnly();
//	}
//private:
//	//StackOnly(const StackOnly&);
//	void* operator new(size_t n);
//
//	StackOnly() {}
//};
//
//int main()
//{
//	//HeapOnly ho;
//	//HeapOnly* p = new HeapOnly;
//	HeapOnly* p = HeapOnly::GetHeapObj();
//	//HeapOnly copy(*p);
//	delete p;
//
//	StackOnly so = StackOnly::GetStackObj();
//	//StackOnly* copyp = new StackOnly(so);
//
//	return 0;
//}
//

//class Singleton
//{
//public:
//	static Singleton* GetInstance()
//	{
//		static Singleton sinst;
//		return &sinst;
//	}
//
//private:
//	Singleton()
//	{}
//
//	// 防拷贝
//	//Singleton(const Singleton&);
//	Singleton(const Singleton&) = delete;
//	// ...
//};
//
//int main()
//{
//	cout << Singleton::GetInstance() << endl;
//	cout << Singleton::GetInstance() << endl;
//	cout << Singleton::GetInstance() << endl;
//	cout << Singleton::GetInstance() << endl;
//}

// 饿汉
//class Singleton
//{
//public:
//	static Singleton* GetInstance()
//	{
//		static Singleton sinst;
//		return &sinst;
//	}
//
//private:
//	Singleton()
//	{}
//
//	// 防拷贝
//	//Singleton(const Singleton&);
//	Singleton(const Singleton&) = delete;
//	// ...
//
//	static Singleton _sinst;
//};
//
//Singleton Singleton::_sinst;
//
//int main()
//{
//	cout << Singleton::GetInstance() << endl;
//	cout << Singleton::GetInstance() << endl;
//	cout << Singleton::GetInstance() << endl;
//	cout << Singleton::GetInstance() << endl;
//}

#include <thread>
#include <mutex>
#include <atomic>
#include <time.h>

class Singleton
{
public:
	//// 线程安全
	//static Singleton* GetInstance()
	//{
	//	if (_spinst == nullptr)
	//	{
	//		_smtx.lock();
	//		if (_spinst == nullptr)
	//		{
	//			//std::this_thread::sleep_for(std::chrono::seconds(1));
	//			_spinst = new Singleton;
	//		}
	//		_smtx.unlock();
	//	}


	//	return _spinst;
	//}

	// 线程安全
	static Singleton* GetInstance()
	{
		if (_spinst == nullptr) // 已经有实例对象以后，不在需要加锁
		{
			_smtx.lock();
			if (_spinst == nullptr) // 没有实例对象时检查时，保证只有一个线程进去申请对象
			{
				//std::this_thread::sleep_for(std::chrono::seconds(1));
				_spinst = new Singleton;
			}
			_smtx.unlock();
		}


		return _spinst;
	}

	static void DelInstance()
	{
		//_smtx.lock();
		if (_spinst != nullptr)
		{
			delete _spinst;
			_spinst = nullptr;
		}
		//_smtx.unlock();
	}
private:
	Singleton()
	{}

	// 防拷贝
	//Singleton(const Singleton&);
	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	// ... 成员

	static Singleton* _spinst;
	static mutex _smtx;
};

class GC
{
public:
	~GC()
	{
		Singleton::DelInstance();
	}
};

GC gc;


Singleton* Singleton::_spinst = nullptr;
mutex Singleton::_smtx;

void func(int n, size_t* begin, size_t* end)
{
	*begin += clock();
	for (size_t i = 0; i < n; ++i)
	{
		Singleton::GetInstance();
	}
	*end += clock();
}

//int main()
//{
//	size_t begin = 0;
//	size_t end = 0;
//
//	size_t n = 1000000;
//	thread t1(func, n, &begin, &end);
//	thread t2(func, n, &begin, &end);
//	thread t3(func, n, &begin, &end);
//	thread t4(func, n, &begin, &end);
//	//thread t5(func, 10);
//	//thread t6(func, 10);
//
//
//	t1.join();
//	t2.join();
//	t3.join();
//	t4.join();
//
//	cout << end - begin << endl;
//
//	//t5.join();
//	//t6.join();
//}


//int main()
//{
//	//char* p1 = new char [4*1024 * 1024 * 1024];
//	/*for (; ; )
//	{
//	printf("%p\n", malloc(100*1024));
//	}*/
//
//	char a = 48;
//	char b = 80;
//	if ((a + 32 - 'a') % 32 == (b + 32 - 'a') % 32)
//	{
//		cout << "相等" << endl;
//	}
//	
//
//	return 0;
//}

//class String
//{
//public:
//	//String()
//	//	:_str(new char[1])
//	//{
//	//	_str[0] = '\0';
//	//}
//
//	//String(char* str)
//	//	:_str(new char[strlen(str)+1])
//	//{
//	//	strcpy(_str, str);
//	//}
//	String(char* str = "")
//		:_str(new char[strlen(str) + 1])
//	{
//		strcpy(_str, str);
//	}
//
//	// copy(s2)
//	String(const String& s)
//		:_str(new char[strlen(s._str)+1])
//	{
//		strcpy(_str, s._str);
//	}
//
//	// s2 = copy
//	String& operator=(const String& s)
//	{
//		if (this != &s)
//		{
//			delete[] _str;
//			_str = new char[strlen(s._str) + 1];
//			strcpy(_str, s._str);
//		}
//
//		return *this;
//	}
//
//	~String()
//	{
//		if (_str)
//		{
//			printf("%p\n", _str);
//			delete[] _str;
//			_str = nullptr;
//		}
//	}
//
//	char& operator[](size_t index)
//	{
//		return _str[index];
//	}
//
//	const char* c_str()
//	{
//		return _str;
//	}
//
//private:
//	char* _str;
//};
//
//int main()
//{
//	String s1;
//	String s2("hello");
//	s2[0] = 'x';
//
//	cout << s1.c_str() << endl;
//	cout << s2.c_str() << endl;
//
//	String copy(s2);
//	copy[0] = 'h';
//	cout << copy.c_str() << endl;
//
//	s1 = copy;
//	cout << s1.c_str() << endl;
//
//	s1 = s1;
//	cout << s1.c_str() << endl;
//
//	return 0;
//}

class String
{
public:
	//String()
	//	:_str(new char[1])
	//{
	//	_str[0] = '\0';
	//}

	//String(char* str)
	//	:_str(new char[strlen(str)+1])
	//{
	//	strcpy(_str, str);
	//}
	typedef char* iterator;
	iterator begin()
	{
		return _str;
	}

	iterator end()
	{
		return _str + _size;
	}

	String(char* str = "")
	{
		_size = _capacity = strlen(str);
		_str = new char[_capacity + 1];

		strcpy(_str, str);
	}

	// copy(s2)
	//String(const String& s)
	//	:_str(new char[strlen(s._str) + 1])
	//{
	//	strcpy(_str, s._str);
	//}

	//// s2 = copy
	//String& operator=(const String& s)
	//{
	//	if (this != &s)
	//	{
	//		delete[] _str;
	//		_str = new char[strlen(s._str) + 1];
	//		strcpy(_str, s._str);
	//	}

	//	return *this;
	//}

	~String()
	{
		if (_str)
		{
			printf("%p\n", _str);
			delete[] _str;
			_str = nullptr;
		}
	}

	void PushBack(char ch);
	void Append(const char* str);
	String& operator+=(const char* str);
	String& operator+=(char ch);

	//operator>
	//operator==
	//operator<

	char& operator[](size_t index)
	{
		return _str[index];
	}

	const char* c_str()
	{
		return _str;
	}

private:
	char* _str;
	size_t _size;
	size_t _capacity;
};

int main()
{
	String s1;
	String s2("hello");

	String::iterator it = s2.begin();
	while (it != s2.end())
	{
		cout << *it << " ";
		++it;
	}
	cout << endl;

	for (auto ch : s2)
	{
		cout << ch << " ";
	}
	cout << endl;

	return 0;
}