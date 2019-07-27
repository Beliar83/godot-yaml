#ifndef GODOTYAML_H
#define GODOTYAML_H

#include <core/Godot.hpp>
#include <gen/Reference.hpp>
#include <gen/NativeScript.hpp>

namespace godot {

class YAML : public Reference {
	GODOT_CLASS(YAML, Reference)

public:
	static void _register_methods();

	YAML();
	~YAML();

	void _init();

	String print(Variant p_value);
	Variant parse(String text);
};

} // namespace godot

#endif