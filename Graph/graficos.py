import matplotlib.pyplot as plt
import numpy as np

# Datos simulados
tiempo = np.linspace(0, 2000, 2000)  # 3000 ms de tiempo total con 1 ms de resolución
estado_pulso = np.zeros_like(tiempo)  # Inicializar todos los valores en 0
estado_pulso[100:300] = 1  # 100ms a 300ms en 1
estado_pulso[800:1400] = 1  # 800ms a 1400ms en 1

# Gráfico de Pulso en el Tiempo
plt.figure(figsize=(12, 6))
plt.plot(tiempo, estado_pulso, drawstyle='steps-post')
plt.title('Gráfico de Pulso en el Tiempo')
plt.xlabel('Tiempo (ms)')
plt.ylabel('Estado del Pulso (HIGH/LOW)')
plt.ylim(-0.1, 1.1)
plt.grid(True)
plt.show()

# Datos simulados de duración del pulso
duraciones = [200, 600, 600, 1000]
tipos_pulso = ['*', '-', 'concat', 'espacio']

# Gráfico de Duración del Pulso
plt.figure(figsize=(12, 6))
plt.bar(range(len(duraciones)), duraciones, color=['blue', 'blue', 'red', 'red'])
plt.xticks(range(len(duraciones)), tipos_pulso)
plt.title('Duración del Pulso y Decodificación Morse')
plt.xlabel('Significado')
plt.ylabel('Duración (ms)')
plt.grid(True)
plt.show()
