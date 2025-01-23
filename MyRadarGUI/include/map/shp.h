#ifndef SHP_H
#define SHP_H

#include "base_objects.h"
#include <vector>
#include <string>
#include <functional>

class Shp
{
public:
    Shp(const std::string& path);
    ~Shp();

    PolyLine getPolyline(Index index);
    PolyGon getPolygon(Index index);
    Point getPoint(Index index);

    Index numEntities() const
    {
        return this->pn_entities_;
    }

    ShapeType getShapeType()
    {
        return this->pn_shape_type_;
    }

    const Box& getBox() const
    {
        return this->_box;
    }


private:
    Index pn_entities_; //
    ShapeType pn_shape_type_; // shap的类型
    void* handle_;
    Box _box;
};



#endif // SHP_H
