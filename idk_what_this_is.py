counter = 0
words = ("apple", "bannana", "peach")
message = "hello there bannana, I'm apple how are the peach doing?"
for word in words:
    if any(word in message):
        print("Found " + word)
        counter += 1
print("Found " + str(counter) + " items")
