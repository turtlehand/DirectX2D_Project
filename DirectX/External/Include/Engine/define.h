#pragma once

#define SINGLE(type) 	friend class GSingleton<type>;\
						type& operator = (const type& _Other) = delete;\
						type();\
						type(const type& _Origin) = delete;\
						~type();

#define DEVICE (GDevice::GetInst()->GetDevice().Get())
#define CONTEXT (GDevice::GetInst()->GetContext().Get())

#define CONTENTPATH (GPathManager::GetContentPath())

#define DT (GTimeManager::GetInst()->DeltaTime())
#define TIME (GTimeManager::GetInst()->AccTime())
#define ENGINEDT (GTimeManager::GetInst()->EngineDeltaTime())
#define ENGINETIME (GTimeManager::GetInst()->EngineAccTime())

#define KEY_CHECK(key)		(GKeyManager::GetInst()->GetKeyState(key))
#define KEY_DOWN(key)		(GKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::DOWN)
#define KEY_PRESSED(key)	(GKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::PRESSED)
#define KEY_UP(key)	(GKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::UP)
#define KEY_NONE(key)		(GKeyManager::GetInst()->GetKeyState(key) == KEY_STATE::NONE)

#define MAX_LAYER 32

#define CLONE(Type)			virtual Type* Clone() override {return new Type(*this);}
#define CLONE_DELETE(Type)	virtual Type* Clone() final {return nullptr;}\
							private:\
							Type* Clone(const Type& _Other) = delete;


