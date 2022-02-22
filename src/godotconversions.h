#include <yaml-cpp/yaml.h>
#include <core/Godot.hpp>
#include <core/Variant.hpp>

using namespace godot;

namespace YAML {

void encode_vector2(Node &node, const Vector2 &vec2);
void encode_vector3(Node &node, const Vector3 &vec3);
Vector2 decode_vector2(const Node &node);
Vector3 decode_vector3(const Node &node);

template <>
struct convert<Variant> {
	static Node encode(const Variant &rhs);
	static bool decode(const Node &node, Variant &variant);
};
} // namespace YAML
