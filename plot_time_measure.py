import matplotlib.pyplot as plt
import numpy as np
import json

def process():
    with open("time_measure.txt", "r") as file:
        data = json.load(file)
    data = np.array(data)
    plt.plot(data[:, 0], data[:, 1])
    plt.title("Time of execution in function of number of threads")
    plt.xlabel("Number of threads")
    plt.ylabel("Time of execution [s]")
    # plt.show()
    plt.savefig('plot.png')
if __name__ == "__main__":
    process()