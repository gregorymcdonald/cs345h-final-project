native "stl" as x in
fun reverse with str =
	(x::reverse_string 1 str)
in

let s = "duck" in
(reverse s)

