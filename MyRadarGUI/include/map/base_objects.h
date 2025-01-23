#ifndef BASE_OBJECTS_H
#define BASE_OBJECTS_H

#include <vector>
#include <list>
#include <map>
#include <string>

using Index = uint32_t;
using NodeId = uint32_t;
using LinkId = uint32_t;

// 地图的范围
struct Box
{
    double x_min;
    double x_max;
    double y_min;
    double y_max;
};

// 描述dbf中数据的字段
struct Row
{
    std::map<std::string, std::string> strings; // 固定长度字段
    std::map<std::string, int> integers;     // 不带小数
    std::map<std::string, double> doubles;   // 带小数
};

enum class ShapeType : int
{
    Invalid =   0x00,
    Point =     0x01,
    PolyLine =  0x03,
    PolyGon =   0x05
};

struct Point
{
    double x;
    double y;
};

// 多边形
struct PolyGon
{
    std::vector<std::vector<Point>> parts;
};

// 圆弧
struct PolyLine
{
    std::vector<std::vector<Point>> parts;
};

#endif // BASE_OBJECTS_H
