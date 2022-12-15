using OpenTK.Graphics.OpenGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using gdi = System.Drawing;

namespace Tower
{
    public class Texture : IDisposable
    {
        public int TextureId { get; private set; }

        public static Texture CreateTexture(byte[] pixels, int w, int h, bool filter = true)
        {
            int tex = GL.GenTexture();
            GL.BindTexture(TextureTarget.Texture2D, tex);
            GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgb, w, h, 0, PixelFormat.Bgr, PixelType.UnsignedByte, pixels);
            int filterFunc = filter ? (int)All.Linear : (int)All.Nearest;
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, filterFunc);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, filterFunc);
            GL.BindTexture(TextureTarget.Texture2D, 0);
            return new Texture { TextureId = tex };
        }

        public static Texture CreateTexture(IntPtr pixels, int w, int h, bool filter = true)
        {
            int tex = GL.GenTexture();
            GL.BindTexture(TextureTarget.Texture2D, tex);
            GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgb, w, h, 0, PixelFormat.Bgr, PixelType.UnsignedByte, pixels);
            int filterFunc = filter ? (int)All.Linear : (int)All.Nearest;
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, filterFunc);
            GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, filterFunc);
            GL.BindTexture(TextureTarget.Texture2D, 0);
            return new Texture { TextureId = tex };
        }

        public static Texture CreateTexture(gdi.Bitmap bmp, bool filter = true)
        {
            var bdata = bmp.LockBits(new gdi.Rectangle(0, 0, bmp.Width, bmp.Height), gdi.Imaging.ImageLockMode.ReadOnly, gdi.Imaging.PixelFormat.Format24bppRgb);
            var texture = CreateTexture(bdata.Scan0, bdata.Width, bdata.Height, filter);
            bmp.UnlockBits(bdata);
            return texture;
        }

        public static Texture LoadTexture(string filename, bool filter = true)
        {
            using (var bmp = new gdi.Bitmap(filename))
            {
                return CreateTexture(bmp, filter);
            }
        }

        public void Dispose()
        {
            GL.DeleteTexture(TextureId);
            TextureId = 0;
        }
    }
}
