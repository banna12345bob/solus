#include "supch.h"
#include "FontRenderer.h"

#undef INFINITE
#include <msdf-atlas-gen.h>

namespace Solus {
    Ref<FontRenderer> FontRenderer::Create(const std::string fontPath)
    {
        msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype();
        if (ft)
        {
            msdfgen::FontHandle* font = msdfgen::loadFont(ft, fontPath.c_str());
            if (font)
            {
                msdfgen::Shape shape;
                if (msdfgen::loadGlyph(shape, font, 'A'))
                {
                    shape.normalize();
                    //                      max. angle
                    msdfgen::edgeColoringSimple(shape, 3.0);
                    //           image width, height
                    msdfgen::Bitmap<float, 3> msdf(32, 32);
                    msdfgen::generateMSDF(msdf, shape, 4.0, 1.0, msdfgen::Vector2(4.0, 4.0));
                    msdfgen::savePng(msdf, "output.png");
                }
                else { SU_CORE_ASSERT(false, "Failed to load character") }
                msdfgen::destroyFont(font);
            }
            else { SU_CORE_ASSERT(false, "Failed to load font") }
            msdfgen::deinitializeFreetype(ft);
        }
        else { SU_CORE_ASSERT(false, "Failed to initalise freetype") }
        return 0;
    }

    void FontRenderer::RenderText(const std::string& text, const glm::vec2& position, const float& rotation, const glm::vec2& scale, const glm::vec4& colour)
    {
        /*OrthographicCameraController m_CameraController = OrthographicCameraController(1280.0f / 720.0f);
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = Characters[*c];
            Renderer2D::BeginScene(m_CameraController.GetCamera());
            Renderer2D::DrawQuad(position, rotation, scale, ch.texture);
            Renderer2D::EndScene();
        }*/

    }
}