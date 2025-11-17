/* Generated with AI, I have no clue if all these work :D

// Basic Text and Labels
ImGui::Text("Basic text");
ImGui::TextColored(ImVec4(1,0,0,1), "Colored text");
ImGui::TextWrapped("Wrapped text that spans multiple lines");
ImGui::TextDisabled("Disabled/grayed text");
ImGui::LabelText("Label", "Value");
ImGui::BulletText("Bullet point text");

// Buttons
ImGui::Button("Basic Button");
ImGui::SmallButton("Small Button");
ImGui::InvisibleButton("invisible", ImVec2(100, 20));
ImGui::ArrowButton("arrow", ImGuiDir_Left);
ImGui::ImageButton("image_btn", textureID, ImVec2(32, 32));

// Checkboxes and Radio Buttons
ImGui::Checkbox("Checkbox", &boolValue);
ImGui::CheckboxFlags("Flags", &flags, flagValue);
ImGui::RadioButton("Radio A", &radioValue, 0);
ImGui::RadioButton("Radio B", &radioValue, 1);

// Sliders
ImGui::SliderFloat("Float", &floatValue, 0.0f, 1.0f);
ImGui::SliderFloat2("Float2", float2Array, 0.0f, 1.0f);
ImGui::SliderFloat3("Float3", float3Array, 0.0f, 1.0f);
ImGui::SliderFloat4("Float4", float4Array, 0.0f, 1.0f);
ImGui::SliderInt("Int", &intValue, 0, 100);
ImGui::SliderInt2("Int2", int2Array, 0, 100);
ImGui::SliderInt3("Int3", int3Array, 0, 100);
ImGui::SliderInt4("Int4", int4Array, 0, 100);
ImGui::VSliderFloat("VSlider", ImVec2(20, 100), &floatValue, 0.0f, 1.0f);
ImGui::SliderAngle("Angle", &angleValue);

// Drag Controls
ImGui::DragFloat("Drag Float", &floatValue);
ImGui::DragFloat2("Drag Float2", float2Array);
ImGui::DragFloat3("Drag Float3", float3Array);
ImGui::DragFloat4("Drag Float4", float4Array);
ImGui::DragInt("Drag Int", &intValue);
ImGui::DragInt2("Drag Int2", int2Array);
ImGui::DragInt3("Drag Int3", int3Array);
ImGui::DragInt4("Drag Int4", int4Array);
ImGui::DragFloatRange2("Range", &minValue, &maxValue);

// Input Fields
ImGui::InputText("Text Input", buffer, sizeof(buffer));
ImGui::InputTextMultiline("Multiline", buffer, sizeof(buffer));
ImGui::InputTextWithHint("Hint", "Enter text here", buffer, sizeof(buffer));
ImGui::InputFloat("Float Input", &floatValue);
ImGui::InputFloat2("Float2 Input", float2Array);
ImGui::InputFloat3("Float3 Input", float3Array);
ImGui::InputFloat4("Float4 Input", float4Array);
ImGui::InputInt("Int Input", &intValue);
ImGui::InputInt2("Int2 Input", int2Array);
ImGui::InputInt3("Int3 Input", int3Array);
ImGui::InputInt4("Int4 Input", int4Array);
ImGui::InputDouble("Double Input", &doubleValue);

// Color Controls
ImGui::ColorEdit3("Color RGB", colorArray);
ImGui::ColorEdit4("Color RGBA", colorArray);
ImGui::ColorPicker3("Color Picker RGB", colorArray);
ImGui::ColorPicker4("Color Picker RGBA", colorArray);
ImGui::ColorButton("Color Button", ImVec4(1,0,0,1));

// Trees and Collapsing Headers
ImGui::TreeNode("Tree Node");
ImGui::TreeNodeEx("Tree Node Extended", ImGuiTreeNodeFlags_DefaultOpen);
ImGui::CollapsingHeader("Collapsing Header");
ImGui::SetNextItemOpen(true, ImGuiCond_Once);

// Selectables and Lists
ImGui::Selectable("Selectable", &selected);
ImGui::Selectable("Selectable with size", &selected, 0, ImVec2(100, 20));
ImGui::ListBox("ListBox", &currentItem, items, itemCount);
ImGui::ListBoxHeader("ListBox Header");
ImGui::ListBoxFooter();

// Combo Boxes
ImGui::Combo("Combo", &currentItem, items, itemCount);
ImGui::BeginCombo("Begin Combo", currentItemText);
ImGui::EndCombo();

// Menus
ImGui::BeginMenuBar();
ImGui::EndMenuBar();
ImGui::BeginMainMenuBar();
ImGui::EndMainMenuBar();
ImGui::BeginMenu("Menu");
ImGui::EndMenu();
ImGui::MenuItem("Menu Item", "Ctrl+M", &selected);
ImGui::MenuItem("Menu Item", "Ctrl+M", selected, enabled);

// Tooltips
ImGui::BeginTooltip();
ImGui::EndTooltip();
ImGui::SetTooltip("Tooltip text");

// Popups and Modals
ImGui::OpenPopup("PopupID");
ImGui::BeginPopup("PopupID");
ImGui::EndPopup();
ImGui::BeginPopupModal("Modal", &open);
ImGui::EndPopup();
ImGui::BeginPopupContextItem("Context");
ImGui::BeginPopupContextWindow("Window Context");
ImGui::BeginPopupContextVoid("Void Context");
ImGui::CloseCurrentPopup();

// Windows
ImGui::Begin("Window Title", &open);
ImGui::End();
ImGui::BeginChild("Child", ImVec2(200, 100));
ImGui::EndChild();

// Tabs
ImGui::BeginTabBar("TabBar");
ImGui::EndTabBar();
ImGui::BeginTabItem("Tab", &open);
ImGui::EndTabItem();
ImGui::SetTabItemClosed("TabToClose");

// Tables (ImGui 1.80+)
ImGui::BeginTable("Table", 3);
ImGui::EndTable();
ImGui::TableNextRow();
ImGui::TableNextColumn();
ImGui::TableSetupColumn("Column");
ImGui::TableHeadersRow();

// Layout and Spacing
ImGui::Separator();
ImGui::SameLine();
ImGui::NewLine();
ImGui::Spacing();
ImGui::Dummy(ImVec2(100, 50));
ImGui::Indent();
ImGui::Unindent();
ImGui::BeginGroup();
ImGui::EndGroup();

// Scrolling
ImGui::SetScrollX(scrollX);
ImGui::SetScrollY(scrollY);
ImGui::SetScrollHereX();
ImGui::SetScrollHereY();
ImGui::SetScrollFromPosX(posX);
ImGui::SetScrollFromPosY(posY);

// Focus and Activation
ImGui::SetItemDefaultFocus();
ImGui::SetKeyboardFocusHere();
ImGui::IsItemHovered();
ImGui::IsItemActive();
ImGui::IsItemFocused();
ImGui::IsItemClicked();
ImGui::IsItemVisible();
ImGui::IsItemEdited();
ImGui::IsItemActivated();
ImGui::IsItemDeactivated();
ImGui::IsItemDeactivatedAfterEdit();

// Mouse and Keyboard
ImGui::IsMouseClicked(ImGuiMouseButton_Left);
ImGui::IsMouseReleased(ImGuiMouseButton_Left);
ImGui::IsMouseDragging(ImGuiMouseButton_Left);
ImGui::IsKeyPressed(ImGuiKey_Space);
ImGui::IsKeyReleased(ImGuiKey_Space);
ImGui::GetMousePos();
ImGui::GetMouseDragDelta();

// Drawing and Graphics
ImDrawList* drawList = ImGui::GetWindowDrawList();
drawList->AddLine(ImVec2(0,0), ImVec2(100,100), IM_COL32(255,0,0,255));
drawList->AddRect(ImVec2(10,10), ImVec2(50,50), IM_COL32(255,255,255,255));
drawList->AddCircle(ImVec2(50,50), 20, IM_COL32(0,255,0,255));
drawList->AddText(ImVec2(10,10), IM_COL32(255,255,255,255), "Text");
ImGui::GetBackgroundDrawList();
ImGui::GetForegroundDrawList();

// Progress Bars
ImGui::ProgressBar(progress, ImVec2(200, 0));
ImGui::ProgressBar(progress, ImVec2(200, 0), "Progress");

// Plot Functions (if available)
ImGui::PlotLines("Lines", values, valueCount);
ImGui::PlotHistogram("Histogram", values, valueCount);

// Style and Theming
ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1,0,0,1));
ImGui::PopStyleColor();
ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 0.5f);
ImGui::PopStyleVar();
ImGui::PushFont(font);
ImGui::PopFont();
ImGui::PushID(id);
ImGui::PopID();
ImGui::PushItemWidth(width);
ImGui::PopItemWidth();
ImGui::PushTextWrapPos(wrapPos);
ImGui::PopTextWrapPos();

// Miscellaneous Utilities
ImGui::GetIO();
ImGui::GetStyle();
ImGui::GetVersion();
ImGui::GetTime();
ImGui::GetFrameCount();
ImGui::CalcTextSize("Text");
ImGui::CalcItemWidth();
ImGui::GetContentRegionAvail();
ImGui::GetContentRegionMax();
ImGui::GetWindowContentRegionMin();
ImGui::GetWindowContentRegionMax();
ImGui::GetCursorPos();
ImGui::SetCursorPos(ImVec2(x, y));
ImGui::GetCursorScreenPos();
ImGui::SetCursorScreenPos(ImVec2(x, y));
ImGui::AlignTextToFramePadding();

// Clipping
ImGui::PushClipRect(ImVec2(0,0), ImVec2(100,100), true);
ImGui::PopClipRect();

// Internal State Access
ImGui::GetStateStorage();
ImGui::GetID("string_id");
ImGui::GetID(ptr);
ImGui::GetItemRectMin();
ImGui::GetItemRectMax();
ImGui::GetItemRectSize();
*/
