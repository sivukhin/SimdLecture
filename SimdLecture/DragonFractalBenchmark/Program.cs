using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Attributes.Jobs;
using BenchmarkDotNet.Running;
using Fractals;

namespace DragonFractalBenchmark
{
    [SimpleJob(targetCount: 10, id: "FastBenchmark")]
    public class DragonFractalBenchmark
    {
        [Benchmark]
        public void DrawDragonFractal() => DragonFractalTask.DrawDragonFractal((x, y) => { }, 10000000, 0);
    }

    internal class Program
    {
        private static void Main(string[] args)
        {
            BenchmarkRunner.Run<DragonFractalBenchmark>();
        }
    }
}