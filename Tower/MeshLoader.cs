using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Tower
{
    public struct Vector3D
    {
        public float X;
        public float Y;
        public float Z;
    }

    public struct Vector2D
    {
        public float X;
        public float Y;
    }

    public static class MeshLoader
    {
        public static (List<Vector3D> vertices, List<Vector3D> normals, List<Vector2D> uvs, List<int> faces) LoadObj(Stream stream)
        {
            List<Vector3D> vertices = new List<Vector3D>();
            List<Vector3D> normals = new List<Vector3D>();
            List<Vector2D> uvs = new List<Vector2D>();
            List<int> faces = new List<int>();

            normals.Add(new Vector3D());
            uvs.Add(new Vector2D());

            using (StreamReader reader = new StreamReader(stream))
            {
                while (!reader.EndOfStream)
                {
                    string line = reader.ReadLine();
                    int commentStart = line.IndexOf('#');
                    if (commentStart >= 0)
                        line = line.Substring(0, commentStart).Trim();
                    if (line.StartsWith("v "))
                    {
                        var v = parseGroup(line.Substring(2));
                        vertices.Add(new Vector3D() { X = v[0], Y = v[1], Z = v[2] });
                    }
                    else if (line.StartsWith("vn"))
                    {
                        var vn = parseGroup(line.Substring(3));
                        normals.Add(new Vector3D() { X = vn[0], Y = vn[1], Z = vn[2] });
                    }
                    else if (line.StartsWith("vt"))
                    {
                        var vt = parseGroup(line.Substring(3));
                        uvs.Add(new Vector2D() { X = vt[0], Y = vt[1] });
                    }
                    else if (line.StartsWith("f"))
                    {
                        var words = line.Substring(2).Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
                        var nums = words.Select(w => w.Split('/')).ToArray();
                        int[] ints = new int[3];
                        foreach (var num in nums)
                        {
                            if (num.Length == 1)
                            {
                                ints[0] = parseI(num[0]) - 1;
                                ints[1] = 0;
                                ints[2] = 0;
                            }
                            else if (num.Length == 2)
                            {
                                ints[0] = parseI(num[0]) - 1;
                                ints[1] = 0;
                                ints[2] = parseI(num[1]);
                            }
                            else if (num.Length == 3)
                            {
                                ints[0] = parseI(num[0]) - 1;
                                ints[1] = parseI(num[2]);
                                ints[2] = string.IsNullOrWhiteSpace(num[1]) ? 0 : parseI(num[1]);
                            }
                            Debug.Assert(ints[0] >= 0 && ints[1] >= 0 && ints[2] >= 0);
                            faces.AddRange(ints);
                        }
                    }
                }
            }

            return (vertices, normals, uvs, faces);
        }

        static float[] parseGroup(string text)
        {
            var words = text.Split(new[] { ' ', '\t' }, StringSplitOptions.RemoveEmptyEntries);
            return words.Select(w => float.Parse(w, NumberStyles.Any, CultureInfo.InvariantCulture)).ToArray();
        }

        static int parseI(string text)
        {
            return int.Parse(text, NumberStyles.Any, CultureInfo.InvariantCulture);
        }

        public static float[] LoadObjToArray(string filename)
        {
            using (var stream = File.OpenRead(filename))
            {
                return LoadObjToArray(stream);
            }
        }

        public static float[] LoadObjToArray(Stream stream)
        {
            var mesh = LoadObj(stream);
            float[] array = new float[mesh.faces.Count * 8];
            for (int i = 0; i < mesh.faces.Count / 3; i++)
            {
                int faceIndex = i * 3;
                var vert = mesh.vertices[mesh.faces[faceIndex + 0]];
                var norm = mesh.normals[mesh.faces[faceIndex + 1]];
                var uv = mesh.uvs[mesh.faces[faceIndex + 2]];
                float[] face = new[]
                {
                    vert.X, vert.Y, vert.Z,
                    norm.X, norm.Y, norm.Z,
                    uv.X, uv.Y
                };
                for (int j = 0; j < 8; j++)
                    array[i * 8 + j] = face[j];
            }
            return array;
        }
    }
}
