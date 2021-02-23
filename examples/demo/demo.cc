﻿#include "engine/vec3.h"

using namespace SimpleEngine;

namespace SimpleTest
{
    struct SimpleTest
    {
        struct Key
        {
            virtual void run(Ckey b, bool s) = 0;
            virtual int Value() = 0;
            static void Run(Ckey b, bool s, params Key[] keys)
            {
                for (Key k : keys)
                    k.run(b, s);
            }

            static vec3 Combine(Key x, Key y, Key z)
            {
                return vec3(x.Value, y.Value, z.Value);
            }
        };
        struct Hold : Key
        {
            bool status = false;
            Ckey b;
            Hold(Ckey b)
            {
                this.b = b;
            }
            override void run(Ckey b, bool s)
            {
                if (this.b.equals(b))
                    status = s;
            }
            bool IsDown
            {
                get
                {
                    return status;
                }
            }
            override int Value
            {
                get
                {
                    return IsDown ? 1 : 0;
                }
            }
        };

        struct PlusMinus : Key
        {
            Key plus;
            Key minus;

            PlusMinus(Key plus, Key minus)
            {
                this.plus = plus;
                this.minus = minus;
            }

            override void run(Ckey b, bool s)
            {
                plus.run(b, s);
                minus.run(b, s);
            }

            override int Value
            {
                get
                {
                    return plus.Value - minus.Value;
                }
            }
        };

        [STAThread] static void
        Main()
        {
            using(RenderFrame frame = RenderFrame.Create("", 640, 480, 32, false))
            {
                Counter timer = Counter();
                Setup.basicOpenGL();

                FileSystem fs = FileSystem();
                fs.addDefaultRoots("pretty good", "simple test");
                MediaLoader loader = MediaLoader(fs);
                //Texture sample = loader.fetch<Texture>("sample.bmp");

                bool running = true;
                World world = World.Load(loader, "level01.lvl");
                //world.add(MeshInstance(loader.fetch<Mesh>("basicroad.obj")));
                Camera cam = Camera();
                //glLoadIdentity();
                float delta = 0;

                Key rightleft = PlusMinus(Hold(Ckey(Keys.D)), Hold(Ckey(Keys.A)));
                Key forback = PlusMinus(Hold(Ckey(Keys.W)), Hold(Ckey(Keys.S)));
                Key updown = PlusMinus(Hold(Ckey(Keys.Space)), Hold(Ckey(Keys.ControlKey)));
                Hold sprint = Hold(Ckey(Keys.ShiftKey));

                frame.setCursorPos(frame.Center);

                vec2 mousesmooth = vec2(0, 0);
                vec3 movement = vec3(0, 0, 0);

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
                    pipe.render(RenderArgs(world, cam, frame.Width, frame.Height));
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

                    mousesmooth = vec2.Curve(vec2(p.X - center.X, p.Y - center.Y), mousesmooth, mousesmoothing);
                    movement = vec3::Curve()(Key.Combine(rightleft, updown, forback).Normalized * (3 + sprint.Value * 3), movement, 5);

                    // math::Quaternion(math::op::vec3::yAxisPositive, -x) * math::Quaternion(mRotation.getRight(), y) * mRotation;

                    quat final = quat.FpsQuat(cam.rotation, mousesmooth.x * sensitivity, mousesmooth.y * sensitivity);
                    cam.location += cam.rotation.getRUI(movement) * delta;
                    cam.rotate(final);

                    for (RenderFrame.Event e : frame.Events)
                    {
                        System.Diagnostics.Debug.WriteLine(e);
                        if (e.button.equals(Ckey(Keys.Escape)))
                            running = false;
                        else
                            Key.Run(e.button, e.down, rightleft, forback, updown, sprint);
                    }
                    timer.Stop();
                    delta = timer.Duration;
                }
            }
        }
    };
}
