#include "map_data.h"
#include <QDebug>
#include <vector>
#include "mainwindow.h"

MapData::MapData()
    : china_loader(MOCT_CHINA_PATH)
{

}
/**
 * @brief MapData::load
 * @说明：获取地图的边界
 */
void MapData::load()
{
    const Box& china_box = china_loader.getBox();
    double minx = china_box.x_min;
    double miny = china_box.y_min;
    double maxx = china_box.x_max;
    double maxy = china_box.y_max;

    std::cout << "left bottom point: (" << minx << ", " << miny << ")" << std::endl;
    std::cout << "right top point: (" << maxx << ", " << maxy << ")" << std::endl;
    box.x_max = maxx;
    box.x_min = minx;
    box.y_max = maxy;
    box.y_min = miny;

}

/**
 * @brief MapData::getChinaMap
 * @说明： 将shp文件保存到vector中
 */
void MapData::getChinaMap()
{
    Index all_num = china_loader.numEntities();
    PolyLine china_polyline;

    for(int i = 0; i < all_num; ++i)
    {
        china_polyline = china_loader._shp.getPolyline(i);
        china_map.push_back(china_polyline);
    }
}











