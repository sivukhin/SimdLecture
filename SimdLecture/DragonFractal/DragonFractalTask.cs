// В этом пространстве имен содержатся средства для работы с изображениями. Чтобы оно стало доступно, в проект был подключен Reference на сборку System.Drawing.dll

using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;

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
            double scale = Math.Sqrt(2);
            double cosPI_4 = Math.Cos(Math.PI / 4);
            double sinPI_4 = Math.Sin(Math.PI / 4);
            double x = 1, y = 0;
		    for (int i = 0; i < iterationsCount; i++)
		    {
		        var transformation = random.NextBit();
		        double newX, newY;
                if (transformation == 0)
                {
                    newX = (x * cosPI_4 - y * sinPI_4) / scale;
                    newY = (x * sinPI_4 + y * cosPI_4) / scale;
                }
                else
                {
                    newX = (-x * cosPI_4 - y * sinPI_4) / Math.Sqrt(2) + 1;
                    newY = (x * sinPI_4 - y * cosPI_4) / Math.Sqrt(2);
                }
		        x = newX;
		        y = newY;
                updatePoint(x, y);
            }
		}
	}
}