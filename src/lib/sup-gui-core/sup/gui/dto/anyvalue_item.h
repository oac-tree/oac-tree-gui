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

#ifndef GUI_DTO_ANYVALUE_ITEM_H_
#define GUI_DTO_ANYVALUE_ITEM_H_

#include <mvvm/model/compound_item.h>

namespace sup::gui
{

//! Base class for all AnyValueItems

class AnyValueItem : public mvvm::CompoundItem
{
public:
  explicit AnyValueItem(const std::string& item_type);

  virtual void SetAnyTypeName(const std::string& type_name);

  std::string GetAnyTypeName() const;

  virtual bool IsScalar() const;
  virtual bool IsStruct() const;
  virtual bool IsArray() const;
};

//! The item to represent AnyValue empty item.

class AnyValueEmptyItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueEmpty";

  explicit AnyValueEmptyItem();
};

//! The item to represent AnyValue scalar.

class AnyValueScalarItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueScalar";

  AnyValueScalarItem();

  void SetAnyTypeName(const std::string& type_name) override;

  bool IsScalar() const override;
};

//! The item to represent AnyValue struct.

class AnyValueStructItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueStruct";

  AnyValueStructItem();

  bool IsStruct() const override;

  AnyValueScalarItem* AddScalarField(const std::string& field_name, const std::string& field_type,
                                     const variant_t& value);
};

//! The item to represent AnyValue array.

class AnyValueArrayItem : public AnyValueItem
{
public:
  static inline const std::string Type = "AnyValueArray";

  AnyValueArrayItem();

  bool IsArray() const override;
};

}  // namespace sup::gui

#endif  // GUI_DTO_ANYVALUE_ITEM_H_
