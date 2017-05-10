using System;
using System.Collections.Generic;
using System.Collections.Specialized;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;
using BenchmarkDotNet.Attributes;
using BenchmarkDotNet.Attributes.Jobs;

namespace SimdLecture
{
    [SimpleJob(targetCount: 10, id: "FastBenchmark")]
    public class SegmentCheckBenchmark
    {
        private readonly int[] data;

        public int L { get; set; } = -(int)1e8;
        
        public int R { get; set; } = (int)1e9;

        public SegmentCheckBenchmark()
        {
            var length = (int)1e8;
            data = GenerateSequence(length);
        }

        private int[] GenerateSequence(int n)
        {
            var random = new Random(0);
            var seq = new int[n];
            for (int i = 0; i < n; i++)
                seq[i] = random.Next();
            return seq;
        }

        [Benchmark]
        public int ForeachCountInRange()
        {
            var count = 0;
            foreach (var element in data)
                if (L <= element && element <= R)
                    count++;
            return count;
        }

        [Benchmark]
        public int SimpleForCountInRange()
        {
            var count = 0;
            for (int i = 0; i < data.Length; i++)
                if (L <= data[i] && data[i] <= R)
                    count++;
            return count;
        }

        [Benchmark]
        public int SimdForCountInRange()
        {
            var vectorSize = Vector<int>.Count;
            var alignedLength = data.Length - data.Length % vectorSize;
            var left = new Vector<int>(L);
            var right = new Vector<int>(R);
            int count = 0;
            for (int i = 0; i < alignedLength; i += vectorSize)
            {
                var slice = new Vector<int>(data, i);
                var result = Vector.LessThanOrEqual(left, slice) & Vector.LessThanOrEqual(slice, right);
                for (int s = 0; s < vectorSize; s++)
                    count -= result[s];
            }
            return count;
        }
    }
}
