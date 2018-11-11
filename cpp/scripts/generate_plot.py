#!/usr/bin/env python3
import json
import math
import argparse
import numpy as np
import seaborn as sns
import matplotlib.pyplot as plt

plt.style.use('seaborn-talk')
sns.set(style='ticks', palette='Set2')
parser = argparse.ArgumentParser(description='Generate chart based on benchmark results')
parser.add_argument('--source', '-s', type=str, help='JSON file with the benchmark results')
parser.add_argument('--dest_prefix', '-d', type=str, help='Prefix of the name of the output png file with graph')
parser.add_argument('--baseline', '-b', type=str, help='Baseline algorithm name')
parser.add_argument('--exclude', '-e', nargs='*', help='Benchmarks to exclude from the resulting plot')
parser.add_argument('--include', '-i', nargs='*', help='Benchmarks to include to the resulting plot')

def x_axis_function(x_value):
    return math.log2(x_value * 8)

def y_axis_function(y_value, x_value):
    return y_value / (x_value * 8)

def filter_unrelevant(results):
    return filter(lambda x: x_axis_function(x[0]) >= 10, results)

def gather_benchmarks(benchmarks, exclude_list, include_list):
    grouped_benchmarks = {}
    for benchmark in benchmarks:
        name, params = benchmark['name'].split('/')
        if include_list and name not in include_list: continue
        if exclude_list and name in exclude_list: continue
        cpu_time = benchmark['cpu_time']
        time_unit = benchmark['time_unit']
        if name not in grouped_benchmarks:
            grouped_benchmarks[name] = []
        grouped_benchmarks[name].append((int(params), float(cpu_time), time_unit))
    for key in grouped_benchmarks.keys():
        grouped_benchmarks[key] = list(filter_unrelevant(list(sorted(grouped_benchmarks[key]))))
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

def finalize_plot(title):
    plt.axvline(x=math.log2(32 * 1024), label='L1d cache size', linestyle='--', color='k')
    plt.axvline(x=math.log2(256 * 1024), label='L2 cache size', linestyle='--', color='k')
    plt.axvline(x=math.log2(30 * 1024 * 1024), label='L3 cache size', linestyle='--', color='k')
    ticks = list(range(10, 27, 2))
    plt.xticks(ticks, map(lambda x: '$2^{{{}}}$'.format(x), ticks))
    plt.xlabel('Array size in bytes')
    plt.ylabel('Latency relative to the baseline')
    plt.ylim(bottom=0)
    plt.legend()
    plt.title(title)

def draw_common_plot(benchmarks, context, dest_prefix):
    plt.clf()
    for benchmark_name in benchmarks.keys():
        benchmark = benchmarks[benchmark_name]
        xs = list(map(lambda x: x_axis_function(x[0]), benchmark))
        ys = list(map(lambda x: y_axis_function(x[1], x[0]), benchmark))
        plt.plot(xs, ys, label=extract_benchmark_name(benchmark_name), marker='o')
    finalize_plot(create_common_plot_title(context))
    plt.savefig('{}_{}_results.png'.format(dest_prefix, extract_benchmark_filename(context)))

def draw_relative_plot(benchmarks, context, baseline_name, dest_prefix):
    plt.clf()
    baseline_data = np.array(list(map(lambda x: y_axis_function(x[1], x[0]), benchmarks[baseline_name])))
    for benchmark_name in benchmarks.keys():
        benchmark = benchmarks[benchmark_name]
        xs = list(map(lambda x: x_axis_function(x[0]), benchmark))
        ys = np.array(list(map(lambda x: y_axis_function(x[1], x[0]), benchmark))) / baseline_data
        plt.plot(xs, ys, label=extract_benchmark_name(benchmark_name), marker='o')
    finalize_plot(create_relative_plot_title(context, baseline_name))
    plt.savefig('{}_{}_results_relative.png'.format(dest_prefix, extract_benchmark_filename(context)))

def main():
    args = parser.parse_args()
    with open(args.source, 'r') as f:
        data = json.load(f)
    benchmarks = gather_benchmarks(data['benchmarks'], args.exclude, args.include)
    draw_common_plot(benchmarks, data['context'], args.dest_prefix)
    if args.baseline:
        draw_relative_plot(benchmarks, data['context'], args.baseline, args.dest_prefix)

if __name__ == '__main__':
    main()
