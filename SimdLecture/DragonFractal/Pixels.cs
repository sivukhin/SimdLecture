using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Linq;

namespace Fractals
{
    public static class DrawingExtensions
    {
        public static RectangleF ExpandToPoint(this RectangleF rectangle, PointF point)
        {
            var leftX = Math.Min(rectangle.Left, point.X);
            var topY = Math.Min(rectangle.Top, point.Y);

            var rightX = Math.Max(rectangle.Right, point.X);
            var bottomY = Math.Max(rectangle.Bottom, point.Y);
            return new RectangleF(leftX, topY, rightX - leftX, bottomY - topY);
        }

        public static Image ClearImage(this Image image, Color color)
        {
            using (var graphics = Graphics.FromImage(image))
                graphics.Clear(color);
            return image;
        }
    }
    public class Pixels : IDisposable
    {
        private readonly Bitmap image;
        private readonly BitmapData imageData;
        private readonly int pixelSize;
        private readonly float scale;
        private readonly double pixelDeltaX;
        private readonly double pixelDeltaY;

        public Pixels(Bitmap image, RectangleF boundingBox)
        {
            this.image = image;
            imageData = image.LockBits(new Rectangle(new Point(0, 0), image.Size), ImageLockMode.ReadWrite, image.PixelFormat);
            pixelSize = imageData.Stride / imageData.Width;
            var scaleX = (imageData.Width - 20) / boundingBox.Width;
            var scaleY = (imageData.Height - 20) / boundingBox.Height;
            scale = Math.Min(scaleX, scaleY);

            pixelDeltaX = -boundingBox.Left - boundingBox.Width / 2 + imageData.Width / 2.0 / scale;
            pixelDeltaY = -boundingBox.Top - boundingBox.Height / 2 + imageData.Height / 2.0 / scale;
        }

        public unsafe void SetPixel(double x, double y)
        {
            var point = TransformPoint(x, y);
            byte* ptr = (byte*)imageData.Scan0 + point.Item2 * imageData.Stride + point.Item1 * pixelSize;
            var value = (int)ptr[0];
            ptr[0] = ptr[1] = (byte)Math.Max(0, value - 3);
        }

        private Tuple<int, int> TransformPoint(double x, double y)
        {
            var nx = (int)((x + pixelDeltaX) * scale);
            var ny = (int)((y + pixelDeltaY) * scale);
            return Tuple.Create(nx, ny);
        }

        public void Dispose()
        {
            image.UnlockBits(imageData);
        }
    }
}