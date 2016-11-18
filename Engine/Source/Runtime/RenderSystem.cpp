#include "RenderSystem.hpp"

#include "Pipeline.hpp"
#include "Shader.hpp"
#include "Material.hpp"
#include "ModelImporter.hpp"
#include "Gizmos.hpp"
#include "RenderSettings.hpp"
#include "Camera.hpp"
#include "Light.hpp"
#include "Scene.hpp"
#include "Screen.hpp"
#include "Graphics.hpp"

namespace FishEngine
{
    void RenderSystem::Init()
    {
        Pipeline::Init();
        Shader::Init();
        Material::Init();
        Model::Init();
        Gizmos::Init();
        //EditorGUI::Init();
        Scene::Init();
        //#ifdef GLM_FORCE_LEFT_HANDED
        glFrontFace(GL_CW);
        //#endif
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        //glEnable(GL_LINE_SMOOTH);
    }

    void RenderSystem::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        auto camera = Camera::main();
        Pipeline::BindCamera(camera);

        Vector4 lightDir(0, 0, 0, 0);
        Matrix4x4 lightVP;
        auto& lights = Light::lights();
        if (lights.size() > 0)
        {
            auto& l = lights.front();
            Pipeline::BindLight(l);
        }

        Pipeline::UpdatePerFrameUniforms();


        /************************************************************************/
        /* Skybox                                                               */
        /************************************************************************/
        Matrix4x4 model = Matrix4x4::Scale(100);
        Graphics::DrawMesh(Model::builtinMesh(PrimitiveType::Sphere), model, RenderSettings::skybox());

        /************************************************************************/
        /* Shadow                                                               */
        /************************************************************************/
        for (auto& l : lights)
        {
            Scene::RenderShadow(l);
        }

        /************************************************************************/
        /* Scene                                                                */
        /************************************************************************/
        auto v = Camera::main()->viewport();
        const int w = Screen::width();
        const int h = Screen::height();
        glViewport(GLint(v.x*w), GLint(v.y*h), GLsizei(v.z*w), GLsizei(v.w*h));
        Scene::Render();

        //if (m_isWireFrameMode)
        //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        /************************************************************************/
        /* Gizmos                                                               */
        /************************************************************************/
        //glEnable(GL_POLYGON_OFFSET_LINE);
        //glPolygonOffset(-1.0, -1.0f);
        glDepthFunc(GL_LEQUAL);
        //glDisable(GL_DEPTH_TEST);
        //auto go = Selection::selectedGameObjectInHierarchy();
        //if (go != nullptr)
        //    go->OnDrawGizmosSelected();
        Scene::OnDrawGizmos();
        glDepthFunc(GL_LESS);
        //glEnable(GL_DEPTH_TEST);
        //glDisable(GL_POLYGON_OFFSET_LINE);

        Gizmos::setColor(Color::red);
        auto& b = Scene::m_bounds;
        Gizmos::DrawWireCube(b.center(), b.size());
    }

    void RenderSystem::Clean()
    {

    }

} // namespace FishEngine

