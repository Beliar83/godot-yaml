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

void PrintParseError(std::string message, ::YAML::Mark errorMark) {
	std::stringstream error_string;
	error_string << "Error parsing yaml: " << message << " at "
				 << "Line " << errorMark.line + 1 << " Column " << errorMark.column + 1 << " (Position " << errorMark.pos << ")";
	Godot::print(error_string.str().c_str());
}

Variant godot::YAML::parse(String text) {
	try {
		::YAML::Node node = ::YAML::Load(text.alloc_c_string());
		return node.as<Variant>();
	} catch (::YAML::ParserException &parserError) {
		PrintParseError(parserError.msg, parserError.mark);
		return Variant();
	} catch (::YAML::TypedBadConversion<Variant> &badConversionError) {
		PrintParseError(badConversionError.msg, badConversionError.mark);
		return Variant();
	} catch (std::exception err) {
		PrintParseError(std::string(err.what()), ::YAML::Mark::null_mark());
		return false;
	}
}