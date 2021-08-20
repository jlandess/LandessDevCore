//
// Created by phoenixflower on 1/3/21.
//
#include <map>
#include "Primitives/General/Immutable.hpp"
#include "Primitives/General/PhoneNumber.hpp"
#include "Graphics/DearImGUI/imgui_impl_datepicker.h"
#include "Async/GrandCentralDispatch.h"
#include "Graphics/DearImGUI/DatePicker.hpp"

#include "Reflection/Reflection.hpp"
namespace LD
{
    class PBXKey
    {
    private:
        LD::ImmutableString<30> mAPIKey;
        LD::ImmutableString<30> mCustomer;
    public:
        const LD::ImmutableString<30> & Key() const noexcept{return this->mAPIKey;}
        LD::ImmutableString<30> & Key()  noexcept{return this->mAPIKey;}

        const LD::ImmutableString<30> & Customer() const noexcept{return this->mAPIKey;}
        LD::ImmutableString<30> & Customer()  noexcept{return this->mAPIKey;}
    };
}



#include "IaaS/HashiVault/HashiVault.hpp"

template<>
struct LD::CT::TypeDescriptor<LD::PBXKey>
{
private:
    static constexpr auto LabelName = LD::ImmutableString{"APIKey"};
    static constexpr auto CusomterName = LD::ImmutableString("Customer");
public:
    static constexpr auto ClassName = ctll::fixed_string{"PBXKey"};

    using MemberList = LD::CT::TypeList<
            LD::CT::EncapsulatedMemberDescriptor<LabelName,LD::CT::SelectOverload<LD::ImmutableString<30> & (PBXKey::*)(),&PBXKey::Key>(),LD::CT::SelectOverload<const LD::ImmutableString<30> & (PBXKey::*)() const,&PBXKey::Key>()>,
            LD::CT::EncapsulatedMemberDescriptor<CusomterName,LD::CT::SelectOverload<LD::ImmutableString<30> & (PBXKey::*)(),&PBXKey::Customer>(),LD::CT::SelectOverload<const LD::ImmutableString<30> & (PBXKey::*)() const,&PBXKey::Customer>()>
    >;


    static constexpr MemberList Members{  };

};
namespace LD
{

    namespace Example
    {
        class Encounters
        {
        private:
            LD::UInteger mNumber;
        public:
            Encounters() noexcept:mNumber{0}
            {

            }
            const LD::UInteger Number() const noexcept
            {
                return this->mNumber;
            }
            Encounters & operator++() noexcept
            {
                this->mNumber++;
                return (*this);
            }
            Encounters & operator++(int) noexcept
            {
                this->mNumber++;
                return (*this);
            }
        };
    }
}
namespace LD
{
    inline auto ToImmutableString(const LD::Example::Encounters & encounter) noexcept
    {
        return LD::ToImmutableString(encounter.Number());
    }
    inline auto ToImmutableString(const std::pair<const LD::PhoneNumber,LD::Example::Encounters> & pair) noexcept
    {
        //std::cout << "being called saucy" << std::endl;
        //LD::CT::IsElementReference()
        auto result = LD::ToImmutableString(pair.first) + LD::ImmutableString{" : " } + LD::ToImmutableString(pair.second);
        return LD::ToImmutableString(pair.first) + LD::ImmutableString{" : " } + LD::ToImmutableString(pair.second);
    }
}

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Graphics/DearImGUI/imgui_impl_glfw.h"
#include "Graphics/DearImGUI/imgui_impl_opengl3.h"
#include "Graphics/DearImGUI/imgui.h"
#include "IO/File.hpp"
#include "Primitives/General/PhoneNumber.hpp"

#include "PBX/Mirta/Mirta.hpp"
#include "VirtuWorks/QueueLog.hpp"
#include "Graphics/DearImGUI/imnodes.h"
# include "Graphics/DearImGUI/NodeEditor/imgui_node_editor.h"
# include "Graphics/DearImGUI/NodeEditor/imgui_node_editor.h"
# define IMGUI_DEFINE_MATH_OPERATORS
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


