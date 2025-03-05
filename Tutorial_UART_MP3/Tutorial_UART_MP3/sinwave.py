import math

x = list(range(112))
y = [round(28 - 27 * math.sin(2 * math.pi * xi / 112)) for xi in x]

print(f"x = {{{', '.join(map(str, x))}}}")
print(f"y = {{{', '.join(map(str, y))}}}")