using System;
using System.Diagnostics;
using System.Drawing;
using System.Drawing.Imaging;

// Это пространство имен содержит средства создания оконных приложений. В частности в нем находится класс Form.
// Для того, чтобы оно стало доступно, в проект был подключен на System.Windows.Forms.dll
using System.Windows.Forms;

namespace Fractals
{
	internal static class Program
	{
        private static Stopwatch Stopwatch = new Stopwatch();
		private static void Main()
		{
            Stopwatch.Start();
		    int iterations = 10000000;
            var image = new Bitmap(800, 800, PixelFormat.Format24bppRgb);
		    image.ClearImage(Color.White);

            using (var pixels = new Pixels(image, EstimateBoundingBox()))
            {
                // ReSharper disable once AccessToDisposedClosure
                DragonFractalTask.DrawDragonFractal((x, y) => pixels.SetPixel(x, y), iterations, 0);
            }
			ShowImageInWindow(image);
		}

	    private static RectangleF EstimateBoundingBox()
	    {
            var boundingBox = new RectangleF(new PointF(1, 0), new SizeF(0, 0));
            DragonFractalTask.DrawDragonFractal((x, y) =>
            {
                boundingBox = boundingBox.ExpandToPoint(new PointF((float)x, (float)y));
            }, 1000, 10);
	        boundingBox.Inflate(0.1f, 0.1f);
	        return boundingBox;
	    }

	    private static void ShowImageInWindow(Bitmap image)
		{
			var form = new Form
			{
				Text = "Harter–Heighway dragon",
				ClientSize = image.Size
			};

			form.Controls.Add(new PictureBox {Image = image, Dock = DockStyle.Fill, SizeMode = PictureBoxSizeMode.CenterImage});
            Stopwatch.Stop();
		    Console.WriteLine(Stopwatch.ElapsedMilliseconds);
            form.ShowDialog();
		}
	}
}