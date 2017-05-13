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
            data = new Random(0).GenerateLongSequence((int)1e8);
        }

        public SumLoopBenchmark(long[] data)
        {
            this.data = data;
        }

        [Benchmark]
        public long LinqSum()
        {
            return data.Sum();
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

        [Benchmark]
        public long Simd2ForSum()
        {
            Vector<long> sums1 = Vector<long>.Zero, sums2 = Vector<long>.Zero;
            var vectorSize = Vector<long>.Count;
            var step = 2 * vectorSize;
            int alignedLength = data.Length - data.Length % step;
            for (int i = 0; i < alignedLength; i += step)
            {
                sums1 += new Vector<long>(data, i);
                sums2 += new Vector<long>(data, i + vectorSize);
            }
            long sum = 0;
            for (int i = 0; i < vectorSize; i++)
                sum += sums1[i] + sums2[i];
            for (int i = alignedLength; i < data.Length; i++)
                sum += data[i];
            return sum;
        }

    }
}