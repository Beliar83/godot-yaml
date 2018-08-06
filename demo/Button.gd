extends Button
# class member variables go here, for example:
# var a = 2
# var b = "textvar"
var yaml

func _ready():
    # Called when the node is added to the scene for the first time.
    # Initialization here
    yaml = preload("res://bin/gdyaml.gdns").new()

#func _process(delta):
#    # Called every frame. Delta is time since last frame.
#    # Update game logic here.
#    pass


func _on_Button_pressed():
    var yaml_file = File.new()
    yaml_file.open("res://test.yaml", File.WRITE)
    var arr = Dictionary()
    arr["Vector2"] =rect_position
    arr["Null"] = null
    arr["PoolIntArray"] = PoolIntArray([1,2,3,4])
    arr["PoolStringArray"] = PoolStringArray(["A", "B", "C"])
    arr["BoolTrue"] = true
    arr["Bool!True"] = !true
    arr["Array"] = [1,2,3,4,5]
    arr["String"] = "Godot"
    var dict = Dictionary()
    dict[true] = 1
    dict["a"] = false
    dict[5] = 23
    arr["Dict"] = dict
    var rect = Rect2()
    rect.position = Vector2(1, 2)
    rect.size = Vector2(5, 10)
    arr["Rect"] = rect
    var aabb = AABB();
    aabb.position = Vector3(1,2,3)
    aabb.size = Vector3(5, 10, 15)
    arr["AABB"] = aabb
    arr["Transform2D-1"] = Transform2D(deg2rad(0), Vector2(1, 5))
    arr["Transform2D-2"] = Transform2D(deg2rad(90), Vector2(1, 5))
    arr["Transform2D-3"] = Transform2D(deg2rad(180), Vector2(1, 5))
    arr["Transform2D-4"] = Transform2D(deg2rad(270), Vector2(1, 5))
    arr["Plane"] = Plane(Vector3(1,2,3), 5)
    arr["Quat"] = Quat(1,2,3,4)
    var basis = Basis(Vector3(1,2,3), Vector3(4,5,6), Vector3(7,8,9))
    arr["Basis"] = basis
    arr["Transform"] = Transform(basis, Vector3(1,2,3))
    arr["Color"] = Color(0.25, 0.5, 0.75, 1.0)
    arr["NodePath"] = self.get_path()
    arr["Object"] = self
    arr["PoolColorArray"] = PoolColorArray([Color(0.25, 0.5, 0.75, 1.0), Color(0.35, 0.6, 0.85, 0.5), Color(0.15, 0.4, 0.65, 0.0)])
    arr["PoolVector2Array"] = PoolVector2Array([Vector2(1,2), Vector2(3,4), Vector2(5,6)])
    arr["PoolVector3Array"] = PoolVector3Array([Vector3(1,2,3), Vector3(4,5,6), Vector3(7,8,9)])
    yaml_file.store_line(yaml.print(arr))
    yaml_file.close()
    yaml_file.open("res://test.yaml", File.READ)
    var test = yaml.parse(yaml_file.get_as_text())
    yaml_file.close()
    print(arr)
    print(arr["PoolColorArray"][0])
    print(test)
    print(test["PoolColorArray"][0])
