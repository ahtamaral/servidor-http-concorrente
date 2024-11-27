
# Example data
'''
'''
import matplotlib.pyplot as plt
import numpy as np

# Example data for 5 vector pairs
x = [10, 30, 60, 120]#, 240]  # Vector for the x-axis

y1 = [4497, 8130, 13566, 22340]#, 69010]
y2 = [3968, 8724, 13110, 26846]#, 80214]
y3 = [3962, 6631, 12656, 25157]#, 60532]
y4 = [4212, 8584, 15600, 24363]#, 77959]
y5 = [3641, 7230, 16786, 21562]#, 1074706]
y6 = [4090, 7930, 13291, 21954]#, 1081156]



# Create the plot
plt.figure(figsize=(10, 6))

# Plot all curves on the same graph
plt.plot(x, y1, label='Sequencial', color='blue', linestyle=':', marker='o')
plt.plot(x, y2, label='Multithread (t=1)', color='green', linestyle=':', marker='o')
plt.plot(x, y3, label='Multithread (t=2)', color='red', linestyle=':', marker='o')
plt.plot(x, y4, label='Multithread (t=4)', color='orange', linestyle=':', marker='o')
plt.plot(x, y5, label='Multithread (t=8)', color='purple', linestyle=':', marker='o')
plt.plot(x, y6, label='I/O multiplexado', color='black', linestyle=':', marker='o')

#plt.ylim(top=90000)
#plt.ylim(bottom=-500)

# Add title and labels
plt.title('Desempenho (Carga de trabalho = 0 ms)')
plt.ylabel('Tempo médio de resposta (ns)')
plt.xlabel('Número de requisições')

# Add legend
plt.legend()

# Show the grid for better visualization
plt.grid(True)

plt.savefig('analysis/results2.png')

# Display the graph
plt.show()