namespace LD
{
    namespace ed = ax::NodeEditor;
    //static ed::EditorContext* g_Context = nullptr;
    struct LinkInfo
    {
        ed::LinkId Id;
        ed::PinId  InputId;
        ed::PinId  OutputId;
    };
    class NodeEditor
    {
    private:
        //void ax::NodeEditor::DestroyEditor(EditorContext* ctx)
        //std::unique_ptr<ed::EditorContext> * mContext;
        std::unique_ptr<ed::EditorContext,void(*)(ed::EditorContext* ctx)> mContext;
        //static ed::EditorContext*   g_Context = nullptr;    // Editor context, required to trace a editor state.
        bool                 g_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
        ImVector<LinkInfo>   g_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
        int                  g_NextLinkId = 100;

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
    public:
        NodeEditor(ed::Config config):mContext{std::unique_ptr<ed::EditorContext,void(*)(ed::EditorContext* ctx)>{ed::CreateEditor(&config),ed::DestroyEditor}}
        {

        }

        void operator()()  noexcept
        {
            auto& io = ImGui::GetIO();

            ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

            ImGui::Separator();

            ed::SetCurrentEditor(this->mContext.get());

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
    };

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



    //static bool                 g_FirstFrame = true;    // Flag set for first frame only, some action need to be executed once.
    //static ImVector<LinkInfo>   g_Links;                // List of live links. It is dynamic unless you want to create read-only view over nodes.
    //static int                  g_NextLinkId = 100;

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

    namespace Example
    {
        void SaveMissedCalls(LD::Date date, std::map<LD::PhoneNumber,Encounters> & map) noexcept
        {
            map.clear();

            LD::HashiVault vault{
                    LD::ImmutableString{"http://192.168.30.50:8200"},
                    LD::ImmutableString{"s.w8hIFCNwl59CMABCvaZ1qlGV"}};

            auto vaultResponse = vault.Query(LD::Type<LD::PBXKey>{},LD::ImmutableString{"demoapplications"},LD::ImmutableString{"square"},LD::Date{date},map);



            auto onKey = [](const LD::PBXKey & key, const LD::Date & date, std::map<LD::PhoneNumber,Encounters> & map) noexcept
            {
                LD::PBX::Mirta pbxInstance{key.Key(),key.Customer()};

                pbxInstance.MissedCalls(date,[](const LD::VW::QueueLog & log, std::map<LD::PhoneNumber,Encounters> & encounters)
                {


                    //std::cout << "being called" << std::endl;


                    //auto number = log.PhoneNumber().Number();


                    encounters[log.PhoneNumber()]++;


                },map);
            };

            auto onError = [](const LD::TransactionError &, const LD::Date & date, std::map<LD::PhoneNumber,Encounters> & map) noexcept
            {

            };

            LD::InvokeVisitation(LD::Overload{onKey,onError},vaultResponse);

        }
        static void glfw_error_callback1(int error, const char* description)
        {
            std::cerr << "[ERROR] GLFW error: " << error << ", " << description << std::endl;
        }

        void teardown(GLFWwindow *window)
        {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();

            if (window != NULL) { glfwDestroyWindow(window); }
            glfwTerminate();
        }

