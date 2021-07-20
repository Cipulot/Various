""" We suppose that the input file is a classic *.txt with a list of lists.
    Rappresentation of the list:    File equivalence:
    [[1,2], [2,3], [5,6], [5]]  --> | 1,2
                                    | 2,3
                                    | 5,6
                                    | 5

    [[1,2], [2,3,4], [1]]       --> | 1,2
                                    | 2,3,4
                                    | 1

    NOTE: we also suppose that there is only one super set in the list.
"""


def remove_superset():
    super_index = -1  # index of the super set (-1 is just a placeholder)

    with open("su_list1.txt", "r") as file:
        # Here I convert each sublist into a set for easier comparison later on
        big_list = [set([int(val) for val in line.split(',')])
                    for line in file]

    # I print the list of sets to check that everything is fine
    print(f"List of sets: {big_list}")

    # Here I loop to cross-check each set with the others
    for i in range(len(big_list)):
        for j in range(i+1, len(big_list)):
            # I check for superset
            if (big_list[i] > big_list[j]):
                super_index = i
                break  # I break the loop if I find a super set

    if super_index != -1:
        print(f"Superset index is {super_index} (starts from 0).")
        del big_list[super_index]
        # Here I convert the set back to a sublist
        big_list = [list(single_set) for single_set in big_list]
        # I print the new list of lists
        print(f"New list of lists is: {big_list} .")
    else:
        print("Nothing here buds...")  # Pepesad


if __name__ == "__main__":
    remove_superset()
