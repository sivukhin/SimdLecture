using System;
using System.Linq;
using System.Numerics;
using System.Reflection;
using System.Reflection.Emit;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Attributes.Jobs;

namespace SimdLecture
{
    [SimpleJob(targetCount: 10, id: "FastBenchmark")]
    public class SumLoopBenchmark
    {
        private readonly long[] data;

        public SumLoopBenchmark()
        {
            var length = (int)1e8;
            data = new Random(0).GenerateSequence(length).Cast<long>().ToArray();
        }

        [Benchmark]
        public long ForeachSum()
        {
            long sum = 0;
            foreach (var element in data)
                sum += element;
            return sum;
        }

        [Benchmark]
        public long SimpleForSum()
        {
            long sum = 0;
            for (int i = 0; i < data.Length; i++)
                sum += data[i];
            return sum;
        }

        [Benchmark]
        public long SimdForSum()
        {
            var sums = Vector<long>.Zero;
            var vectorSize = Vector<long>.Count;
            int alignedLength = data.Length - data.Length % vectorSize;
            for (int i = 0; i < alignedLength; i += vectorSize)
                sums += new Vector<long>(data, i);
            long sum = 0;
            for (int i = 0; i < vectorSize; i++)
                sum += sums[i];
            for (int i = alignedLength; i < data.Length; i++)
                sum += data[i];
            return sum;
        }
    }
}