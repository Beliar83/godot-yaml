#include <regex>
#include <sstream>
#include <string>

#include "godotconversions.h"
#include <yaml-cpp/yaml.h>
#include <Godot.hpp>
#include <Variant.hpp>

using namespace godot;

namespace YAML {

void encode_vector2(Node &node, const Vector2 &vec2) {
	node.push_back(vec2.x);
	node.push_back(vec2.y);
}

void encode_vector3(Node &node, const Vector3 &vec3) {
	node.push_back(vec3.x);
	node.push_back(vec3.y);
	node.push_back(vec3.z);
}

void encode_array(Node &node, const Array &arr) {
	for (int i = 0; i < arr.size(); ++i) {
		node.push_back(arr[i]);
	}
}

Vector2 decode_vector2(const Node &node) {
	Vector2 rhs;
	rhs.x = node[0].as<real_t>();
	rhs.y = node[1].as<real_t>();
	return rhs;
}

Vector3 decode_vector3(const Node &node) {
	Vector3 rhs;
	rhs.x = node[0].as<real_t>();
	rhs.y = node[1].as<real_t>();
	rhs.z = node[2].as<real_t>();
	return rhs;
}

void decode_array(const Node &node, Array &array) {
	for (YAML::const_iterator child = node.begin(); child != node.end(); ++child) {
		array.push_back(child->as<Variant>());
	}
}

Node convert<Variant>::encode(const Variant &rhs) {
	YAML::Node node;
	std::ostringstream oss;
	Variant::Type var_type = rhs.get_type();
	oss << "Godot/Variant/" << var_type;
	node.SetTag(oss.str());
	switch (var_type) {
		case Variant::NIL:
			node.push_back(Null);
			break;
		case Variant::VECTOR2:
			encode_vector2(node, rhs);
			break;
		case Variant::VECTOR3:
			encode_vector3(node, rhs);
			break;
		case Variant::POOL_INT_ARRAY:
		case Variant::POOL_REAL_ARRAY:
		case Variant::POOL_STRING_ARRAY:
		case Variant::ARRAY:
			encode_array(node, rhs);
			break;
		case Variant::INT:
			node.push_back((int)rhs);
			break;
		case Variant::REAL:
			node.push_back((double)rhs);
			break;
		case Variant::STRING:
			String string = (String)rhs;
			node.push_back(string.alloc_c_string());
			break;
	}
	return node;
}

bool convert<Variant>::decode(const YAML::Node &node, Variant &variant) {
	std::regex type_expression(R"((?:\/)?(\w+))");
	std::string search = node.Tag();
	std::sregex_iterator pos(search.begin(), search.end(), type_expression);
	std::vector<std::string> tokens;
	std::sregex_iterator end;
	for (; pos != end; ++pos) {
		tokens.push_back(pos->str(1));
	}
	if (tokens.empty()) {
		::Godot::print("No tokens");
		return false;
	}
	if (tokens[0] == "Godot") {
		if (tokens[1] == "Variant") {
			auto var_type = static_cast<Variant::Type>(std::stoi(tokens[2]));
			switch (var_type) {
				case Variant::NIL: {
					variant = Variant();
					break;
				}
				case Variant::VECTOR2: {
					variant = decode_vector2(node);
					break;
				}
				case Variant::VECTOR3: {
					variant = decode_vector3(node);
					break;
				}
				case Variant::POOL_INT_ARRAY: {
					variant = godot::PoolIntArray();
				}
				case Variant::POOL_REAL_ARRAY: {
					variant = godot::PoolRealArray();
				}
				case Variant::POOL_STRING_ARRAY: {
					variant = godot::PoolStringArray();
				}
				case Variant::ARRAY: {
					if (variant.get_type() == Variant::NIL) {
						variant = Array();
					}
					godot::Array array = (Array)variant;
					decode_array(node, array);
					variant = array;
					break;
				}
				case Variant::INT: {
					variant = node[0].as<int64_t>();
					break;
				}
				case Variant::REAL: {
					variant = node[0].as<double>();
					break;
				}
				case Variant::STRING: {
					variant = String(node[0].as<std::string>().c_str());
					break;
				}
				default: {
					std::stringstream message;
					message << "Variant type " << var_type << " not yet supported";
					Godot::print(message.str().c_str());
					return false;
				}
			}
			return true;
		}
		return false;
	}
	return false;
}
} // namespace YAML
