using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleEngine
{
    public class Actor
    {
        MeshDef mesh;
        Dictionary<string, Animation> animations = new Dictionary<string, Animation>();

        public Actor(MeshDef mesh)
        {
            this.mesh = mesh;
        }

        public Animation this[string name]
        {
            get
            {
                return animations[name];
            }
        }

        public Actor add(string name, Animation an)
        {
            animations.Add(name, an);
            return this;
        }

        public MeshDef Mesh
        {
            get
            {
                return mesh;
            }
        }

        public IEnumerable<KeyValuePair<string, Animation>> Animations
        {
            get
            {
                foreach (KeyValuePair<string, Animation> kvp in animations)
                {
                    yield return kvp;
                }
            }
        }
    }
}
