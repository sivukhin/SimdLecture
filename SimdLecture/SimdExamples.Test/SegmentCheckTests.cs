using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using FluentAssertions;
using NUnit.Framework;
using SimdLecture;

namespace SimdExamples.Test
{
    public abstract class SegmentCheckTests
    {
        protected abstract Func<int[], int, int, int> checker { get; }

        [Test]
        public void TestEmptyArray()
        {
            checker(new int[] {}, -1, 1).Should().Be(0);
        }

        [TestCase(-2, 0)]
        [TestCase(0, 1)]
        [TestCase(2, 0)]
        public void TestSingleElementArray(int element, int result)
        {
            checker(new[] {element}, -1, 1).Should().Be(result);
        }

        public static TestCaseData[] checkSegmentTests =
        {
            ParamsTestCaseData.Create(new[] {1, -2, -3, 3, 0, 2}).Returns(4),
            ParamsTestCaseData.Create(new[] {-3, -4, 5, 4, 3, 3, -10}).Returns(0),
            ParamsTestCaseData.Create(new[] {-2, 1, 0, 0, 2, 1}).Returns(6)
        };
        [TestCaseSource(nameof(checkSegmentTests))]
        public int TestArray(int[] array)
        {
            return checker(array, -2, 2);
        }

        public static TestCaseData[] checkSegmentLongTests =
        {
            ParamsTestCaseData.Create(new[] {1, 6, 3}.RepeatSequence(10).ToArray()).Returns(20),
            ParamsTestCaseData.Create(new[] {6, -10, 7, 8}.RepeatSequence(10).ToArray()).Returns(0),
            ParamsTestCaseData.Create(new[] {1, 5, -5, 0, 0}.RepeatSequence(10).ToArray()).Returns(50)
        };
        [TestCaseSource(nameof(checkSegmentLongTests))]
        public int TestLongArray(int[] array)
        {
            return checker(array, -5, 5);
        }
    }

    [TestFixture]
    public class SimdForCountInRangeTests : SegmentCheckTests
    {
        protected override Func<int[], int, int, int> checker
            => (data, l, r) => new SegmentCheckBenchmark(data, l, r).SimdForCountInRange();
    }

    [TestFixture]
    public class SimdOptimizedForCountInRangeTests : SegmentCheckTests
    {
        protected override Func<int[], int, int, int> checker
            => (data, l, r) => new SegmentCheckBenchmark(data, l, r).SimdOptimizedForCountInRange();
    }

    [TestFixture]
    public class ForeachCountInRangeTests : SegmentCheckTests
    {
        protected override Func<int[], int, int, int> checker
            => (data, l, r) => new SegmentCheckBenchmark(data, l, r).ForeachCountInRange();
    }

    [TestFixture]
    public class SimpleForCountInRangeTests : SegmentCheckTests
    {
        protected override Func<int[], int, int, int> checker
            => (data, l, r) => new SegmentCheckBenchmark(data, l, r).SimpleForCountInRange();
    }
}
