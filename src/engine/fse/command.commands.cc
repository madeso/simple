#include "command.commands.h"

#include <stdexcept>


#include "command.bindbuffer.h"
#include "command.setvec2uniform.h"
#include "engine/xml.h"
#include "fmt/core.h"

namespace simple::fse::commands
{
    std::shared_ptr<Command> Create(std::shared_ptr<xml::Element> el, std::shared_ptr<Provider> pro)
    {
        std::string name = el->GetName();
        if (name == "bindbuffer")
        {
            return std::make_shared<BindBufferCommand>(el, pro);
        }
        else if (name == "setu2")
        {
            return std::make_shared<SetVec2Uniform>(el, pro);
        }
        else
        {
            throw std::runtime_error(fmt::format("{} is not a valid command", name));
        }
    }
}
