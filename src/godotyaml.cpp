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


void _YAML::_register_methods() {
    register_method("print", &_YAML::print);
}

_YAML::_YAML()  {
    // initialize any variables here
}

_YAML::~_YAML()  {
    // add your cleanup here
}

String _YAML::print(Variant p_value) {
    YAML::Node node;
    node.push_back(p_value);
    std::stringstream yaml;
    yaml << node;
    return String(yaml.str().c_str());
}

