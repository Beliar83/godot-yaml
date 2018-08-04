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
    var arr = Array()
    arr.append(rect_position)
    arr.append(null)
    arr.append(PoolIntArray([1,2,3,4]))
    arr.append(PoolStringArray(["A", "B", "C"]))
    arr.append(true)
    arr.append(!true)
    arr.append([1,2,3,4,5])
    arr.append("Godot")
    var dict = Dictionary()
    dict[true] = 1
    dict["a"] = false
    dict[5] = 23
    arr.append(dict)
    var rect = Rect2()
    rect.position = Vector2(1,2)
    rect.size = Vector2(5, 10)
    arr.append(rect)
    var aabb = AABB();
    aabb.position = Vector3(1,2,3)
    aabb.size = Vector3(5, 10, 15)
    arr.append(aabb)
    arr.append(Transform2D(deg2rad(0), Vector2(1, 5)))
    arr.append(Transform2D(deg2rad(90), Vector2(1, 5)))
    arr.append(Transform2D(deg2rad(180), Vector2(1, 5)))
    arr.append(Transform2D(deg2rad(270), Vector2(1, 5)))
    yaml_file.store_line(yaml.print(arr))
    yaml_file.close()
    yaml_file.open("res://test.yaml", File.READ)
    var test = yaml.parse(yaml_file.get_as_text())
    yaml_file.close()
    print(arr)
    print(test)
