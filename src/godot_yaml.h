#ifndef GODOT_YAML_GODOT_YAML_H
#define GODOT_YAML_GODOT_YAML_H

#include <core/Godot.hpp>
#include <gen/Reference.hpp>
#include <gen/NativeScript.hpp>
#include "c4/yml/parse.hpp"
#include "gen/File.hpp"
#include "c4/yml/common.hpp"
#include "ryml.hpp"
#include <gen/JSONParseResult.hpp>

class YAML : public godot::Reference {
	GODOT_CLASS(YAML, Reference)

private:
    c4::yml::Parser parser;
    ::godot::JSONParseResult* parseResult;

public:

	static void _register_methods();

    YAML();
	~YAML() = default;

	void _init() {};

	::godot::String print(::godot::Variant p_value);
    ::godot::Ref<::godot::JSONParseResult> parse(::godot::String text);
    void handle_error(int64_t line, const ::godot::String& message);
};

#endif // GODOT_YAML_GODOT_YAML_H