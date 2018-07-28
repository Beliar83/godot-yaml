#include <string>
#include <regex>
#include <sstream>

#include <Godot.hpp>
#include <Variant.hpp>
#include <yaml-cpp/yaml.h>
#include "godotconversions.h"

using namespace godot;

namespace YAML {

void encode_vector2(Node& node, const Vector2& vec2)
{
    node.push_back(vec2.x);
    node.push_back(vec2.y);
}

void encode_vector3(Node& node, const Vector3& vec3)
{
    node.push_back(vec3.x);
    node.push_back(vec3.y);
    node.push_back(vec3.z);
}

Vector2 decode_vector2(const Node& node)
{
    Vector2 rhs;
    rhs.x = node[0].as<real_t>();
    rhs.y = node[1].as<real_t>();
    return rhs;
}

Vector3 decode_vector3(const Node& node)
{
    Vector3 rhs;
    rhs.x = node[0].as<real_t>();
    rhs.y = node[1].as<real_t>();
    rhs.z = node[2].as<real_t>();
    return rhs;
}

Node convert<Variant>::encode(const Variant& rhs){
    YAML::Node node;
    std::ostringstream oss;
    Variant::Type var_type = rhs.get_type();
    oss << "Godot/Variant/" << var_type;
    node.SetTag(oss.str());
    switch (var_type)
    {
        case Variant::Type::NIL:
            node.push_back("null");
            break;
        case Variant::Type::VECTOR2:
            encode_vector2(node, rhs);
            break;
        case Variant::Type::VECTOR3:
            encode_vector3(node, rhs);
            break;
    }
    return node;
}

bool convert<Variant>::decode(const YAML::Node& node, Variant& variant)
{
    std::regex type_expression(R"((?:\/)?(\w+))");
    std::string search = node.Tag();
    std::sregex_iterator pos(search.begin(), search.end(), type_expression);        
    std::vector<std::string> tokens;
    std::sregex_iterator end;
    for (; pos != end; ++pos)
    {
        tokens.push_back(pos->str(1));
    }
    if (tokens.empty())
    {
        ::Godot::print("No tokens");
        return false;
    }
    if (tokens[0] == "Godot")
    {
        if (tokens[1] == "Variant")
        {
            auto var_type = static_cast<Variant::Type>(std::stoi(tokens[2]));
            switch (var_type)
            {
                case Variant::Type::NIL:
                    variant = Variant();
                    break;
                case Variant::Type::VECTOR2:
                    variant = decode_vector2(node);
                    break;
                case Variant::Type::VECTOR3:
                    variant = decode_vector3(node);
                    break;
            }
            return true;
        }
        return false;
    }
    return false;
}
}
