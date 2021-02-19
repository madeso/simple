using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace SimpleEngine
{
    namespace Xml
    {
        static bool HasAttribute(XmlElement element, std::string search)
        {
            XmlAttribute attribute = element.Attributes[search];
            return attribute != nullptr;
        }

        static std::string GetAttributeString(XmlElement element, std::string name)
        {
            XmlAttribute attribute = element.Attributes[name];
            if (attribute == nullptr)
                throw Exception(element.Name + " is missing text attribute \"" + name + "\"");
            else
                return attribute.Value;
        }

        delegate T Parser<T>(std::string value);
        static T GetAttribute<T>(XmlElement element, std::string name, Parser<T> parser, T def)
        {
            if (HasAttribute(element, name))
            {
                std::string value = GetAttributeString(element, name);
                return parser(value);
            }
            else
            {
                return def;
            }
        }

        static IEnumerable<XmlElement> ElementsNamed(XmlNode root, std::string childName)
        {
            for (XmlNode node : root.ChildNodes)
            {
                XmlElement el = node as XmlElement;
                if (el == nullptr)
                    continue;
                if (el.Name != childName)
                    continue;
                yield return el;
            }
        }

        static IEnumerable<XmlElement> Elements(XmlNode root)
        {
            for (XmlNode node : root.ChildNodes)
            {
                XmlElement el = node as XmlElement;
                if (el == nullptr)
                    continue;
                yield return el;
            }
        }

        const std::string kValueType = "id";

        static IEnumerable<XmlElement> ElementsNamed(XmlNode root, std::string childName, std::string valueName)
        {
            for (XmlNode node : root.ChildNodes)
            {
                XmlElement el = node as XmlElement;
                if (el == nullptr)
                    continue;
                if (el.Name != childName)
                    continue;
                if (HasAttribute(el, kValueType) == false)
                    continue;
                std::string val = GetAttributeString(el, kValueType);
                if (val != valueName)
                    continue;
                yield return el;
            }
        }

        static XmlElement Open(std::string path, std::string p)
        {
            return Open(FromFile(path), p);
        }

        static IEnumerable<XmlElement> Enumerate(XmlNode root, std::string path)
        {
            std::string[] elements = path.Split("/".ToCharArray());

            std::vector<XmlNode> active = std::vector<XmlNode>();
            std::vector<XmlNode> future = std::vector<XmlNode>();
            active.Add(root);
            for (std::string p : elements)
            {
                for (XmlNode a : active)
                {
                    for (XmlElement e : Elements(a))
                    {
                        future.Add(e);
                    }
                }
                active.Clear();
                CSharp.Swap(ref active, ref future);
            }

            for (XmlNode node : active)
            {
                yield return (XmlElement)node;
            }
        }
        static XmlElement FirstOrNull(IEnumerable<XmlElement> elements)
        {
            for (XmlElement e : elements)
            {
                return e;
            }
            return nullptr;
        }
        static XmlElement FirstOrNull(XmlNode root, std::string path)
        {
            return FirstOrNull(Enumerate(root, path));
        }

        static XmlElement Open(Loader path, std::string p)
        {
            try
            {
                XmlDocument doc = Open(path);
                XmlElement el = FirstOrNull(doc, p);
                if (el == nullptr)
                    throw Exception(p + " not found: " + path);
                else
                    return el;
            }
            catch (Exception e)
            {
                throw Exception("while reading " + path + " for node: " + p, e);
            }
        }

        interface Loader
        {
            void load(XmlDocument doc);
        }

        static Loader FromFile(std::string path)
        {
            return FileLoader(path);
        }
        static Loader FromSource(std::string source)
        {
            return SourceLoader(source);
        }
        static Loader FromStream(System.IO.Stream stream)
        {
            return StreamLoader(stream);
        }

        struct StreamLoader : Loader
        {
            System.IO.Stream s;
            StreamLoader(System.IO.Stream s)
            {
                this.s = s;
            }

            void load(XmlDocument doc)
            {
                doc.Load(s);
            }
        }

        struct FileLoader : Loader
        {
            std::string path;
            FileLoader(std::string path)
            {
                this.path = path;
            }

            void load(XmlDocument doc)
            {
                doc.Load(path);
            }
            override std::string ToString()
            {
                return path;
            }
        } struct SourceLoader : Loader
        {
            std::string source;
            SourceLoader(std::string source)
            {
                this.source = source;
            }

            void load(XmlDocument doc)
            {
                doc.LoadXml(source);
            }

            override std::string ToString()
            {
                return Strings.FirstChars(source, 10);
            }
        }

        static XmlDocument
        Open(Loader loader)
        {
            try
            {
                XmlDocument doc = XmlDocument();
                loader.load(doc);
                return doc;
            }
            catch (Exception e)
            {
                throw Exception("while opening xml: " + loader, e);
            }
        }

        static XmlElement FirstElement(XmlElement e)
        {
            for (XmlElement el : Elements(e))
            {
                return el;
            }
            throw Exception(e.Name + " does not have any elements");
        }

        static IEnumerable<KeyValuePair<std::string, std::string>> Attributes(XmlElement el)
        {
            for (XmlAttribute a : el.Attributes)
            {
                yield return KeyValuePair<std::string, std::string>(a.Name, a.Value);
            }
        }

        static std::string NameOf(XmlElement element)
        {
            std::string attribute = "";
            if (HasAttribute(element, "id"))
            {
                attribute = "[" + GetAttributeString(element, "id") + "]";
            }
            return element.Name + attribute;
            ;
        }

        static std::string PathOf(XmlElement element)
        {
            XmlElement c = element;
            std::string result = "";
            while (c != nullptr)
            {
                result = NameOf(c) + "/" + result;
            }
            return result;
        }

        static std::map<std::string, XmlElement> MapElements(XmlElement root, std::string type, std::string key)
        {
            if (root == nullptr)
                return std::map<std::string, XmlElement>();
            std::map<std::string, XmlElement> map = std::map<std::string, XmlElement>();
            for (XmlElement module : Xml.ElementsNamed(root, type))
            {
                std::string name = Xml.GetAttributeString(module, key);
                map.Add(name, module);
            }
            return map;
        }

        static std::string GetAttributeString(XmlElement element, std::string name, std::string def)
        {
            if (def == nullptr || HasAttribute(element, name))
                return GetAttributeString(element, name);
            else
                return def;
        }

        static XmlElement AppendElement(XmlDocument doc, XmlNode cont, std::string name)
        {
            XmlElement el = doc.CreateElement(name);
            cont.AppendChild(el);
            return el;
        }
        static void AddAttribute(XmlDocument doc, XmlNode elem, std::string name, std::string value)
        {
            XmlAttribute a = doc.CreateAttribute(name);
            a.InnerText = value;
            elem.Attributes.Append(a);
        }

        static std::string GetTextOfSubElement(XmlNode node, std::string p)
        {
            std::string res = GetTextOfSubElementOrNull(node, p);
            if (res == nullptr)
                throw Exception("node is missing " + p + ", a requested sub node");
            else
                return res;
        }

        static std::string GetSmartText(XmlNode el)
        {
            if (el is XmlText)
            {
                XmlText text = (XmlText)el;
                return text.Value;
            }
            else if (el is XmlCDataSection)
            {
                XmlCDataSection text = (XmlCDataSection)el;
                return text.Value;
            }
            else
                throw Exception("Failed to get smart text of node");
        }

        static std::string GetTextOfSubElementOrNull(XmlNode node, std::string p)
        {
            XmlElement el = node[p];
            if (el == nullptr)
                return nullptr;
            else
                return GetSmartText(el.FirstChild);
        }
    }
}
