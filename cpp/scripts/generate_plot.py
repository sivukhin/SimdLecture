#!/usr/bin/env python3
import json
import math
import argparse
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

sns.set(style='ticks', palette='Set2')
parser = argparse.ArgumentParser(description='Generate chart based on benchmark results')
parser.add_argument('--source', '-s', type=str, help='JSON file with the benchmark results')
parser.add_argument('--baseline', '-b', type=str, help='Baseline algorithm name')

def gather_benchmarks(benchmarks):
    grouped_benchmarks = {}
    for benchmark in benchmarks:
        name, params = benchmark['name'].split('/')
        cpu_time = benchmark['cpu_time']
        time_unit = benchmark['time_unit']
        if name not in grouped_benchmarks:
            grouped_benchmarks[name] = []
        grouped_benchmarks[name].append((int(params), float(cpu_time), time_unit))
    for key in grouped_benchmarks.keys():
        grouped_benchmarks[key] = list(sorted(grouped_benchmarks[key]))
    return grouped_benchmarks

def extract_benchmark_name(benchmark_name):
    return benchmark_name.split('_')[0]

def extract_benchmark_filename(context):
    return context['executable'].split('/')[-1].split('.')[0]

def create_common_plot_title(context):
    mhz_per_cpu = context['mhz_per_cpu']
    cpu_scaling_enabled = context['cpu_scaling_enabled']
    build_type = context['library_build_type']
    executable = extract_benchmark_filename(context)
    return ('Benchmarks from {}\n(Build type: {}, CPU scaling enabled: {}, MHz per CPU: {})'
            .format(executable, build_type, cpu_scaling_enabled, mhz_per_cpu))

def create_relative_plot_title(context, baseline_name):
    mhz_per_cpu = context['mhz_per_cpu']
    cpu_scaling_enabled = context['cpu_scaling_enabled']
    build_type = context['library_build_type']
    executable = extract_benchmark_filename(context)
    return ('Benchmarks from {} relative to {}\n(Build type: {}, CPU scaling enabled: {}, MHz per CPU: {})'
            .format(executable, baseline_name, build_type, cpu_scaling_enabled, mhz_per_cpu))

def draw_common_plot(benchmarks, context):
    plt.clf()
    for benchmark_name in benchmarks.keys():
        benchmark = benchmarks[benchmark_name]
        xs = list(map(lambda x: math.log2(x[0]), benchmark))
        ys = list(map(lambda x: x[1] / x[0], benchmark))
        plt.plot(xs, ys, label=extract_benchmark_name(benchmark_name))
    plt.legend()
    plt.title(create_common_plot_title(context))
    plt.savefig('{}_results.png'.format(extract_benchmark_filename(context)))

def draw_relative_plot(benchmarks, context, baseline_name):
    plt.clf()
    baseline_data = list(map(lambda x: x[1] / x[0], benchmarks[baseline_name]))
    for benchmark_name in benchmarks.keys():
        benchmark = benchmarks[benchmark_name]
        xs = list(map(lambda x: math.log2(x[0]), benchmark))
        ys = list(map(lambda t: t[0] / t[1], zip(map(lambda x: x[1] / x[0], benchmark), baseline_data)))
        plt.plot(xs, ys, label=extract_benchmark_name(benchmark_name))
    plt.ylim(bottom=0)
    plt.legend()
    plt.title(create_relative_plot_title(context, baseline_name))
    plt.savefig('{}_results_relative.png'.format(extract_benchmark_filename(context)))

def main():
    args = parser.parse_args()
    with open(args.source, 'r') as f:
        data = json.load(f)
    benchmarks = gather_benchmarks(data['benchmarks'])
    draw_common_plot(benchmarks, data['context'])
    if args.baseline:
        draw_relative_plot(benchmarks, data['context'], args.baseline)

if __name__ == '__main__':
    main()
