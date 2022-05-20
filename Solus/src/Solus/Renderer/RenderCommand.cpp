#include "supch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Solus {

	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI;

}