using NUnit.Framework;

namespace SimdExamples.Test
{
    public static class ParamsTestCaseData
    {
        public static TestCaseData Create<T>(params T[] items)
        {
            return new TestCaseData(items);
        }
    }
}