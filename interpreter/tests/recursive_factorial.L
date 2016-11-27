(lambda f. ((lambda x. (f(x x))) (lambda x. (f(x x)))) (lambda f, n. if n=0 then 1 else n*(f(n-1))) 5)
