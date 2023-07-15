import sys, math
print(sys.argv[1])
print(sys.argv[2])


with open(sys.argv[1], "r") as f:
    content = f.read()

content = content.replace("${VAR_DEFINED_IN_PY}", f"{math.pi}")

with open(sys.argv[2], "tw") as f:
    f.write(content)