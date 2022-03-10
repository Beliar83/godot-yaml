#[derive(Copy, Clone, Hash, Eq, PartialEq, Debug)]
pub enum VariantType {
    Nil,
    // atomic types
    Bool,
    Int,
    Real,
    String,

    // math types
    Vector2, // 5
    Rect2,
    Vector3,
    Transform2D,
    Plan,
    Quat, // 10
    Aaab,
    Basis,
    Transform,

    // misc types
    Color,
    NodePath, // 15
    _RID,
    Object,
    Dictionary,
    Array,

    // arrays
    PoolByteArray, // 20
    PoolIntArray,
    PoolRealArray,
    PoolStringArray,
    PoolVector2Array,
    PoolVector3Array, // 25
    PoolColorArray,

    VariantMax,
}

impl Default for VariantType {
    fn default() -> Self {
        VariantType::Nil
    }
}
