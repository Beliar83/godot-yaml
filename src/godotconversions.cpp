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
	node["x"] = vec2.x;
	node["y"] = vec2.y;
}

void encode_vector3(Node &node, const Vector3 &vec3) {
	node["x"] = vec3.x;
	node["y"] = vec3.y;
	node["z"] = vec3.z;
}

void encode_array(Node &node, const Array &arr) {
	for (int i = 0; i < arr.size(); ++i) {
		node.push_back(arr[i]);
	}
}

Vector2 decode_vector2(const Node &node) {
	Vector2 rhs;
	rhs.x = node["x"].as<real_t>();
	rhs.y = node["y"].as<real_t>();
	return rhs;
}

Vector3 decode_vector3(const Node &node) {
	Vector3 rhs;
	rhs.x = node["x"].as<real_t>();
	rhs.y = node["y"].as<real_t>();
	rhs.z = node["z"].as<real_t>();
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
	bool needsTag = false;
	switch (var_type) {
		case Variant::NIL: {
			node = Null;
			break;
		}
		case Variant::VECTOR2: {
			needsTag = true;
			encode_vector2(node, rhs);
			break;
		}
		case Variant::VECTOR3: {
			needsTag = true;
			encode_vector3(node, rhs);
			break;
		}
		case Variant::POOL_INT_ARRAY:
		case Variant::POOL_REAL_ARRAY:
		case Variant::POOL_STRING_ARRAY:
			// Pool arrays need a tag to correctly decode them as a pool.
			needsTag = true;
		case Variant::ARRAY: {
			encode_array(node, rhs);
			break;
		}
		case Variant::INT: {
			node = (int)rhs;
			break;
		}
		case Variant::REAL: {
			node = (double)rhs;
			break;
		}
		case Variant::STRING: {
			String string = (String)rhs;
			node = string.alloc_c_string();
			break;
		}
		case Variant::BOOL: {
			node = (bool)rhs;
			break;
		}
	}
	if (needsTag) {
		node.SetTag(oss.str());
	}
	return node;
}

// Tries to convert a node to a Godot Variant. There should be (almost?) no value that is not converted.
bool convert<Variant>::decode(const YAML::Node &node, Variant &variant) {
	std::regex type_expression(R"((?:\/)?(\w+))");
	std::string search = node.Tag();
	std::sregex_iterator pos(search.begin(), search.end(), type_expression);
	std::vector<std::string> tokens;
	std::sregex_iterator end;
	for (; pos != end; ++pos) {
		tokens.push_back(pos->str(1));
	}
	if (!tokens.empty()) {
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
						if (variant.get_type() == Variant::NIL) {
							variant = Array();
						}
						godot::Array array = (Array)variant;
						decode_array(node, array);
						variant = array;
						break;
					}
					case Variant::INT: {
						variant = node.as<int64_t>();
						break;
					}
					case Variant::REAL: {
						variant = node.as<double>();
						break;
					}
					case Variant::STRING: {
						variant = String(node.as<std::string>().c_str());
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
	}
	// Try to determine the type, first match will return, so order will matter.
	if (node.IsNull()) {
		variant = Variant();
		// Godot::print("Determined: Nil");
		return true;
	}
	if (node.IsSequence()) {
		variant = Array();
		// Godot::print("Determined: Array");
		decode_array(node, (Array)variant);
		return true;
	}
	try {
		variant = node.as<int64_t>();
		// Godot::print("Determined: Int64");
		return true;
	} catch (::YAML::TypedBadConversion<int64_t> err) {
	}
	try {
		variant = node.as<double>();
		// Godot::print("Determined: Real");
		return true;
	} catch (::YAML::TypedBadConversion<double> err) {
	}
	try {
		variant = node.as<bool>();
		return true;
	} catch (::YAML::TypedBadConversion<bool> err) {
	}
	// Probably catches anything else (except empty values)
	try {
		variant = String(node.as<std::string>().c_str());
		// Godot::print("Guessed: String");
		return true;
	} catch (::YAML::TypedBadConversion<std::string> err) {
	}
	// Will probably never be reached.
	std::stringstream message;
	message << "Could not determine type of node at Line " << node.Mark().line + 1 << " Column " << node.Mark().column + 1 << " (Position " << node.Mark().pos << ")";
	Godot::print(message.str().c_str());
	return false;
}
} // namespace YAML
