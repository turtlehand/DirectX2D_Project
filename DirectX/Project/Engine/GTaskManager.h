#pragma once
#include "GSingleton.h"

class GEntity;

class GTaskManager :
    public GSingleton<GTaskManager>
{
private:
    SINGLE(GTaskManager);

private:
    vector<tTask> m_vecTask;
    vector<GEntity*> m_vecGC;

    bool m_LevelChanged;

public:
    void Progress();
    void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }

    bool IsLevelChanged() { return m_LevelChanged; }

};

