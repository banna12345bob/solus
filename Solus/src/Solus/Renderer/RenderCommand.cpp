#include "supch.h"

#include "Solus/Renderer/RenderCommand.h"

namespace Solus {

	Scope<RenderAPI> RenderCommand::s_RenderAPI = RenderAPI::Create();

}