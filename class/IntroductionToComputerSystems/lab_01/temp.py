a=[13,6,3,4,8,7]

for i in a:
    for c in range(ord("a"),ord("z")):
        if c&15==i:
            print(chr(c),end="")
    for c in range(ord("A"),ord("Z")):
        if c&15==i:
            print(chr(c),end="")
    print()
