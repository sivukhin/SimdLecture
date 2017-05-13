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


|       Method |        Mean |    StdErr |     StdDev |      Median |
|------------- |------------ |---------- |----------- |------------ |
|      LinqSum | 722.8332 ms | 7.3144 ms | 23.1301 ms | 715.2691 ms |
|   ForeachSum |  85.7738 ms | 0.8433 ms |  2.5298 ms |  85.1099 ms |
| SimpleForSum |  93.6699 ms | 1.5757 ms |  4.9829 ms |  91.0276 ms |
|   SimdForSum |  79.7799 ms | 0.1841 ms |  0.5523 ms |  79.9257 ms |