        void setImGuiStyle(float highDPIscaleFactor)
        {
            ImGuiStyle &style = ImGui::GetStyle();

            // https://github.com/ocornut/imgui/issues/707#issuecomment-415097227
            style.Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
            style.Colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
            style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
            style.Colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
            style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
            style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
            style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
            style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
            style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
            style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
            style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
            style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
            style.Colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
            style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
            style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
            style.Colors[ImGuiCol_Header]                 = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
            style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
            style.Colors[ImGuiCol_HeaderActive]           = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
            style.Colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
            style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
            style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
            style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
            style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
            style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
            style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
            style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
            //style.Colors[ImGuiCol_ModalWindowDarkening]   = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
            style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
            style.Colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
            style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

            style.WindowPadding            = ImVec2(8, 6);
            style.WindowRounding           = 0.0f;
            style.FramePadding             = ImVec2(5, 7);
            //style.FrameRounding            = 0.0f;
            style.ItemSpacing              = ImVec2(5, 5);
            // style.ItemInnerSpacing         = ImVec2(1, 1);
            // style.TouchExtraPadding        = ImVec2(0, 0);
            // style.IndentSpacing            = 6.0f;
            // style.ScrollbarSize            = 12.0f;
            // style.ScrollbarRounding        = 16.0f;
            // style.GrabMinSize              = 20.0f;
            // style.GrabRounding             = 2.0f;
            // style.WindowTitleAlign.x = 0.50f;
            // style.FrameBorderSize = 0.0f;
            // style.WindowBorderSize = 1.0f;

            style.ScaleAllSizes(highDPIscaleFactor);
        }
        static void framebuffer_size_callback(GLFWwindow *window, int width, int height)
        {
            glViewport(0, 0, width, height);
        }
        int IMGUIExample1()
        {
            glfwSetErrorCallback(glfw_error_callback1);

            if (!glfwInit())
            {
                std::cerr << "[ERROR] Couldn't initialize GLFW\n";
                return -1;
            }
            else
            {
                std::cout << "[INFO] GLFW initialized\n";
            }

            // setup GLFW window
            //std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
            glfwWindowHint(GLFW_DOUBLEBUFFER , 1);
            glfwWindowHint(GLFW_DEPTH_BITS, 24);
            glfwWindowHint(GLFW_STENCIL_BITS, 8);

            //glfwWindowHint(
                    //GLFW_OPENGL_PROFILE,
                    //GLFW_OPENGL_CORE_PROFILE
            //);

            std::string glsl_version = "";
#ifdef __APPLE__
            // GL 3.2 + GLSL 150
    glsl_version = "#version 150";
    glfwWindowHint( // required on Mac OS
        GLFW_OPENGL_FORWARD_COMPAT,
        GL_TRUE
        );
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif __linux__
            // GL 3.2 + GLSL 150
            glsl_version = "#version 130";
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#elif _WIN32
            // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
#endif

            float highDPIscaleFactor = 1.0;
#ifdef _WIN32
            // if it's a HighDPI monitor, try to scale everything
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    float xscale, yscale;
    glfwGetMonitorContentScale(monitor, &xscale, &yscale);
    std::cout << "[INFO] Monitor scale: " << xscale << "x" << yscale << std::endl;
    if (xscale > 1 || yscale > 1)
    {
        highDPIscaleFactor = xscale;
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    }
#elif __APPLE__
            // to prevent 1200x800 from becoming 2400x1600
    // and some other weird resizings
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif


            GLFWwindow *window = glfwCreateWindow(
                    500,
                    500,
                    "no yo momma",
                    NULL,
                    NULL
            );
            if (!window)
            {
                std::cerr << "[ERROR] Couldn't create a GLFW window\n";
                teardown(NULL);
                return -1;
            }
            // watch window resizing
            glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
            glfwMakeContextCurrent(window);


            glfwSwapInterval(1);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cerr << "[ERROR] Couldn't initialize GLAD" << std::endl;
                teardown(window);
                return -1;
            }
            else
            {
                std::cout << "[INFO] GLAD initialized\n";
            }

            std::cout << "[INFO] OpenGL renderer: "
                      << glGetString(GL_RENDERER)
                      << std::endl;

            std::cout << "[INFO] OpenGL from glad "
                      << GLVersion.major << "." << GLVersion.minor
                      << std::endl;

            int actualWindowWidth, actualWindowHeight;
            glfwGetWindowSize(window, &actualWindowWidth, &actualWindowHeight);
            std::cout << "[INFO] Window size: " << actualWindowWidth << "x" << actualWindowHeight << std::endl;
            glViewport(0, 0, actualWindowWidth, actualWindowHeight);


            // --- Dear ImGui

            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;

            io.Fonts->AddFontFromFileTTF("verdana.ttf", 18.0f * highDPIscaleFactor, NULL, NULL);

            setImGuiStyle(highDPIscaleFactor);

            // setup platform/renderer bindings
            ImGui_ImplGlfw_InitForOpenGL(window, true);
            ImGui_ImplOpenGL3_Init(glsl_version.c_str());

            bool show_demo_window = false;
            bool show_another_window = false;
            // colors are set in RGBA, but as float
            ImVec4 background = ImVec4(35/255.0f, 35/255.0f, 35/255.0f, 1.00f);

            float backgroundR = 0.1f,
                    backgroundG = 0.3f,
                    backgroundB = 0.2f;

            glClearColor(backgroundR, backgroundG, backgroundB, 1.0f);

            std::map<LD::PhoneNumber,Encounters>  map;
            char dateBuffer[100];
            memset(dateBuffer,0,sizeof(char)*100);

            ed::EditorContext* context = nullptr;
            Application_Initialize(context);
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



