
# Example data
'''
'''
import matplotlib.pyplot as plt
import numpy as np

# Example data for 5 vector pairs
x = [10, 30, 60, 120, 240]  # Vector for the x-axis

y1 = [499, 1927, 4162, 23639, 109680]
y2 = [512, 1499, 3015, 6027, 12066]
y3 = [259, 756, 1494, 3079, 6020]
y4 = [148, 396, 787, 1526, 3042]
y5 = [99, 216, 406, 772, 1534]

xmex = [10, 30, 60, 120, 240]
y6 = [517, 1715, 3844, 13497, 90124]

# Create the plot
plt.figure(figsize=(10, 6))

# Plot all curves on the same graph
plt.plot(x, y1, label='Sequencial', color='blue', linestyle=':', marker='o')
plt.plot(x, y2, label='Multithread (t=1)', color='green', linestyle=':', marker='o')
plt.plot(x, y3, label='Multithread (t=2)', color='red', linestyle=':', marker='o')
plt.plot(x, y4, label='Multithread (t=4)', color='orange', linestyle=':', marker='o')
plt.plot(x, y5, label='Multithread (t=8)', color='purple', linestyle=':', marker='o')
plt.plot(xmex, y6, label='I/O multiplexado', color='black', linestyle=':', marker='o')

plt.ylim(top=26000)
plt.ylim(bottom=-500)

# Add title and labels
plt.title('Desempenho (Carga de trabalho = 50 ms)')
plt.ylabel('Tempo médio de resposta (ms)')
plt.xlabel('Número de requisições')

# Add legend
plt.legend()

# Show the grid for better visualization
plt.grid(True)

plt.savefig('analysis/results.png')

# Display the graph
plt.show()
