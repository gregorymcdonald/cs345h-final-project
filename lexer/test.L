import "strings.L" in
import "network.L" in 
import "does_not_exist.L" as bad in
export 1 in
export "string" in
export identifier as something_else in
native "filename.c" as function_name in
(function_name something_else::value) 
