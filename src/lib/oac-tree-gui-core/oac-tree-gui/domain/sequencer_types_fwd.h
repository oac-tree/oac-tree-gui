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

#ifndef SEQUENCERGUI_DOMAIN_OAC_TREE_TYPES_FWD_H_
#define SEQUENCERGUI_DOMAIN_OAC_TREE_TYPES_FWD_H_

//! @file
//! Forward declarations and type aliases to some of sequencer domain types.

namespace sup::oac_tree
{
class AttributeDefinition;
class Instruction;
class Procedure;
class ProcedurePreamble;
class Runner;
class Variable;
class Workspace;

class InstructionInfo;
class VariableInfo;
class JobInfo;
class WorkspaceInfo;
class IJobInfoIO;
class IJob;
}  // namespace sup::oac_tree

using attribute_definition_t = sup::oac_tree::AttributeDefinition;
using instruction_t = sup::oac_tree::Instruction;
using preamble_t = sup::oac_tree::ProcedurePreamble;
using procedure_t = sup::oac_tree::Procedure;
using runner_t = sup::oac_tree::Runner;
using variable_t = sup::oac_tree::Variable;
using workspace_t = sup::oac_tree::Workspace;

#endif  // SEQUENCERGUI_DOMAIN_OAC_TREE_TYPES_FWD_H_
