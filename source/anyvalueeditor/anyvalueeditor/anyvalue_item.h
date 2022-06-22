/******************************************************************************
 *
 * Project       : Graphical User Interface for SUP Sequencer
 *
 * Description   : Integrated development environment for Sequencer procedures
 *
 * Author        : Gennady Pospelov (IO)
 *
 * Copyright (c) : 2010-2022 ITER Organization,
 *                 CS 90 046
 *                 13067 St. Paul-lez-Durance Cedex
 *                 France
 *
 * This file is part of ITER CODAC software.
 * For the terms and conditions of redistribution or use of this software
 * refer to the file ITER-LICENSE.TXT located in the top level directory
 * of the distribution package.
 *****************************************************************************/

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_ITEM_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_ITEM_H_


#include <mvvm/model/compound_item.h>

namespace anyvalueeditor
{

class AnyValueItem : public mvvm::CompoundItem
{
public:
  static inline const std::string Type = "AnyValue";

  AnyValueItem();

  void SetAnyTypeName(const std::string& type_name);

  std::string GetAnyTypeName() const;

};


}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_ITEM_H_
