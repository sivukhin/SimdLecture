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
            data = GenerateSequence(length);
        }

        #region Initialization
        private long[] GenerateSequence(int n)
        {
            var random = new Random(0);
            var seq = new long[n];
            for (int i = 0; i < n; i++)
                seq[i] = random.Next();
            return seq;
        }
        #endregion

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

        #region Miscellaneous
        //[Benchmark]
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

        //[Benchmark]
        public long LinqSum() => data.Sum(x => x);

        //[Benchmark]
        public long LinqAggregate() => data.Aggregate((long)0, (current, element) => current + element);

        //[Benchmark]
        public long SimdUnrolledForSum()
        {
            var sums = Vector<long>.Zero;
            int vectorSize = Vector<long>.Count;
            var stepSize = vectorSize * 4;
            int alignedLength = data.Length - data.Length % stepSize;
            for (int i = 0; i < alignedLength; i += stepSize)
            {
                sums += new Vector<long>(data, i);
                sums += new Vector<long>(data, i + vectorSize);
                sums += new Vector<long>(data, i + 2 * vectorSize);
                sums += new Vector<long>(data, i + 3 * vectorSize);
            }
            long sum = 0;
            for (int i = 0; i < vectorSize; i++)
                sum += sums[i];
            for (int i = alignedLength; i < data.Length; i++)
                sum += data[i];
            return sum;
        }

        //[Benchmark]
        public long GeneratedUnrolledForSum()
        {
            int alignedLength = data.Length -data.Length % 4;
            var sum = GeneratedUnrolledAlignedSumMethod()(alignedLength);
            for (int i = alignedLength; i < data.Length; i++)
                sum += data[i];
            return sum;
        }

        private Func<int, long> GeneratedUnrolledAlignedSumMethod()
        {
            var dynamicMethod = new DynamicMethod(
                nameof(GeneratedUnrolledAlignedSumMethod), typeof(long), new[] { typeof(SumLoopBenchmark), typeof(int) },
                typeof(SumLoopBenchmark));
            GenerateMapMethodBody(dynamicMethod.GetILGenerator());
            return (Func<int, long>)dynamicMethod.CreateDelegate(typeof(Func<int, long>), this);
        }

        private void GenerateMapMethodBody(ILGenerator generator)
        {
            var sumVar = generator.DeclareLocal(typeof(long));
            var dataVar = generator.DeclareLocal(typeof(long[]));
            var indexVar = generator.DeclareLocal(typeof(int));
            var lengthVar = generator.DeclareLocal(typeof(int));
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

            generator.Emit(OpCodes.Ldarg_1);
            generator.Emit(OpCodes.Stloc, lengthVar);

            generator.Emit(OpCodes.Br_S, startLoopLabel);

            generator.MarkLabel(bodyLoopLabel);
            generator.Emit(OpCodes.Ldloc, sumVar);
            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldelem_I8);

            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_1);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Ldelem_I8);
            generator.Emit(OpCodes.Add);

            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_2);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Ldelem_I8);
            generator.Emit(OpCodes.Add);

            generator.Emit(OpCodes.Ldloc, dataVar);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_3);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Ldelem_I8);
            generator.Emit(OpCodes.Add);

            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Stloc, sumVar);

            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldc_I4_4);
            generator.Emit(OpCodes.Add);
            generator.Emit(OpCodes.Stloc, indexVar);

            generator.MarkLabel(startLoopLabel);
            generator.Emit(OpCodes.Ldloc, indexVar);
            generator.Emit(OpCodes.Ldloc, lengthVar);

            generator.Emit(OpCodes.Blt_S, bodyLoopLabel);

            generator.Emit(OpCodes.Ldloc, sumVar);
            generator.Emit(OpCodes.Ret);
        }
        #endregion
    }
}