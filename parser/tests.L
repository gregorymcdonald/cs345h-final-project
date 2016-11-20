(* Test precedence and associativity of list operators *)
fun f with l1, l2 =
	if (isNil l1) then 
		l2 
	else
		!l1@(f #l1 l2)
in
0
---
(* Test farthest right extension of let, fun, and lambda bindings *)
(* This hangs the provided parser: "let x = 2 in let x = 3 in x + x" *)
let a = 2 in let b = 3 in
lambda c. lambda d. d + c in
fun f1 with e = fun f2 with f = 4 in f2 + f1
---
(* Test construction of id_list in proper order *)
lambda a,b,c,d,e,f,g . 
fun f with h, i, j, k, l, m, n = n + m + l + k + j + i + h in
f + g + f + e + d + c + b + a
----
(* Test precedence and tokening of binary operators and single expression application *)
((0) * ("duck") / (Nil) + (3) - (4) & (5) | (6) = (7) <> (8) < (9) > (10) <= (11) >= (12) @ (13))
---
(* Test branching of if statements *)
if 0 then 1 else
	if let a = 2 in 3 then fun f with b = 4 in 5 else
		if lambda c.c 6 then (if 7 then 8 else 9) else
			if "duck" then (x 10) else
				if print 11 then readInt else
					if readString then !12 else
						if #13 then 14@15 else
							if Nil then isNil 16 else
								done