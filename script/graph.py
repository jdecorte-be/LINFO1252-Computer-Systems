import matplotlib.pyplot as plt
import csv
import numpy as np
from matplotlib.lines import Line2D
import os


def process_performance_data(csv_file, output_dir="graph", problem_name=None):
    os.makedirs(output_dir, exist_ok=True)
    if problem_name is None:
        problem_name = os.path.splitext(os.path.basename(csv_file))[0].replace(
            "perf_", ""
        )

    nthreads = []
    times = []
    plt.clf()

    with open(csv_file, "r") as current_file:
        plots = csv.reader(current_file, delimiter=",")
        next(plots)
        for row in plots:
            nthreads.append(int(row[0]))
            for i, value in enumerate(row[1:]):
                if len(times) <= i:
                    times.append([])
                times[i].append(float(value))

    mean_times = [
        np.mean([times[i][j] for i in range(len(times))]) for j in range(len(nthreads))
    ]
    std_devs = [
        np.std([times[i][j] for i in range(len(times))]) for j in range(len(nthreads))
    ]

    for time_series in times:
        plt.plot(nthreads, time_series, "o", color="orange")

    plt.plot(nthreads, mean_times, "-o", label="Moyenne", color="black", linewidth=1)
    plt.errorbar(
        nthreads,
        mean_times,
        yerr=std_devs,
        fmt="o",
        color="red",
        capsize=5,
        label="Écart-type",
    )

    legend_elements = [
        Line2D(
            [0],
            [0],
            marker="o",
            color="w",
            markerfacecolor="orange",
            markersize=8,
            label="Mesures",
        ),
        Line2D([0], [0], color="black", linewidth=1, label="Moyenne"),
        Line2D([0], [0], color="red", markersize=5, label="Écart-type"),
    ]

    plt.xlabel("Nombre de threads")
    plt.ylabel("Temps d'exécution (seconde)")
    plt.title(f"Analyse {problem_name}")
    plt.legend(handles=legend_elements)
    plt.grid(True)
    plt.xticks(nthreads, labels=nthreads)

    output_file = os.path.join(output_dir, f"graphe_{problem_name}.png")
    plt.savefig(output_file)
    print(f"Graph saved: {output_file}")
    plt.close()


def main():
    import argparse

    parser = argparse.ArgumentParser()
    parser.add_argument("csv_file")
    parser.add_argument("--output-dir", default="graph")
    parser.add_argument("--problem-name")
    args = parser.parse_args()
    process_performance_data(args.csv_file, args.output_dir, args.problem_name)


if __name__ == "__main__":
    main()
