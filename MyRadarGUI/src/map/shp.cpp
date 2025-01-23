#include "shp.h"
#include "shape/shapefil.h"
#include <string>
#include <sstream>
#include <memory>
#include <stdexcept>
#include <algorithm>
#include <iostream>
/**
 * @ 内部链接属性
 * @ internal 内部
 */
namespace
{
inline Point toPoint(SHPObject* object)
{
    Point point;
    point.x = object->padfX[0];
    point.y = object->padfY[0];
    return point;
}

inline PolyLine toPolyline(SHPObject* object)
{
    PolyLine polyline;

    if(object->nParts == 0 || object->nParts == 1)
    {
        std::vector<Point> part;
        for(int i = 0; i < object->nVertices; ++i)
        {
            Point point;
            point.x = object->padfX[i];
            point.y= object->padfY[i];
            part.emplace_back(std::move(point));
        }
        polyline.parts.emplace_back(part);
    }else
    {
        //nVertices是所有part的总长度，panPartStart是代表每个part的起始点
        int part_index = 1;
        int next_part_start_index = object->panPartStart[part_index]; //第i个part的起始位置
        std::vector<Point> part;
        for (int i = 0; i < object->nVertices; ++i) {
            Point point;
            point.x = object->padfX[i];
            point.y = object->padfY[i];
            part.emplace_back(std::move(point));

            if (i + 1 == next_part_start_index) {
                polyline.parts.emplace_back(std::move(part));
                part.clear();
                if (part_index + 1 == object->nParts) //到达最后一个part
                    next_part_start_index = object->nVertices;
                else
                    next_part_start_index = object->panPartStart[++part_index];
            }
        }
    }
    return polyline;
}
inline PolyGon toPolygon(SHPObject *object)
{
    PolyGon polygon;
    if (object->nParts == 1) { //只有一块
        std::vector<Point> part;
        for (int i = 0; i < object->nVertices; ++i) {
            Point point;
            point.x = object->padfX[i];
            point.y = object->padfY[i];
            part.emplace_back(std::move(point));
        }
        polygon.parts.emplace_back(std::move(part));
    } else {
        //nVertices是所有part的总长度，panPartStart是代表每个part的起始点
        int part_index = 1;
        int next_part_start_index = object->panPartStart[part_index]; //第i个part的起始位置
        std::vector<Point> part;
        for (int i = 0; i < object->nVertices; ++i) {
            Point point;
            point.x = object->padfX[i];
            point.y = object->padfY[i];
            part.emplace_back(std::move(point));

            if (i + 1 == next_part_start_index) {
                polygon.parts.emplace_back(std::move(part));
                part.clear();
                if (part_index + 1 == object->nParts) //到达最后一个part
                    next_part_start_index = object->nVertices;
                else
                    next_part_start_index = object->panPartStart[++part_index];
            }
        }
    }

    return polygon;
}

}

/**
 * @brief Shp::Shp
 * @param path
 */
Shp::Shp(const std::string& path) :
    pn_entities_(0),
    pn_shape_type_(ShapeType::Invalid),
    handle_(static_cast<void*>(SHPOpen(path.c_str(), "rb")))
{
    if(handle_ == nullptr)
        throw std::runtime_error("Faild to SHPOpen...");

    int shape_type = static_cast<int>(ShapeType::Invalid);
    // 存放地图的边界
    double min_bound[4] = {0};
    double max_bound[4] = {0};

    /**
     * @brief SHPGetInfo
     * @获取shape的全局信息
     */
    SHPGetInfo(
        static_cast<SHPHandle>(this->handle_),
        reinterpret_cast<int*> (&this->pn_entities_),
        &shape_type, min_bound, max_bound);

    this->pn_shape_type_ = static_cast<ShapeType>(shape_type);  //是哪种类型

    //形状个数
    if (this->pn_entities_ <= 0) {
        std::cerr << "_n_entities is invalid: " << this->pn_entities_ << std::endl;
        throw std::runtime_error("SHPGetInfo returns invalid entity num...");
    }

    this->_box.x_min = min_bound[0];
    this->_box.x_max = max_bound[0];
    this->_box.y_min = min_bound[1];
    this->_box.y_max = max_bound[1];

    std::cout << "box xmin(" << this->_box.x_min << ") xmax(" << this->_box.x_max
              << ") ymin(" << this->_box.y_min << ") ymax(" << this->_box.y_max << ")" << std::endl;

    std::cout << "[" << path << "] shape type[" << static_cast<int>(this->pn_shape_type_)
              << "] entity num[" << this->pn_entities_ << "]" << std::endl;

}

/**
 * @brief Shp::~Shp
 */
Shp::~Shp()
{
    SHPClose(static_cast<SHPHandle>(this->handle_));
}


/**
 * @brief Shp::getPolyline
 * @param index
 * @return
 * @说明：获得圆弧形状的经纬度
 */
PolyLine Shp::getPolyline(Index index)
{
    if(this->pn_shape_type_ != ShapeType::PolyLine)
        throw std::runtime_error("this shape type isn't polyline!");

    std::unique_ptr<SHPObject, void(*)(SHPObject*)> object(
        SHPReadObject(static_cast<SHPHandle>(this->handle_),static_cast<int>(index)),
        SHPDestroyObject);

    if(object == nullptr)
        throw std::invalid_argument("index may be out of range");

    return toPolyline(object.get()); //返回object中保存的指针，
}
/**
 * @brief Shp::getPolygon
 * @param index
 * @return
 * @说明：获得圆弧形状的经纬度
 */
PolyGon Shp::getPolygon(Index index)
{
    if (this->pn_shape_type_ != ShapeType::PolyGon) //多边形
        throw std::runtime_error("this shape type isn't polygon!");

    std::unique_ptr<SHPObject, void(*)(SHPObject *)> object(
        SHPReadObject(
            static_cast<SHPHandle>(this->handle_),
            static_cast<int>(index)),
        SHPDestroyObject);

    if (object == nullptr)
        throw std::invalid_argument("index may be out of range");

    return toPolygon(object.get()); //返回object中保存的指针，
}
/**
 * @brief Shp::getPoint
 * @param index
 * @return
 * @说明：得到形状为point点的所有经纬度
 * 获取第index形状的经纬度，
 */
Point Shp::getPoint(Index index)
{
    if (this->pn_shape_type_ != ShapeType::Point) //多边形
        throw std::runtime_error("this shape type isn't polygon!");

    std::unique_ptr<SHPObject, void(*)(SHPObject *)> object(
        SHPReadObject(
            static_cast<SHPHandle>(this->handle_),
            static_cast<int>(index)),
        SHPDestroyObject);

    if (object == nullptr)
        throw std::invalid_argument("index may be out of range");

    return toPoint(object.get()); //返回object中保存的指针，
}
