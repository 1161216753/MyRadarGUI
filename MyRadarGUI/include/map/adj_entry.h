#ifndef ADJ_ENTRY_H
#define ADJ_ENTRY_H
#include <utility>
#include <map>
#include <memory>
#include <vector>
#include <cmath>
#include <QString>
#include "base_objects.h"


struct China
{
    China(Index i, PolyGon &&p)
        : index(i), polygon(std::move(p))
    {}
    ~China() {}

    Index index;
    QString id;
    PolyGon polygon;
};


struct AdjMap
{
    std::vector<std::unique_ptr<China>> raw_china;  // raw 代表原始数据
    std::map<NodeId, std::unique_ptr<China>> china_map;
    AdjMap() {}
    AdjMap(const AdjMap&) = delete;
    AdjMap& operator=(const AdjMap&) = delete;
};


#endif // ADJ_ENTRY_H
