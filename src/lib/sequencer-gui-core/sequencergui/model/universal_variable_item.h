/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2023 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef SEQUENCERGUI_MODEL_UNIVERSAL_VARIABLE_ITEM_H_
#define SEQUENCERGUI_MODEL_UNIVERSAL_VARIABLE_ITEM_H_

#include <sequencergui/domain/sequencer_types_fwd.h>
#include <sequencergui/model/variable_item.h>

namespace sequencergui
{

class UniversalVariableItem : public VariableItem
{
public:
  static inline const std::string Type = "UniversalVariable";

  using VariableItem::VariableItem;
  UniversalVariableItem();

  std::unique_ptr<SessionItem> Clone(bool make_unique_id) const override;

  std::string GetDomainType() const override;


  /**
   * @brief Static method to create VariableItem representing domain variable.
   * @param domain_type The typename of the domain variable.
   * @return VariableItem representing default initialised domain variable.
   */
  static std::unique_ptr<VariableItem> CreateVariableItem(const std::string& domain_type);

private:
  void InitFromDomainImpl(const variable_t* variable) override;
  void SetupDomainImpl(variable_t* variable) const override;

  std::string m_domain_type;  //!< Domain typename.
};

}  // namespace sequencergui

#endif  // SEQUENCERGUI_MODEL_UNIVERSAL_VARIABLE_ITEM_H_
