#pragma once

#include <string>
#include <libds/amt/implicit_sequence.h>
#include <libds/amt/explicit_hierarchy.h>
#include "node.h"
//#include <functional>


using namespace std;
using namespace ds::amt;

class IteratorManager {

public:

    IteratorManager(MultiWayExplicitHierarchy<Node*>& viacCestnaHierarchia);
    ~IteratorManager();

    void prejdiNaNizsiNode(int indexSyna);
    void prejdiNaVyssiNode();

    Hierarchy<MultiWayExplicitHierarchyBlock<Node*>>::PreOrderHierarchyIterator getPreOrderBegin();
    Hierarchy<MultiWayExplicitHierarchyBlock<Node*>>::PreOrderHierarchyIterator getPreOrderEnd();

    MultiWayExplicitHierarchyBlock<Node*>* getAktualnyVrchol();

    Node& operator*();

private:
    MultiWayExplicitHierarchyBlock<Node*>* aktualnyVrcholHierarchie;
    MultiWayExplicitHierarchy<Node*>* viacCestnaHierarchia;
};

IteratorManager::IteratorManager(MultiWayExplicitHierarchy<Node*>& viacCestnaHierarchia)
{
    this->viacCestnaHierarchia = &viacCestnaHierarchia;
    this->aktualnyVrcholHierarchie = viacCestnaHierarchia.accessRoot();
}

IteratorManager::~IteratorManager()
{
    aktualnyVrcholHierarchie = nullptr;
}

void IteratorManager::prejdiNaVyssiNode()
{
    this->aktualnyVrcholHierarchie = static_cast<MultiWayExplicitHierarchyBlock<Node*>*>(this->aktualnyVrcholHierarchie->parent_);
}

MultiWayExplicitHierarchyBlock<Node*>* IteratorManager::getAktualnyVrchol()
{
    return this->aktualnyVrcholHierarchie;
}

void IteratorManager::prejdiNaNizsiNode(int indexSyna)
{
    this->aktualnyVrcholHierarchie = static_cast<MultiWayExplicitHierarchyBlock<Node*>*>(this->aktualnyVrcholHierarchie->sons_->access(indexSyna)->data_);
}

Node& IteratorManager::operator*()
{
    return *this->aktualnyVrcholHierarchie->data_;
}

Hierarchy<MultiWayExplicitHierarchyBlock<Node*>>::PreOrderHierarchyIterator IteratorManager::getPreOrderBegin()
{
    return Hierarchy<MultiWayExplicitHierarchyBlock<Node*>>::PreOrderHierarchyIterator(this->viacCestnaHierarchia, this->aktualnyVrcholHierarchie);
}

Hierarchy<MultiWayExplicitHierarchyBlock<Node*>>::PreOrderHierarchyIterator IteratorManager::getPreOrderEnd()
{
    return Hierarchy<MultiWayExplicitHierarchyBlock<Node*>>::PreOrderHierarchyIterator(this->viacCestnaHierarchia, nullptr);
}