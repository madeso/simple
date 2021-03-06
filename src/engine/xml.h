#pragma once

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace tinyxml2
{
    class XMLDocument;
    class XMLElement;
}

namespace simple
{
    namespace xml
    {
        struct Element
        {
            std::shared_ptr<tinyxml2::XMLDocument> document;
            tinyxml2::XMLElement* element;

            Element(std::shared_ptr<tinyxml2::XMLDocument> d, tinyxml2::XMLElement* e);
            ~Element();

            std::shared_ptr<Element> GetChild(const std::string& name);
            std::string GetName();
        };

        std::shared_ptr<Element> Open(const std::string& file, const std::string& root);

        std::optional<std::string> GetAttributeStringOrNull(std::shared_ptr<Element> el, const std::string& name);
        std::string GetAttributeString(std::shared_ptr<Element> el, const std::string& name);
        std::vector<std::shared_ptr<Element>> Elements(std::shared_ptr<Element> el);
        std::vector<std::shared_ptr<Element>> ElementsNamed(std::shared_ptr<Element> el, const std::string& name);
        std::string GetTextOfSubElement(std::shared_ptr<Element> el, const std::string& name);

        template <typename T, typename F>
        T GetAttribute(std::shared_ptr<Element> el, const std::string& name, F&& f, T def)
        {
            auto val = GetAttributeStringOrNull(el, name);
            if (val)
            {
                return f(*val);
            }
            else
            {
                return def;
            }
        }
    }
}
