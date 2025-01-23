#ifndef CHINA_LOADER_H
#define CHINA_LOADER_H

#include "loader.h"
#include <string>
#include <iostream>
#include <cmath>

#define MOCT_CHINA_PATH "E:\\MyRadarGUI\\MyRadarGUI\\data\\china_basic_map\\bou2_4l"

class ChinaLoader : public Loader
{
public:
    // 委托构造
    ChinaLoader(const std::string& path) : Loader(path) {}

    // 重写父类的函数
    virtual void load(AdjMap &adj_map) override;

    PolyGon getPolygon(Index index)
    {
        return this->_shp.getPolygon(index);
    }

    Row getRow(Index index)
    {
        return this->_dbf.getRow(index);
    }

};


#endif // CHINA_LOADER_H
