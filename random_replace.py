import random
big_axx_list = []

with open("big_axx_file.txt", "r") as f:
    big_axx_list = f.read().split()
    number_of_changes = round(len(big_axx_list) / 2)
    for i in range(0, number_of_changes):
        big_axx_list[random.randint(0, len(big_axx_list)-1)] = "cum"

print(big_axx_list)