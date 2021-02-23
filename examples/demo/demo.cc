#include <memory>
#include <vector>

#include "engine/ckey.h"
#include "engine/filesystem.h"
#include "engine/medialoader.h"
#include "engine/quat.h"
#include "engine/vec2.h"
#include "engine/vec3.h"

using namespace SimpleEngine;

namespace SimpleTest
{
    struct Key
    {
        virtual void run(Ckey b, bool s) = 0;
        virtual int Value() = 0;

        static void Run(Ckey b, bool s, const std::vector<std::shared_ptr<Key>>& keys)
        {
            for (auto k : keys)
                k->run(b, s);
        }

        static vec3 Combine(std::shared_ptr<Key> x, std::shared_ptr<Key> y, std::shared_ptr<Key> z)
        {
            return vec3(x->Value(), y->Value(), z->Value());
        }
    };

    struct Hold : public Key
    {
        bool status = false;
        Ckey b;

        Hold(Ckey b)
        {
            this.b = b;
        }
        void run(Ckey b, bool s) override
        {
            if (this.b.equals(b))
                status = s;
        }
        bool IsDown()
        {
            return status;
        }
        int Value() override
        {
            return IsDown() ? 1 : 0;
        }
    };

    struct PlusMinus : public Key
    {
        std::shared_ptr<Key> plus;
        std::shared_ptr<Key> minus;

        PlusMinus(std::shared_ptr<Key> p, std::shared_ptr<Key> m)
        {
            plus = p;
            minus = m;
        }

        void run(Ckey b, bool s) override
        {
            plus->run(b, s);
            minus->run(b, s);
        }

        int Value() override
        {
            return plus->Value() - minus->Value();
        }
    };

    struct Demo
    {
        Camera cam = Camera();

        bool running = true;

        vec2 mousesmooth = vec2(0, 0);
        vec3 movement = vec3(0, 0, 0);

        float sensitivity = 0.1f;
        float mousesmoothing = 6;

        std::shared_ptr<Key> rightleft = std::make_shared<PlusMinus>(std::make_shared<Hold>(Ckey(Keys.D)), std::make_shared<Hold>(Ckey(Keys.A)));
        std::shared_ptr<Key> forback = std::make_shared<PlusMinus>(std::make_shared<Hold>(Ckey(Keys.W)), std::make_shared<Hold>(Ckey(Keys.S)));
        std::shared_ptr<Key> updown = std::make_shared<PlusMinus>(std::make_shared<Hold>(Ckey(Keys.Space)), std::make_shared<Hold>(Ckey(Keys.ControlKey)));
        std::shared_ptr<Hold> sprint = std::make_shared<Hold>(std::make_shared<Ckey>(Keys.ShiftKey));

        std::shared_ptr<World> world;
        std::shared_ptr<Pipeline> pipe;

        void Main()
        {
            Setup::basicOpenGL();

            FileSystem fs = FileSystem();
            fs.addDefaultRoots("pretty good", "simple test");
            MediaLoader loader = MediaLoader(fs);
            //Texture sample = loader.fetch<Texture>("sample.bmp");

            world = World::Load(loader, "level01.lvl");
            //world.add(MeshInstance(loader.fetch<Mesh>("basicroad.obj")));

            pipe = Pipeline::Create("pipeline.xml", loader, frame.Width, frame.Height);
        }

        void Render(float delta)
        {
            /*fbo.updateTexture(delegate()
                    {
                        world.render(frame.Width, frame.Height, cam);
                    });*/
            pipe->render(RenderArgs(world, cam, frame.Width, frame.Height));
            /*Shader.Bind(shader);
                    FullscreenQuad.render(fbo, frame.Width, frame.Height);
                    Shader.Unbind();*/
        }

        void Run(float delta, const vec2& mouse_movement)
        {
            /*frame.begin();
                    world.render(frame.Width, frame.Height, cam);
                    frame.swap();*/
            mousesmooth = vec2::Curve(mouse_movement, mousesmooth, mousesmoothing);
            movement = vec3::Curve(Key::Combine(rightleft, updown, forback).Normalized() * (3 + sprint.Value() * 3), movement, 5);

            // math::Quaternion(math::op::vec3::yAxisPositive, -x) * math::Quaternion(mRotation.getRight(), y) * mRotation;

            quat final = quat::FpsQuat(cam.rotation, mousesmooth.x * sensitivity, mousesmooth.y * sensitivity);
            cam.location += cam.rotation.getRUI(movement) * delta;
            cam.rotate(final);
        }

        void OnKey()
        {
            System.Diagnostics.Debug.WriteLine(e);
            if (e.button.equals(Ckey(Keys.Escape)))
                running = false;
            else
                Key::Run(e.button, e.down, rightleft, forback, updown, sprint);
        }
    };
}
