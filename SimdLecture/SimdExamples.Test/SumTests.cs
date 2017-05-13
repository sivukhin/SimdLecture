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
            ParamsTestCaseData.Create(new []{0L, 1L, 2L}).Returns(3),
            ParamsTestCaseData.Create(new [] {1L, 0L, -1L}).Returns(0),
            ParamsTestCaseData.Create(new [] {(long)1e10, (long)1e10}).Returns((long)2e10),
            ParamsTestCaseData.Create(new [] {(long)1e10, -(long)1e10}).Returns(0L),
            ParamsTestCaseData.Create(new [] {0L, -1L, -1L}).Returns(-2L),
        };

        [TestCaseSource(nameof(sumTests))]
        public long TestArray(long[] elements)
        {
            return summator(elements);
        }

        public static TestCaseData[] sumLongTests = {
            ParamsTestCaseData.Create(new []{0L, 1L, 2L}.RepeatSequence(10).ToArray()).Returns(30),
            ParamsTestCaseData.Create(new [] {1L, 0L, -1L}.RepeatSequence(100).ToArray()).Returns(0),
            ParamsTestCaseData.Create(new [] {(long)1e10, (long)1e10}.RepeatSequence(100).ToArray()).Returns((long)2e12),
            ParamsTestCaseData.Create(new [] {(long)1e10, -(long)1e10}.RepeatSequence(100).ToArray()).Returns(0L),
            ParamsTestCaseData.Create(new [] {0L, -1L, -1L}.RepeatSequence(20).ToArray()).Returns(-40L),
        };

        [TestCaseSource(nameof(sumLongTests))]
        public long TestLongArray(long[] elements)
        {
            return summator(elements);
        }

        public static TestCaseData[] sumLongOddTests = {
            ParamsTestCaseData.Create(new []{0L, 1L, 2L}.RepeatSequence(11).ToArray()).Returns(33),
            ParamsTestCaseData.Create(new [] {1L, 0L, -1L}.RepeatSequence(101).ToArray()).Returns(0),
            ParamsTestCaseData.Create(new [] {(long)1e10, (long)1e10, 0}.RepeatSequence(101).ToArray()).Returns(101 * (long)2e10),
            ParamsTestCaseData.Create(new [] {(long)1e10, -(long)1e10, 0}.RepeatSequence(101).ToArray()).Returns(0L),
            ParamsTestCaseData.Create(new [] {0L, -1L, -1L}.RepeatSequence(21).ToArray()).Returns(-42L),
        };

        [TestCaseSource(nameof(sumLongOddTests))]
        public long TestLongOddArray(long[] elements)
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
