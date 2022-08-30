/****************************************************************************
  FileName     [ zxGraphMgr.h ]
  PackageName  [ graph ]
  Synopsis     [ Define ZX-graph manager ]
  Author       [ Cheng-Hua Lu ]
  Copyright    [ Copyleft(c) 2022-present DVLab, GIEE, NTU, Taiwan ]
****************************************************************************/

#ifndef ZX_GRAPH_MGR_H
#define ZX_GRAPH_MGR_H

#include <iostream>
#include <vector>
#include "zxGraph.h"
#include "zxDef.h"

extern ZXGraphMgr *zxGraphMgr;
using namespace std;

typedef vector<ZXGraph* > ZXGraphList;

class ZXGraphMgr{
    
    public:
        ZXGraphMgr(){
            _graphList.clear();
            _gListItr = _graphList.begin();
            _nextID = 0;
        }
        ~ZXGraphMgr(){}
        void reset();

        // Test
        bool isID(size_t id) const;

        // Setter and Getter
        ZXGraphList getGraphList() const { return _graphList; }
        ZXGraphList::iterator getgListItr() const { return _gListItr; }
        size_t getNextID() const { return _nextID; }
        ZXGraph* getGraph() const { return _graphList[_gListItr - _graphList.begin()]; }
        void setNextID(size_t id) { _nextID = id; }
        

        // Add and Remove
        void addZXGraph(size_t id);
        void removeZXGraph(size_t id);

        // Action
        void checkout2ZXGraph(size_t id);

    private:
        size_t                          _nextID;
        ZXGraphList                     _graphList;
        ZXGraphList::iterator           _gListItr;
};


#endif // ZX_GRAPH_MGR_H
