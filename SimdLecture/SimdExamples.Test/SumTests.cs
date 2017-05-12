using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NUnit.Framework;
using FluentAssertions;
using SimdLecture;

namespace SimdExamples.Test
{
    public abstract class SumTests
    {
        protected abstract Func<long[], long> summator { get; }

        [Test]
        public void TestEmptyArray()
        {
            summator(new long[] {}).Should().Be(0);
        }
        
        [TestCase(0)]
        [TestCase(1)]
        [TestCase(-1)]
        public void TestSingleElementArray(long element)
        {
            summator(new[] {element}).Should().Be(element);
        }

        public static TestCaseData[] sumTests = {
            ParamsTestCaseData.Create(0L, 1L, 2L).Returns(3),
            ParamsTestCaseData.Create(1L, 0L, -1L).Returns(0),
            ParamsTestCaseData.Create((long)1e10, (long)1e10).Returns((long)2e10),
            ParamsTestCaseData.Create((long)1e10, -(long)1e10).Returns(0L),
            ParamsTestCaseData.Create(0L, -1L, -1L).Returns(-2L),
        };

        [TestCaseSource(nameof(sumTests))]
        public long TestArray(long[] elements)
        {
            return summator(elements);
        }
    }

    [TestFixture]
    public class SimdForSumTests : SumTests
    {
        protected override Func<long[], long> summator => data => new SumLoopBenchmark(data).SimdForSum();
    }
    [TestFixture]
    public class ForeachSumTests : SumTests
    {
        protected override Func<long[], long> summator => data => new SumLoopBenchmark(data).ForeachSum();
    }
    [TestFixture]
    public class SimpleForSumTests : SumTests
    {
        protected override Func<long[], long> summator => data => new SumLoopBenchmark(data).SimpleForSum();
    }
}
