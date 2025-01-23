#ifndef LOADER_H
#define LOADER_H
#include <iostream>
#include <string>
#include <dbf.h>
#include <shp.h>
#include <adj_entry.h>
#include <base_objects.h>

class Loader
{
public:
    Loader(const std::string &path);
    virtual void load(AdjMap& adj_map) = 0;
    const Box &getBox() const
    {
        return this->_shp.getBox();
    }
    virtual ~Loader() {}

    Index numEntities() const
    {
        return this->_n_entities;
    }
    AdjMap _adj_map;
    Dbf _dbf;
    Shp _shp;
private:

    Index _n_entities;
    Index _current_index;
};


#endif // LOADER_H
