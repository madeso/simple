#include "engine/xml.h"

#include <cassert>
#include <stdexcept>

#include "fmt/core.h"
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
        if (el == nullptr)
        {
            throw std::runtime_error(fmt::format("unable to find child element {}", name));
        }
        return std::make_shared<Element>(document, el);
    }

    std::string ErrorToString(tinyxml2::XMLError err)
    {
        switch (err)
        {
        case tinyxml2::XML_SUCCESS:
            return "Success";
        case tinyxml2::XML_NO_ATTRIBUTE:
            return "No_attribute";
        case tinyxml2::XML_WRONG_ATTRIBUTE_TYPE:
            return "Wrong_attribute_type";
        case tinyxml2::XML_ERROR_FILE_NOT_FOUND:
            return "Error_file_not_found";
        case tinyxml2::XML_ERROR_FILE_COULD_NOT_BE_OPENED:
            return "Error_file_could_not_be_opened";
        case tinyxml2::XML_ERROR_FILE_READ_ERROR:
            return "Error_file_read_error";
        case tinyxml2::XML_ERROR_PARSING_ELEMENT:
            return "Error_parsing_element";
        case tinyxml2::XML_ERROR_PARSING_ATTRIBUTE:
            return "Error_parsing_attribute";
        case tinyxml2::XML_ERROR_PARSING_TEXT:
            return "Error_parsing_text";
        case tinyxml2::XML_ERROR_PARSING_CDATA:
            return "Error_parsing_cdata";
        case tinyxml2::XML_ERROR_PARSING_COMMENT:
            return "Error_parsing_comment";
        case tinyxml2::XML_ERROR_PARSING_DECLARATION:
            return "Error_parsing_declaration";
        case tinyxml2::XML_ERROR_PARSING_UNKNOWN:
            return "Error_parsing_unknown";
        case tinyxml2::XML_ERROR_EMPTY_DOCUMENT:
            return "Error_empty_document";
        case tinyxml2::XML_ERROR_MISMATCHED_ELEMENT:
            return "Error_mismatched_element";
        case tinyxml2::XML_ERROR_PARSING:
            return "Error_parsing";
        case tinyxml2::XML_CAN_NOT_CONVERT_TEXT:
            return "Can_not_convert_text";
        case tinyxml2::XML_NO_TEXT_NODE:
            return "No_text_node";
        case tinyxml2::XML_ELEMENT_DEPTH_EXCEEDED:
            return "Element_depth_exceeded";
        default:
            return "Unknown error";
        }
    }

    std::shared_ptr<Element> Open(const std::string& file, const std::string& root)
    {
        auto document = std::make_shared<tinyxml2::XMLDocument>();
        const auto err = document->LoadFile(file.c_str());
        if (err != tinyxml2::XML_SUCCESS)
        {
            throw std::runtime_error(fmt::format("unable to load file '{}' ({})", file, ErrorToString(err)));
        }
        auto* el = document->FirstChildElement(root.c_str());

        if (el == nullptr)
        {
            throw std::runtime_error(fmt::format("unable to find root element {}", root));
        }

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
        auto str = GetAttributeStringOrNull(el, name);
        if (!str)
        {
            throw std::runtime_error(fmt::format("unable to find attribute {}", name));
        }
        return *str;
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
        assert(el->element && "element is null");
        auto e = el->GetChild(name);
        return e->element->GetText();
    }

}
