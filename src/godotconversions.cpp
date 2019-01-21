#include <regex>
#include <sstream>
#include <string>

#include "godotconversions.h"
#include <yaml-cpp/yaml.h>
#include <Godot.hpp>
#include <Variant.hpp>

using namespace godot;

namespace YAML {

std::map<Variant::Type, std::string> type_names =
		{
			{ Variant::VECTOR2, "Vector2" },
			{ Variant::RECT2, "Rect2" },
			{ Variant::VECTOR3, "Vector3" },
			{ Variant::TRANSFORM2D, "Transform2D" },
			{ Variant::PLANE, "Plane" },
			{ Variant::QUAT, "Quat" },
			{ Variant::RECT3, "AABB" },
			{ Variant::BASIS, "Basis" },
			{ Variant::TRANSFORM, "Transform" },
			{ Variant::COLOR, "Color" },
			{ Variant::NODE_PATH, "NodePath" },
			{ Variant::POOL_BYTE_ARRAY, "PoolByteArray" },
			{ Variant::POOL_REAL_ARRAY, "PoolRealArray" },
			{ Variant::POOL_INT_ARRAY, "PoolIntArray" },
			{ Variant::POOL_STRING_ARRAY, "PoolStringArray" },
			{ Variant::POOL_VECTOR2_ARRAY, "PoolVector2Array" },
			{ Variant::POOL_VECTOR3_ARRAY, "PoolVector3Array" },
			{ Variant::POOL_COLOR_ARRAY, "PoolColorArray" },
		};

void encode_vector2(Node &node, const Vector2 &vec2) {
	node["x"] = vec2.x;
	node["y"] = vec2.y;
}

void encode_vector3(Node &node, const Vector3 &vec3) {
	node["x"] = vec3.x;
	node["y"] = vec3.y;
	node["z"] = vec3.z;
}

void encode_rect2(Node &node, const Rect2 &rec2) {
	node["position"] = static_cast<Variant>(rec2.pos);
	node["size"] = static_cast<Variant>(rec2.size);
}

void encode_aabb(Node &node, const AABB &aabb) {
	node["position"] = static_cast<Variant>(aabb.position);
	node["size"] = static_cast<Variant>(aabb.size);
}

void encode_transform(Node &node, const Transform &transf) {
	node["basis"] = static_cast<Variant>(transf.basis);
	node["origin"] = static_cast<Variant>(transf.origin);
}

void encode_transform2d(Node &node, const Transform2D &transf2d) {
	node["x"] = static_cast<Variant>(transf2d.elements[0]);
	node["y"] = static_cast<Variant>(transf2d.elements[1]);
	node["origin"] = static_cast<Variant>(transf2d.elements[2]);
}

void encode_plane(Node &node, const Plane &plane) {
	node["normal"] = static_cast<Variant>(plane.normal);
	node["d"] = static_cast<Variant>(plane.d);
}

void encode_quat(Node &node, const Quat &quat) {
	node["w"] = static_cast<Variant>(quat.w);
	node["x"] = static_cast<Variant>(quat.x);
	node["y"] = static_cast<Variant>(quat.y);
	node["z"] = static_cast<Variant>(quat.z);
}

void encode_basis(Node &node, const Basis &basis) {
	// Using basis.x, basis.y and basis.z leads to all 3 nodes being the same value.
	node["x"] = static_cast<Variant>(basis.elements[0]);
	node["y"] = static_cast<Variant>(basis.elements[1]);
	node["z"] = static_cast<Variant>(basis.elements[2]);
}

void encode_array(Node &node, const Array &arr) {
	for (int i = 0; i < arr.size(); ++i) {
		node.push_back(arr[i]);
	}
}

void encode_dictionary(Node &node, const Dictionary &dict) {
	Array keys = dict.keys();
	Array values = dict.values();
	for (int i = 0; i < keys.size(); ++i) {
		Variant key = keys[i];
		Variant value = values[i];
		node[key] = value;
	}
}

void encode_color(Node &node, const Color &color) {
	node["r"] = static_cast<Variant>(color.r);
	node["g"] = static_cast<Variant>(color.g);
	node["b"] = static_cast<Variant>(color.b);
	node["a"] = static_cast<Variant>(color.a);
}

void encode_node_path(Node &node, const NodePath &node_path) {
	node = static_cast<Variant>(godot::String(node_path));
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

Rect2 decode_rect2(const Node &node) {
	Vector2 pos = node["position"].as<Variant>();
	Vector2 size = node["size"].as<Variant>();
	return Rect2(pos, size);
}

AABB decode_aabb(const Node &node) {
	Vector3 pos = node["position"].as<Variant>();
	Vector3 size = node["size"].as<Variant>();
	return AABB(pos, size);
}

Transform decode_transform(const Node &node) {
	Basis basis = node["basis"].as<Variant>();
	Vector3 origin = node["origin"].as<Variant>();
	return Transform(basis, origin);
}

Transform2D decode_transform2d(const Node &node) {
	Vector2 x_axis = node["x"].as<Variant>();
	Vector2 y_axis = node["y"].as<Variant>();
	Vector2 origin = node["origin"].as<Variant>();
	return Transform2D(x_axis.x, x_axis.y, y_axis.x, y_axis.y, origin.x, origin.y);
}

Plane decode_plane(const Node &node) {
	float d = node["d"].as<Variant>();
	Vector3 normal = node["normal"].as<Variant>();
	return Plane(normal, d);
}

Quat decode_quat(const Node &node) {
	float w = node["w"].as<Variant>();
	float x = node["x"].as<Variant>();
	float y = node["y"].as<Variant>();
	float z = node["z"].as<Variant>();
	return Quat(x, y, z, w);
}

Basis decode_basis(const Node &node) {
	Vector3 x = node["x"].as<Variant>();
	Vector3 y = node["y"].as<Variant>();
	Vector3 z = node["z"].as<Variant>();
	return Basis(x, y, z);
}

void decode_array(const Node &node, Array &array) {
	for (YAML::const_iterator child = node.begin(); child != node.end(); ++child) {
		array.push_back(child->as<Variant>());
	}
}

Dictionary decode_dictionary(const Node &node) {
	Dictionary dict;
	std::map<Variant, Variant> map = node.as<std::map<Variant, Variant>>();
	typedef std::map<Variant, Variant>::const_iterator it_type;
	for (it_type iterator = map.begin(); iterator != map.end(); ++iterator) {
		dict[iterator->first] = iterator->second;
	}
	return dict;
}

Color decode_color(const Node &node) {
	real_t r = node["r"].as<Variant>();
	real_t g = node["g"].as<Variant>();
	real_t b = node["b"].as<Variant>();
	if (node["a"]) {
		real_t a = node["a"].as<Variant>();
		return Color(r, g, b, a);
	} else {
		return Color(r, g, b);
	}
}

NodePath decode_node_path(const Node &node) {
	return NodePath(godot::String(node.as<std::string>().c_str()));
}

Node convert<Variant>::encode(const Variant &rhs) {
	YAML::Node node;
	std::ostringstream oss;
	Variant::Type var_type = rhs.get_type();
	oss << "Godot/" << type_names[var_type];
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
		case Variant::POOL_VECTOR2_ARRAY:
		case Variant::POOL_VECTOR3_ARRAY:
		case Variant::POOL_COLOR_ARRAY:
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
			String string = rhs.operator String();
			node = string.alloc_c_string();
			break;
		}
		case Variant::BOOL: {
			node = (bool)rhs;
		}
		case Variant::DICTIONARY: {
			encode_dictionary(node, rhs);
			break;
		}
		case Variant::RECT2: {
			encode_rect2(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::RECT3: {
			encode_aabb(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::TRANSFORM: {
			encode_transform(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::TRANSFORM2D: {
			encode_transform2d(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::PLANE: {
			encode_plane(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::QUAT: {
			encode_quat(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::BASIS: {
			encode_basis(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::COLOR: {
			encode_color(node, rhs);
			needsTag = true;
			break;
		}
		case Variant::NODE_PATH: {
			encode_node_path(node, rhs);
			needsTag = true;
			break;
		}
		default:
			node = static_cast<Variant>(rhs.operator String());
			break;
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
		std::string token_godot = tokens[0];
		std::transform(token_godot.begin(), token_godot.end(), token_godot.begin(), ::tolower);
		if (token_godot == "godot" && tokens.size() == 2) {
			Variant::Type var_type;
			std::string type_value = tokens[1];
			std::transform(type_value.begin(), type_value.end(), type_value.begin(), ::tolower);
			bool found = false;
			for (std::map<Variant::Type, std::string>::iterator it = type_names.begin(); it != type_names.end(); ++it) {
				std::string value = it->second;
				std::transform(value.begin(), value.end(), value.begin(), ::tolower);
				if (value == type_value) {
					var_type = it->first;
					found = true;
					break;
				}
			}
			if (!found) {
				var_type = static_cast<Variant::Type>(std::stoi(tokens[2]));
			}
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
					godot::Array array = Array();
					decode_array(node, array);
					variant = godot::PoolIntArray(array);
					break;
				}
				case Variant::POOL_REAL_ARRAY: {
					godot::Array array = Array();
					decode_array(node, array);
					variant = godot::PoolRealArray(array);
					break;
				}
				case Variant::POOL_STRING_ARRAY: {
					godot::Array array = Array();
					decode_array(node, array);
					variant = godot::PoolStringArray(array);
					break;
				}
				case Variant::POOL_VECTOR2_ARRAY: {
					godot::Array array = Array();
					decode_array(node, array);
					variant = godot::PoolVector2Array(array);
					break;
				}
				case Variant::POOL_VECTOR3_ARRAY: {
					godot::Array array = Array();
					decode_array(node, array);
					variant = godot::PoolVector3Array(array);
					break;
				}
				case Variant::POOL_COLOR_ARRAY: {
					godot::Array array = Array();
					decode_array(node, array);
					variant = godot::PoolColorArray(array);
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
				case Variant::RECT2: {
					variant = decode_rect2(node);
					break;
				}
				case Variant::RECT3: {
					variant = decode_aabb(node);
					break;
				}
				case Variant::TRANSFORM: {
					variant = decode_transform(node);
					break;
				}
				case Variant::TRANSFORM2D: {
					variant = decode_transform2d(node);
					break;
				}
				case Variant::PLANE: {
					variant = decode_plane(node);
					break;
				}
				case Variant::QUAT: {
					variant = decode_quat(node);
					break;
				}
				case Variant::BASIS: {
					variant = decode_basis(node);
					break;
				}
				case Variant::COLOR: {
					variant = decode_color(node);
					break;
				}
				case Variant::NODE_PATH: {
					variant = decode_node_path(node);
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
	} // namespace YAML
	// Try to determine the type, first match will return, so order will matter.
	if (node.IsNull()) {
		variant = Variant();
		// Godot::print("Determined: Nil");
		return true;
	}
	if (node.IsSequence()) {
		Array array = Array();
		// Godot::print("Determined: Array");
		decode_array(node, array);
		variant = array;
		return true;
	}
	if (node.IsMap()) {
		variant = decode_dictionary(node);
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
