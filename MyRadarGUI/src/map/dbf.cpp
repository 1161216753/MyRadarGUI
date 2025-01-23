#include "dbf.h"
#include "shape/shapefil.h"
#include <utility>
#include <iostream>
#include <algorithm>
#include <QDebug>

/**
 * @brief Dbf::Dbf
 * @param path
 * @构造函数
 * @从DBF读取字段，记录长度
 */

Dbf::Dbf(const std::string &path)
    : _handle(static_cast<void*>(DBFOpen(path.c_str(),"rb")))
{
    if(this->_handle == nullptr)
        throw std::runtime_error("Faild to DBFOpen...");

    // 有效的字段个数为多少
    this->_n_fields = DBFGetFieldCount(static_cast<DBFHandle>(this->_handle));
    // 记录个数
    this->_n_entities = DBFGetRecordCount(static_cast<DBFHandle>(this->_handle));

    std::cout << "[" << path << "]" << std::endl;
    std::cout << "dbf field number"<< this->_n_fields<< std::endl;
    std::cout << "dbf entities number " << this->_n_entities<<std::endl;

    // 将数据存放在字段中、
    for(int i = 0; i < _n_fields; i++)
    {
        std::vector<char> field_name_buf(12, '\0');
        DBFFieldType dbftype = DBFGetFieldInfo(
            static_cast<DBFHandle>(this->_handle),
            i,  // 第 i 个 字段
            field_name_buf.data(),
            nullptr,
            nullptr);
        Dbf::Type type;
        switch (dbftype) {
        case FTString:
            type = Type::String;   //固定长度的字符串字段
            break;
        case FTInteger:
            type = Type::Integer;  //不带小数的数字字段
            break;
        case FTDouble:
            type = Type::Double;  //带小数的数字字段
            break;
        case FTLogical:
            type = Type::Logical;  //逻辑字段
            break;
        case FTInvalid:
        default:
            type = Type::Invalid;  //无法识别的字段类型
            break;
        }
        // 存在数组里
        this->_Fields.emplace_back(type, std::string(field_name_buf.data()));
    }
}

/**
 * @brief Dbf::~Dbf
 * @retun 释放资源
 */
Dbf::~Dbf()
{
    // 关闭 dbf 文件
    DBFClose( static_cast<DBFHandle>(this->_handle));
}

/**
 * @brief Dbf::designateValidFields
 * @param field_names
 * @功能：判断所给的字段名称field_name是否存在于DBF中
 * @_designated_field_indices用于保存_fields中的第几个字段被指定
 */
void Dbf::designateValidFields(const std::vector<std::string> &field_names)
{
    for (size_t i = 0; i < this->_Fields.size(); ++i) {
        for (const auto &field_name : field_names) {
            if (field_name == this->_Fields[i].name) {

                //_fields中的第i个字段被指定
                std::cout << "field designated: " << field_name << std::endl;
                this->_designated_field_indices.push_back(i);
                break;
            }
        }
    }
}

/**
 * @brief Dbf::getStringField
 * @param index
 * @param field_name
 * @return
 */
std::string Dbf::getStringField(Index index, const std::string &field_name)
{
    // 根据名字获取在第几个字段中
    int field_index = DBFGetFieldIndex(static_cast<DBFHandle>(this->_handle),field_name.c_str());
    if(field_index == -1)
    {
        throw std::invalid_argument("invalid arg in Dbf::getStringField");
    }

    // 根据记录和字段获取第几个值，获取内容
    const char* field_value = DBFReadStringAttribute(static_cast<DBFHandle>(this->_handle), index, field_index);
    if(field_value == nullptr || field_value[0] == '\0')
    {
        return std::string();
    }else
    {
        return std::string(field_value);
    }
}

/**
 * @brief Dbf::getRow
 * @param index 要查询的是第几条记录
 * @return
 */
Row Dbf::getRow(Index index)
{
    Row row;
    for (auto i : this->_designated_field_indices) {
        switch (this->_Fields[i].type) {

        case Type::Double:  //查询第i个字段的第index值，将其作为double返回
            row.doubles[this->_Fields[i].name] =
                DBFReadDoubleAttribute(
                    static_cast<DBFHandle>(this->_handle),
                    static_cast<int>(index), static_cast<int>(i));
            break;

        case Type::Integer:  //查询第i个字段的第index值，将其作为整数返回
            row.integers[this->_Fields[i].name] =
                DBFReadIntegerAttribute(
                    static_cast<DBFHandle>(this->_handle),
                    static_cast<int>(index), static_cast<int>(i));
            break;

        case Type::String: //查询第i个字段的第index值，将其作为string返回
        {
            const char *val = DBFReadStringAttribute(
                static_cast<DBFHandle>(this->_handle),
                static_cast<int>(index), static_cast<int>(i));
            row.strings[this->_Fields[i].name] = (val == nullptr) ?
                                                     std::string() : std::string(val);
            break;
        }

        default:
            throw std::invalid_argument("Unknown field type in dbf...");
        }
    }
    return row;  //存放的是index/double/string型字段所对应的值
}



Dbf::Field::Field(Dbf::Type typeIn, std::string &&nameIn) :
    type(typeIn), name(std::move(nameIn))
{
}

Dbf::Field::Field(const Dbf::Field &other) : type(other.type), name(other.name) {}

/**
 * @brief Dbf::Field::operator =
 * @param other
 * @return 重载赋值构造函数
 */
Dbf::Field& Dbf::Field::operator=(const Dbf::Field &other)
{
    if (this == &other)
        return *this;
    this->type = other.type;
    this->name = other.name;

    return *this;
}

/**
 * @brief Dbf::toString
 * @param type
 * @return 将字段类型转换为string格式
 */
const char *Dbf::toString(const Type &type)
{
    switch (type) {
    case Type::String: return "String";
    case Type::Integer: return "Integer";
    case Type::Double: return "Double";
    case Type::Logical: return "Logical";
    case Type::Invalid:
    default:
        return "Invalid";
    }
}
