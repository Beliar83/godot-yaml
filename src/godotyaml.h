#ifndef GODOTYAML_H
#define GODOTYAML_H

#include <Godot.hpp>

namespace godot {

class YAML : public godot::GodotScript<Reference> {
    GODOT_CLASS(YAML)

public:
    static void _register_methods();

    YAML();
    ~YAML();

	String print(Variant p_value);
	Variant parse(String text);
};

}

#endif