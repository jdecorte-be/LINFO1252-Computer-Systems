import matplotlib.pyplot as plt
import csv
import numpy as np
from matplotlib.lines import Line2D

for problem in ["philo", "prodcons", "writeread"]:
    headers = []
    nthreads = []
    times = []
    plt.clf()

    current_file = open(f'perf_finale_1/perf_{problem}.csv', 'r')
    plots = csv.reader(current_file, delimiter=',')
    headers = next(plots)
    
    for row in plots:
        nthreads.append(int(row[0]))
        for i, value in enumerate(row[1:]):
            if len(times) <= i:
                times.append([])
            times[i].append(float(value))
    current_file.close()

    mean_times = []
    std_devs = []

    for j in range(len(nthreads)):
        values_for_j = [times[i][j] for i in range(len(times))]
        mean_times.append(np.mean(values_for_j))

        std_devs.append(np.std(values_for_j))


    for time_series in times:
        plt.plot(nthreads, time_series, 'o', color='orange')

    plt.plot(nthreads, mean_times, '-o', label='Moyenne', color='black', linewidth=1)
    plt.errorbar(nthreads, mean_times, yerr=std_devs, fmt='o', color='red', capsize=5, label='Écart-type')

    legend_elements = [
        Line2D([0], [0], marker='o', color='w', markerfacecolor='orange', markersize=8, label='Mesures'),
        Line2D([0], [0], color='black', linewidth=1, label='Moyenne'),
        Line2D([0], [0], color='red', markersize=5, label='Écart-type')
    ]

    title = ""
    if problem == 'philo':
        title = '\ndes philosophes'
    elif problem == 'prodcons':
        title = 'des producteurs \net des consommateurs'
    elif problem == 'writeread':
        title = 'des écrivains \net des lecteurs'
    
    plt.xlabel("Nombre de threads")
    plt.ylabel("Temps d'exécution (seconde)")
    plt.title(f"Analyse de la performance du problème {title} avec la moyenne et l'écart-type")
    plt.legend(handles=legend_elements)
    plt.grid(True)
    plt.xticks(nthreads, labels=nthreads)
    plt.savefig(f'graphe_perf_1/graphe_{problem}.png')
