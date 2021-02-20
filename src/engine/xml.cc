#include "tinyxml2/tinyxml2.h"

namespace SimpleEngine
{
    namespace Xml
    {
        bool HasAttribute(std::shared_ptr<Xml::Element> element, std::string search)
        {
            XmlAttribute attribute = element.Attributes[search];
            return attribute != nullptr;
        }

        std::string GetAttributeString(std::shared_ptr<Xml::Element> element, std::string name)
        {
            XmlAttribute attribute = element.Attributes[name];
            if (attribute == nullptr)
                throw std::runtime_error(element.Name + " is missing text attribute \"" + name + "\"");
            else
                return attribute.Value;
        }

        delegate T Parser<T>(std::string value);
        T GetAttribute<T>(std::shared_ptr<Xml::Element> element, std::string name, Parser<T> parser, T def)
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

        IEnumerable<std::shared_ptr<Xml::Element>> ElementsNamed(XmlNode root, std::string childName)
        {
            for (XmlNode node : root.ChildNodes)
            {
                std::shared_ptr<Xml::Element> el = node as std::shared_ptr<Xml::Element>;
                if (el == nullptr)
                    continue;
                if (el.Name != childName)
                    continue;
                yield return el;
            }
        }

        IEnumerable<std::shared_ptr<Xml::Element>> Elements(XmlNode root)
        {
            for (XmlNode node : root.ChildNodes)
            {
                std::shared_ptr<Xml::Element> el = node as std::shared_ptr<Xml::Element>;
                if (el == nullptr)
                    continue;
                yield return el;
            }
        }

        const std::string kValueType = "id";

        IEnumerable<std::shared_ptr<Xml::Element>> ElementsNamed(XmlNode root, std::string childName, std::string valueName)
        {
            for (XmlNode node : root.ChildNodes)
            {
                std::shared_ptr<Xml::Element> el = node as std::shared_ptr<Xml::Element>;
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

        std::shared_ptr<Xml::Element> Open(std::string path, std::string p)
        {
            return Open(FromFile(path), p);
        }

        IEnumerable<std::shared_ptr<Xml::Element>> Enumerate(XmlNode root, std::string path)
        {
            std::string[] elements = path.Split("/".ToCharArray());

            std::vector<XmlNode> active = std::vector<XmlNode>();
            std::vector<XmlNode> future = std::vector<XmlNode>();
            active.Add(root);
            for (std::string p : elements)
            {
                for (XmlNode a : active)
                {
                    for (std::shared_ptr<Xml::Element> e : Elements(a))
                    {
                        future.Add(e);
                    }
                }
                active.Clear();
                CSharp.Swap(ref active, ref future);
            }

            for (XmlNode node : active)
            {
                yield return (std::shared_ptr<Xml::Element>)node;
            }
        }
        std::shared_ptr<Xml::Element> FirstOrNull(IEnumerable<std::shared_ptr<Xml::Element>> elements)
        {
            for (std::shared_ptr<Xml::Element> e : elements)
            {
                return e;
            }
            return nullptr;
        }
        std::shared_ptr<Xml::Element> FirstOrNull(XmlNode root, std::string path)
        {
            return FirstOrNull(Enumerate(root, path));
        }

        std::shared_ptr<Xml::Element> Open(Loader path, std::string p)
        {
            try
            {
                XmlDocument doc = Open(path);
                std::shared_ptr<Xml::Element> el = FirstOrNull(doc, p);
                if (el == nullptr)
                    throw std::runtime_error(p + " not found: " + path);
                else
                    return el;
            }
            catch (std::runtime_error e)
            {
                throw std::runtime_error("while reading " + path + " for node: " + p, e);
            }
        }

        interface Loader
        {
            void load(XmlDocument doc);
        }

        Loader FromFile(std::string path)
        {
            return FileLoader(path);
        }
        Loader FromSource(std::string source)
        {
            return SourceLoader(source);
        }
        Loader FromStream(System.IO.Stream stream)
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
            std::string ToString() const
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

            std::string ToString() const
            {
                return Strings.FirstChars(source, 10);
            }
        }

        XmlDocument
        Open(Loader loader)
        {
            try
            {
                XmlDocument doc = XmlDocument();
                loader.load(doc);
                return doc;
            }
            catch (std::runtime_error e)
            {
                throw std::runtime_error("while opening xml: " + loader, e);
            }
        }

        std::shared_ptr<Xml::Element> FirstElement(std::shared_ptr<Xml::Element> e)
        {
            for (std::shared_ptr<Xml::Element> el : Elements(e))
            {
                return el;
            }
            throw std::runtime_error(e.Name + " does not have any elements");
        }

        IEnumerable<KeyValuePair<std::string, std::string>> Attributes(std::shared_ptr<Xml::Element> el)
        {
            for (XmlAttribute a : el.Attributes)
            {
                yield return KeyValuePair<std::string, std::string>(a.Name, a.Value);
            }
        }

        std::string NameOf(std::shared_ptr<Xml::Element> element)
        {
            std::string attribute = "";
            if (HasAttribute(element, "id"))
            {
                attribute = "[" + GetAttributeString(element, "id") + "]";
            }
            return element.Name + attribute;
            ;
        }

        std::string PathOf(std::shared_ptr<Xml::Element> element)
        {
            std::shared_ptr<Xml::Element> c = element;
            std::string result = "";
            while (c != nullptr)
            {
                result = NameOf(c) + "/" + result;
            }
            return result;
        }

        std::map<std::string, std::shared_ptr<Xml::Element>> MapElements(std::shared_ptr<Xml::Element> root, std::string type, std::string key)
        {
            if (root == nullptr)
                return std::map<std::string, std::shared_ptr<Xml::Element>>();
            std::map<std::string, std::shared_ptr<Xml::Element>> map = std::map<std::string, std::shared_ptr<Xml::Element>>();
            for (std::shared_ptr<Xml::Element> module : Xml::ElementsNamed(root, type))
            {
                std::string name = Xml::GetAttributeString(module, key);
                map.Add(name, module);
            }
            return map;
        }

        std::string GetAttributeString(std::shared_ptr<Xml::Element> element, std::string name, std::string def)
        {
            if (def == nullptr || HasAttribute(element, name))
                return GetAttributeString(element, name);
            else
                return def;
        }

        std::shared_ptr<Xml::Element> AppendElement(XmlDocument doc, XmlNode cont, std::string name)
        {
            std::shared_ptr<Xml::Element> el = doc.CreateElement(name);
            cont.AppendChild(el);
            return el;
        }
        void AddAttribute(XmlDocument doc, XmlNode elem, std::string name, std::string value)
        {
            XmlAttribute a = doc.CreateAttribute(name);
            a.InnerText = value;
            elem.Attributes.Append(a);
        }

        std::string GetTextOfSubElement(XmlNode node, std::string p)
        {
            std::string res = GetTextOfSubElementOrNull(node, p);
            if (res == nullptr)
                throw std::runtime_error("node is missing " + p + ", a requested sub node");
            else
                return res;
        }

        std::string GetSmartText(XmlNode el)
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
                throw std::runtime_error("Failed to get smart text of node");
        }

        std::string GetTextOfSubElementOrNull(XmlNode node, std::string p)
        {
            std::shared_ptr<Xml::Element> el = node[p];
            if (el == nullptr)
                return nullptr;
            else
                return GetSmartText(el.FirstChild);
        }
    }
}
