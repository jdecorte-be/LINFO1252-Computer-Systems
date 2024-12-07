import matplotlib.pyplot as plt
import csv
import numpy as np
from matplotlib.lines import Line2D
import argparse
import os

def process_performance_data(csv_file, output_dir='graph', problem_name=None):

    os.makedirs(output_dir, exist_ok=True)
    
    if problem_name is None:
        problem_name = os.path.splitext(os.path.basename(csv_file))[0].replace('perf_', '')
    
    nthreads = []
    times = []
    
    plt.clf()
    
    with open(csv_file, 'r') as current_file:
        plots = csv.reader(current_file, delimiter=',')
        next(plots)
        
        for row in plots:
            nthreads.append(int(row[0]))
            for i, value in enumerate(row[1:]):
                if len(times) <= i:
                    times.append([])
                times[i].append(float(value))
    
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
    
    title_map = {
        'philo': 'des philosophes',
        'prodcons': 'des producteurs \net des consommateurs',
        'writeread': 'des écrivains \net des lecteurs'
    }
    title = title_map.get(problem_name, problem_name)
    
    plt.xlabel("Nombre de threads")
    plt.ylabel("Temps d'exécution (seconde)")
    plt.title(f"Analyse de la performance du problème {title} avec la moyenne et l'écart-type")
    plt.legend(handles=legend_elements)
    plt.grid(True)
    plt.xticks(nthreads, labels=nthreads)
    
    output_file = os.path.join(output_dir, f'graphe_{problem_name}.png')
    plt.savefig(output_file)
    print(f"Graph saved to {output_file}")
    
    plt.close()

def main():
    parser = argparse.ArgumentParser(description='Process performance data and create graphs')
    parser.add_argument('csv_file', type=str, help='Path to the input CSV file')
    parser.add_argument('--output-dir', type=str, default='graph', 
                        help='Directory to save output graphs (default: grah)')
    parser.add_argument('--problem-name', type=str, 
                        help='Name of the problem (optional, will be derived from filename if not provided)')
    
    args = parser.parse_args()
    
    process_performance_data(
        args.csv_file, 
        output_dir=args.output_dir, 
        problem_name=args.problem_name
    )

if __name__ == '__main__':
    main()