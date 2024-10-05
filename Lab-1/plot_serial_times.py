import matplotlib.pyplot as plt
import pandas as pd

# Read the CSV file
data = pd.read_csv('serial_times.csv')

# Extract the iteration and time columns
iterations = data['Iteration']
times = data['Time']

# Plot the data
plt.figure(figsize=(10, 6))
plt.plot(iterations, times, marker='o', linestyle='-', color='b', label='Execution Time')

# Adding labels and title
plt.xlabel('Iteration')
plt.ylabel('Execution Time (seconds)')
plt.title('Execution Time for Serial Operations Over Iterations')
plt.legend()

# Show the plot
plt.grid(True)
plt.show()
