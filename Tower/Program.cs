using OpenTK.Graphics.OpenGL;
using OpenTK.Mathematics;
using OpenTK.Windowing;
using OpenTK.Windowing.Desktop;
using System.Drawing;
using System.Reflection;
using Tower;

GameWindowSettings settings = new GameWindowSettings()
{
    RenderFrequency = 60,
    UpdateFrequency = 60
};

NativeWindowSettings nsettings = new NativeWindowSettings()
{
    Title = "Tower",
    Size = new OpenTK.Mathematics.Vector2i(800,600)
};

GameWindow wnd = new GameWindow(settings, nsettings);

Vector3 lightDirection = new Vector3(-1, -1, 1).Normalized();
Shaders shader = Shaders.LoadShader();
float[] tower = MeshLoader.LoadObjToArray("ArrowTower.obj");
Model mdlTower = new Model(tower, new Bitmap("palette.png"), false);
mdlTower.Transformation = Matrix4.CreateScale(10 / 64f);
var grass = new float[]
{
    -1, 0, -1,    0, 1, 0,       0, 0,
    1, 0, -1,   0, 1, 0,       1, 0,
    1, 0, 1,  0, 1, 0,       1, 1,

    -1, 0, -1,   0, 1, 0,       0, 0,
    1, 0, 1,  0, 1, 0,       1, 1,
    -1, 0, 1,   0, 1, 0,       0, 1,
};
var mdlGrass = new Model(grass, new Bitmap("GrassDirt.png"), false);
mdlGrass.Transformation = Matrix4.CreateScale(0.5f);
Matrix4 transform = Matrix4.Identity;

float rotation = 0;
wnd.RenderFrame += (e) =>
{
    rotation += (float)e.Time * MathF.PI / -3;

    wnd.MakeCurrent();
    GL.Enable(EnableCap.DepthTest);
    GL.ClearColor(Color.CadetBlue);
    GL.ClearDepth(1);
    GL.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);

    int w = wnd.ClientRectangle.Size.X;
    int h = wnd.ClientRectangle.Size.Y;
    GL.Viewport(0, 0, w, h);
    var perspective = Matrix4.CreatePerspectiveFieldOfView(MathF.PI / 2, (float)w / h, 0.1f, 10);
    transform =
        Matrix4.CreateRotationY(rotation)
        * Matrix4.CreateTranslation(0, -0.75f, -1.5f);

    GL.UseProgram(shader.ProgramId);
    shader.Set("lightDirection", lightDirection);
    shader.Set("projection", perspective);

    GL.BindTexture(TextureTarget.Texture2D, mdlTower.Texture.TextureId);
    shader.Set("modelview", mdlTower.Transformation * transform);
    shader.Set("mainTex", mdlTower.Texture.TextureId);
    mdlTower.VertexBuffer.Draw();

    GL.BindTexture(TextureTarget.Texture2D, mdlGrass.Texture.TextureId);
    shader.Set("modelview", mdlGrass.Transformation * transform);
    shader.Set("mainTex", mdlGrass.Texture.TextureId);
    mdlGrass.VertexBuffer.Draw();

    wnd.SwapBuffers();
};
wnd.Run();