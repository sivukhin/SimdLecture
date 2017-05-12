using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace SimdLecture
{
    public static class RandomExtensions
    {
        public static int[] GenerateSequence(this Random random, int length)
        {
            var seq = new int[length];
            for (int i = 0; i < length; i++)
                seq[i] = random.Next();
            return seq;
        }
    }
}
