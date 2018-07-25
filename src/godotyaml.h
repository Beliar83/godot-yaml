#ifndef GODOT_YAML_H
#define GODOT_YAML_H

#include <Godot.hpp>

namespace godot {

class _YAML;

class YAMLParseResult : public godot::GodotScript<Reference> {
    GODOT_CLASS(YAMLParseResult)

private:

	friend class _YAML;

	Error error;
	String error_string;
	int error_line;

    Variant result;

public:
	void set_error(Error p_error);
	Error get_error() const;

	void set_error_string(const String &p_error_string);
	String get_error_string() const;

	void set_error_line(int p_error_line);
	int get_error_line() const;

	void set_result(const Variant &p_result);
    Variant get_result() const;

    static void _register_methods();

    YAMLParseResult();
    ~YAMLParseResult();

};

class _YAML : public godot::GodotScript<Reference> {
    GODOT_CLASS(YAML)

public:
    static void _register_methods();

    _YAML();
    ~_YAML();

	String print(Variant p_value);
};

}

#endif