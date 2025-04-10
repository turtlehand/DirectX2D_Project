#pragma once

template  <typename T>
class GSingleton
{
private:
	static T* g_This;
	
public:
	static T* GetInst()
	{
		if (g_This == nullptr)
			g_This = new T;
		return g_This;
	}

	static void Destroy()
	{
		if (g_This != nullptr) {
			delete g_This;
			g_This = nullptr;
		}
	}

protected:
	GSingleton() {
		typedef void(*FUNC)(void);
		atexit((FUNC)&GSingleton::Destroy);
	}
	~GSingleton() {};

};

template<typename T>
T* GSingleton<T>::g_This = nullptr;

