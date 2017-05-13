using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimdLecture
{
    public static class RandomExtensions
    {
        public static int[] GenerateIntSequence(this Random random, int length)
        {
            var seq = new int[length];
            for (int i = 0; i < length; i++)
                seq[i] = random.Next();
            return seq;
        }

        public static long[] GenerateLongSequence(this Random random, int length)
        {
            var seq = new long[length];
            for (int i = 0; i < length; i++)
                seq[i] = random.Next();
            return seq;
        }
    }
}
