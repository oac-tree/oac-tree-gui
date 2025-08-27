/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2025 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 * SPDX-License-Identifier: MIT
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file LICENSE located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#include "plugin_settings_editor.h"

#include <oac_tree_gui/components/text_edit_controller.h>
#include <oac_tree_gui/domain/i_domain_plugin_service.h>
#include <oac_tree_gui/model/plugin_settings_item.h>
#include <oac_tree_gui/model/sequencer_settings_constants.h>
#include <oac_tree_gui/model/text_edit_item.h>

#include <sup/gui/widgets/detailed_message_box.h>

#include <mvvm/utils/string_utils.h>
#include <mvvm/widgets/widget_utils.h>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QPlainTextEdit>
#include <QVBoxLayout>

namespace oac_tree_gui
{

namespace
{

/**
 * @brief Returns multi-line string representing loaded plugins information.
 */
std::string GetLoadedPluginTextPresentation(
    const std::vector<std::pair<std::string, bool>> &plugin_info)
{
  std::vector<std::string> loaded_plugin_lines;
  for (const auto &info : plugin_info)
  {
    const std::string load_result = info.second ? "[Loaded]" : "[Failed]";
    loaded_plugin_lines.push_back(load_result + "  " + info.first);
  }
  return mvvm::utils::VectorToString(loaded_plugin_lines, "\n");
}

QString GetPluginDirListDescription()
{
  const QString result{R"RAW(# Specify plugin directories here, one per line.
# Directories are searched in the order they are listed.
# Lines starting with '#' are treated as comments and ignored.

/home/user/my_plugins
/usr/lib/oac-tree/plugins
)RAW"};
  return result;
}

QString GetPluginListDescription()
{
  const QString result{R"RAW(# Specify plugin filenames here, one per line.
# Lines starting with '#' are treated as comments and ignored.
# Suffix/preffix lib and .so will be added automatically if missing.
# If the file name contains a path, the name will be used as it is.

liboac_tree_example_plugin.so
oac_tree_another_plugin
/opt/sofware/lib/oac-tree/plugins/liboac_tree_another_plugin.so
)RAW"};
  return result;
}

}  // namespace

PluginSettingsEditor::PluginSettingsEditor(IDomainPluginService &plugin_service,
                                           QWidget *parent_widget)
    : sup::gui::SessionItemWidget(parent_widget)
    , m_plugin_service(plugin_service)
    , m_description(new QLabel)
    , m_dir_list_checkbox(new QCheckBox)
    , m_dir_list_edit(new QPlainTextEdit)
    , m_plugin_list_checkbox(new QCheckBox)
    , m_plugin_list_edit(new QPlainTextEdit)
    , m_dir_list_controller(std::make_unique<TextEditController>(
          TextControllerContext{m_dir_list_checkbox, m_dir_list_edit}))
    , m_plugin_list_controller(std::make_unique<TextEditController>(
          TextControllerContext{m_plugin_list_checkbox, m_plugin_list_edit}))
{
  auto layout = new QVBoxLayout(this);
  layout->setContentsMargins(mvvm::utils::UnitSize(0.5), 0, 0, 0);
  layout->setSpacing(mvvm::utils::UnitSize(1.75));
  layout->addLayout(CreateDescriptionLayout().release());
  layout->addLayout(CreateDirListLayout().release());
  layout->addLayout(CreatePluginListLayout().release());
  layout->addStretch(1);
}

PluginSettingsEditor::~PluginSettingsEditor() = default;

void PluginSettingsEditor::SetItem(mvvm::SessionItem *item)
{
  SetPluginSettingsItem(dynamic_cast<PluginSettingsItem *>(item));
}

void PluginSettingsEditor::SetPluginSettingsItem(PluginSettingsItem *item)
{
  if (!item)
  {
    return;
  }

  m_dir_list_controller->SetItem(item->GetItem<TextEditItem>(constants::kPluginDirListProperty));
  m_plugin_list_controller->SetItem(item->GetItem<TextEditItem>(constants::kPluginListProperty));
}

std::unique_ptr<QLayout> PluginSettingsEditor::CreateDescriptionLayout()
{
  auto result = std::make_unique<QVBoxLayout>();
  result->setSpacing(mvvm::utils::UnitSize(1.0));

  auto label = new QLabel("Plugin Settings");
  QFont font = label->font();
  font.setBold(true);
  label->setFont(font);
  result->addWidget(label);

  const QString label_text =
      QString(
          "Define custom plugin directories and/or plugin filenames to be used by "
          "the oac-tree application. Check currently loaded plugins %1.")
          .arg(mvvm::utils::ClickableText("here"));

  m_description->setWordWrap(true);
  m_description->setText(label_text);
  connect(m_description, &QLabel::linkActivated, this,
          [this](auto) { SummonLoadedPluginDialog(); });

  result->addWidget(m_description);

  return result;
}

std::unique_ptr<QLayout> PluginSettingsEditor::CreateDirListLayout()
{
  auto result = std::make_unique<QVBoxLayout>();
  result->setSpacing(mvvm::utils::UnitSize(0.5));

  auto h_layout = std::make_unique<QHBoxLayout>();
  h_layout->addWidget(m_dir_list_checkbox);
  h_layout->addWidget(new QLabel("Use custom plugin directories to search for plugins"));
  h_layout->addStretch(1);

  result->addLayout(h_layout.release());
  result->addWidget(m_dir_list_edit);

  m_dir_list_edit->setPlaceholderText(GetPluginDirListDescription());
  m_dir_list_edit->setToolTip(GetPluginDirListDescription());
  m_dir_list_edit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);

  return result;
}

std::unique_ptr<QLayout> PluginSettingsEditor::CreatePluginListLayout()
{
  auto result = std::make_unique<QVBoxLayout>();
  result->setSpacing(mvvm::utils::UnitSize(0.5));

  auto h_layout = std::make_unique<QHBoxLayout>();
  h_layout->addWidget(m_plugin_list_checkbox);

  const QString label_text = "Load custom plugin using filenames";
  h_layout->addWidget(new QLabel(label_text));

  h_layout->addStretch(1);

  result->addLayout(h_layout.release());
  result->addWidget(m_plugin_list_edit);

  m_plugin_list_edit->setPlaceholderText(GetPluginListDescription());
  m_plugin_list_edit->setToolTip(GetPluginListDescription());
  m_plugin_list_edit->setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);

  return result;
}

void PluginSettingsEditor::SummonLoadedPluginDialog()
{
  sup::gui::MessageEvent message;
  message.title = "Loaded Plugins";
  message.text = "Following plugins have been loaded on the application startup:";
  message.detailed = GetLoadedPluginTextPresentation(m_plugin_service.GetPluginLoadInfo());

  sup::gui::DetailedMessageBox box("PluginLoadMessageBox", message, this);
  auto font = box.GetTextEdit()->font();
  font.setFamily("monospace");
  box.GetTextEdit()->setFont(font);
  box.exec();
}

}  // namespace oac_tree_gui
