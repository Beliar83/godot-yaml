#include <ryml_std.hpp>
#include <c4/format.hpp>
#include <regex>
#include <sstream>
#include <string>
#include "godotconversions.h"
#include <core/Variant.hpp>
#include <map>
#include <cstdio>

using namespace godot;

std::map<Variant::Type, const char*> TYPE_NAMES =
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

namespace c4 {
namespace yml {

void encode_vector_2(ryml::NodeRef *node, const Vector2 &vector_2) {
	*node |= ryml::MAP;
	node->append_child() << ryml::key("x") << vector_2.x;
	node->append_child() << ryml::key("y") << vector_2.y;
}

Vector2 decode_vector_2(ryml::NodeRef const &node) {
	Vector2 vector_2;
	node["x"] >> vector_2.x;
	node["y"] >> vector_2.y;
	return vector_2;
}

void encode_vector_3(ryml::NodeRef *node, const Vector3 &vector_3) {
	*node |= ryml::MAP;
	node->append_child() << ryml::key("x") << vector_3.x;
	node->append_child() << ryml::key("y") << vector_3.y;
	node->append_child() << ryml::key("z") << vector_3.z;
}

Vector3 decode_vector_3(ryml::NodeRef const &node) {
	Vector3 vector_3;
	node["x"] >> vector_3.x;
	node["y"] >> vector_3.y;
	node["z"] >> vector_3.z;
	return vector_3;
}

void encode_rect_2(ryml::NodeRef *node, const Rect2 &rec_2) {
	*node |= ryml::MAP;
	auto position = node->append_child();
	position |= ryml::MAP;
	position << ryml::key("position");
	position.append_child()	<< ryml::key("x") << rec_2.position.x;
	position.append_child()	<< ryml::key("y") << rec_2.position.y;
	auto size = node->append_child();
	size |= ryml::MAP;
	size << ryml::key("size");
	size.append_child()	<< ryml::key("x") << rec_2.size.x;
	size.append_child()	<< ryml::key("y") << rec_2.size.y;
}

Rect2 decode_rect_2(ryml::NodeRef const &node) {
	Rect2 rect;
	node["position"]["x"] >> rect.position.x;
	node["position"]["y"] >> rect.position.y;
	node["size"]["x"] >> rect.size.x;
	node["size"]["y"] >> rect.size.y;
	return rect;
}

void encode_aabb(ryml::NodeRef *node, const AABB &aabb) {
	*node |= ryml::MAP;
	auto position = node->append_child();
	position |= ryml::MAP;
	position << ryml::key("position");
    encode_vector_3(&position, aabb.position);
	auto size = node->append_child();
	size |= ryml::MAP;
	size << ryml::key("size");
    encode_vector_3(&size, aabb.size);
}

AABB decode_aabb(yml::NodeRef const &node) {
	AABB aabb;
	aabb.position = decode_vector_3(node["position"]);
	aabb.size = decode_vector_3(node["size"]);
	return aabb;
}

void encode_basis(ryml::NodeRef *node, const Basis &basis) {
	*node |= ::ryml::MAP;
	auto axis = node->append_child();
	axis << ::ryml::key("x");
    encode_vector_3(&axis, basis.elements[0]);

	axis = node->append_child();
	axis << ::ryml::key("y");
    encode_vector_3(&axis, basis.elements[1]);

	axis = node->append_child();
	axis << ::ryml::key("z");
    encode_vector_3(&axis, basis.elements[2]);
}

Basis decode_basis(yml::NodeRef const &node) {
    Vector3 x = decode_vector_3(node["x"]);
    Vector3 y = decode_vector_3(node["y"]);
    Vector3 z = decode_vector_3(node["z"]);
    return {x, y, z};
}


void encode_transform(ryml::NodeRef *node, const Transform &transform) {
	*node |= ryml::MAP;
	auto basis = node->append_child();
	basis << ryml::key("basis");
	encode_basis(&basis, transform.basis);
	auto origin = node->append_child();
	origin << ryml::key("origin");
    encode_vector_3(&origin, transform.origin);
}

Transform decode_transform(ryml::NodeRef const &node) {
	Transform transform;
	transform.basis = decode_basis(node["basis"]);
	transform.origin = decode_vector_3(node["origin"]);
	return transform;
}

void encode_transform_2_d(ryml::NodeRef *node, const Transform2D &transform_2_d) {
	*node |= ryml::MAP;
	auto x_axis = node->append_child();
    x_axis << ryml::key("x");
    encode_vector_2(&x_axis, transform_2_d.get_axis(0));
	auto y_axis = node->append_child();
    y_axis << ryml::key("y");
    encode_vector_2(&y_axis, transform_2_d.get_axis(1));
	auto origin = node->append_child();
	origin << ryml::key("origin");
    encode_vector_2(&origin, transform_2_d.get_origin());
}

Transform2D decode_transform_2_d(ryml::NodeRef const &node) {
	Transform2D transform_2_d;
    transform_2_d.set_axis(0, decode_vector_2(node["x"]));
    transform_2_d.set_axis(1, decode_vector_2(node["y"]));
    transform_2_d.set_origin(decode_vector_2(node["origin"]));
	return transform_2_d;
}

void encode_plane(ryml::NodeRef *node, const Plane &plane) {
	*node |= ryml::MAP;
	auto normal = node ->append_child();
	normal << ryml::key("normal");
    encode_vector_3(&normal, plane.normal);
	auto distance = node ->append_child();
	distance << ryml::key("d") << plane.d;
}

Plane decode_plane(ryml::NodeRef const &node) {
	Plane plane;
	plane.normal = decode_vector_3(node["normal"]);
	node["d"] >> plane.d;
	return plane;
}

void encode_quat(ryml::NodeRef *node, const Quat &quat) {
	*node |= ryml::MAP;
	node->append_child() << ryml::key("x") << quat.x;
	node->append_child() <<  ryml::key("y") << quat.y;
	node->append_child() <<  ryml::key("z") << quat.z;
	node->append_child() << ryml::key("w") << quat.w;
}

Quat decode_quat(ryml::NodeRef const &node) {
	Quat quat;
	node["x"] >> quat.x;
	node["y"] >> quat.y;
	node["z"] >> quat.z;
	node["w"] >> quat.w;
	return quat;
}

void encode_array(ryml::NodeRef *node, const Array &arr) {
	*node |= ryml::SEQ;
	for (int i = 0; i < arr.size(); ++i) {
		node->append_child() << arr[i];
	}
}

Array decode_array(ryml::NodeRef const &node) {
	Array array;
	for (auto child = node.begin(); child != node.end(); ++child) {
		::godot::Variant variant;
		*child >> variant;
		array.push_back(variant);
	}
	return array;
}

void encode_dictionary(ryml::NodeRef *node, const Dictionary &dict) {
	Array keys = dict.keys();
	Array values = dict.values();
	*node |= ryml::MAP;
	for (int i = 0; i < keys.size(); ++i) {
		Variant key = keys[i];
		Variant value = values[i];
		auto key_string = std::string(key.operator String().ascii().get_data());
		node->append_child() << ::ryml::key(key_string) << value;
	}
}

Dictionary decode_dictionary(ryml::NodeRef const &node) {
	Dictionary dict;
	for (auto iterator = node.begin(); iterator != node.end(); ++iterator) {
		auto child_node = *iterator;

		auto key = String(std::string(child_node.key().data(), child_node.key().len).c_str());
		Variant value;
		node[child_node.key()] >> value;
		dict[key] = value;
	}
	return dict;
}

void encode_color(ryml::NodeRef *node, const Color &color) {
	*node |= ryml::MAP;
	node->append_child() << ryml::key("r") << color.r;
	node->append_child() << ryml::key("g") << color.g;
	node->append_child() << ryml::key("b") << color.b;
	node->append_child() << ryml::key("a") << color.a;
}

Color decode_color(ryml::NodeRef const &node) {
	Color color;
	node["r"] >> color.r;
	node["g"] >> color.g;
	node["b"] >> color.b;
	if (node.has_child("a")) {
		node["a"] >> color.a;
	}
	return color;
}

void write(ryml::NodeRef *node, const ::godot::Variant &variant) {
		Variant::Type var_type = variant.get_type();
		bool needs_tag = false;
		switch (var_type) {
			case Variant::NIL: {
				node->set_val(nullptr);
				break;
			}
			case Variant::VECTOR2: {
                needs_tag = true;
                encode_vector_2(node, variant);
				break;
			}
			case Variant::VECTOR3: {
                needs_tag = true;
                encode_vector_3(node, variant);
				break;
			}
			case Variant::POOL_INT_ARRAY:
			case Variant::POOL_REAL_ARRAY:
			case Variant::POOL_STRING_ARRAY:
			case Variant::POOL_VECTOR2_ARRAY:
			case Variant::POOL_VECTOR3_ARRAY:
			case Variant::POOL_COLOR_ARRAY:
			case Variant::POOL_BYTE_ARRAY:
				// Pool arrays need a tag to correctly decode them as a pool.
                needs_tag = true;
			case Variant::ARRAY: {
				encode_array(node, variant);
				break;
			}
			case Variant::INT: {
				*node << variant.operator int();
				break;
			}
			case Variant::REAL: {
				*node << variant.operator double ();
				break;
			}
			case Variant::STRING: {
				*node |= VALQUO;
				String string = variant.operator String();
				csubstr str_val = ::ryml::to_csubstr(string.alloc_c_string());
				*node << str_val;
				break;
			}
			case Variant::BOOL: {
				if (variant.operator bool()) {
					*node << "true";
				} else {
					*node << "false";
				}
				break;
			}
			case Variant::DICTIONARY: {
				encode_dictionary(node, variant);
				break;
			}
			case Variant::RECT2: {
                encode_rect_2(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::RECT3: {
				encode_aabb(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::TRANSFORM: {
				encode_transform(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::TRANSFORM2D: {
                encode_transform_2_d(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::PLANE: {
				encode_plane(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::QUAT: {
				encode_quat(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::BASIS: {
				encode_basis(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::COLOR: {
				encode_color(node, variant);
                needs_tag = true;
				break;
			}
			case Variant::NODE_PATH: {
				*node |= VALQUO;
				String string = variant.operator String();
				csubstr str_val = ::ryml::to_csubstr(string.alloc_c_string());
				*node << str_val;
                needs_tag = true;
				break;
			}
			default:
				*node << Variant(variant.operator String());
				break;
		}
		if (needs_tag) {
			auto buf = new char[256];
			sprintf(buf, "Godot/%s", TYPE_NAMES[var_type]);
			node->set_val_tag(ryml::to_csubstr(buf));
		}

	}

	////	 Tries to convert a node to a Godot Variant. There should be (almost?) no value that is not converted.

	bool read(ryml::NodeRef const& node, Variant *variant)
	{
		if (node.has_val_tag()) {
			auto tag = node.val_tag();
			auto tag_string = std::string(tag.data(), tag.len).substr(1);
			std::regex type_expression(R"((?:\/)?(\w+))");
			std::sregex_iterator pos(tag_string.begin(), tag_string.end(), type_expression);
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
					for (auto it = TYPE_NAMES.begin(); it != TYPE_NAMES.end(); ++it) {
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
//							Godot::print("Determined: Nil");
							*variant = Variant();
							break;
						}
						case Variant::VECTOR2: {
//							Godot::print("Determined: VECTOR2");
							*variant = decode_vector_2(node);
							break;
						}
						case Variant::VECTOR3: {
//							Godot::print("Determined: VECTOR3");
							*variant = decode_vector_3(node);
							break;
						}
						case Variant::POOL_BYTE_ARRAY: {
//							Godot::print("Determined: POOL_BYTE_ARRAY");
							*variant = godot::PoolByteArray(decode_array(node));
							break;
						}
						case Variant::POOL_INT_ARRAY: {
//							Godot::print("Determined: POOL_INT_ARRAY");
							*variant = godot::PoolIntArray(decode_array(node));
							break;
						}
						case Variant::POOL_REAL_ARRAY: {
//							Godot::print("Determined: POOL_REAL_ARRAY");
							*variant = godot::PoolRealArray(decode_array(node));
							break;
						}
						case Variant::POOL_STRING_ARRAY: {
//							Godot::print("Determined: POOL_STRING_ARRAY");
							*variant = godot::PoolStringArray(decode_array(node));
							break;
						}
						case Variant::POOL_VECTOR2_ARRAY: {
//							Godot::print("Determined: POOL_VECTOR2_ARRAY");
							*variant = godot::PoolVector2Array(decode_array(node));
							break;
						}
						case Variant::POOL_VECTOR3_ARRAY: {
//							Godot::print("Determined: POOL_VECTOR3_ARRAY");
							*variant = godot::PoolVector3Array(decode_array(node));
							break;
						}
						case Variant::POOL_COLOR_ARRAY: {
//							Godot::print("Determined: POOL_COLOR_ARRAY");
							*variant = godot::PoolColorArray(decode_array(node));
							break;
						}
						case Variant::INT: {
//							Godot::print("Determined: INT");
							int64_t value;
							node >> value;
							*variant = value;
							break;
						}
						case Variant::REAL: {
//							Godot::print("Determined: REAL");
							double value;
							node >> value;
							*variant = value;
							break;
						}
						case Variant::STRING: {
//							Godot::print("Determined: STRING");
							*variant = std::string(node.val().data(), node.val().len).c_str();
							break;
						}
						case Variant::RECT2: {
//							Godot::print("Determined: RECT2");
							*variant = decode_rect_2(node);
							break;
						}
						case Variant::RECT3: {
//							Godot::print("Determined: RECT3");
							*variant = decode_aabb(node);
							break;
						}
						case Variant::TRANSFORM: {
//							Godot::print("Determined: TRANSFORM");
							*variant = decode_transform(node);
							break;
						}
						case Variant::TRANSFORM2D: {
//							Godot::print("Determined: TRANSFORM2D");
							*variant = decode_transform_2_d(node);
							break;
						}
						case Variant::PLANE: {
//							Godot::print("Determined: PLANE");
							*variant = decode_plane(node);
							break;
						}
						case Variant::QUAT: {
//							Godot::print("Determined: QUAT");
							*variant = decode_quat(node);
							break;
						}
						case Variant::BASIS: {
//							Godot::print("Determined: BASIS");
							*variant = decode_basis(node);
							break;
						}
						case Variant::COLOR: {
//							Godot::print("Determined: COLOR");
							*variant = decode_color(node);
							break;
						}
						case Variant::NODE_PATH: {
//							Godot::print("Determined: NODE_PATH");
							*variant = NodePath(std::string(node.val().data(), node.val().len).c_str());
							break;
						}
						default: {
							std::stringstream message;
							message << "Variant type " << var_type << " not yet supported";
							Godot::print(message.str().c_str());
							return false;
						}
					}
				}
			}
		} else {
//			Godot::print("No tag");
			if (node.is_seq()) {
//				Godot::print("Determined: Array");
				*variant = decode_array(node);
				return true;
			}
			if (node.is_map()) {
//				Godot::print("Determined: Dictionary");
				*variant = decode_dictionary(node);
				return true;
			}
			// Try to determine the type, first match will return, so order will matter.
            if (!node.is_val_quoted()) {
                if (node.val_is_null()) {
                    *variant = Variant();
//						Godot::print("Determined: Nil");
                    return true;
                }
                int64_t int_val;
                if (::c4::atox(node.val(), &int_val)) {
//						Godot::print("Determined: INT");
                    *variant = int_val;
                    return true;
                }

                double float_val;
                if (::c4::atod(node.val(), &float_val)) {
//						Godot::print("Determined: float");
                    *variant = float_val;
                    return true;
                }

                bool bool_val;
                if (from_chars(node.val(), &bool_val)) {
//						Godot::print("Determined: bool");
                    *variant = bool_val;
                    return true;
                }
            }

            *variant = std::string(node.val().data(), node.val().len).c_str();
            return true;
		}
		return true;
	}
} // namespace yaml
} // namespace c4
