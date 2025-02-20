from cs50 import get_int

height = get_int("Height: ")

while(height < 1 or height > 8):
  height = get_int("Height: ")



blocks = 1
space = height-1


def printRow(blocks, space):
  for i in range(space):
    print(" ", end="")
  for i in range(blocks):
    print("#", end="")
  print(" ", end="")
  print(" ", end="")
  for i in range(blocks):
    print("#", end="")
  print("")

for i in range(height):
  printRow(blocks, space)
  blocks+=1
  space-=1






