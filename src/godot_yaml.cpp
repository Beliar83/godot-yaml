#include "godot_yaml.h"
#include "c4/yml/tree.hpp"
#include "gen/File.hpp"
#include "godotconversions.h"
#include <core/GodotGlobal.hpp>
#include <ryml_std.hpp>
#include <sstream>
#include <utility>

void YAML::_register_methods() {
    register_method("print", &YAML::print);
    register_method("parse", &YAML::parse);
}

void on_error(const char *msg, size_t len, ryml::Location loc, void *user_data) {
    std::stringstream error_string_stream;
    auto message_string = std::string(msg, len);
    auto name_string = std::string(loc.name.str, loc.name.len);
    error_string_stream << "Error parsing yaml at line " << loc.line << ": " << message_string;
    auto error_string = ::godot::String(error_string_stream.str().c_str());
    ERR_PRINT(error_string);
    ((YAML *) user_data)->handle_error(loc.line, error_string);
}

YAML::YAML() {
    ryml::Callbacks callbacks;
    callbacks.m_error = on_error;
    callbacks.m_user_data = this;
    ::ryml::set_callbacks(callbacks);
}

::godot::String YAML::print(::godot::Variant variant) {// NOLINT(performance-unnecessary-value-param)
    ::ryml::Tree tree;
    tree.rootref() << variant;
    return {::ryml::emitrs<std::string>(tree).c_str()};
}

::godot::Ref<::godot::JSONParseResult> YAML::parse(::godot::String text) { // NOLINT(performance-unnecessary-value-param)
    parser = c4::yml::Parser();
    parseResult = ::godot::JSONParseResult::_new();
    parseResult->set_error(GODOT_OK);
    parseResult->set_result({});
    parseResult->set_error_line(0);
    parseResult->set_error_string("");
    auto text_string = ::ryml::to_substr(text.alloc_c_string());
    auto tree = parser.parse_in_place("", text_string);
    if (parseResult->get_error() == ::godot::Error::OK) {
        ::godot::Variant variant;
        tree.rootref() >> variant;
        parseResult->set_result(variant);
    }
    return parseResult;
}

void YAML::handle_error(int64_t line, const ::godot::String& message) {
    parser.stop();
    parseResult->set_error(GODOT_ERR_PARSE_ERROR);
    parseResult->set_error_string(message);
    parseResult->set_error_line(line);
}
