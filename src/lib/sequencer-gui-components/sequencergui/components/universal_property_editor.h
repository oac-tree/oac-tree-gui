/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2024 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPONENTS_UNIVERSAL_PROPERTY_EDITOR_H_
#define SEQUENCERGUI_COMPONENTS_UNIVERSAL_PROPERTY_EDITOR_H_

#include <QWidget>

class QTreeView;

namespace mvvm
{
class SessionItem;
class ItemViewComponentProvider;
}  // namespace mvvm

namespace sup::gui
{
class CustomHeaderView;
}

namespace sequencergui
{

/**
 * @brief The UniversalPropertyEditor provide two column tree for editing properties of universal
 * variables and instructions.
 *
 * @details For the moment it's just a normal property editor. It will be further extended tiwth
 * additional tool buttons and context menu.
 */

class UniversalPropertyEditor : public QWidget
{
  Q_OBJECT

public:
  explicit UniversalPropertyEditor(QWidget* parent = nullptr);
  ~UniversalPropertyEditor() override;

  void SetItem(mvvm::SessionItem* item);

private:
  void ReadSettings();
  void WriteSettings();
  void AdjustTreeAppearance();

  QTreeView* m_tree_view{nullptr};
  sup::gui::CustomHeaderView* m_custom_header{nullptr};
  std::unique_ptr<mvvm::ItemViewComponentProvider> m_component_provider;
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_COMPONENTS_UNIVERSAL_PROPERTY_EDITOR_H_
