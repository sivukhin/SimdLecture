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

		    double angle1 = Math.PI / 4;
		    double angle2 = Math.PI - Math.PI / 4;
            double scale = Math.Sqrt(2);
            var transform1 = new Matrix3x2(
                (float)(+Math.Cos(angle1) / scale), (float)(Math.Sin(angle1) / scale), 
                (float)(-Math.Sin(angle1) / scale), (float)(Math.Cos(angle1) / scale),
                0, 0);
		    var transform2 = new Matrix3x2(
		        (float)(Math.Cos(angle2) / scale), (float)(Math.Sin(angle2) / scale),
		        (float)(-Math.Sin(angle2) / scale), (float)(Math.Cos(angle2) / scale),
		        1, 0);
            var point = new Vector2(1, 0);
            for (int i = 0; i < iterationsCount; i++)
		    {
		        var transformation = random.NextBit();
		        point = Vector2.Transform(point, transformation == 0 ? transform1 : transform2);
		        updatePoint(point.X, point.Y);
		    }
		}
	}
}