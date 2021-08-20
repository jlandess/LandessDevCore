//
// Created by phoenixflower on 7/31/21.
//
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Graphics/DearImGUI/imgui_impl_glfw.h"
#include "Graphics/DearImGUI/imgui_impl_opengl3.h"
#include "Graphics/DearImGUI/imgui.h"
#include <map>
#include "Primitives/General/Immutable.hpp"
#include "Graphics/DearImGUI/imgui_impl_glfw.h"
#include "Graphics/DearImGUI/imgui_impl_opengl3.h"
#include "Graphics/DearImGUI/imgui.h"
#include "IO/File.hpp"
#include "Primitives/General/PhoneNumber.hpp"

#include "PBX/Mirta/Mirta.hpp"
#include "VirtuWorks/QueueLog.hpp"
#include "Graphics/DearImGUI/imnodes.h"
#include "Graphics/DearImGUI/NodeEditor/imgui_node_editor.h"
#include "Graphics/DearImGUI/NodeEditor/imgui_node_editor.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Graphics/DearImGUI/imgui_internal.h"
#include "Graphics/DearImGUI/Button.hpp"
#include "Graphics/DearImGUI/TextInput.hpp"
#include "Graphics/DearImGUI/TextLabel.hpp"
#include "Graphics/DearImGUI/TextColored.hpp"
#include "Graphics/DearImGUI/TreeNode.hpp"
#include "Graphics/DearImGUI/Window.hpp"
#include "Graphics/DearImGUI/Dummy.hpp"
#include "Graphics/DearImGUI/SameLine.hpp"
#include "Graphics/DearImGUI/PopupModal.hpp"
namespace ed = ax::NodeEditor;
struct LinkInfo
{
    ed::LinkId Id;
    ed::PinId  InputId;
    ed::PinId  OutputId;
};
void teardown(GLFWwindow *window)
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (window != NULL) { glfwDestroyWindow(window); }
    glfwTerminate();
}
namespace PVEGUIExample
{

    //static ed::EditorContext*   g_Context = nullptr;    // Editor context, required to trace a editor state.
    void Application_Initialize(ed::EditorContext*& g_Context)
    {
        ed::Config config;
        config.SettingsFile = "Simple.json";
        g_Context = ed::CreateEditor(&config);
    }

    void Application_Finalize(ed::EditorContext*& g_Context)
    {
        ed::DestroyEditor(g_Context);
    }


    void ImGuiEx_BeginColumn()
    {
        ImGui::BeginGroup();
    }

    void ImGuiEx_NextColumn()
    {
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::BeginGroup();
    }

