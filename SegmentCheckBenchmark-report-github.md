``` ini

BenchmarkDotNet=v0.10.0
OS=Microsoft Windows NT 6.2.9200.0
Processor=Intel(R) Core(TM) i7-4510U CPU 2.00GHz, ProcessorCount=4
Frequency=2533201 Hz, Resolution=394.7575 ns, Timer=TSC
Host Runtime=Clr 4.0.30319.42000, Arch=64-bit RELEASE [RyuJIT]
GC=Concurrent Workstation
JitModules=clrjit-v4.6.1087.0
Job Runtime(s):
	Clr 4.0.30319.42000, Arch=64-bit RELEASE [RyuJIT]

Job=FastBenchmark  TargetCount=10  

```
|                       Method |        Mean |    StdDev |      Median |
|----------------------------- |------------ |---------- |------------ |
|          ForeachCountInRange | 479.1452 ms | 4.9701 ms | 479.6608 ms |
|        SimpleForCountInRange | 498.8664 ms | 5.3339 ms | 496.7145 ms |
|          SimdForCountInRange |  96.3900 ms | 2.1391 ms |  96.4487 ms |
| SimdOptimizedForCountInRange |  43.5919 ms | 1.0294 ms |  43.6912 ms |
