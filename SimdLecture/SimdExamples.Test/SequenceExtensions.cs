using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimdExamples.Test
{
    public static class SequenceExtensions
    {
        public static IEnumerable<T> RepeatSequence<T>(this IList<T> sequence, int count)
        {
            for (int i = 0; i < count; i++)
                foreach (var element in sequence)
                    yield return element;
        }
    }
}
