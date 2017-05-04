using System;
using System.Diagnostics;
using System.Diagnostics.Eventing.Reader;
using System.Linq;
using System.Reflection;
using System.Reflection.Emit;
using System.Runtime.InteropServices.WindowsRuntime;
using BenchmarkDotNet.Attributes.Jobs;
using BenchmarkDotNet.Running;
using BenchmarkDotNet.Attributes;

namespace SimdLecture
{
    [SimpleJob(targetCount: 10, id: "FastBenchmark")]
    public class SumLoopBenchmark
    {
        public delegate long SumDelegate();
        private long[] data;
        private int length;

        public long[] GenerateSequence(int n)
        {
            var seq = new long[n];
            for (int i = 0; i < n; i++)
                seq[i] = i;
            return seq;
        }

        public SumLoopBenchmark()
        {
            length = (int)3e7;
            data = GenerateSequence(length);
        }

        [Benchmark]
        public long LinqSum() => data.Sum(x => x);

        [Benchmark]
        public long LinqAggregate() => data.Aggregate((long)0, (current, element) => current + element);

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
        public long UnrolledForSum()
        {
            long sum = 0;
            int alignedLength = data.Length - data.Length % 4;
            for (int i = 0; i < alignedLength; i += 4)
            {
                sum += data[i] + data[i + 1] + data[i + 2] + data[i + 3];
            }
            for (int i = alignedLength; i < data.Length; i++)
                sum += data[i];
            return sum;
        }

        private Func<int, long> GeneratedUnrolledAlignedSumMethod()
        {
            var dynamicMethod = new DynamicMethod(
                nameof(GeneratedUnrolledAlignedSumMethod), typeof(long), new[] {typeof(SumLoopBenchmark), typeof(int)}, typeof(SumLoopBenchmark));

            GenerateMapMethodBody(dynamicMethod.GetILGenerator());

            return (Func<int, long>)dynamicMethod.CreateDelegate(typeof(Func<int, long>), this);
        }

        [Benchmark]
        public long GeneratedUnrolledForSum()
        {
            int alignedLength = length - length % 4;
            var sum = GeneratedUnrolledAlignedSumMethod()(alignedLength);
            for (int i = alignedLength; i < length; i++)
                sum += data[i];
            return sum;
        }

        private void GenerateMapMethodBody(ILGenerator generator)
        {
            var sumVar = generator.DeclareLocal(typeof(long));
            var dataVar = generator.DeclareLocal(typeof(int[]));
            var indexVar = generator.DeclareLocal(typeof(int));
            generator.Emit(OpCodes.Ldc_I4_0);
            generator.Emit(OpCodes.Conv_I8);
            generator.Emit(OpCodes.Stloc, sumVar);

            generator.Emit(OpCodes.Ldarg_0);
            generator.Emit(OpCodes.Ldfld, typeof(SumLoopBenchmark)
                .GetFields(BindingFlags.NonPublic | BindingFlags.Instance)
                .Single(f => f.Name == "data"));
            generator.Emit(OpCodes.Stloc, dataVar);

            var startLoopLabel = generator.DefineLabel();
            var bodyLoopLabel = generator.DefineLabel();
            generator.Emit(OpCodes.Ldc_I4_0);
            generator.Emit(OpCodes.Stloc, indexVar);
            generator.Emit(OpCodes.Br_S, startLoopLabel);

            generator.MarkLabel(bodyLoopLabel);
            generator.Emit(OpCodes.Ldloc, sumVar);
            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldelem_I4);

            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_1);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Ldelem_I4);
            generator.Emit(OpCodes.Add);

            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_2);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Ldelem_I4);
            generator.Emit(OpCodes.Add);

            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_3);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Ldelem_I4);
            generator.Emit(OpCodes.Add);

            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Stloc, sumVar);

            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_4);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Stloc, indexVar);

            generator.MarkLabel(startLoopLabel);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldarg_1);
            generator.Emit(OpCodes.Conv_I4);

            generator.Emit(OpCodes.Blt_S, bodyLoopLabel);

            generator.Emit(OpCodes.Ldloc, sumVar);
            generator.Emit(OpCodes.Ret);
        }
    }

    class Program
    {
        static void Main(string[] args)
        {
            BenchmarkRunner.Run<SumLoopBenchmark>();
        }
    }
}
