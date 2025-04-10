#pragma once

template<typename T>
class Ptr
{
private:
	T* m_Asset;

public:
	T* Get() const { return m_Asset; }
	T** GetAdressOf() const  { return &m_Asset; }

	bool operator! () const
	{
		if (m_Asset == nullptr)
			return true;
		return false;
	}

	bool operator == (T* _Asset) const
	{
		if (m_Asset == _Asset)
			return true;
		return false;
	}

	bool operator == (const  Ptr<T>& _Other) const
	{
		if (m_Asset == _Other.m_Asset)
			return true;
		return false;
	}

	bool operator != (T* _Asset) const
	{
		if (m_Asset == _Asset)
			return false;
		return true;
	}

	bool operator != (const  Ptr<T>& _Other) const
	{
		if (m_Asset == _Other.m_Asset)
			return false;
		return true;
	}

	Ptr<T>& operator = (T* _Origin)
	{
		if (m_Asset != nullptr)
			m_Asset->Release();

		m_Asset = _Origin;

		if (m_Asset != nullptr)
			m_Asset->AddRef();

		return *this;
	}

	Ptr<T>& operator = (const Ptr<T>& _Origin)
	{
		if (m_Asset != nullptr)
			m_Asset->Release();

		m_Asset = _Origin.m_Asset;

		if (m_Asset != nullptr)
			m_Asset->AddRef();

		return *this;
	}

	T* operator->() const
	{ 
		return m_Asset;
	}

public:
	Ptr() : m_Asset(nullptr)
	{
		
	}

	Ptr(T* _Origin) : m_Asset(_Origin)
	{
		if (m_Asset != nullptr)
			m_Asset->AddRef();
	}

	Ptr(const Ptr<T>& _Origin) : m_Asset(_Origin.m_Asset)
	{
		if (m_Asset != nullptr)
			m_Asset->AddRef();
	}

	~Ptr()
	{
		if(m_Asset != nullptr)
			m_Asset->Release();
	}

};

/// nullptr을 위해 _Left는 Void 포인터로
template<typename T>
bool operator == (void* _Left, Ptr<T> _Right)
{
	return _Left == _Right.Get();
}

/// nullptr을 위해 _Left는 Void 포인터로
template<typename T>
bool operator != (void* _Left, Ptr<T> _Right)
{
	return _Left != _Right.Get();
}
