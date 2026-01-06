/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP oac-tree
 *
 * Description   : Integrated development environment for oac-tree procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2026 ITER Organization,
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

#ifndef OAC_TREE_GUI_DOMAIN_DOMAIN_OBJECT_TYPE_REGISTRY_H_
#define OAC_TREE_GUI_DOMAIN_DOMAIN_OBJECT_TYPE_REGISTRY_H_

#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

namespace oac_tree_gui
{

/**
 * @brief The DomainObjectTypeRegistry class holds a collection of object names associated with
 * plugin name.
 *
 * It can be used in the following scenario. We load sequencer plugins one after another, and call
 * an Update method for all instruction names registered in InstructionRegistry. After the last
 * plugin is loaded, we will get full info from which plugin the given instruction type came from.
 */
class DomainObjectTypeRegistry
{
public:
  using get_object_names_t = std::function<std::vector<std::string>()>;

  /**
   * @brief Default c-tor.
   */
  DomainObjectTypeRegistry() = default;

  /**
   * @brief Consrtucts the object and initializes it with the given function.
   *
   * This function will be called to retrieve the list of object names when Update method is called.
   *
   * @param func A callback used to retrive list of object names.
   */
  explicit DomainObjectTypeRegistry(get_object_names_t func);

  /**
   * @brief Updates plugin name information using a callback provided via c-tor.
   * @param plugin_name The name of the last loaded sequencer plugin.
   */
  void Update(const std::string& plugin_name);

  /**
   * @brief Updates plugin name information for given object names.
   *
   * If object names are new to this DomainObjectTypeRegistry, their names will be associated with
   * the given plugin name. If object names have been already registered for another plugin name, it
   * will remain like that.
   *
   * The name of the plugin can be empty, which means initial state of the program, before any load.
   *
   * @param plugin_name The name of the last loaded sequencer plugin.
   * @param object_names Collection of names registered so far in object factory.
   */
  void Update(const std::string& plugin_name, const std::vector<std::string>& object_names);

  /**
   * @brief Returns the name of the plugin where the given object name is registered.
   *
   * If object name is not known, will return an empty optional.
   */
  std::optional<std::string> GetPluginName(const std::string& object_name) const;

  /**
   * @brief Returns objects names registered for the given plugin name.
   */
  std::vector<std::string> GetObjectNames(const std::string& plugin_name) const;

private:
  std::map<std::string, std::string> m_object_name_to_plugin_name;
  get_object_names_t m_get_object_names;
};

}  // namespace oac_tree_gui

#endif  // OAC_TREE_GUI_DOMAIN_DOMAIN_OBJECT_TYPE_REGISTRY_H_
