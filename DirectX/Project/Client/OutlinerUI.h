#pragma once
#include "EditorUI.h"

class TreeNode;
class GGameObject;
class GPrefab;

class OutlinerUI :
    public EditorUI
{
private:
    class TreeUI* m_Tree;

public:
    void Renew();                           // 레벨에서 오브젝트가 변경되면 OutLiner를 수정한다.

private:
    void Selection(DWORD_PTR _Node);        // 인스펙터에게 선택된 게임오브젝트(노드)를 표시하게 해준다.
    void SelectPrefab(DWORD_PTR _Node);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _TargetNode);
    void AddGameObject(TreeNode* _Parent, GGameObject* _Objcet);    // 순회

    void AddPrefab(DWORD_PTR _PayLoad, DWORD_PTR _TargetObject);  // Prefab, Object

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    OutlinerUI();
    ~OutlinerUI();
};
