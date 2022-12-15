using OpenTK.Graphics.OpenGL;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tower
{
    public class VertexBuffer : IDisposable
    {
        public int VBO { get; private set; }
        public int VAO { get; private set; }
        public PrimitiveType PrimitiveType { get; private set; }
        public int VerticesCount { get; private set; }

        public static VertexBuffer CreateBuffer(PrimitiveType primitiveType, float[] vertices)
        {
            int vbo = GL.GenBuffer();
            GL.BindBuffer(BufferTarget.ArrayBuffer, vbo);
            GL.BufferData<float>(BufferTarget.ArrayBuffer, vertices.Length * 4, vertices, BufferUsageHint.StaticDraw);

            int vao = GL.GenVertexArray();
            GL.BindVertexArray(vao);
            GL.EnableVertexAttribArray(0);
            GL.VertexAttribPointer(0, 3, VertexAttribPointerType.Float, true, 4 * 8, 0);
            GL.EnableVertexAttribArray(1);
            GL.VertexAttribPointer(1, 3, VertexAttribPointerType.Float, true, 4 * 8, 4 * 3);
            GL.EnableVertexAttribArray(2);
            GL.VertexAttribPointer(2, 2, VertexAttribPointerType.Float, true, 4 * 8, 4 * 6);
            GL.BindVertexArray(0);

            return new VertexBuffer()
            {
                VBO = vbo,
                VAO = vao,
                PrimitiveType = primitiveType,
                VerticesCount = vertices.Length / 8
            };
        }

        public void Dispose()
        {
            GL.DeleteBuffer(VBO);
            GL.DeleteVertexArray(VAO);
            VBO = 0;
            VAO = 0;
        }

        public void Draw()
        {
            GL.BindVertexArray(VAO);
            GL.DrawArrays(PrimitiveType, 0, VerticesCount);
        }
    }
}
