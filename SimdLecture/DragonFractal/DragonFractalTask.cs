// В этом пространстве имен содержатся средства для работы с изображениями. Чтобы оно стало доступно, в проект был подключен Reference на сборку System.Drawing.dll

using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.Numerics;

namespace Fractals
{
    public class RandomBitSource
    {
        private Random Random { get; }
        private uint value;
        private int shift;
        public RandomBitSource(int seed)
        {
            Random = new Random(seed);
            value = 0;
            shift = -1;
        }

        public uint NextBit()
        {
            if (shift == 32)
            {
                value = (uint)Random.Next();
                shift = -1;
            }
            shift++;
            return (value >> shift) & 1;
        }
    }
	public static class DragonFractalTask
	{
		public static void DrawDragonFractal(Action<double, double> updatePoint, int iterationsCount, int seed)
		{
		    var random = new RandomBitSource(seed);

		    var transform1 = (Complex.One + Complex.ImaginaryOne) / 2;
		    var transform2 = -(Complex.One - Complex.ImaginaryOne) / 2;
		    var point = Complex.One;
            for (int i = 0; i < iterationsCount; i++)
		    {
		        var transformation = random.NextBit();
		        point = transformation == 0 ? Complex.Multiply(transform1, point) : 1 + Complex.Multiply(transform2, point);
		        updatePoint(point.Real, point.Imaginary);
		    }
		}
	}
}