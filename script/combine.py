import matplotlib.pyplot as plt
import csv
import argparse
import os
import glob


def process_all_performance_data(
    csv_dir, output_dir="graph", combined_name="combined_plot"
):
    os.makedirs(output_dir, exist_ok=True)
    csv_files = glob.glob(os.path.join(csv_dir, "*.csv"))
    if not csv_files:
        print(f"No CSV files found in {csv_dir}")
        return
    plt.clf()
    for csv_file in csv_files:
        nthreads, times = read_csv_data(csv_file)
        label = os.path.splitext(os.path.basename(csv_file))[0]
        plt.plot(nthreads, times[0], "-o", label=label)
    plt.xlabel("Nombre de threads")
    plt.ylabel("Temps d'exécution (seconde)")
    plt.title(f"Analyse des performances ({combined_name})")
    plt.legend()
    plt.grid(True)
    plt.xticks(sorted(set(nthreads)))
    output_file = os.path.join(output_dir, f"{combined_name}.png")
    plt.savefig(output_file)
    print(f"✅ Combined graph saved to {output_file}")
    plt.close()


def read_csv_data(csv_file):
    nthreads, times = [], []
    with open(csv_file, "r") as current_file:
        plots = csv.reader(current_file, delimiter=",")
        next(plots)
        for row in plots:
            nthreads.append(int(row[0]))
            for i, value in enumerate(row[1:]):
                if len(times) <= i:
                    times.append([])
                times[i].append(float(value))
    return nthreads, times


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("csv_dir", type=str)
    parser.add_argument("--output-dir", type=str, default="graph")
    parser.add_argument("--combined-name", type=str, default="combined_plot")
    args = parser.parse_args()
    process_all_performance_data(
        args.csv_dir, output_dir=args.output_dir, combined_name=args.combined_name
    )


if __name__ == "__main__":
    main()
