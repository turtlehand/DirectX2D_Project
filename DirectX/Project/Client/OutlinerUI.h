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
    void Renew();                           // �������� ������Ʈ�� ����Ǹ� OutLiner�� �����Ѵ�.

private:
    void Selection(DWORD_PTR _Node);        // �ν����Ϳ��� ���õ� ���ӿ�����Ʈ(���)�� ǥ���ϰ� ���ش�.
    void SelectPrefab(DWORD_PTR _Node);
    void DragDrop(DWORD_PTR _DragNode, DWORD_PTR _TargetNode);
    void AddGameObject(TreeNode* _Parent, GGameObject* _Objcet);    // ��ȸ

    void AddPrefab(DWORD_PTR _PayLoad, DWORD_PTR _TargetObject);  // Prefab, Object

public:
    virtual void Init() override;
    virtual void Update_UI() override;
    virtual void Render_UI() override;

public:
    OutlinerUI();
    ~OutlinerUI();
};
