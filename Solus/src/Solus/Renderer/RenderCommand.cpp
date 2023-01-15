#include "supch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Solus {

	Scope<RenderAPI> RenderCommand::s_RenderAPI = CreateScope<OpenGLRenderAPI>();

}