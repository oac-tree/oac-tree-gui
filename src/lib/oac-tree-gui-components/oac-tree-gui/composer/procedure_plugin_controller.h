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
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_COMPOSER_PROCEDURE_PLUGIN_CONTROLLER_H_
#define SEQUENCERGUI_COMPOSER_PROCEDURE_PLUGIN_CONTROLLER_H_

#include <mvvm/signals/event_types.h>

#include <memory>

namespace mvvm
{
class ModelListener;
}

namespace oac_tree_gui
{

/**
 * @brief The ProcedurePluginController class updates plugin preamble of ProcedureItem when it sees
 * change in used instructions and variables.
 */
class ProcedurePluginController
{
public:
  explicit ProcedurePluginController(mvvm::ISessionModel *model);
  ~ProcedurePluginController();

private:
  /**
   * @brief Updates procedure preamble somewhere up in the hierarchy.
   *
   * It is assumed, that the given item belongs to the ProcedureItem hierarchy and is related to
   * insert/remove of instruction or variable. A call to this function will update plugin names on
   * board of procedure preamble.
   */
  void UpdateProcedurePreamble(const mvvm::SessionItem *item);

  /**
   * @brief Checks if given item is related to variable Workspace, or instruction container.
   */
  bool IsRelevantParent(const mvvm::SessionItem *item);

  /**
   * @brief Updates procedure preamble, if necessary, when instruction or variable is inserted into
   * the procedure.
   */
  void OnItemInsertedEvent(const mvvm::ItemInsertedEvent &event);

  /**
   * @brief Updates procedure preamble, if necessary, when instruction or variable is removed from
   * the procedure.
   */
  void OnItemRemovedEvent(const mvvm::ItemRemovedEvent &event);

  std::unique_ptr<mvvm::ModelListener> m_listener;
};

}  // namespace oac_tree_gui

#endif  // SEQUENCERGUI_COMPOSER_PROCEDURE_PLUGIN_CONTROLLER_H_
