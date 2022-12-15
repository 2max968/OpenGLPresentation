using OpenTK;
using OpenTK.Mathematics;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Tower;

namespace Tower
{
    public class Model : IDisposable
    {
        static Shaders tmp;

        public VertexBuffer VertexBuffer { get; set; }
        public Texture Texture { get; set; }
        public Matrix4 Transformation { get; set; }

        public Model(float[] vertexArray, Bitmap texture, bool textureFiltering = true)
        {
            VertexBuffer = VertexBuffer.CreateBuffer(OpenTK.Graphics.OpenGL.PrimitiveType.Triangles, vertexArray);
            Texture = Texture.CreateTexture(texture, textureFiltering);
            Transformation = Matrix4.Identity;
        }

        public void Dispose()
        {
            VertexBuffer?.Dispose();
            Texture?.Dispose();
        }
    }
}