            LD::TaskDispatcher dispatcher(8);
            LD::Promise<std::map<LD::PhoneNumber,Encounters>> promise;
            LD::IMGUI::Button fetchedMissedButton{LD::ImmutableString{"Fetch missed calls"},[](
                    LD::ElementReference<std::map<LD::PhoneNumber,Encounters>> map,
                    auto textInput,
                    LD::ElementReference<LD::TaskDispatcher> dispatcher,
                    LD::ElementReference<LD::Promise<std::map<LD::PhoneNumber,Encounters>>> promise)
            {

                auto possibleDate = LD::FromStringView(LD::Type<LD::VW::DateTime>{},LD::StringView{LD::Get(textInput).Text().Data()},LD::Get(map));


                auto onDate = [&](const LD::Context<LD::TransactionResult,LD::VW::DateTime,std::map<LD::PhoneNumber,Encounters>&> & context) noexcept
                {
                    LD::Date currentDate = LD::Get(LD::Get<1>(context)).Date();
                    (*promise) = dispatcher->EnqueueWithPromise([](LD::Date date)
                    {
                        std::map<LD::PhoneNumber,Encounters>  map;
                        //auto fileName = LD::ImmutableString{"/home/phoenixflower/SeaDrive/My Libraries/PBXTests/"}+LD::ToImmutableString(LD::PBX::MT::Date{date});
                        //LD::IO::File outputFile{LD::StringView{fileName.Data()},LD::IO::WriteOnly{}};
                        LD::PBX::Mirta pbxInstance{LD::ImmutableString{"EbcHbsnXAqSrTKBb"},LD::ImmutableString{"CarrieLandess"}};
                        pbxInstance.MissedCalls(date,
                        [](const LD::VW::QueueLog & log, std::map<LD::PhoneNumber,Encounters> & encounters)
                        {

                            //std::cout << "being called" << std::endl;
                            //auto number = log.PhoneNumber().Number();
                            encounters[log.PhoneNumber()]++;


                        },map);

                        return map;

                    },currentDate);
                    //SaveMissedCalls(currentDate,LD::Get(LD::Get<2>(context)));
                };
                auto onError = [](const LD::Context<LD::TransactionError,std::map<LD::PhoneNumber,Encounters>&> &) noexcept
                {
                    std::cout << "error " << std::endl;
                };

                LD::MultiMatch(LD::Overload{onDate,onError},possibleDate);
            },map,dateCollector,dispatcher,promise};
            auto abc123 = *map.begin();

            //LD::CT::DebugTemplate<decltype(abc123)>{};
            LD::UInteger screenWidth = 0;
            LD::UInteger screenHeight;
            int initialWidth = 0, initialHeight = 0;
            glfwGetFramebufferSize(window, &initialWidth, &initialHeight);

            //0.0f, 10.0f
            int major, minor, rev;
            glfwGetVersion(&major,&minor,&rev);


            LD::IMGUI::Button ultimateOrb{LD::ImmutableString{"Buy Ultimate Orb"},[]()
            {
                ImGui::CloseCurrentPopup();
            }};




            auto onDateChosen = [](LD::BasicDate<LD::Day<LD::UInteger>,LD::Month<LD::UInteger>,LD::Year<LD::UInteger>> date)
            {

                std::cout
                << date.Month().NativeRepresentation().Value()
                << "/"
                << date.Day().NativeRepresentation().Value()
                << "/"
                <<  date.Year().NativeRepresentation().Value()
                << "\n";
            };
            LD::IMGUI::BasicDatePicker datePicker{LD::ImmutableString{"Date To Choose"},onDateChosen,LD::Type<LD::BasicDate<LD::Day<LD::UInteger>,LD::Month<LD::UInteger>,LD::Year<LD::UInteger>>>{}};

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
                datePicker,
                fetchedMissedButton,
                LD::IMGUI::Dummy{LD::Detail::tVec2<LD::Float>{0.0f, 15.0f}},
                LD::IMGUI::TreeNode {LD::ImmutableString{"Results"},map}
            };


            bool firstFrame = true;
            ImVector<LinkInfo> glinks;
            int linkID = 100;
            while (!glfwWindowShouldClose(window))
            {
                if (promise.HasCompleted())
                {
                    std::cout << "promised fufilled" << std::endl;
                    map = promise.GetFuture().Get();
                    promise = LD::Promise<std::map<LD::PhoneNumber,Encounters>>{};
                };
                // the frame starts with a clean scene
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
                // start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();
                Application_Frame(context,firstFrame,glinks,linkID);
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
            Application_Finalize(context);
            teardown(window);
            return 0;
        }
    }
}