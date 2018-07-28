#include "godotyaml.h"
#include "godotconversions.h"
#include <yaml-cpp/yaml.h>

using namespace godot;

void YAMLParseResult::_register_methods()  {
}

YAMLParseResult::YAMLParseResult()  {
    // initialize any variables here
}

YAMLParseResult::~YAMLParseResult()  {
    // add your cleanup here
}

void YAMLParseResult::set_error(Error p_error) {
    error = p_error;
}

Error YAMLParseResult::get_error() const {
    return error;
}

void YAMLParseResult::set_error_string(const String &p_error_string) {
    error_string = p_error_string;
}

String YAMLParseResult::get_error_string() const {
    return error_string;
}


void YAMLParseResult::set_error_line(int p_error_line) {
    error_line = p_error_line;
}

int YAMLParseResult::get_error_line() const {
    return error_line;
}

void YAMLParseResult::set_result(const Variant &p_result) {
    result = p_result;
}

Variant YAMLParseResult::get_result() const {
    return result;
}


void godot::YAML::_register_methods() {
    register_method("print", &godot::YAML::print);
    register_method("parse", &godot::YAML::parse);
}

godot::YAML::YAML()  {
    // initialize any variables here
}

godot::YAML::~YAML()  {
    // add your cleanup here
}

String godot::YAML::print(Variant p_value) {
    ::YAML::Node node;
    node.push_back(p_value);
    std::stringstream yaml;
    yaml << node;
    return String(yaml.str().c_str());
}

Variant godot::YAML::parse(String text) {
    ::Godot::print(text);
    ::YAML::Node node = ::YAML::Load(text.alloc_c_string());
    try
    {
        return node[0].as<Variant>();
    }
    catch (::YAML::TypedBadConversion<Variant> err)
    {
        ::Godot::print(err.msg.c_str());
        return Variant();
    }
}