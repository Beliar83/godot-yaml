#include <core/Godot.hpp>
#include <core/Variant.hpp>
#include <ryml.hpp>
//

namespace c4 {
namespace yml {
	bool read(::ryml::NodeRef const&node, godot::Variant *variant);
	void write(::ryml::NodeRef *node, const ::godot::Variant &variant);
} // namespace yaml
} // namespace c4
