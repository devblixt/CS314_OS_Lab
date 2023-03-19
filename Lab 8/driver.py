import subprocess
import matplotlib.pyplot as plt
import sys

x = 100
z = 30
y_values = range(int(sys.argv[2]), int(sys.argv[3]))
results = {
    "FIFO": [],
    "LRU": [],
    "Random": [],
}

for y in range(int(sys.argv[2]), int(sys.argv[3])):
    cmd = ['a.exe', str(x), str(y), str(z), sys.argv[1]]
    output = subprocess.check_output(cmd, shell=True)
    print(output.decode('utf-8'))
    percentages = output.decode().strip().split()
    results["FIFO"].append(float(percentages[0]))
    results["LRU"].append(float(percentages[1]))
    results["Random"].append(float(percentages[2]))

plt.plot(y_values, results["FIFO"], label="FIFO")
plt.plot(y_values, results["LRU"], label="LRU")
plt.plot(y_values, results["Random"], label="Random")
plt.xlabel("Value of y")
plt.ylabel("Percentage of page faults")
plt.title(f"Page Replacement Algorithm Performance (x={x}, z={z})")
plt.legend()
plt.show()