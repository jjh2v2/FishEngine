#include "HierarchyWidget.hpp"
#include <FishGUI/Draw.hpp>
#include <FishGUI/FishGUI.hpp>
#include <FishGUI/Theme.hpp>
#include <FishGUI/Input.hpp>
#include <FishEngine/Scene.hpp>

//#include <algorithm>

using namespace FishGUI;
using namespace FishEngine;

HierarchyWidget::HierarchyWidget(const char* name) : Super(name)
{
	m_selectionModel.SetSelectionType( SelectionType::Multi );
	m_imContext->ymargin = 0;
	auto go = GameObject::Create();
	go->setName("Scene");
	m_root = go->transform();
	m_unfolded.insert(m_root);
	m_model.SetRootItem(m_root);
}