    void ImGuiEx_EndColumn()
    {
        ImGui::EndGroup();
    }
    void Application_Frame(ed::EditorContext*& g_Context, bool & g_FirstFrame,ImVector<LinkInfo> & g_Links,int & g_NextLinkId)
    {
        auto& io = ImGui::GetIO();

        ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

        ImGui::Separator();

        ed::SetCurrentEditor(g_Context);

        // Start interaction with editor.
        ed::Begin("My Editor", ImVec2(0.0, 0.0f));

        int uniqueId = 1;

        //
        // 1) Commit known data to editor
        //

        // Submit Node A
        ed::NodeId nodeA_Id = uniqueId++;
        ed::PinId  nodeA_InputPinId = uniqueId++;
        ed::PinId  nodeA_OutputPinId = uniqueId++;

        if (g_FirstFrame)
            ed::SetNodePosition(nodeA_Id, ImVec2(10, 10));
        ed::BeginNode(nodeA_Id);
        ImGui::Text("Node A");
        ed::BeginPin(nodeA_InputPinId, ed::PinKind::Input);
        ImGui::Text("-> In");
        ed::EndPin();
        ImGui::SameLine();
        ed::BeginPin(nodeA_OutputPinId, ed::PinKind::Output);
        ImGui::Text("Out ->");
        ed::EndPin();
        ed::EndNode();

        // Submit Node B
        ed::NodeId nodeB_Id = uniqueId++;
        ed::PinId  nodeB_InputPinId1 = uniqueId++;
        ed::PinId  nodeB_InputPinId2 = uniqueId++;
        ed::PinId  nodeB_OutputPinId = uniqueId++;

        if (g_FirstFrame)
            ed::SetNodePosition(nodeB_Id, ImVec2(210, 60));
        ed::BeginNode(nodeB_Id);
        ImGui::Text("Node B");
        ImGuiEx_BeginColumn();
        ed::BeginPin(nodeB_InputPinId1, ed::PinKind::Input);
        ImGui::Text("-> In1");
        ed::EndPin();
        ed::BeginPin(nodeB_InputPinId2, ed::PinKind::Input);
        ImGui::Text("-> In2");
        ed::EndPin();
        ImGuiEx_NextColumn();
        ed::BeginPin(nodeB_OutputPinId, ed::PinKind::Output);
        ImGui::Text("Out ->");
        ed::EndPin();
        ImGuiEx_EndColumn();
        ed::EndNode();


        ed::NodeId nodeC_Id = uniqueId++;
        ed::PinId  nodeC_InputPinId1 = uniqueId++;
        ed::PinId  nodeC_InputPinId2 = uniqueId++;
        ed::PinId  nodeC_OutputPinId = uniqueId++;
        ed::BeginNode(nodeC_Id);
        ImGui::Text("Node C");
        ImGuiEx_BeginColumn();
        ed::BeginPin(nodeC_InputPinId1, ed::PinKind::Input);
        ImGui::Text("-> In1");
        ed::EndPin();
        ed::BeginPin(nodeC_InputPinId2, ed::PinKind::Input);
        ImGui::Text("-> In2");
        ed::EndPin();
        ImGuiEx_NextColumn();
        ed::BeginPin(nodeC_OutputPinId, ed::PinKind::Output);
        ImGui::Text("Out ->");
        ed::EndPin();
        ImGuiEx_EndColumn();
        ed::EndNode();

        // Submit Links
        for (auto& linkInfo : g_Links)
            ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

        //
        // 2) Handle interactions
        //

        // Handle creation action, returns true if editor want to create new object (node or link)
        if (ed::BeginCreate())
        {
            ed::PinId inputPinId, outputPinId;
            if (ed::QueryNewLink(&inputPinId, &outputPinId))
            {
                // QueryNewLink returns true if editor want to create new link between pins.
                //
                // Link can be created only for two valid pins, it is up to you to
                // validate if connection make sense. Editor is happy to make any.
                //
                // Link always goes from input to output. User may choose to drag
                // link from output pin or input pin. This determine which pin ids
                // are valid and which are not:
                //   * input valid, output invalid - user started to drag new ling from input pin
                //   * input invalid, output valid - user started to drag new ling from output pin
                //   * input valid, output valid   - user dragged link over other pin, can be validated

                if (inputPinId && outputPinId) // both are valid, let's accept link
                {
                    // ed::AcceptNewItem() return true when user release mouse button.
                    if (ed::AcceptNewItem())
                    {
                        // Since we accepted new link, lets add one to our list of links.
                        g_Links.push_back({ ed::LinkId(g_NextLinkId++), inputPinId, outputPinId });

                        // Draw new link.
                        ed::Link(g_Links.back().Id, g_Links.back().InputId, g_Links.back().OutputId);
                    }

                    // You may choose to reject connection between these nodes
                    // by calling ed::RejectNewItem(). This will allow editor to give
                    // visual feedback by changing link thickness and color.
                }
            }
        }
        ed::EndCreate(); // Wraps up object creation action handling.


        // Handle deletion action
        if (ed::BeginDelete())
        {
            // There may be many links marked for deletion, let's loop over them.
            ed::LinkId deletedLinkId;
            while (ed::QueryDeletedLink(&deletedLinkId))
            {
                // If you agree that link can be deleted, accept deletion.
                if (ed::AcceptDeletedItem())
                {
                    // Then remove link from your data.
                    for (auto& link : g_Links)
                    {
                        if (link.Id == deletedLinkId)
                        {
                            g_Links.erase(&link);
                            break;
                        }
                    }
                }

                // You may reject link deletion by calling:
                // ed::RejectDeletedItem();
            }
        }
        ed::EndDelete(); // Wrap up deletion action



        // End of interaction with editor.
        ed::End();

        if (g_FirstFrame)
            ed::NavigateToContent(0.0f);

        ed::SetCurrentEditor(nullptr);

        g_FirstFrame = false;

        // ImGui::ShowMetricsWindow();
    }
}
namespace LD
{
    namespace Example
    {
        static void glfw_error_callback2(int error, const char* description)
        {
            fprintf(stderr, "Glfw Error %d: %s\n", error, description);
        }
        int PVEGUIDeploymentExample()
        {
            // Setup window
            glfwSetErrorCallback(glfw_error_callback2);
            if (!glfwInit())
                return 1;

            // Decide GL+GLSL versions
#ifdef __APPLE__
            // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
            // GL 3.0 + GLSL 130
            const char* glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
            //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
            //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

            // Create window with graphics context
            GLFWwindow* window = glfwCreateWindow(500, 500, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
            if (window == NULL)
                return 1;
            glfwMakeContextCurrent(window);
            glfwSwapInterval(1); // Enable vsync

            // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
            bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
            bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
            bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
            bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
            bool err = false;
    glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
            bool err = false;
    glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
            bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
            if (err)
            {
                fprintf(stderr, "Failed to initialize OpenGL loader!\n");
                return 1;
            }

            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            // Setup Dear ImGui style
            ImGui::StyleColorsDark();
            //ImGui::StyleColorsClassic();

            // Setup Platform/Renderer backends
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version);


            bool show_demo_window = false;
            bool show_another_window = false;
            // colors are set in RGBA, but as float
            ImVec4 background = ImVec4(35/255.0f, 35/255.0f, 35/255.0f, 1.00f);

            float backgroundR = 0.1f,
                    backgroundG = 0.3f,
                    backgroundB = 0.2f;

            glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);

