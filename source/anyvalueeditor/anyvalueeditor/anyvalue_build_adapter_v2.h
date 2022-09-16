/******************************************************************************
 *
 * Project       : Supervision and automation system EPICS interface
 *
 * Description   : Library of SUP components for EPICS network protocol
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

#ifndef ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_V2_H_
#define ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_V2_H_

#include <anyvalueeditor/dto_types_fwd.h>
#include <sup/dto/basic_scalar_types.h>

#include <memory>
#include <string>

namespace anyvalueeditor
{

//! Builds AnyValue in step-wise manner by calling methods to add fields and structs.

class AnyValueBuildAdapterV2
{
public:
  AnyValueBuildAdapterV2();
  ~AnyValueBuildAdapterV2();

private:
  struct AnyValueBuildAdapterV2Impl;
  std::unique_ptr<AnyValueBuildAdapterV2Impl> p_impl;
};

}  // namespace anyvalueeditor

#endif  // ANYVALUEEDITOR_ANYVALUEEDITOR_ANYVALUE_BUILD_ADAPTER_V2_H_
