#ifndef DBF_H
#define DBF_H

#include <string>
#include <functional>
#include <vector>
#include <map>

#include "base_objects.h"

class Dbf
{
public:
    Dbf(const std::string& path);
    ~Dbf();

    std::string getStringField(Index index, const std::string& field_name);
    Row getRow(Index index);
    void designateValidFields(const std::vector<std::string> &field_names);

    Index numEntities() const
    {
        return this->_n_entities;
    }
private:
    enum class Type : int32_t
    {
        String,
        Integer,
        Double,
        Logical,
        Invalid
    };

    static const char* toString(const Type& type);

    // 用来描述pdf中字段的类型和名称
    struct Field
    {
        // 字段类型
        Type type;
        std::string name; // 字段名字

        Field();
        Field(Dbf::Type typeIn, std::string &&nameIn);
        Field(const Field&);
        Field& operator=(const Field&);
    };

    std::vector<Field> _Fields; // 保存字段的数组
    std::vector<size_t> _designated_field_indices;  // 保存第几个字段被指定

    Index _n_entities;  // DBF的记录个数
    int _n_fields; // 字段个数
    void* _handle;

};



#endif // DBF_H
