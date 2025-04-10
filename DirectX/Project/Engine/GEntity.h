#pragma once

class GEntity
{
private:
	static UINT g_NextID;
	const UINT m_ID;
	wstring m_Name;

public:
	void SetName(const wstring& _Name) { m_Name = _Name; }
	const wstring& GetName() { return m_Name; }
	UINT GetID() { return m_ID; }

public:
	virtual GEntity* Clone() = 0;

public:
	GEntity();
	GEntity(const GEntity& _Other);
	virtual ~GEntity();

};

