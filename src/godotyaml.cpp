#include "godotyaml.h"
#include "godotconversions.h"
#include <yaml-cpp/yaml.h>

using namespace godot;

void godot::YAML::_register_methods() {
	register_method("print", &godot::YAML::print);
	register_method("parse", &godot::YAML::parse);
}

godot::YAML::YAML() {
	// initialize any variables here
}

godot::YAML::~YAML() {
	// add your cleanup here
}

String godot::YAML::print(Variant p_value) {
	::YAML::Node node;
	node = p_value;
	std::stringstream yaml;
	yaml << node;
	return String(yaml.str().c_str());
}

Variant godot::YAML::parse(String text) {
	::Godot::print(text);
	::YAML::Node node = ::YAML::Load(text.alloc_c_string());
	try {
		return node.as<Variant>();
	} catch (::YAML::TypedBadConversion<Variant> err) {
		std::stringstream error_string;
		error_string << "Error parsing yaml: " << err.msg << " at "
					 << "Line " << err.mark.line + 1 << " Column " << err.mark.column + 1 << " (Position " << err.mark.pos << ")";
		Godot::print(error_string.str().c_str());
		return Variant();
	}
}