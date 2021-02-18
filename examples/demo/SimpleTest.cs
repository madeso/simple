#region License

#endregion License

using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Runtime.InteropServices;
using SimpleEngine;
using System.Windows.Forms;
using SimpleEngine.fse;

namespace SimpleTest
{
    public sealed class SimpleTest
    {
        public abstract class Key
        {
            public abstract void run(Ckey b, bool s);
            public abstract int Value { get; }
            public static void Run(Ckey b, bool s, params Key[] keys)
            {
                foreach (Key k in keys) k.run(b, s);
            }

            public static vec3 Combine(Key x, Key y, Key z)
            {
                return new vec3(x.Value, y.Value, z.Value);
            }
        }
        public class Hold : Key
        {
            bool status = false;
            Ckey b;
            public Hold(Ckey b)
            {
                this.b = b;
            }
            public override void run(Ckey b, bool s)
            {
                if (this.b.equals(b)) status = s;
            }
            public bool IsDown
            {
                get
                {
                    return status;
                }
            }
            public override int Value
            {
                get
                {
                    return IsDown ? 1 : 0; ;
                }
            }
        }

        public class PlusMinus : Key
        {
            Key plus;
            Key minus;

            public PlusMinus(Key plus, Key minus)
            {
                this.plus = plus;
                this.minus = minus;
            }

            public override void run(Ckey b, bool s)
            {
                plus.run(b, s);
                minus.run(b, s);
            }

            public override int Value
            {
                get
                {
                    return plus.Value - minus.Value;
                }
            }
        }

        [STAThread]
        public static void Main()
        {
            using (RenderFrame frame = RenderFrame.Create("", 640, 480, 32, false))
            {
                Counter timer = new Counter();
                Setup.basicOpenGL();

                FileSystem fs = new FileSystem();
                fs.addDefaultRoots("pretty good", "simple test");
                MediaLoader loader = new MediaLoader(fs);
                //Texture sample = loader.fetch<Texture>("sample.bmp");

                bool running = true;
                World world = World.Load(loader, "level01.lvl");
                //world.add(new MeshInstance(loader.fetch<Mesh>("basicroad.obj")));
                Camera cam = new Camera();
                //Gl.glLoadIdentity();
                float delta = 0;

                Key rightleft = new PlusMinus(new Hold(new Ckey(Keys.D)), new Hold(new Ckey(Keys.A)));
                Key forback = new PlusMinus(new Hold(new Ckey(Keys.W)), new Hold(new Ckey(Keys.S)));
                Key updown = new PlusMinus(new Hold(new Ckey(Keys.Space)), new Hold(new Ckey(Keys.ControlKey)));
                Hold sprint = new Hold(new Ckey(Keys.ShiftKey));

                frame.setCursorPos(frame.Center);

                vec2 mousesmooth = new vec2(0, 0);
                vec3 movement = new vec3(0, 0, 0);

                float sensitivity = 0.1f;
                float mousesmoothing = 6;

                Pipeline pipe = Pipeline.Create("pipeline.xml", loader, frame.Width, frame.Height); 

                while (running)
                {
                    timer.Start();
                    Application.DoEvents();

                    /*fbo.updateTexture(delegate()
                    {
                        world.render(frame.Width, frame.Height, cam);
                    });*/

                    frame.begin(); 
                    pipe.render(new RenderArgs(world, cam, frame.Width, frame.Height));
                    /*Shader.Bind(shader);
                    FullscreenQuad.render(fbo, frame.Width, frame.Height);
                    Shader.Unbind();*/
                    frame.swap();

                    /*frame.begin();
                    world.render(frame.Width, frame.Height, cam);
                    frame.swap();*/

                    System.Drawing.Point p = frame.getCursorPos();
                    System.Drawing.Point center = frame.Center;
                    frame.setCursorPos(center);

                    mousesmooth = vec2.Curve(new vec2(p.X - center.X, p.Y - center.Y), mousesmooth, mousesmoothing);
                    movement = vec3.Curve(Key.Combine(rightleft, updown, forback).Normalized * (3+sprint.Value*3) , movement, 5);

                    // math::Quaternion(math::op::vec3::yAxisPositive, -x) * math::Quaternion(mRotation.getRight(), y) * mRotation;

                    quat final = quat.FpsQuat(cam.rotation, mousesmooth.x*sensitivity, mousesmooth.y*sensitivity);
                    cam.location += cam.rotation.getRUI(movement)*delta;
                    cam.rotate(final);

                    foreach (RenderFrame.Event e in frame.Events)
                    {
                        System.Diagnostics.Debug.WriteLine(e);
                        if (e.button.equals(new Ckey(Keys.Escape))) running = false;
                        else Key.Run(e.button, e.down, rightleft, forback, updown, sprint);
                    }
                    timer.Stop();
                    delta = timer.Duration;
                }
            }
        }   
    }
}