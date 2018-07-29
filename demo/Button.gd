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
    yaml_file.store_line(yaml.print(arr))
    yaml_file.close()
    yaml_file.open("res://test.yaml", File.READ)
    var test = yaml.parse(yaml_file.get_as_text())
    yaml_file.close()
    print(arr)
    print(test)
    
