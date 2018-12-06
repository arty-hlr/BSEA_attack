with open("./tests/times", "r") as f:
    average = 0;
    n = 0
    for line in f:
        average += float(line)
        n += 1

average /= n
print("Average time taken by implementation is {:6f}".format(average))