            //std::map<LD::PhoneNumber,Encounters>  map;
            char dateBuffer[100];
            memset(dateBuffer,0,sizeof(char)*100);


            ed::EditorContext* context = nullptr;
            PVEGUIExample::Application_Initialize(context);
            int abc;
            int counter = 0;
            LD::IMGUI::Button counterButton{LD::ImmutableString{"Counter Button"},[](LD::ElementReference<int> integer)
            {
                int & counter = LD::Get(integer);
                std::cout << "counter button clicked\n";
                counter++;
                if (counter == 9) { ImGui::OpenPopup("Easter egg"); }
            },counter};


            LD::IMGUI::TextInput dateCollector{LD::ImmutableString{"Date"},LD::ImmutableString<20>{}};



            std::map<int,int> map;
            int initialWidth = 0, initialHeight = 0;
            LD::UInteger screenWidth = 0;
            LD::UInteger screenHeight;
            glfwGetFramebufferSize(window, &initialWidth, &initialHeight);

            int major, minor, rev;
            glfwGetVersion(&major,&minor,&rev);
            LD::IMGUI::Window window1{
                    LD::Detail::tVec2<LD::UInteger>{10,10},LD::Detail::tVec2<LD::UInteger>{initialWidth,initialHeight},LD::ImmutableString{"Controls"},
                    LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f,1.0f}},
                    LD::IMGUI::TextColored {LD::ImmutableString{"Time"},LD::Detail::tVec4<LD::Float>{1.0f, 0.0f, 1.0f, 1.0f}},
                    LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f,3.0f}},
                    LD::IMGUI::TextColored {LD::ImmutableString{"Application"},LD::Detail::tVec4<LD::Float>{1.0,0.0,1.0,1.0}},
                    LD::IMGUI::TextLabel{LD::ImmutableString{"Main Window Width: "},screenWidth},
                    LD::IMGUI::TextLabel{LD::ImmutableString{"Main Window Height: "},screenHeight},
                    LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f, 3.0f}},
                    LD::IMGUI::TextColored {LD::ImmutableString{"GLFW"},LD::Detail::tVec4<LD::Float>{1.0f, 0.0f, 1.0f, 1.0f}},
                    LD::IMGUI::TextLabel{LD::ImmutableString{"GLFW Version: "},LD::UInteger(major),LD::ImmutableString{"."},LD::UInteger(minor),LD::ImmutableString{"."},LD::UInteger(minor)},
                    LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f, 10.0f}},
                    LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f, 10.0f}},
                    LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f, 10.0f}},
                    counterButton,
                    LD::IMGUI::SameLine{},
                    LD::IMGUI::TextLabel{LD::ImmutableString{"counter = "},counter},
                    LD::IMGUI::PopupModal{LD::ImmutableString{"Easter egg"}},
                    dateCollector,
                    LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f, 15.0f}},
                    LD::IMGUI::TreeNode {LD::ImmutableString{"Results"},map}
            };

            bool firstFrame = true;
            ImVector<LinkInfo> glinks;
            int linkID = 100;

            while (!glfwWindowShouldClose(window))
            {
                /*
                if (promise.HasCompleted())
                {
                    std::cout << "promised fufilled" << std::endl;
                    map = promise.GetFuture().Get();
                    promise = LD::Promise<std::map<LD::PhoneNumber,Encounters>>{};
                };
                 */
                // the frame starts with a clean scene
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                // start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                PVEGUIExample::Application_Frame(context,firstFrame,glinks,linkID);
                int glfw_width = 0, glfw_height = 0, controls_width = 0;
                // get the window size as a base for calculating widgets geometry
                glfwGetFramebufferSize(window, &glfw_width, &glfw_height);

                LD::IMGUI::RenderingContext context{LD::UInteger(glfw_width),LD::UInteger (glfw_height)};
                screenWidth = context.Width();
                screenHeight = context.Height();
                window1(context);
                ImGui::Render();
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                glfwSwapBuffers(window);
                glfwPollEvents();
            }
            PVEGUIExample::Application_Finalize(context);
            teardown(window);
            return 0;

        }
    }
}