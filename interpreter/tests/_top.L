native "stl" as x in
fun cat_reversed with s1, s2 =
	((x::reverse_string 1 s1) + (x::reverse_string 1 s2))
in

(cat_reversed "dog" "quack")

