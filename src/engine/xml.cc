#include "engine/xml.h"

#include "tinyxml2/tinyxml2.h"

namespace SimpleEngine::Xml
{
    Element::Element(std::shared_ptr<tinyxml2::XMLDocument> d, tinyxml2::XMLElement* e)
        : document(d)
        , element(e)
    {
    }

    Element::~Element()
    {
    }

    std::shared_ptr<Element> Element::GetChild(const std::string& name)
    {
        auto* el = element->FirstChildElement(name.c_str());
        return std::make_shared<Element>(document, el);
    }

    std::shared_ptr<Element> Open(const std::string& file, const std::string& root)
    {
        auto document = std::make_shared<tinyxml2::XMLDocument>();
        document->LoadFile(root.c_str());
        auto* el = document->FirstChildElement(root.c_str());

        return std::make_shared<Element>(document, el);
    }

    std::optional<std::string> GetAttributeStringOrNull(std::shared_ptr<Element> el, const std::string& name)
    {
        const tinyxml2::XMLAttribute* att = el->element->FindAttribute(name.c_str());
        if (att)
        {
            return att->Value();
        }
        else
        {
            return std::nullopt;
        }
    }

    std::string GetAttributeString(std::shared_ptr<Element> el, const std::string& name)
    {
        return *GetAttributeStringOrNull(el, name);
    }

    std::vector<std::shared_ptr<Element>> ElementsNamed(std::shared_ptr<Element> root, const std::string& name)
    {
        std::vector<std::shared_ptr<Element>> r;
        for (tinyxml2::XMLElement* el = root->element->FirstChildElement(name.c_str()); el; el = el->NextSiblingElement(name.c_str()))
        {
            r.emplace_back(std::make_shared<Element>(root->document, el));
        }
        return r;
    }

    std::string GetTextOfSubElement(std::shared_ptr<Element> el, const std::string& name)
    {
        return el->element->GetText();
    }

}
