#ifndef GODOTYAML_H
#define GODOTYAML_H

#include <core/Godot.hpp>
#include <gen/Reference.hpp>
#include <gen/NativeScript.hpp>

class YAML : public godot::Reference {
	GODOT_CLASS(YAML, Reference)

public:
	static void _register_methods();

	YAML();
	~YAML();

	void _init();

	::godot::String print(::godot::Variant p_value);
	::godot::Variant parse(::godot::String text);
};

#endif