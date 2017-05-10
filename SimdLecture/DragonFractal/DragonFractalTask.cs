// В этом пространстве имен содержатся средства для работы с изображениями. Чтобы оно стало доступно, в проект был подключен Reference на сборку System.Drawing.dll

using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;

namespace Fractals
{
	internal static class DragonFractalTask
	{
		public static void DrawDragonFractal(Pixels pixels, int iterationsCount, int seed)
		{
            var stopwatch = new Stopwatch();
		    stopwatch.Start();

		    var random = new Random(seed);
		    double x = 1, y = 0;
		    for (int i = 0; i < iterationsCount; i++)
		    {
		        var transformation = random.Next() & 1;
		        double newX, newY;
                if (transformation == 0)
                { 
		            newX = (x * Math.Cos(Math.PI / 4) - y * Math.Sin(Math.PI / 4)) / Math.Sqrt(2);
                    newY = (x * Math.Sin(Math.PI / 4) + y * Math.Cos(Math.PI / 4)) / Math.Sqrt(2);
                }
                else
                {
                    newX = (x * Math.Cos(Math.PI - Math.PI / 4) - y * Math.Sin(Math.PI - Math.PI / 4)) / Math.Sqrt(2) + 1;
                    newY = (x * Math.Sin(Math.PI - Math.PI / 4) + y * Math.Cos(Math.PI - Math.PI / 4)) / Math.Sqrt(2);
                }
		        x = newX;
		        y = newY;
                pixels.SetPixel(x, y);
            }
		    Console.WriteLine(stopwatch.ElapsedMilliseconds);
		}
	}
}