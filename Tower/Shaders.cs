using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;
using OpenTK;
using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;

namespace Tower
{
    public class Shaders : IDisposable
    {
        public static readonly string Vertex = @"#version 310 es
precision highp float;

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUv;

uniform mat4 modelview;
uniform mat4 projection;

out vec3 normal;
out vec2 uv;

void main()
{
    gl_Position = projection * modelview * vec4(aPosition, 1.0);
    normal = normalize(transpose(inverse(mat3(modelview))) * aNormal);
    
    uv = aUv;
}
";

        public static readonly string Fragment = @"#version 310 es
precision highp float;

in vec3 normal;
in vec2 uv;

uniform vec3 lightDirection;
uniform sampler2D mainTex;

out vec4 fragColor;

void main()
{
    float v = dot(normal, lightDirection) * -0.5 + 0.7;
    vec4 color = texture(mainTex, uv);
    fragColor = vec4(color.rgb * v, color.a);
}
";

        Dictionary<string, int> uniformLocations = new Dictionary<string, int>();

        public int ProgramId { get; private set; }

        public static Shaders LoadShader()
        {
            Console.WriteLine($"GLSL Version: {GL.GetString(StringName.ShadingLanguageVersion)}");

            int vertexId = GL.CreateShader(ShaderType.VertexShader);
            GL.ShaderSource(vertexId, 1, new[] { Vertex }, new[] { Vertex.Length });
            GL.CompileShader(vertexId);

            GL.GetShader(vertexId, ShaderParameter.CompileStatus, out int vertexCompileStatus);
            if (vertexCompileStatus == 0)
            {
                GL.GetShaderInfoLog(vertexId, 1024, out _, out string infoLog);
                GL.DeleteShader(vertexId);
                throw new Exception("Vertex Shader: " + infoLog);
            }

            int fragmentId = GL.CreateShader(ShaderType.FragmentShader);
            GL.ShaderSource(fragmentId, 1, new[] { Fragment }, new[] { Fragment.Length });
            GL.CompileShader(fragmentId);

            GL.GetShader(fragmentId, ShaderParameter.CompileStatus, out int fragmentCompileStatus);
            if (fragmentCompileStatus == 0)
            {
                GL.GetShaderInfoLog(fragmentId, 1024, out _, out string infoLog);
                GL.DeleteShader(vertexId);
                GL.DeleteShader(fragmentId);
                throw new Exception("Fragment Shader: " + infoLog);
            }

            int programId = GL.CreateProgram();
            GL.AttachShader(programId, vertexId);
            GL.AttachShader(programId, fragmentId);
            GL.LinkProgram(programId);

            GL.GetProgram(programId, ProgramParameter.LinkStatus, out int linkProgramStatus);
            if (linkProgramStatus == 0)
            {
                GL.GetProgramInfoLog(programId, 1024, out int length, out string infoLog);
                GL.DeleteShader(vertexId);
                GL.DeleteShader(fragmentId);
                throw new Exception("Linking program: " + infoLog);
            }

            GL.DeleteShader(vertexId);
            GL.DeleteShader(fragmentId);

            return new Shaders() { ProgramId = programId };
        }

        int getUniformLocation(string name)
        {
            if (!uniformLocations.ContainsKey(name))
                uniformLocations.Add(name, GL.GetUniformLocation(ProgramId, name));
            return uniformLocations[name];
        }

        public void Set(string name, Matrix4 mat)
        {
            GL.UseProgram(ProgramId);
            int location = getUniformLocation(name);
            GL.UniformMatrix4(location, false, ref mat);
        }

        public void Set(string name, Vector3 vec)
        {
            GL.UseProgram(ProgramId);
            int location = getUniformLocation(name);
            GL.Uniform3(location, ref vec);
        }

        public void Set(string name, int textureId)
        {
            GL.UseProgram(ProgramId);
            int location = getUniformLocation(name);
            GL.BindSampler(location, textureId);
        }

        public void Dispose()
        {
            GL.DeleteProgram(ProgramId);
            ProgramId = 0;
        }
    }
}
