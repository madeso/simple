using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    struct Actor
    {
        MeshDef mesh;
        std::map<std::string, Animation> animations = std::map<std::string, Animation>();

        Actor(MeshDef mesh)
        {
            this.mesh = mesh;
        }

        Animation this [std::string name] {
            get
            {
                return animations[name];
            }
        }

        Actor
        add(std::string name, Animation an)
        {
            animations.Add(name, an);
            return this;
        }

        MeshDef Mesh
        {
            get
            {
                return mesh;
            }
        }

        IEnumerable<KeyValuePair<std::string, Animation>> Animations
        {
            get
            {
                for (KeyValuePair<std::string, Animation> kvp : animations)
                {
                    yield return kvp;
                }
            }
        }
    }
}
